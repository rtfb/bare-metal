# Makefile - build script */

# build environment
BUILDDIR := build
PREFIX ?= /usr
ARMGNU ?= $(PREFIX)/bin/arm-none-eabi

# source files
SOURCES_ASM := $(wildcard *.S)
SOURCES_C   := $(wildcard *.c)

# object files
OBJ_FILES   := $(patsubst %.S,%.o,$(SOURCES_ASM))
OBJ_FILES   += $(patsubst %.c,%.o,$(SOURCES_C))
OBJS        := $(addprefix $(BUILDDIR)/, $(OBJ_FILES))

# Build flags
DEPENDFLAGS := -MD -MP
INCLUDES    := -I include
BASEFLAGS   := -O2 -fpic -pedantic -pedantic-errors -nostdlib
BASEFLAGS   += -ffreestanding -fomit-frame-pointer -mcpu=arm1176jzf-s
WARNFLAGS   := -Wall -Wextra -Wshadow -Wcast-align -Wwrite-strings
WARNFLAGS   += -Wredundant-decls -Winline
WARNFLAGS   += -Wno-attributes -Wno-deprecated-declarations
WARNFLAGS   += -Wno-div-by-zero -Wno-endif-labels -Wfloat-equal
WARNFLAGS   += -Wformat=2 -Wno-format-extra-args -Winit-self
WARNFLAGS   += -Winvalid-pch -Wmissing-format-attribute
WARNFLAGS   += -Wmissing-include-dirs -Wno-multichar
WARNFLAGS   += -Wredundant-decls -Wshadow
WARNFLAGS   += -Wno-sign-compare -Wswitch -Wsystem-headers -Wundef
WARNFLAGS   += -Wno-pragmas -Wno-unused-but-set-parameter
WARNFLAGS   += -Wno-unused-but-set-variable -Wno-unused-result
WARNFLAGS   += -Wwrite-strings -Wdisabled-optimization -Wpointer-arith
WARNFLAGS   += -Werror
ASFLAGS     := $(INCLUDES) $(DEPENDFLAGS) -D__ASSEMBLY__
CFLAGS      := $(INCLUDES) $(DEPENDFLAGS) $(BASEFLAGS) $(WARNFLAGS)
CFLAGS      += -std=gnu99

# build rules
all: $(BUILDDIR)/kernel.img

include $(wildcard $(BUILDDIR)/*.d)

$(BUILDDIR)/kernel.elf: $(OBJS) link-arm-eabi.ld
	$(ARMGNU)-ld $(OBJS) -Tlink-arm-eabi.ld -o $@

$(BUILDDIR)/kernel.img: $(BUILDDIR)/kernel.elf
	$(ARMGNU)-objcopy $(BUILDDIR)/kernel.elf -O binary $(BUILDDIR)/kernel.img

clean:
	$(RM) -f $(OBJS) $(BUILDDIR)/kernel.elf $(BUILDDIR)/kernel.img

distclean: clean
	$(RM) -f $(BUILDDIR)/*.d

# C.
$(BUILDDIR)/%.o: %.c Makefile
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

# AS.
$(BUILDDIR)/%.o: %.S Makefile
	@mkdir -p $(BUILDDIR)
	$(ARMGNU)-gcc $(ASFLAGS) -c $< -o $@
