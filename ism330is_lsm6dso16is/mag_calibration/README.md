## 1 - Introduction

This ISPU example implements the magnetometer calibration.

The accelerometer is configured in high-performance mode at 104 Hz.

The magnetometer (LIS2MDL) is connected through the sensor-hub interface and it is configured in high-performance mode at 100 Hz. The sensor-hub configuration to apply is provided in the *output* folder as a separate file and must loaded before the example main configuration.

The MotionMC library is provided as binary in the *ispu/lib* folder.

The outputs are as follows:

* Calibrated magnetometer x-axis [uT/50] as float mapped starting from ISPU_DOUT_00_L (10h)
* Calibrated magnetometer y-axis [uT/50] as float mapped starting from ISPU_DOUT_02_L (14h)
* Calibrated magnetometer z-axis [uT/50] as float mapped starting from ISPU_DOUT_04_L (18h)
* Magnetometer x-axis offset [uT/50] as float mapped starting from ISPU_DOUT_06_L (1Ch)
* Magnetometer y-axis offset [uT/50] as float mapped starting from ISPU_DOUT_08_L (20h)
* Magnetometer z-axis offset [uT/50] as float mapped starting from ISPU_DOUT_10_L (24h)
* Magnetometer calibration level as uint8_t mapped starting from ISPU_DOUT_12_L (28h)


## 2 - Device orientation

None.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the calibrated magnetometer for the new sample is computed and available in output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2022 STMicroelectronics**