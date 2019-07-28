Welcome\! This repository contains the source code for:

  - NodeMCU Software
  - ESP8266 RC
  - Android APP

# NodeMCU Software Overview

Please take a few minutes to review the overview below before diving into the code:

## NodeMCU Software

NodeMCU software is a simple remote control application for the ESP8266 module. It supports both Connect by IP and Scan Nearby and uses Socket for remote communication.

Be aware: The application has not been tested for memory footage, security vulnerbility, and other runtime exceptions.

## Arduino code

The remote_control.ino file contain source code for the ESP8266 RC car.

## Android App

The app is provided as a reference for how similar ideas can be applied for Android app development.

# Getting Started

## Prerequisites

* You must be running Windows 1903 (build >= 10.0.18362.0) or above in order to run NodeMCU Software
  - **As of May 2019** this build is only available through Windows Insider Program. You may register and configure Insider Program through your device's system settings.
* You will need at least [VS 2017](https://visualstudio.microsoft.com/downloads/) installed
* You will need Arduino IDE for compilation and uploading Arduino code
* You will need Android Studio for the Android app

> 👉 If you find something missing from these files, feel free to contribute to any of my files anywhere in the repository (or make some new ones\!)

## Open Source License

Copyright <2019> <ShangJin Li>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
