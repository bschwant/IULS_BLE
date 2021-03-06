##########################################################################################################################
# File automatically-generated by STM32forVSCode
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# target
######################################
TARGET = IULS_BLE_DEV


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
Core/Src/app_debug.c \
Core/Src/app_entry.c \
Core/Src/command.c \
Core/Src/hw_timerserver.c \
Core/Src/hw_uart.c \
Core/Src/interrupt.c \
Core/Src/main.c \
Core/Src/queue.c \
Core/Src/retarget.c \
Core/Src/stm32_lpm_if.c \
Core/Src/stm32wbxx_hal_msp.c \
Core/Src/stm32wbxx_it.c \
Core/Src/system_stm32wbxx.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_cortex.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_dma.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_dma_ex.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_exti.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_flash.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_flash_ex.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_gpio.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_hsem.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_ipcc.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_pwr.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_pwr_ex.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_rcc.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_rcc_ex.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_rng.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_rtc.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_rtc_ex.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_tim.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_tim_ex.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_uart.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_uart_ex.c \
Middlewares/ST/STM32_WPAN/ble/core/auto/ble_gap_aci.c \
Middlewares/ST/STM32_WPAN/ble/core/auto/ble_gatt_aci.c \
Middlewares/ST/STM32_WPAN/ble/core/auto/ble_hal_aci.c \
Middlewares/ST/STM32_WPAN/ble/core/auto/ble_hci_le.c \
Middlewares/ST/STM32_WPAN/ble/core/auto/ble_l2cap_aci.c \
Middlewares/ST/STM32_WPAN/ble/core/template/osal.c \
Middlewares/ST/STM32_WPAN/ble/svc/Src/svc_ctl.c \
Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci/shci.c \
Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/hci_tl.c \
Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/hci_tl_if.c \
Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/shci_tl.c \
Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/shci_tl_if.c \
Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/tl_mbox.c \
Middlewares/ST/STM32_WPAN/utilities/dbg_trace.c \
Middlewares/ST/STM32_WPAN/utilities/otp.c \
Middlewares/ST/STM32_WPAN/utilities/stm_list.c \
Middlewares/ST/STM32_WPAN/utilities/stm_queue.c \
STM32_WPAN/App/app_ble.c \
STM32_WPAN/App/custom_app.c \
STM32_WPAN/App/custom_stm.c \
STM32_WPAN/Target/hw_ipcc.c \
Utilities/lpm/tiny_lpm/stm32_lpm.c \
Utilities/sequencer/stm32_seq.c


CPP_SOURCES = \


# ASM sources
ASM_SOURCES =  \
startup_stm32wb55xx_cm4.s



#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
POSTFIX = "
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
GCC_PATH="/usr/local/bin
ifdef GCC_PATH
CXX = $(GCC_PATH)/$(PREFIX)g++$(POSTFIX)
CC = $(GCC_PATH)/$(PREFIX)gcc$(POSTFIX)
AS = $(GCC_PATH)/$(PREFIX)gcc$(POSTFIX) -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy$(POSTFIX)
SZ = $(GCC_PATH)/$(PREFIX)size$(POSTFIX)
else
CXX = $(PREFIX)g++
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DSTM32WB55xx \
-DUSE_HAL_DRIVER


# CXX defines
CXX_DEFS =  \
-DSTM32WB55xx \
-DUSE_HAL_DRIVER


# AS includes
AS_INCLUDES = \

# C includes
C_INCLUDES =  \
-ICore/Inc \
-IDrivers/CMSIS/Device/ST/STM32WBxx/Include \
-IDrivers/CMSIS/Include \
-IDrivers/STM32WBxx_HAL_Driver/Inc \
-IDrivers/STM32WBxx_HAL_Driver/Inc/Legacy \
-IMiddlewares/ST/STM32_WPAN \
-IMiddlewares/ST/STM32_WPAN/ble \
-IMiddlewares/ST/STM32_WPAN/ble/core \
-IMiddlewares/ST/STM32_WPAN/ble/core/auto \
-IMiddlewares/ST/STM32_WPAN/ble/core/template \
-IMiddlewares/ST/STM32_WPAN/ble/svc/Inc \
-IMiddlewares/ST/STM32_WPAN/ble/svc/Src \
-IMiddlewares/ST/STM32_WPAN/interface/patterns/ble_thread \
-IMiddlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci \
-IMiddlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl \
-IMiddlewares/ST/STM32_WPAN/utilities \
-ISTM32_WPAN/App \
-IUtilities/lpm/tiny_lpm \
-IUtilities/sequencer



# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CXXFLAGS = $(MCU) $(CXX_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections -feliminate-unused-debug-types

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Add additional flags
CFLAGS += 
ASFLAGS += 
CXXFLAGS += 

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"
CXXFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = stm32wb55xx_flash_cm4.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = \


# Additional LD Flags from config file
ADDITIONALLDFLAGS = -specs=nano.specs 

LDFLAGS = $(MCU) $(ADDITIONALLDFLAGS) -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of cpp program objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))

# list of C objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.cpp STM32Make.make | $(BUILD_DIR) 
	$(CXX) -c $(CXXFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.cxx STM32Make.make | $(BUILD_DIR) 
	$(CXX) -c $(CXXFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cxx=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.c STM32Make.make | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s STM32Make.make | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) STM32Make.make
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

#######################################
# flash
#######################################
flash: $(BUILD_DIR)/$(TARGET).elf
	"/usr/local/bin/openocd" -f ./openocd.cfg -c "program $(BUILD_DIR)/$(TARGET).elf verify reset exit"

#######################################
# erase
#######################################
erase: $(BUILD_DIR)/$(TARGET).elf
	"/usr/local/bin/openocd" -f ./openocd.cfg -c "init; reset halt; stm32wbx mass_erase 0; exit"

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
	
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***