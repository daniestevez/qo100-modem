/* -*- c++ -*- */
/*
 * Copyright 2021 Daniel Estevez <daniel@destevez.net>
 *
 * This file is part of qo100_modem
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef INCLUDED_QO100_MODEM_ACQUISITION_H
#define INCLUDED_QO100_MODEM_ACQUISITION_H

#include <gnuradio/sync_block.h>
#include <qo100_modem/api.h>
#include <vector>

namespace gr {
namespace qo100_modem {

/*!
 * \brief Performs acquisition of the pilot PRBS, adding synchronization tags
 * \ingroup qo100_modem
 *
 */
class QO100_MODEM_API acquisition : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<acquisition> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of qo100_modem::acquisition.
     *
     * To avoid accidental use of raw pointers, qo100_modem::acquisition's
     * constructor is in a private implementation
     * class. qo100_modem::acquisition::make is the public interface for
     * creating new instances.
     */
    static sptr make(const std::vector<float>& prbs,
                     size_t tdm_rate,
                     size_t fft_oversample = 2,
                     float threshold = 20.0f);
};

} // namespace qo100_modem
} // namespace gr

#endif /* INCLUDED_QO100_MODEM_ACQUISITION_H */
