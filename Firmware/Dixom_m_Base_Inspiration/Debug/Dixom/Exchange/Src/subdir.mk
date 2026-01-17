################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Dixom/Exchange/Src/DIX_can.c \
../Dixom/Exchange/Src/DIX_i2c.c \
../Dixom/Exchange/Src/DIX_spi.c \
../Dixom/Exchange/Src/DIX_uart.c \
../Dixom/Exchange/Src/DIX_usb.c \
../Dixom/Exchange/Src/Exchange.c \
../Dixom/Exchange/Src/Memory_Eeprom.c \
../Dixom/Exchange/Src/Memory_Spi.c \
../Dixom/Exchange/Src/Memory_Stm.c 

OBJS += \
./Dixom/Exchange/Src/DIX_can.o \
./Dixom/Exchange/Src/DIX_i2c.o \
./Dixom/Exchange/Src/DIX_spi.o \
./Dixom/Exchange/Src/DIX_uart.o \
./Dixom/Exchange/Src/DIX_usb.o \
./Dixom/Exchange/Src/Exchange.o \
./Dixom/Exchange/Src/Memory_Eeprom.o \
./Dixom/Exchange/Src/Memory_Spi.o \
./Dixom/Exchange/Src/Memory_Stm.o 

C_DEPS += \
./Dixom/Exchange/Src/DIX_can.d \
./Dixom/Exchange/Src/DIX_i2c.d \
./Dixom/Exchange/Src/DIX_spi.d \
./Dixom/Exchange/Src/DIX_uart.d \
./Dixom/Exchange/Src/DIX_usb.d \
./Dixom/Exchange/Src/Exchange.d \
./Dixom/Exchange/Src/Memory_Eeprom.d \
./Dixom/Exchange/Src/Memory_Spi.d \
./Dixom/Exchange/Src/Memory_Stm.d 


# Each subdirectory must supply rules for building sources it contributes
Dixom/Exchange/Src/%.o Dixom/Exchange/Src/%.su Dixom/Exchange/Src/%.cyclo: ../Dixom/Exchange/Src/%.c Dixom/Exchange/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F405xx -c -I../Core/Inc -I../Dixom -I../Dixom/Platform/Inc -I../Dixom/Platform/Update -I../Dixom/Exchange/Inc -I../Dixom/Calculations/Inc -I../Dixom/Settings -I../Dixom/Pereferiya/STM32 -I../Dixom/Pereferiya/RTC_3231/Inc -I../Dixom/Pereferiya/Arduino/Inc -I../Dixom/Pereferiya/DeviceHID -I../Dixom/Module/UsbAudio -I../Dixom/Module/FmRadio -I../Dixom/Module/FmRadio/SI4741 -I../Dixom/Module/FmRadio/RDA5807FP -I../Dixom/Module/DSP -I../Dixom/Module/DSP/SigmaStudio/Inc -I../Dixom/Module/Display -I../Dixom/Module/Display/DisplayText/Inc -I../Dixom/Module/Display/DisplayOled/Inc -I../Dixom/Module/Display/DisplayNextion -I../Dixom/Module/Bluetooth -I../Dixom/Module/AudioDAC -I../Dixom/Menu/Inc -I../Dixom/Control -I../Dixom/Control/AnalogControl -I../Dixom/Module/DSP/SigmaStudio/Inc_ADAU1452 -I../Dixom/Can -I../Dixom/Pereferiya/GPIO/Inc -I../Usb/COMPOSITE/Class/COMPOSITE/Inc -I../Usb/COMPOSITE/Class/HID_KEYBOARD/Inc -I../Usb/COMPOSITE/Class/HID_CUSTOM/Inc -I../Usb/COMPOSITE/App -I../Usb/COMPOSITE/Core/Inc -I../Usb/COMPOSITE/Target -I../Usb/COMPOSITE/Class/CDC_ACM/Inc -I../Usb -I../Dixom/Module/AudioDAC/AMP_CrystalSix140D -I../Dixom/Module/AudioDAC/DAC_PCM1690 -I../Dixom/Module/AudioDAC/DAC_ADAU1962x -I../Dixom/Module/AudioDAC/DAC_AK4440 -I../Dixom/Module/AudioDAC/DAC_AK4458VN -I../Dixom/Module/AudioDAC/AMP_TAS3251 -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.1/Middlewares/Third_Party/FreeRTOS/Source/include -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.1/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.1/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.1/Drivers/CMSIS/Include -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Middlewares/Third_Party/FreeRTOS/Source/include -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Dixom-2f-Exchange-2f-Src

clean-Dixom-2f-Exchange-2f-Src:
	-$(RM) ./Dixom/Exchange/Src/DIX_can.cyclo ./Dixom/Exchange/Src/DIX_can.d ./Dixom/Exchange/Src/DIX_can.o ./Dixom/Exchange/Src/DIX_can.su ./Dixom/Exchange/Src/DIX_i2c.cyclo ./Dixom/Exchange/Src/DIX_i2c.d ./Dixom/Exchange/Src/DIX_i2c.o ./Dixom/Exchange/Src/DIX_i2c.su ./Dixom/Exchange/Src/DIX_spi.cyclo ./Dixom/Exchange/Src/DIX_spi.d ./Dixom/Exchange/Src/DIX_spi.o ./Dixom/Exchange/Src/DIX_spi.su ./Dixom/Exchange/Src/DIX_uart.cyclo ./Dixom/Exchange/Src/DIX_uart.d ./Dixom/Exchange/Src/DIX_uart.o ./Dixom/Exchange/Src/DIX_uart.su ./Dixom/Exchange/Src/DIX_usb.cyclo ./Dixom/Exchange/Src/DIX_usb.d ./Dixom/Exchange/Src/DIX_usb.o ./Dixom/Exchange/Src/DIX_usb.su ./Dixom/Exchange/Src/Exchange.cyclo ./Dixom/Exchange/Src/Exchange.d ./Dixom/Exchange/Src/Exchange.o ./Dixom/Exchange/Src/Exchange.su ./Dixom/Exchange/Src/Memory_Eeprom.cyclo ./Dixom/Exchange/Src/Memory_Eeprom.d ./Dixom/Exchange/Src/Memory_Eeprom.o ./Dixom/Exchange/Src/Memory_Eeprom.su ./Dixom/Exchange/Src/Memory_Spi.cyclo ./Dixom/Exchange/Src/Memory_Spi.d ./Dixom/Exchange/Src/Memory_Spi.o ./Dixom/Exchange/Src/Memory_Spi.su ./Dixom/Exchange/Src/Memory_Stm.cyclo ./Dixom/Exchange/Src/Memory_Stm.d ./Dixom/Exchange/Src/Memory_Stm.o ./Dixom/Exchange/Src/Memory_Stm.su

.PHONY: clean-Dixom-2f-Exchange-2f-Src

