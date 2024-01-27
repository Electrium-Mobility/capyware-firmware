# _Sample project_
### Hardware 
We are going to develop on the ESP32-S3. This chip provides all the functionality we primary need, and is honestly
probably a bit overkill. It comes with BLE, ADC, and GPIO pins.

### Firmware Overview
We're going to use the ESP-IDF (Espressif IoT Development Framework) for our main development platform.

### Setting up Dev Environment

1. Follow this install link [here](https://github.com/espressif/vscode-esp-idf-extension/blob/master/docs/tutorial/install.md). If you're on Mac/Linux, you will have prerequisite steps.
2. Make sure to do the optional step and Save Configuration Settings. Make sure to use the GitHub download server: ![Alt text](image.png).
   

### Build Instructions

This is the simplest buildable example. The example is used by command `idf.py create-project`
that copies the project to user specified path and set it's name. For more information follow the [docs page](https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html#start-a-new-project)


## How to use example
We encourage the users to use the example as a template for the new projects.
A recommended way is to follow the instructions on a [docs page](https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html#start-a-new-project).

## Example folder contents

The project **sample_project** contains one source file in C language [main.c](main/main.c). The file is located in folder [main](main).

ESP-IDF projects are built using CMake. The project build configuration is contained in `CMakeLists.txt`
files that provide set of directives and instructions describing the project's source files and targets
(executable, library, or both). 

Below is short explanation of remaining files in the project folder.

```
├── CMakeLists.txt
├── main
│   ├── CMakeLists.txt
│   └── main.c
└── README.md                  This is the file you are currently reading
```
Additionally, the sample project contains Makefile and component.mk files, used for the legacy Make based build system. 
They are not used or needed when building with CMake and idf.py.
e.png).
   