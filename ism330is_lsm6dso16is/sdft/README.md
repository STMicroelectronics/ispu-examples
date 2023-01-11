## 1 - Introduction

This ISPU example implements the *Sliding Discrete Fourier Transform (SDFT)* algorithm, which efficiently computes the DFT on a sliding window using dynamic programming.

The accelerometer is configured in high-performance mode at 26 Hz.

NB: for demonstration purposes, only the accelerometer z-axis is used for computing the magnitude of 16 DFT components (from 0 Hz to 13 Hz).

The sdft library is provided as binary in the *ispu/lib* folder.

The outputs are as follows:

* |DFT_00| as float mapped starting from ISPU_DOUT_00_L (10h)
* |DFT_01| as float mapped starting from ISPU_DOUT_02_L (14h)
* |DFT_02| as float mapped starting from ISPU_DOUT_04_L (18h)
* |DFT_03| as float mapped starting from ISPU_DOUT_06_L (1Ch)
* |DFT_04| as float mapped starting from ISPU_DOUT_08_L (20h)
* |DFT_05| as float mapped starting from ISPU_DOUT_10_L (24h)
* |DFT_06| as float mapped starting from ISPU_DOUT_12_L (28h)
* |DFT_07| as float mapped starting from ISPU_DOUT_14_L (2Ch)
* |DFT_08| as float mapped starting from ISPU_DOUT_16_L (30h)
* |DFT_09| as float mapped starting from ISPU_DOUT_18_L (34h)
* |DFT_10| as float mapped starting from ISPU_DOUT_20_L (38h)
* |DFT_11| as float mapped starting from ISPU_DOUT_22_L (3Ch)
* |DFT_12| as float mapped starting from ISPU_DOUT_24_L (40h)
* |DFT_13| as float mapped starting from ISPU_DOUT_26_L (44h)
* |DFT_14| as float mapped starting from ISPU_DOUT_28_L (48h)
* |DFT_15| as float mapped starting from ISPU_DOUT_30_L (4Ch)

## 2 - Device orientation

None.


## 3 - Interrupts

The configuration generates an interrupt in INT1 when the absolute value of the
DFT is computed on the current sliding window and available in the output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2022 STMicroelectronics**