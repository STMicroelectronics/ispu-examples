## 1 - Introduction

The *stred* software package provides template and example projects and libraries for MEMS devices embedding the STRed-ISPU (intelligent sensor processing unit).

Each project can be built using the command line (make) or any Eclipse based IDE, including STM32CubeIDE, provided that the STRed-ISPU plugins are installed.

Prebuilt files containing the ready to use device configuration are available in the *output* folder of each example project. For the examples related to STMicroelectronics libraries, also the library in binary format is available to be integrated in other projects based on user needs.

## 2 - Command line

In order to build a project using the command line, the STRed-ISPU toolchain must be installed on the system. Besides the toolchain, the *make* build tool must be installed.

The toolchain can be downloaded for Windows, Linux and MacOS at the following link: [https://codex.cro.st.com/file/showfiles.php?group_id=8034](https://codex.cro.st.com/file/showfiles.php?group_id=8034).

The toolchain must be extracted from the downloaded archive to any folder on the system. The *bin* folder inside the installed toolchain must be added to the PATH environment variable as follows:

* On Windows:

  * Open the Start search, type "env" and choose "Edit the system environment variables".
  * Click on the "Environment Variables…" button.
  * Under the "System Variables" section, find the row with "Path" in the "Variable" column, select it and click "Edit...".
  * Click on the "Browse..." button, browse into the folder where the toolchain is installed, select the *bin* folder and click "OK".
  * Close all remaining windows by clicking "OK".
  * Open a new terminal. The toolchain should now be available to be called from the command line (e.g., the *reisc-gcc* command should be found if the user tries to run it).

* On Linux and MacOS:

  * Add to your ".bashrc" or equivalent shell startup file the following line:

    ```shell
    export PATH=$PATH":/path/to/toolchain/bin"
    ```
    
  * Open a new terminal. The toolchain should now be available to be called from the command line (e.g., the *reisc-gcc* command should be found if the user tries to run it).

Once the toolchain is installed, in order to build a project, the user must enter the *stred/make* directory inside the project and run *make*. Enjoy.

## 3 - Eclipse

In order to build a project using an Eclipse based IDE, the STRed-ISPU plugins must be installed into the IDE that the user desires to use.

In order to install the plugins, from the main menu, go to "Help", click on "Install New Software...", click on "Add…", insert the URL corresponding to the operating system used in the "Location" field and click the “Add" button. Then, select all the items, click on the "Next" button, accept the license terms and click on the "Finish" button.

The URL to use is:

* On Windows: [http://reisc-sdk.codex.cro.st.com/eclipse_update_site/win](http://reisc-sdk.codex.cro.st.com/eclipse_update_site/win)
* On Linux: [http://reisc-sdk.codex.cro.st.com/eclipse_update_site/linux](http://reisc-sdk.codex.cro.st.com/eclipse_update_site/linux)
* On MacOS: [http://reisc-sdk.codex.cro.st.com/eclipse_update_site/macos](http://reisc-sdk.codex.cro.st.com/eclipse_update_site/macos)

Once the plugins are installed, in order to build a project, the user must go to "File", click on "Import...". Under "General" select "Existing Project into Workspace", click on the "Next" button, click on the "Browse" button, select the folder containing the project and click on the "Finish" button. The project is now imported and can be built. Enjoy.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2022 STMicroelectronics**