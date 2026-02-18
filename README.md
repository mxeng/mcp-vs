## MCP - Visual Studio Code version

This directory contains all that is needed to set up and get started with the visual studio code version of the microcontroller project.

To get started follow the directions below
 - Download a zip of the repository which will contain the code and libraries used during the project
 - Extract the zip to a location you will work from for the project
 - If Visual Studio **Code** is not installed on your device go to https://code.visualstudio.com/Download and install Visual Studio Code (note not Visual Studio)
 - Open Visual Studio Code and select the extensions tab on the left (ctrl + shift + x) and install all of these which are not already installed; PlatformIO IDE, C/C++, C/C++ extension Pack, Serial Monitor and CMake.
 - After extensions are installed restart Visual Studio Code
 - Select the PlatformIO tab on the left, select open project and select the mcp-vs folder which contains the platformio.ini file, it should end in "MCP_VS-main\MCP_VS-main\mcp-vs"
 - Inside explorer (ctrl + shift + r) select the platformio.ini file and change the upload port to the port assigned to your arduino
   - This can be found by running "mode" in the terminal
 - Add code to the Controller.c or Robot.c files then click build or upload under the controller or robot tasks in the platformIO tab

## platformio.ini Config
### USB Port
To configure the port that the robot uploads to or monitors, you will need to update the following lines:
- monitor_port
- upload_port

To find what these should be set as:

On windows you can run `mode` in the terminal, and change the "com" port to match the one listed; 

On Mac you can run `ls dev/cu.usb*` in the terminal to get a value that looks like "/dev/cu.usbserial-XXXX" replace the "comX" in the file with this value.
