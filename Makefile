#--------------------------------------Makefile-------------------------------------

# Source directory
SRC_DIR := src
# Object files
BLD_DIR := object
# Header directory
INC_DIR := include
# Linker directory
SCT_DIR := linker

# Source files
CFILES := $(wildcard $(SRC_DIR)/*.c)
# Object files
OFILES := $(CFILES:$(SRC_DIR)/%.c=$(BLD_DIR)/%.o)
# Header files
HFILES := $(wildcard $(INC_DIR)/*.h)
# Linker files
LFILES := $(wildcard $(SCT_DIR)/*.ld)
# Target file
TARGET := kernel8

# Compilers
CC = aarch64-none-elf-gcc

# Flags
GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
LDFLAGS = -nostdlib

OPT = -serial stdio

all: $(TARGET).img

$(TARGET).img: $(BLD_DIR)/boot.o $(OFILES)
	@echo -------------------------------------
	aarch64-none-elf-ld $(LDFLAGS) $(BLD_DIR)/boot.o $(OFILES) -T $(LFILES) -o $(BLD_DIR)/$(TARGET).elf
	aarch64-none-elf-objcopy -O binary $(BLD_DIR)/$(TARGET).elf $(TARGET).img
	@echo -------------------------------------

# ---------------------------------------------------------------------------------
$(BLD_DIR)/boot.o: $(SRC_DIR)/boot.S | $(BLD_DIR)
	$(CC) $(GCCFLAGS) -c $< -o $@

$(BLD_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR) | $(BLD_DIR)
	$(CC) $(GCCFLAGS) -c $< -o $@ -I$(INC_DIR)
# ---------------------------------------------------------------------------------

$(BLD_DIR):
	if not exist "$(BLD_DIR)" mkdir $(BLD_DIR)

clean:
	@echo -------------------------------------
	@echo -- Deleting .o files:
	rmdir /s /q $(BLD_DIR)
	@echo -- Deleting .img file:
	del *.img
	@echo -------------------------------------

diff:
	@git status
	@git diff --stat
	
run:
	qemu-system-aarch64 -M raspi3 -kernel $(TARGET).img $(OPT)

.PHONY: all clean diff