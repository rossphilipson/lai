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

#include <efibase.h>
#include <string.h>
#include <stdbool.h>
#include <compiler.h>
#include <misc.h>
#include <page.h>
#include <cmdline.h>
#include <printk.h>
#include <eficore.h>
#include <eficonfig.h>
#include <btfe64.h>

static EFI_HANDLE       parent_image_handle;
static EFI_HANDLE       parent_device_handle;
static EFI_DEVICE_PATH *device_path;
static void            *init_base; /* before reloc */
static uint64_t         init_size; /* original size */

static EFI_FILE_IO_INTERFACE *efi_file_system = NULL;

static uint8_t btfe64_config_file[EFI_MAX_CONFIG_FILE];

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

static void efi_debug_print_w(const char *pfx, const wchar_t *wstr)
{
    char *p = wtoa_alloc(wstr);
    printk("%s %s\n", pfx, p);
    BS->FreePool(p);
}

#define efi_debug_print_s(p, s) printk("%s %s\n", p, s)

static void efi_form_config_path(wchar_t *path)
{
    wchar_t *ptr = path + wcslen(path);

    efi_debug_print_w("IMAGE PATH:", path);

    /* Form the config file path */
    while (ptr >= path) {
        if (*ptr == L'.') {
            memcpy((ptr + 1), L"cfg\0", 8);
            break;
        }
        ptr--;
    }

    efi_debug_print_w("CONFIG PATH:", path);
}

static EFI_STATUS efi_load_config(void)
{
    EFI_STATUS            status;
    wchar_t              *file_path = NULL;
    EFI_PHYSICAL_ADDRESS  addr = BTX64_MAX_IMAGE_MEM;
    void                 *buffer = NULL;
    uint64_t              size;
    const char           *cmdline;
    efi_file_t           *cfg;

    /* Get file path for BYFE64 image and config */
    status = BS->AllocatePool(EfiLoaderData,
                              (EFI_MAX_PATH + 4)*sizeof(wchar_t),
                              (void**)&file_path);
    if (EFI_ERROR(status)) {
        printk("Failed to alloc image path buffer - status: %d\n", status);
        return status;
    }

    status = efi_device_path_to_text(device_path,
                                     file_path,
                                     EFI_MAX_PATH);
    if (EFI_ERROR(status)) {
        printk("Failed to get config path - status: %d\n", status);
        goto err;
    }

    efi_form_config_path(file_path);

    /* Read the config into RT memory and store */
    status = efi_read_file(efi_file_system,
                           file_path,
                           EfiRuntimeServicesData,
                           &size,
                           &addr);
    if (EFI_ERROR(status)) {
        printk("Failed to read config file - status: %d\n", status);
        goto err;
    }

    if (size > EFI_MAX_CONFIG_FILE) {
        status = EFI_INVALID_PARAMETER;
        printk("Config file too big - size: %d\n", size);
        BS->FreePool((void*)addr);
        goto err;
    }

    /* Make a copy of the raw BTX64 config in the MLE */
    memcpy(btfe64_config_file, (void*)addr, size);
    cfg = efi_get_file(EFI_FILE_BTX64_CONFIG);
    cfg->u.base = btfe64_config_file;
    cfg->size = size;

    /* Prep for reading */
    efi_cfg_pre_parse(cfg);

    BS->FreePool((void*)addr);
    BS->FreePool(file_path);

    /* Copy and parse the command line */
    memset(g_cmdline, '\0', sizeof(g_cmdline));
    cmdline = efi_cfg_get_value(cfg, SECTION_BTX64, ITEM_OPTIONS);
    if (cmdline)
        strncpy(g_cmdline, cmdline, sizeof(g_cmdline)-1);

    parse_cmdline(false);

    /* Initialize all logging targets */
    printk_init(INIT_PRE_LAUNCH);

    return EFI_SUCCESS;

err:
    if (file_path)
        BS->FreePool(file_path);

    return status;
}

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
    printk("BTX64 EFI Entry Point: %p\n", efi_main);

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
        printk("BTX64 FATAL! Cannot get loaded image information\n");
        goto out;
    }

    /* Open the file system for the boot partition once up front */
    status = BS->OpenProtocol(parent_device_handle,
                              &FileSystemProtocol,
                              (void**)&efi_file_system,
                              parent_image_handle,
                              NULL,
                              EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
    if (EFI_ERROR(status)) {
        printk("Failed to open FileSystemProtocol - status: %d\n", status);
        goto out;
    }

    /* Load the configuration files and information */
    status = efi_load_config();
    if (EFI_ERROR(status)) {
        printk("Failed to load configuration file - status: %d\n", status);
        goto out;
    }

    efi_debug_print_i();

    /* DEBUG */
    print_test_chars();

    printk(BTX64_INFO"******************* BTX64 *******************\n");
    printk(BTX64_INFO"   %s\n", BTX64_CHANGESET);
    printk(BTX64_INFO"   command line: %s\n", g_cmdline);
    printk(BTX64_INFO"*********************************************\n");

    /* TODO interesting stuff here */

    printk("BTX64 resetting system...");
    efi_debug_pause();
    status = EFI_SUCCESS;

out:
    ST->RuntimeServices->ResetSystem(EfiResetShutdown, status, 0, NULL);

    return status;
}
