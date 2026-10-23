@echo off
if not exist build mkdir build

echo Compiling files...
arm-none-eabi-gcc -c -I./drivers -mcpu=cortex-m3 -mthumb Bootloader.c -o build/Bootloader.o
arm-none-eabi-gcc -c -I./drivers -mcpu=cortex-m3 -mthumb drivers/GPIO.c -o build/GPIO.o  
arm-none-eabi-gcc -c -I./drivers -mcpu=cortex-m3 -mthumb drivers/UART.c -o build/UART.o
arm-none-eabi-gcc -c -I./drivers -mcpu=cortex-m3 -mthumb drivers/flash_driver.c -o build/flash_driver.o
arm-none-eabi-gcc -c -I./drivers -mcpu=cortex-m3 -mthumb drivers/CRC_driver.c -o build/CRC_driver.o



arm-none-eabi-gcc -c -mcpu=cortex-m3 -mthumb startup_stm32f103xx.c -o build/startup_stm32f103xx.o


echo Linking objects...
arm-none-eabi-ld -T LinkerScript.ld build/startup_stm32f103xx.o build/Bootloader.o build/UART.o build/GPIO.o build/flash_driver.o build/CRC_driver.o  -o build/Bootloader.elf
arm-none-eabi-objcopy -O binary build/Bootloader.elf build/Bootloader.bin



echo Build Finished Successfully!
pause
