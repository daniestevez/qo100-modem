/* -*- c++ -*- */
/*
 * Copyright 2021 Daniel Estevez <daniel@destevez.net>
 *
 * This file is part of qo100_modem
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include "acquisition_impl.h"
#include <gnuradio/io_signature.h>
#include <gnuradio/math.h>
#include <cstring>

namespace gr {
namespace qo100_modem {

acquisition::sptr acquisition::make(const std::vector<float>& prbs,
                                    size_t tdm_rate,
                                    size_t fft_oversample,
                                    float threshold)
{
    return gnuradio::make_block_sptr<acquisition_impl>(
        prbs, tdm_rate, fft_oversample, threshold);
}


/*
 * The private constructor
 */
acquisition_impl::acquisition_impl(const std::vector<float>& prbs,
                                   size_t tdm_rate,
                                   size_t fft_oversample,
                                   float threshold)
    : gr::sync_block("acquisition",
                     gr::io_signature::make(1, 1, sizeof(gr_complex)),
                     gr::io_signature::make(1, 2, sizeof(gr_complex))),
      d_src_id(pmt::intern(alias())),
      d_fft_oversample(fft_oversample),
      d_sample_window(prbs.size() * tdm_rate),
      d_max_bin(fft_oversample * prbs.size() / 2),
      d_threshold(threshold * threshold), // we store amplitude^2
      d_fft_fwd(d_sample_window * fft_oversample),
      d_fft_rev(d_sample_window * fft_oversample)
{
    set_unsync();
    set_prbs_fft_conj(prbs, tdm_rate);

    message_port_register_in(pmt::mp("resync"));
    set_msg_handler(pmt::mp("resync"),
                    [this](pmt::pmt_t msg) { this->handle_resync(msg); });
}

/*
 * Our virtual destructor.
 */
acquisition_impl::~acquisition_impl() {}

int acquisition_impl::work(int noutput_items,
                           gr_vector_const_void_star& input_items,
                           gr_vector_void_star& output_items)
{
    const gr_complex* in = reinterpret_cast<const gr_complex*>(input_items[0]);
    gr_complex* out = reinterpret_cast<gr_complex*>(output_items[0]);

    const auto fft_size = d_sample_window * d_fft_oversample;

    const bool has_fft_output = output_items.size() >= 2;
    gr_complex* fft_out =
        has_fft_output ? reinterpret_cast<gr_complex*>(output_items[1]) : NULL;

    if (d_synced) {
        /* We are synced. We skip all FFTs */
        std::memcpy(out, in, noutput_items * sizeof(gr_complex));
        if (has_fft_output) {
            std::memset(fft_out, 0, noutput_items * sizeof(gr_complex));
        }
        return noutput_items;
    }

    for (int j = 0; j < noutput_items / d_sample_window; ++j) {
        std::memcpy(d_fft_fwd.get_inbuf(), in, d_sample_window * sizeof(gr_complex));
        std::memset(d_fft_fwd.get_inbuf() + d_sample_window,
                    0,
                    d_sample_window * (d_fft_oversample - 1) * sizeof(gr_complex));
        d_fft_fwd.execute();

        float maxval = -1.0f;
        int max_k = 0;
        int best_argmax = 0;
        gr_complex max_z(0);
        for (int k = 0; k < d_prbs_fft_conj.size(); ++k) {
            volk_32fc_x2_multiply_32fc(d_fft_rev.get_inbuf(),
                                       d_fft_fwd.get_outbuf(),
                                       d_prbs_fft_conj[k].data(),
                                       fft_size);
            d_fft_rev.execute();
            uint32_t argmax;
            // TODO: do not look over all the FFT
            volk_32fc_index_max_32u(&argmax, d_fft_rev.get_outbuf(), fft_size);
            gr_complex z = d_fft_rev.get_outbuf()[argmax];
            float thismax = z.real() * z.real() + z.imag() * z.imag();
            if (thismax > maxval) {
                maxval = thismax;
                max_k = k;
                best_argmax = argmax;
                max_z = z;
                if (has_fft_output) {
                    int half_window = d_sample_window / 2;
                    std::memcpy(fft_out,
                                d_fft_rev.get_outbuf(),
                                half_window * sizeof(gr_complex));
                    std::memcpy(fft_out + half_window,
                                d_fft_rev.get_outbuf() +
                                    d_sample_window * d_fft_oversample -
                                    (d_sample_window - half_window),
                                (d_sample_window - half_window) * sizeof(gr_complex));
                }
            }
        }

        if (maxval >= d_threshold) {
            int corr_pos = best_argmax % d_sample_window;
            float phase_est = gr::fast_atan2f(max_z.imag(), max_z.real());
            float freq_est = 2.0f * GR_M_PI *
                             static_cast<float>(max_k - static_cast<int>(d_max_bin)) /
                             static_cast<float>(fft_size);
            int max_output = has_fft_output ? 2 : 1;
            for (int output = 0; output < max_output; ++output) {
                add_item_tag(output,
                             nitems_written(0) + j * d_sample_window + corr_pos,
                             pmt::intern("phase_est"),
                             pmt::from_double(phase_est),
                             d_src_id);
                add_item_tag(output,
                             nitems_written(0) + j * d_sample_window + corr_pos,
                             pmt::intern("freq_est"),
                             pmt::from_double(freq_est),
                             d_src_id);
            }
            set_sync();
        }

        std::memcpy(out, in, d_sample_window * sizeof(gr_complex));

        in += d_sample_window;
        out += d_sample_window;
        if (has_fft_output) {
            fft_out += d_sample_window;
        }
    }

    return noutput_items;
}

void acquisition_impl::set_prbs_fft_conj(const std::vector<float>& prbs, size_t tdm_rate)
{
    const auto fft_size = d_sample_window * d_fft_oversample;
    std::memset(d_fft_fwd.get_inbuf(), 0, fft_size * sizeof(gr_complex));
    for (int j = 0; j < prbs.size(); ++j) {
        for (int k = 0; k < d_fft_oversample; ++k) {
            d_fft_fwd.get_inbuf()[k * d_sample_window + j * tdm_rate] = prbs[j];
        }
    }
    d_fft_fwd.execute();
    // We abuse the fft_fwd inbuf to contain the complex conjugate
    volk_32fc_conjugate_32fc(d_fft_fwd.get_inbuf(), d_fft_fwd.get_outbuf(), fft_size);

    d_prbs_fft_conj.clear();
    for (int j = 0; j <= 2 * d_max_bin; ++j) {
        int shift = j - d_max_bin;
        int start = shift <= 0 ? -shift : fft_size - shift;
        d_prbs_fft_conj.push_back(volk::vector<gr_complex>());
        d_prbs_fft_conj.back().reserve(fft_size);
        std::memcpy(d_prbs_fft_conj.back().data(),
                    d_fft_fwd.get_inbuf() + start,
                    (fft_size - start) * sizeof(gr_complex));
        std::memcpy(d_prbs_fft_conj.back().data() + (fft_size - start),
                    d_fft_fwd.get_inbuf(),
                    start * sizeof(gr_complex));
    }
}

void acquisition_impl::set_sync()
{
    d_synced = true;
    set_output_multiple(1);
}

void acquisition_impl::set_unsync()
{
    d_synced = false;
    set_output_multiple(d_sample_window);
}

void acquisition_impl::handle_resync(pmt::pmt_t msg) { set_unsync(); }

} /* namespace qo100_modem */
} /* namespace gr */
