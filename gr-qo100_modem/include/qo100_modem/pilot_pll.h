/* -*- c++ -*- */
/*
 * Copyright 2021 Daniel Estevez <daniel@destevez.net>
 *
 * This file is part of qo100_modem
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef INCLUDED_QO100_MODEM_PILOT_PLL_H
#define INCLUDED_QO100_MODEM_PILOT_PLL_H

#include <gnuradio/block.h>
#include <qo100_modem/api.h>

namespace gr {
namespace qo100_modem {

/*!
 * \brief PLL using the pilot BPSK symbols in the waveform
 * \ingroup qo100_modem
 *
 */
class QO100_MODEM_API pilot_pll : virtual public gr::block
{
public:
    typedef std::shared_ptr<pilot_pll> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of qo100_modem::pilot_pll.
     *
     * To avoid accidental use of raw pointers, qo100_modem::pilot_pll's
     * constructor is in a private implementation
     * class. qo100_modem::pilot_pll::make is the public interface for
     * creating new instances.
     */
    static sptr make(const std::vector<float>& prbs, size_t tdm_rate);
};

} // namespace qo100_modem
} // namespace gr

#endif /* INCLUDED_QO100_MODEM_PILOT_PLL_H */
