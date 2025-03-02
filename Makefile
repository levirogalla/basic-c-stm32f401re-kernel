TARGET = firmware
DIST_DIR = dist

# Toolchain
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# MCU flags
MCU = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16

# Compiler flags
# change -O0 to -O2 for release
CFLAGS = $(MCU) \
         -O0 -ggdb -Wall \
         -ffunction-sections -fdata-sections \
         -DUSE_HAL_DRIVER \
         -DSTM32F401xE \
         -ICore/Inc \
         -IDrivers/STM32F4xx_HAL_Driver/Inc \
         -IDrivers/CMSIS/Device/ST/STM32F4xx/Include \
         -IDrivers/CMSIS/Include

# Linker flags
LDFLAGS = $(MCU) \
          -TSTM32F401RETX_FLASH.ld \
          -Wl,--gc-sections \
          --specs=nano.specs \
          -u _printf_float

# Sources
C_SOURCES = $(wildcard Core/Src/*.c) \
            $(wildcard Drivers/STM32F4xx_HAL_Driver/Src/*.c)

ASM_SOURCES = $(wildcard Core/Startup/*.s) \
              $(wildcard Core/Src/*.s)

# Object files (with dist directory)
OBJS = $(patsubst %.c, $(DIST_DIR)/%.o, $(C_SOURCES)) \
       $(patsubst %.s, $(DIST_DIR)/%.o, $(ASM_SOURCES))

# Create dist directory structure
$(shell mkdir -p $(DIST_DIR)/Core/Src)
$(shell mkdir -p $(DIST_DIR)/Core/Startup)
$(shell mkdir -p $(DIST_DIR)/Drivers/STM32F4xx_HAL_Driver/Src)

.PHONY: all clean flash

all: $(DIST_DIR)/$(TARGET).bin $(DIST_DIR)/$(TARGET).hex

$(DIST_DIR)/$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@
	$(SIZE) $@

$(DIST_DIR)/%.bin: $(DIST_DIR)/%.elf
	$(OBJCOPY) -O binary $< $@

$(DIST_DIR)/%.hex: $(DIST_DIR)/%.elf
	$(OBJCOPY) -O ihex $< $@

$(DIST_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DIST_DIR)/%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(DIST_DIR)

flash: $(DIST_DIR)/$(TARGET).elf
	openocd -f openocd.cfg -c "program $< verify reset exit"