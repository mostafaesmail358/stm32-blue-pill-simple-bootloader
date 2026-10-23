@echo off
if not exist build mkdir build

echo Compiling files...
arm-none-eabi-gcc -c  -mcpu=cortex-m3 -mthumb app.c -o build/app.o
arm-none-eabi-gcc -c  -mcpu=cortex-m3 -mthumb GPIO.c -o build/GPIO.o  


arm-none-eabi-gcc -c -mcpu=cortex-m3 -mthumb startup_stm32f103xx.c -o build/startup_stm32f103xx.o


echo Linking objects...
arm-none-eabi-ld -T LinkerScript.ld build/startup_stm32f103xx.o build/app.o build/GPIO.o  -o build/app.elf
arm-none-eabi-objcopy -O binary build/app.elf build/app.bin



echo Build Finished Successfully!
pause
