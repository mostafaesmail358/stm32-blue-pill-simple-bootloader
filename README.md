This file is a simple bootloader for stm32 bluepill board. It is not proffesional or the most reliable, it is only explains
the basic idea of bootloader.

Bootloader is a code that make use of communication peripherals such as USB or UART in our case to flash an appliacation to the MCU.

Firstly we need to flash the bootloader using ST-LINK Utility or CUBEide. The next step is to compile the application we
need to upload and get the .bin file. Finally, run the python code to send the .bin file to the MCU.

After flashing the app normally if pin PA0 is held floating, the board will run the app after the reset. If you want to
upload a new app connect PA0 to 3.3v pin and reset now you can run the python code to upload the new app. please, make 
sure to disconnect the PA0 pin after that.

You need to determine the COM port of the USB to UART converter module and modify the python code. You can change the 
frame data size from the python code.
