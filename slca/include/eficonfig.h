/*
 * eficonfig.h: EFI related config definitions.
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

#ifndef __EFI_CONFIG_H__
#define __EFI_CONFIG_H__

#define EFI_MAX_PATH 512
#define EFI_MAX_CONFIG_FILE 1024 /* plenty of room for a config file */

/* BTX64 config */
#define SECTION_BTX64 "btfe64"
# define ITEM_OPTIONS "options"
# define ITEM_XENPATH "xenpath"

typedef enum efi_file_select {
    EFI_FILE_INVALID = 0,
    EFI_FILE_BTX64_CONFIG,
    EFI_FILE_MAX
} efi_file_select_t;

typedef struct {
    union {
        uint8_t *base;
        EFI_PHYSICAL_ADDRESS addr;
    } u;
    uint64_t size;
} efi_file_t;

typedef struct {
    void     *base;
    uint64_t  size;
    uint64_t  desc_size;
    uint32_t  desc_ver;
} efi_memmap_t;

efi_file_t *efi_get_file(efi_file_select_t sel);
efi_memmap_t *efi_get_memmap(bool final);
void efi_set_postebs(void);
bool efi_is_postebs(void);
void efi_cfg_pre_parse(efi_file_t *config);
char *efi_cfg_get_value(efi_file_t *config, const char *section,
                        const char *item);

#endif /* __EFI_CONFIG_H__ */
