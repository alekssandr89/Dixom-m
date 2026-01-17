################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Dixom/Module/AudioDAC/AudioDAC.c 

OBJS += \
./Dixom/Module/AudioDAC/AudioDAC.o 

C_DEPS += \
./Dixom/Module/AudioDAC/AudioDAC.d 


# Each subdirectory must supply rules for building sources it contributes
Dixom/Module/AudioDAC/%.o Dixom/Module/AudioDAC/%.su Dixom/Module/AudioDAC/%.cyclo: ../Dixom/Module/AudioDAC/%.c Dixom/Module/AudioDAC/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F405xx -c -I../Core/Inc -I../Dixom -I../Dixom/Platform/Inc -I../Dixom/Platform/Update -I../Dixom/Exchange/Inc -I../Dixom/Calculations/Inc -I../Dixom/Settings -I../Dixom/Pereferiya/STM32 -I../Dixom/Pereferiya/RTC_3231/Inc -I../Dixom/Pereferiya/Arduino/Inc -I../Dixom/Pereferiya/DeviceHID -I../Dixom/Module/UsbAudio -I../Dixom/Module/FmRadio -I../Dixom/Module/FmRadio/SI4741 -I../Dixom/Module/FmRadio/RDA5807FP -I../Dixom/Module/DSP -I../Dixom/Module/DSP/SigmaStudio/Inc -I../Dixom/Module/Display -I../Dixom/Module/Display/DisplayText/Inc -I../Dixom/Module/Display/DisplayOled/Inc -I../Dixom/Module/Display/DisplayNextion -I../Dixom/Module/Bluetooth -I../Dixom/Module/AudioDAC -I../Dixom/Menu/Inc -I../Dixom/Control -I../Dixom/Control/AnalogControl -I../Dixom/Module/DSP/SigmaStudio/Inc_ADAU1452 -I../Dixom/Can -I../Dixom/Pereferiya/GPIO/Inc -I../Usb/COMPOSITE/Class/COMPOSITE/Inc -I../Usb/COMPOSITE/Class/HID_KEYBOARD/Inc -I../Usb/COMPOSITE/Class/HID_CUSTOM/Inc -I../Usb/COMPOSITE/App -I../Usb/COMPOSITE/Core/Inc -I../Usb/COMPOSITE/Target -I../Usb/COMPOSITE/Class/CDC_ACM/Inc -I../Usb -I../Dixom/Module/AudioDAC/AMP_CrystalSix140D -I../Dixom/Module/AudioDAC/DAC_PCM1690 -I../Dixom/Module/AudioDAC/DAC_ADAU1962x -I../Dixom/Module/AudioDAC/DAC_AK4440 -I../Dixom/Module/AudioDAC/DAC_AK4458VN -I../Dixom/Module/AudioDAC/AMP_TAS3251 -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.1/Middlewares/Third_Party/FreeRTOS/Source/include -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.1/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.1/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.1/Drivers/CMSIS/Include -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Middlewares/Third_Party/FreeRTOS/Source/include -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/alekk/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Dixom-2f-Module-2f-AudioDAC

clean-Dixom-2f-Module-2f-AudioDAC:
	-$(RM) ./Dixom/Module/AudioDAC/AudioDAC.cyclo ./Dixom/Module/AudioDAC/AudioDAC.d ./Dixom/Module/AudioDAC/AudioDAC.o ./Dixom/Module/AudioDAC/AudioDAC.su

.PHONY: clean-Dixom-2f-Module-2f-AudioDAC

