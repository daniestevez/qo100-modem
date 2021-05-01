#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Copyright 2021 Daniel Estevez <daniel@destevez.net>
#
# This file is part of qo100_modem
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

'''
This is the GNU Radio QO-100 module.

This OOT module implements a 32APSK narrowband modem to be used
over the QO-100 NB transponder.
'''

__author__ = 'Daniel Estevez'
__copyright__ = 'Copyright 2021 Daniel Estevez'
__email__ = 'daniel@destevez.net'
__license__ = 'GPL-3.0'

# import pybind11 generated symbols into the qo100_modem namespace
from .qo100_modem_python import *

# import any pure python here
#
