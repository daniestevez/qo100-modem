/* -*- c++ -*- */
/*
 * Copyright 2021 Daniel Estevez <daniel@destevez.net>
 *
 * This file is part of qo100_modem
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include "pilot_pll_impl.h"
#include <gnuradio/expj.h>
#include <gnuradio/io_signature.h>
#include <gnuradio/math.h>

namespace gr {
namespace qo100_modem {

pilot_pll::sptr pilot_pll::make(const std::vector<float>& prbs, size_t tdm_rate)
{
    return gnuradio::make_block_sptr<pilot_pll_impl>(prbs, tdm_rate);
}


/*
 * The private constructor
 */
pilot_pll_impl::pilot_pll_impl(const std::vector<float>& prbs, size_t tdm_rate)
    : gr::block("pilot_pll",
                gr::io_signature::make(1, 1, sizeof(gr_complex)),
                gr::io_signature::make(2, 2, sizeof(gr_complex))),
      d_prbs(prbs),
      d_tdm_rate(tdm_rate),
      d_synced(false),
      d_current_symbol(0),
      d_current_pilot(0),
      d_freq(0.0f),
      d_phase(0.0f),
      d_alpha(0.1f),
      d_beta(1e-3f),
      d_amplitude_alpha(0.1f),
      d_amplitude(1.0f)
{
}

/*
 * Our virtual destructor.
 */
pilot_pll_impl::~pilot_pll_impl() {}

int pilot_pll_impl::general_work(int noutput_items,
                                 gr_vector_int& ninput_items,
                                 gr_vector_const_void_star& input_items,
                                 gr_vector_void_star& output_items)
{
    const gr_complex* in = reinterpret_cast<const gr_complex*>(input_items[0]);
    gr_complex* out_data = reinterpret_cast<gr_complex*>(output_items[0]);
    gr_complex* out_pilot = reinterpret_cast<gr_complex*>(output_items[1]);
    size_t nout_data = 0;
    size_t nout_pilot = 0;

    std::vector<tag_t> tags_phase;
    std::vector<tag_t> tags_freq;
    get_tags_in_range(tags_phase,
                      0,
                      nitems_read(0),
                      nitems_read(0) + noutput_items,
                      pmt::intern("phase_est"));
    get_tags_in_range(tags_freq,
                      0,
                      nitems_read(0),
                      nitems_read(0) + noutput_items,
                      pmt::intern("freq_est"));

    for (int j = 0; j < noutput_items; ++j) {
        if (!tags_phase.empty()) {
            if (tags_phase[0].offset - nitems_read(0) == static_cast<size_t>(j)) {
                d_phase = static_cast<float>(pmt::to_double(tags_phase[0].value));
                tags_phase.erase(tags_phase.begin());
                set_sync();
            }
        }
        if (!tags_freq.empty()) {
            if (tags_freq[0].offset - nitems_read(0) == static_cast<size_t>(j)) {
                d_freq = static_cast<float>(pmt::to_double(tags_freq[0].value));
                tags_freq.erase(tags_freq.begin());
                set_sync();
            }
        }

        gr_complex x = *in++;

        if (!d_synced) {
            /* We are not synced. Do nothing with the current sample */
            continue;
        }

        x *= d_amplitude;
        x *= gr_expj(-d_phase);

        if (d_current_symbol == 0) {
            /* this is a pilot symbol */
            *out_pilot++ = x;
            ++nout_pilot;

            gr_complex x_wipe = x * d_prbs[d_current_pilot];
            float phase_err = gr::fast_atan2f(x_wipe.imag(), x_wipe.real());
            if (phase_err > GR_M_PI) {
                phase_err -= 2 * GR_M_PI;
            } else if (phase_err < -GR_M_PI) {
                phase_err += 2 * GR_M_PI;
            }
            d_phase += d_alpha * phase_err;
            d_freq += d_beta * phase_err;
            float amplitude_err = 1.0f - std::abs(x_wipe);
            d_amplitude += d_amplitude_alpha * amplitude_err;
            d_current_pilot = (d_current_pilot + 1) % d_prbs.size();
        } else {
            /* this is a data symbol */
            if ((d_current_symbol == 1) & (d_current_pilot == 1)) {
                /* first data symbol */
                add_item_tag(0,
                             nitems_written(0) + nout_data,
                             pmt::intern("sync"),
                             pmt::from_long(0),
                             pmt::PMT_NIL);
            }
            *out_data++ = x;
            ++nout_data;
        }
        d_current_symbol = (d_current_symbol + 1) % d_tdm_rate;
        d_phase += d_freq;
        if (d_phase > GR_M_PI) {
            d_phase -= 2 * GR_M_PI;
        } else if (d_phase < -GR_M_PI) {
            d_phase += 2 * GR_M_PI;
        }
    }

    consume(0, noutput_items);
    produce(0, nout_data);
    produce(1, nout_pilot);

    return WORK_CALLED_PRODUCE;
}

void pilot_pll_impl::set_sync()
{
    d_current_symbol = 0;
    d_current_pilot = 0;
    d_synced = true;
    d_amplitude = 1.0f;
}

} /* namespace qo100_modem */
} /* namespace gr */
