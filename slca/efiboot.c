/*
 * efiboot.c: EFI boot entry, early relocation and load code.
 *
 * Copyright (c) 2017 Assured Information Security.
 *
 * Ross Philipson <philipsonr@ainfosec.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the Intel Corporation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <config.h>
#include <efibase.h>
#include <string.h>
#include <stdbool.h>
#include <misc.h>
#include <page.h>
#include <printk.h>
#include <eficore.h>
#include <tb_error.h>
#include <tboot.h>

static EFI_HANDLE       parent_image_handle;
static EFI_HANDLE       parent_device_handle;
static EFI_DEVICE_PATH *device_path;
static void            *init_base; /* before reloc */
static uint64_t         init_size; /* original size */

#ifdef EFI_DEBUG
static void efi_debug_pause(void)
{
    EFI_STATUS    status;
    EFI_INPUT_KEY key;

    ST->ConIn->Reset(ST->ConIn, FALSE);
    while ((status = ST->ConIn->ReadKeyStroke(ST->ConIn, &key)) == EFI_NOT_READY);
}

static void efi_debug_print_i(void)
{
    printk("EFI init:\n");
    printk("  parent_image_handle  = %p\n", parent_image_handle);
    printk("  parent_device_handle = %p\n", parent_device_handle);
    printk("  device_path          = %p\n", device_path);
    printk("  init_base            = %p\n", init_base);
    printk("  init_size            = %x\n", (uint32_t)init_size);

    efi_debug_pause();
}

#define efi_debug_print_s(p, s) printk("%s %s\n", p, s)

#else
#define efi_debug_pause()
#define efi_debug_print_i()
#define efi_debug_print_s(p, s)
#endif

EFI_STATUS efi_main(EFI_HANDLE ImageHandle,
                    EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_STATUS        status;
    EFI_LOADED_IMAGE *loaded_image;

    /* Store the system table for future use in other functions */
    ST = SystemTable;
    BS = ST->BootServices;
    RT = ST->RuntimeServices;

    /* So we can use printk via EFI console protocol */
    printk_init(INIT_EARLY_EFI);
    printk("TBOOT EFI Entry Point: %p\n", efi_main);

    status = BS->HandleProtocol(ImageHandle,
                                &LoadedImageProtocol,
                                (VOID*)&loaded_image);

    if (!EFI_ERROR(status)) {
        /* Device we were loaded from, EFI partition */
        parent_device_handle = loaded_image->DeviceHandle;
        device_path = loaded_image->FilePath;
        init_base = loaded_image->ImageBase;
        init_size = loaded_image->ImageSize;
        parent_image_handle = ImageHandle;
    }
    else {
        printk("TBOOT FATAL! Cannot get loaded image information\n");
        ST->RuntimeServices->ResetSystem(EfiResetShutdown, status, 0, NULL);
    }

    efi_debug_print_i();

    /* TODO interesting stuff here */

    return status;
}
