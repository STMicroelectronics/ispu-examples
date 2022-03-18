## 1 - Introduction

This is a template for creating a new project for STRed-ISPU.

The *ispu* folder may be copied by itself (no dependencies other than the CLI toolchain or IDE environment that must be installed) and used as a starting blank project to implement any solution running on STRed-ISPU.


## 2 - Structure and how to use

The *ispu* folder contains:

* ***src***:
  * *main.c*: main source file template. This is where the user code must be implemented:
    * The *main* function should not be modified.
    * The user should fill in *algo_00_init* and *algo_00* respectively with the initialization and running code of the algorithm.
    * Please note that analogous functions (e.g., *algo_01_init* and *algo_01*) can be added in order to have additional algorithms (up to 30).
* ***ispu_utils***: folder containing several utilities. Usually, there is no need to modify these files.
  * *boot.ld*: linker script.
  * *crt0.S*: startup file.
  * *global.S*: additional assembly file.
  * *reg_map.h*: register map defines.
  * *peripherals.h*: cast defines and others.
* ***conf.txt***: file containing sensor configuration. Please refer to the help of the *ispu_gen* command line tool (included in the toolchain or Eclipse IDE) for how to modify the configuration.
* ***make***: folder containing the makefile for building using the command line.
* ***eclipse***: folder containing the Eclipse project.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2022 STMicroelectronics**