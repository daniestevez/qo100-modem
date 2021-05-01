/*
 * Copyright 2021 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/***********************************************************************************/
/* This file is automatically generated using bindtool and can be manually edited  */
/* The following lines can be configured to regenerate this file during cmake      */
/* If manual edits are made, the following tags should be modified accordingly.    */
/* BINDTOOL_GEN_AUTOMATIC(0)                                                       */
/* BINDTOOL_USE_PYGCCXML(0)                                                        */
/* BINDTOOL_HEADER_FILE(pilot_pll.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(ec66d3718decf147d37a5c4fef80904c)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <qo100_modem/pilot_pll.h>
// pydoc.h is automatically generated in the build directory
#include <pilot_pll_pydoc.h>

void bind_pilot_pll(py::module& m)
{

    using pilot_pll    = ::gr::qo100_modem::pilot_pll;


    py::class_<pilot_pll, gr::block, gr::basic_block,
        std::shared_ptr<pilot_pll>>(m, "pilot_pll", D(pilot_pll))

        .def(py::init(&pilot_pll::make),
           py::arg("prbs"),
           py::arg("tdm_rate"),
           D(pilot_pll,make)
        )
        



        ;




}








