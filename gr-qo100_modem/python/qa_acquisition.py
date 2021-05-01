#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2021 Daniel Estevez <daniel@destevez.net>
#
# This file is part of qo100_modem
#
# SPDX-License-Identifier: GPL-3.0-or-later
#
#

from gnuradio import gr, gr_unittest
# from gnuradio import blocks
try:
    from qo100_modem import acquisition
except ImportError:
    import os
    import sys
    dirname, filename = os.path.split(os.path.abspath(__file__))
    sys.path.append(os.path.join(dirname, "bindings"))
    from qo100_modem import acquisition

class qa_acquisition(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_instance(self):
        # FIXME: Test will fail until you pass sensible arguments to the constructor
        instance = acquisition()

    def test_001_descriptive_test_name(self):
        # set up fg
        self.tb.run()
        # check data


if __name__ == '__main__':
    gr_unittest.run(qa_acquisition)
