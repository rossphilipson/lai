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
CFLAGS += -DBTFE64_CHANGESET=\""$(shell ((hg parents --template "{isodate|isodate} {rev}:{node|short}" >/dev/null && hg parents --template "{isodate|isodate} {rev}:{node|short}") || echo "2016-05-18 12:00 -0800 1.9.4") 2>/dev/null)"\"


.PHONY: all
all: efitf.lds efitf.efi efitf.dsm

efitf.efi: $(OBJ)
	$(LD) efitf.lds $(LDFLAGS) $^ -o $@

# Grrr someone is frobbing the stack
tpm_20.o: tpm_20.c
	$(CC) $(CFLAGS) -mno-stack-arg-probe -o $@ -c $<

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.S
	$(CC) $(AFLAGS) -c $< -o $@

efitf.lds:
	$(PWD)/genlds.sh $(CC)

efitf.dsm:
	mingw-objdump -d efitf.efi > efitf.dsm

.PHONY: clean
clean:
	rm -f efitf.efi *.o efitf.dsm efitf.lds
