## 1 - Introduction

This ISPU example implements the *Sliding Discrete Fourier Transform (SDFT)* algorithm for computing the magnitude of the DFT components (from 0 Hz to 13 Hz) of the accelerometer z-axis over a sliding window of 32 samples.

The accelerometer is configured in high-performance mode at 26 Hz.

The sdft library is provided as binary in the *ispu/lib* folder.

The outputs are as follows:

* DFT magnitude as an array of 16 float values mapped starting from ISPU_DOUT_00_L (10h) to ISPU_DOUT_30_L (4Ch)

## 2 - Device orientation

None.

## 3 - Interrupts

The configuration generates an interrupt in INT1 when the DFT magnitude is computed on the current sliding window and available in the output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2022 STMicroelectronics**