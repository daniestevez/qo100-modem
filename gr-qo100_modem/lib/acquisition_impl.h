/* -*- c++ -*- */
/*
 * Copyright 2021 Daniel Estevez <daniel@destevez.net>
 *
 * This file is part of qo100_modem
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef INCLUDED_QO100_MODEM_ACQUISITION_IMPL_H
#define INCLUDED_QO100_MODEM_ACQUISITION_IMPL_H

#include <gnuradio/fft/fft.h>
#include <qo100_modem/acquisition.h>
#include <volk/volk.h>
#include <vector>

namespace gr {
namespace qo100_modem {

class acquisition_impl : public acquisition
{
private:
    pmt::pmt_t d_src_id;
    size_t d_fft_oversample;
    size_t d_sample_window;
    size_t d_max_bin;
    float d_threshold;
    fft::fft_complex_fwd d_fft_fwd;
    fft::fft_complex_rev d_fft_rev;
    std::vector<volk::vector<gr_complex>> d_prbs_fft_conj;
    bool d_synced;

    void set_prbs_fft_conj(const std::vector<float>& prbs, size_t tdm_rate);
    void set_sync();
    void set_unsync();

public:
    acquisition_impl(const std::vector<float>& prbs,
                     size_t tdm_rate,
                     size_t fft_oversample,
                     float threshold);
    ~acquisition_impl();

    // Where all the action really happens
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);

    void handle_resync(pmt::pmt_t msg);
};

} // namespace qo100_modem
} // namespace gr

#endif /* INCLUDED_QO100_MODEM_ACQUISITION_IMPL_H */
