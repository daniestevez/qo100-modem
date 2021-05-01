# QO-100 32APSK narrowband modem

This GNU Radio out-of-tree module contains an experimental high-speed narrowband
modem for its use over the QO-100 NB transponder and other similar use cases.

The modulation used is a single carrier APSK waveform inspired in DVB-S2. The
main features are the following:
* Baudrate 2570 symbols/s
* RRC filtering with excess bandwidth 0.05
* Occupied bandwidth 2700 Hz
* BPSK pilot symbols every 50 symbols
* 32APSK data symbols
* Raw bitrate 12593 bps

Currently only the modulation, demodulation and synchronization is
implemented. The next step is to find a suitable FEC that allows error-free copy
at the target CN0 of 50 dB·Hz.

## Requirements

This OOT module supports GNU Radio 3.9 only. The following OOT modules are
required:

* [gr-satellites](https://github.com/daniestevez/gr-satellites)
* [gr-pdu_utils](https://github.com/sandialabs/gr-pdu_utils)

## Installation

gr-qo100_modem can be compiled and installed as any OOT:

```
mkdir build
cd build
cmake ..
make
sudo make install
```

Some data paths need to be adjusted to point to the data files in these
repository. Look in
* `hier/ber_calc.grc`
* `examples/loopback_test.grc`
* `examples/qo100_test.grc`

After installation, it is necessary to open each of the hierarchical flowgraphs
in the `hier/` folder with `gnuradio-companion` and click the "Generate the
flowgraph" button. There is a bug in the generation of the demodulator GUI
flowgraph, so the `qo100_demodulator_gui.py` file inside `hier` needs to be
copied into `~/.grc_gnuradio/` after generating the hierarchical flowgraphs.

## Example flowgraphs

There are three example flowgraphs included in the `examples/` folder:

* `loopback_test.grc` performs a simulation of the modem over an AWGN channel.
* `recording_test.grc` can be used to demodulate an IQ recording, such as the
one included in the `32apsk/` folder of this repository.
* `qo100_test.grc` performs an over-the-air test over the QO-100 NB transponder
using my
[qo100-groundstation](https://github.com/daniestevez/qo100-groundstation)
software stack. This flowgraph should be customized for other groundstations.
