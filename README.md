# QO100 modem

This repository contains experiments and analysis related to the development
of a narrowband data modem for the NB transponder of QO100 (Es'hail 2).

The target design is for a signal able to pack as much information as possible
in a 2.7kHz SSB bandwidth and which gives error free copy at a power level equal
to the BPSK transponder beacon.

The leading modem candidate is a 32APSK waveform, for which a GNU Radio
implementation in the gr-qo100_modem is provided. There are also some older
experiments with a differentially-encoded 8PSK waveform.
