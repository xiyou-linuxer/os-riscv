PLATFORM := qemu
V := @
TARGET := target
SRC := src
FW := ../bootloader
QEMU := qemu-system-riscv64
GCCPREFIX := riscv64-unknown-elf-

CC := $(GCCPREFIX)gcc
LD := $(GCCPREFIX)ld

CFLAGS := -mcmodel=medany -std=gnu99 -Wno-unused -Werror
CFLAGS += -fno-builtin -nostdinc
CFLAGS += -nostdlib -g
CFLAGS += -I./include

LDFLAGS	:= -nostdlib

OBJCOPY := $(GCCPREFIX)objcopy
OBJDUMP := $(GCCPREFIX)objdump
GDB := $(GCCPREFIX)gdb

KERNEL := $(TARGET)/kernel
BIN := $(TARGET)/kernel.bin

# sbi
OPENSBI_ELF := $(FW)/fw_jump.elf
OPENSBI_BIN := $(FW)/fw_jump.bin

RUSTSBI_ELF := $(FW)/rustsbi-qemu
RUSTSBI_BIN := $(FW)/rustsbi-qemu.bin

# platform
ifeq ($(platform), k210)
OBJS += $(SRC)/entry_k210.o
else
OBJS += $(SRC)/entry.o
endif

# objs
OBJS += \
  $(SRC)/main.o \
  $(SRC)/sbi.o \
  $(SRC)/console.o \
  $(SRC)/stdio.o \
  $(SRC)/string.o \
  $(SRC)/timer.o \
  $(SRC)/exception.o \
  $(SRC)/exception_entry.o \
  $(SRC)/panic.o \
  $(SRC)/syscall.o

# linker
LINKER := $(SRC)/linker.ld

kernel: $(OBJS) $(LINKER)
	if [ ! -d "./target" ]; then mkdir target; fi
	@$(LD) $(LDFLAGS) -T $(LINKER) -o $(TARGET)/kernel $(OBJS)

$(BIN): kernel
	$(OBJCOPY) $(KERNEL) --strip-all -O binary $@

qemu: $(BIN)
	$(V)$(QEMU) \
		-machine virt \
		-nographic \
		-bios $(RUSTSBI_BIN) \
		-device loader,file=$(BIN),addr=0x80200000

debug: $(BIN)
	@tmux new-session -d \
		"$(QEMU) -machine virt -nographic -bios $(OPENSBI_ELF) -device loader,file=$(BIN),addr=0x80200000 \
		-s -S" && \
		tmux split-window -h "$(GDB) -ex 'file $(KERNEL)' -ex 'set arch riscv:rv64' -ex 'target remote localhost:1234'" && \
		tmux -2 attach-session -d


clean:
	-rm -i $(SRC)/*.o \
	$(TARGET)/*
