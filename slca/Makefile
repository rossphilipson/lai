# Build with GCC compiler, note .rodata section in .lds file
#CC = gcc
# Build with MinGW compiler, note .rdata section in .lds file
CC = x86_64-w64-mingw32-gcc
LD = x86_64-w64-mingw32-ld
CFLAGS = -ffreestanding -fPIE -fno-exceptions -fshort-wchar -mno-ms-bitfields # Too much: -DTPM_TRACE
CFLAGS += -I/usr/local/include/efi -I/usr/local/include/efi/x86_64 -I/usr/local/include/efi/protocol -Iinclude
AFLAGS = -D__ASSEMBLY__ $(patsubst -std=gnu%,,$(CFLAGS))
LDFLAGS = -nostdlib -pie --no-seh --subsystem=10 -e efi_main
ASM = bits.S
SRC = $(wildcard *.c)
OBJ = $(ASM:.S=.o) # must be first
OBJ += $(SRC:.c=.o)

# Banner
CFLAGS += -DBTX64_CHANGESET=\""$(shell (echo "Boot Test Framework EFI" && uname -m && date) 2>/dev/null)"\"

.PHONY: all
all: btx64.lds btx64.efi btx64.dsm

btx64.efi: $(OBJ)
	$(LD) btx64.lds $(LDFLAGS) $^ -o $@

# Grrr someone is frobbing the stack
tpm_20.o: tpm_20.c
	$(CC) $(CFLAGS) -mno-stack-arg-probe -o $@ -c $<

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.S
	$(CC) $(AFLAGS) -c $< -o $@

btx64.lds:
	$(PWD)/genlds.sh $(CC)

btx64.dsm:
	mingw-objdump -d btx64.efi > btx64.dsm

.PHONY: clean
clean:
	rm -f btx64.efi *.o btx64.dsm btx64.lds
