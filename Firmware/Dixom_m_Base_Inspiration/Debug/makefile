################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Usb/COMPOSITE/Target/subdir.mk
-include Usb/COMPOSITE/Core/Src/subdir.mk
-include Usb/COMPOSITE/Class/HID_KEYBOARD/Src/subdir.mk
-include Usb/COMPOSITE/Class/HID_CUSTOM/Src/subdir.mk
-include Usb/COMPOSITE/Class/COMPOSITE/Src/subdir.mk
-include Usb/COMPOSITE/Class/CDC_ACM/Src/subdir.mk
-include Usb/COMPOSITE/App/subdir.mk
-include Middlewares/FreeRTOS/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/subdir.mk
-include Dixom/Settings/subdir.mk
-include Dixom/Platform/Update/subdir.mk
-include Dixom/Platform/subdir.mk
-include Dixom/Pereferiya/RTC_3231/Src/subdir.mk
-include Dixom/Pereferiya/GPIO/Src/subdir.mk
-include Dixom/Pereferiya/DeviceHID/subdir.mk
-include Dixom/Pereferiya/Arduino/Src/subdir.mk
-include Dixom/Module/UsbAudio/subdir.mk
-include Dixom/Module/FmRadio/SI4741/subdir.mk
-include Dixom/Module/FmRadio/RDA5807FP/subdir.mk
-include Dixom/Module/FmRadio/subdir.mk
-include Dixom/Module/Display/DisplayText/Src/subdir.mk
-include Dixom/Module/Display/DisplayOled/Src/subdir.mk
-include Dixom/Module/Display/DisplayNextion/subdir.mk
-include Dixom/Module/Display/subdir.mk
-include Dixom/Module/DSP/SigmaStudio/Src/subdir.mk
-include Dixom/Module/DSP/subdir.mk
-include Dixom/Module/Bluetooth/subdir.mk
-include Dixom/Module/AudioDAC/DAC_PCM1690/subdir.mk
-include Dixom/Module/AudioDAC/DAC_AK4458VN/subdir.mk
-include Dixom/Module/AudioDAC/DAC_AK4440/subdir.mk
-include Dixom/Module/AudioDAC/DAC_ADAU1962x/subdir.mk
-include Dixom/Module/AudioDAC/AMP_TAS3251/subdir.mk
-include Dixom/Module/AudioDAC/AMP_CrystalSix140D/subdir.mk
-include Dixom/Module/AudioDAC/subdir.mk
-include Dixom/Menu/Src/subdir.mk
-include Dixom/Exchange/Src/subdir.mk
-include Dixom/Control/AnalogControl/subdir.mk
-include Dixom/Control/subdir.mk
-include Dixom/Can/subdir.mk
-include Dixom/Calculations/Src/subdir.mk
-include Dixom/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Dixom_m_Base_Inspiration
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Dixom_m_Base_Inspiration.elf \

MAP_FILES += \
Dixom_m_Base_Inspiration.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Dixom_m_Base_Inspiration.list \

OBJCOPY_BIN += \
Dixom_m_Base_Inspiration.bin \


# All Target
all: main-build

# Main-build Target
main-build: Dixom_m_Base_Inspiration.elf secondary-outputs

# Tool invocations
Dixom_m_Base_Inspiration.elf Dixom_m_Base_Inspiration.map: $(OBJS) $(USER_OBJS) C:\dixom_m\Firmware\Dixom_m_Base_Inspiration\STM32F405RGTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Dixom_m_Base_Inspiration.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\dixom_m\Firmware\Dixom_m_Base_Inspiration\STM32F405RGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Dixom_m_Base_Inspiration.map" -Wl,--gc-sections -static -u _printf_float -u _scanf_float  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Dixom_m_Base_Inspiration.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Dixom_m_Base_Inspiration.list"
	@echo 'Finished building: $@'
	@echo ' '

Dixom_m_Base_Inspiration.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Dixom_m_Base_Inspiration.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Dixom_m_Base_Inspiration.bin Dixom_m_Base_Inspiration.elf Dixom_m_Base_Inspiration.list Dixom_m_Base_Inspiration.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
