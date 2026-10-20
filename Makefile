##############################################################################
# STM32F446RE bare-metal CMSIS project
#
#   make                 build build/<TARGET>.elf/.hex/.bin (Debug, -Og -g3)
#   make DEBUG=0         optimized build (-O2)
#   make clean
#   make flash           program via STM32_Programmer_CLI (STM32CubeProgrammer)
#   make flash-openocd   program via OpenOCD + ST-LINK
#   make size / disasm
##############################################################################

TARGET    := stm32f446re_can
BUILD_DIR := build
DEBUG     ?= 1

# ---------------------------------------------------------------- toolchain
PREFIX  ?= arm-none-eabi-
CC      := $(PREFIX)gcc
AS      := $(PREFIX)gcc -x assembler-with-cpp
OBJCOPY := $(PREFIX)objcopy
OBJDUMP := $(PREFIX)objdump
SIZE    := $(PREFIX)size

# ---------------------------------------------------------------- sources
C_SOURCES := $(wildcard Core/Src/*.c)
ASM_SOURCES := Startup/startup_stm32f446xx.s
LDSCRIPT  := STM32F446RETx_FLASH.ld

C_INCLUDES := \
  -ICore/Inc \
  -IDrivers/CMSIS/Include \
  -IDrivers/CMSIS/Device/ST/STM32F4xx/Include

C_DEFS := \
  -DSTM32F446xx \
  -DHSE_VALUE=8000000U

# ---------------------------------------------------------------- flags
MCU := -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard

ifeq ($(DEBUG),1)
  OPT := -Og -g3 -DDEBUG
else
  OPT := -O2 -g
endif

CFLAGS  := $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -std=gnu11 \
           -Wall -Wextra -Wshadow -Wno-unused-parameter \
           -ffunction-sections -fdata-sections -fno-common \
           -MMD -MP
ASFLAGS := $(MCU) $(OPT) -Wall
LDFLAGS := $(MCU) -T$(LDSCRIPT) --specs=nano.specs \
           -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref \
           -Wl,--gc-sections -Wl,--print-memory-usage \
           -Wl,--no-warn-rwx-segments \
           -Wl,--start-group -lc -lm -Wl,--end-group

# ---------------------------------------------------------------- shell portability
# Works from cmd.exe/PowerShell (Windows make without sh) and from POSIX shells.
ifeq ($(shell echo "x"),x)
  MKDIR = mkdir -p $(1)
  RMDIR = rm -rf $(1)
else
  MKDIR = if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
  RMDIR = if exist "$(subst /,\,$(1))" rmdir /s /q "$(subst /,\,$(1))"
endif

# ---------------------------------------------------------------- objects
OBJ_DIR := $(BUILD_DIR)/obj
OBJECTS := $(addprefix $(OBJ_DIR)/,$(notdir $(C_SOURCES:.c=.o))) \
           $(addprefix $(OBJ_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

ELF := $(BUILD_DIR)/$(TARGET).elf
HEX := $(BUILD_DIR)/$(TARGET).hex
BIN := $(BUILD_DIR)/$(TARGET).bin

# ---------------------------------------------------------------- rules
.PHONY: all clean size disasm flash flash-openocd

all: $(ELF) $(HEX) $(BIN)
	@$(SIZE) $(ELF)

$(OBJ_DIR)/%.o: %.c Makefile | $(OBJ_DIR)
	@echo CC  $<
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(OBJ_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(OBJ_DIR)/%.o: %.s Makefile | $(OBJ_DIR)
	@echo AS  $<
	@$(AS) -c $(ASFLAGS) $< -o $@

$(ELF): $(OBJECTS) $(LDSCRIPT) Makefile
	@echo LD  $@
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(HEX): $(ELF)
	@$(OBJCOPY) -O ihex $< $@

$(BIN): $(ELF)
	@$(OBJCOPY) -O binary -S $< $@

$(OBJ_DIR):
	@$(call MKDIR,$@)

size: $(ELF)
	@$(SIZE) -A -x $(ELF)

disasm: $(ELF)
	@$(OBJDUMP) -d -S $(ELF) > $(BUILD_DIR)/$(TARGET).dis
	@echo wrote $(BUILD_DIR)/$(TARGET).dis

flash: $(ELF)
	STM32_Programmer_CLI -c port=SWD mode=UR -w $(ELF) -v -rst

flash-openocd: $(ELF)
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program $(ELF) verify reset exit"

clean:
	@$(call RMDIR,$(BUILD_DIR))

-include $(wildcard $(OBJ_DIR)/*.d)
