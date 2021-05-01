/* -*- c++ -*- */
/*
 * Copyright 2021 Daniel Estevez <daniel@destevez.net>
 *
 * This file is part of qo100_modem
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef INCLUDED_QO100_MODEM_PILOT_PLL_IMPL_H
#define INCLUDED_QO100_MODEM_PILOT_PLL_IMPL_H

#include <qo100_modem/pilot_pll.h>
#include <vector>

namespace gr {
namespace qo100_modem {

class pilot_pll_impl : public pilot_pll
{
private:
    std::vector<float> d_prbs;
    size_t d_tdm_rate;
    bool d_synced;
    size_t d_current_symbol;
    size_t d_current_pilot;
    float d_freq;
    float d_phase;
    float d_alpha;
    float d_beta;
    float d_amplitude_alpha;
    float d_amplitude;

    void set_sync();

public:
    pilot_pll_impl(const std::vector<float>& prbs, size_t tdm_rate);
    ~pilot_pll_impl();

    // Where all the action really happens
    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace qo100_modem
} // namespace gr

#endif /* INCLUDED_QO100_MODEM_PILOT_PLL_IMPL_H */
