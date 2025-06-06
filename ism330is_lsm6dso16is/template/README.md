## 1 - Introduction

This is a template for creating a new project for the ISPU.

The *ispu* folder may be copied by itself (no dependencies other than the CLI toolchain or IDE environment that must be installed) and used as a starting project to implement any solution running on the ISPU.


## 2 - Structure and how to use

The *ispu* folder contains:

* ***eclipse***: folder containing the Eclipse project files. A ***release*** subfolder containing the output files is created upon build.
* ***ispu_utils***: folder containing several utilities. Usually, there is no need to modify these files.
  * ***boot.ld***: linker script.
  * ***crt0.S***: startup assembly code.
  * ***global.S***: additional assembly code.
  * ***peripherals.h***: cast macros and more.
  * ***reg_map.h***: register map macros.
* ***make***: folder containing the Makefile for building using the command line. A ***bin*** subfolder containing the output files is created upon build.
* ***src***:
  * ***main.c***: main source file template. This is where the user code must be implemented:
    * The `main` function should not be modified.
    * The user should fill in `algo_00_init` and `algo_00` functions respectively with the initialization and running code of the algorithm.

      ```c
      void __attribute__ ((signal)) algo_00_init(void)
      {
        // TODO: initialize algorithm
      }

      void __attribute__ ((signal)) algo_00(void)
      {
        // TODO: read and process sensor data

        // interrupt generation (set bit 0 for algo 0, bit 1 for algo 1, etc.)
        int_status = int_status | 0x1u;
      }
      ```

      Note that `algo_00_init` is executed only when the first data sample is generated by the sensor (after boot or after disabling and re-enabling the algorithm), while `algo_00` is executed every time a new data sample is generated by the sensor (only one sample every few samples may be processed if the ISPU rate is configured to be lower than the sensor output data rate). For more details on the processing flow of the ISPU, please refer to section 9.1 of the application notes of the devices [ISM330IS](https://www.st.com/resource/en/application_note/an5850-ism330is-alwayson-3axis-accelerometer-and-3axis-gyroscope-with-ispu--intelligent-sensor-processing-unit-stmicroelectronics.pdf) and [LSM6DSO16IS](https://www.st.com/resource/en/application_note/an5799-lsm6dso16is-alwayson-3axis-accelerometer-and-3axis-gyroscope-with-ispu--intelligent-sensor-processing-unit-stmicroelectronics.pdf).
    * Analogous functions (for example, `algo_01_init` and `algo_01`) can be added in order to implement additional algorithms (up to 30).
    * The global variable `int_status` can be used to report events to an external microcontroller. Normally, each bit is dedicated to one algorithm (or anyway a type of event), and it should be set to `1` when the corresponding event is to be reported. In the template, function `algo_00` always sets the first bit to `1` in order to report an event every time a new data sample is processed by the algorithm.

      ```c
      static volatile uint32_t int_status;

      ...

      void __attribute__ ((signal)) algo_00(void)
      {
        ...

        // interrupt generation (set bit 0 for algo 0, bit 1 for algo 1, etc.)
        int_status = int_status | 0x1u;
      }
      ```

      After the execution of the configured algorithms to process the current data sample, the value of `int_status` is copied to the ISPU_INT_STATUS registers and is used to generate interrupts on the INT1 and INT2 pins based on the configuration available in registers ISPU_INT1_CTRL and ISPU_INT2_CTRL.

      ```c
      int main(void)
      {
        ...

        while (true) {
          ...

          // get interrupt flags
          uint8_t int_pin = 0u;
          int_pin |= ((int_status & cast_uint32_t(ISPU_INT1_CTRL)) > 0u) ? 0x01u : 0x00u;
          int_pin |= ((int_status & cast_uint32_t(ISPU_INT2_CTRL)) > 0u) ? 0x02u : 0x00u;

          // set status registers and generate interrupts
          cast_uint32_t(ISPU_INT_STATUS) = int_status;
          cast_uint8_t(ISPU_INT_PIN) = int_pin;
        }
      }
      ```

  * Other source files may be added, for example files implementing functions for specific algorithms, that may then be called from ***main.c***.
* ***conf.txt***: file containing the sensor configuration to add to the output files. This can be used to configure, for example, the output data rate and full scale of the sensor, the rate of processing of the ISPU, and the generation of interrupts.

  The file included in the template already contains a minimal configuration:
  * Accelerometer at 26 Hz, with 8 g full scale, and in high-performance mode
  * ISPU processing at 26 Hz
  * Interrupts for ISPU events enabled on INT1 (in pulsed mode)
  * INT2 configured with the ISPU sleep signal (useful for execution time measurements)
  * Algorithm 00 enabled (with interrupt on INT1 also enabled)

  ```
  acc_odr 26
  acc_fs 8
  acc_mode high_performance

  ispu_irq_rate 26
  ispu_int1 enable
  ispu_sleep_int2 enable
  ispu_latch disable

  algo 0 enable
  algo_int1 0 enable
  ```

  For all available settings and their syntax, refer to the **CONFIGURATION** section of the help of the `ispu_gen` tool, which can be displayed by running `ispu_gen -h -d imu_22` from the command line or can be found going to "Help", "Help Contents", and navigating to "ISPU Tools Documentation", "ispu_gen" if using an Eclipse-based IDE.
* ***meta.txt***: file containing the metadata to add to the output files. This can be used to add information about the outputs of the ISPU: position, data type, length, and, optionally, labels to associate to specific output values.

  Since the format of the output data written to the ISPU_DOUT registers depends on the specific user code, STMicroelectronics tools (for example, [MEMS Studio](https://www.st.com/en/development-tools/mems-studio.html)) utilize this information to display the ISPU outputs in their proper format.

  Here below is the metadata file for the norm example. In this case, the output contains the original accelerometer data for the 3 axes, in `int16_t` format, followed by the norm, which is a `float` value. Since all outputs are consecutive and stored starting from the first ISPU_DOUT register, the position does not need to be explicitly specified. The length, being equal to 1 for all outputs, can also be omitted.

  ```
  description "Norm"

  output "Acc x [LSB]" int16_t
  output "Acc y [LSB]" int16_t
  output "Acc z [LSB]" int16_t
  output "Acc norm [LSB]" float
  ```

  For all the details on the syntax for this file, refer to the **METADATA** section of the help of the `ispu_gen` tool, which can be displayed by running `ispu_gen -h -d imu_22` from the command line or can be found going to "Help", "Help Contents", and navigating to "ISPU Tools Documentation", "ispu_gen" if using an Eclipse-based IDE.
* ***shub.txt***: file containing the sensor hub configuration to add to the output files. This can be used to configure an external sensor attached via sensor hub and to configure the sensor hub to read data from the external sensor, so that it can be processed by the ISPU.

  Here below is the sensor hub file for the example of fitness activity. In this case, two write operations are used to turn on the external pressure sensor at 25 Hz, with an averaging setting of 32 and block data update enabled. A read operation is then configured to read the 3 bytes of pressure data, which will be performed at the specified rate of 26 Hz.

  ```
  shub_odr 26

  # LPS22DF
  w 0xBA 0x10 0x23 # pressure @ 25 Hz, averaging 32
  w 0xBA 0x11 0x08 # block data update
  r 0xBA 0x28 3 # pressure data
  ```

  For the syntax to use for this file, refer to the **SENSOR HUB** section of the help of the `ispu_gen` tool, which can be displayed by running `ispu_gen -h -d imu_22` from the command line or can be found going to "Help", "Help Contents", and navigating to "ISPU Tools Documentation", "ispu_gen" if using an Eclipse-based IDE.

------

**More information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2022 STMicroelectronics**
