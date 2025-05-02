/*
 * eficonfig.c: EFI related configuration settings and data.
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
#include <stdbool.h>
#include <types.h>
#include <ctype.h>
#include <string.h>
#include <misc.h>
#include <eficore.h>
#include <eficonfig.h>
#include <printk.h>

static efi_file_t efi_files[EFI_FILE_MAX] = {0};

static efi_memmap_t *efi_memmap_final;

/* Is this pre or post EBS */
static bool postebs = false;

efi_file_t *efi_get_file(efi_file_select_t sel)
{
    return &efi_files[sel];
}

efi_memmap_t *efi_get_memmap(bool final)
{
    return efi_memmap_final;
}

void efi_set_postebs(void)
{
    postebs = true;
}

bool efi_is_postebs(void)
{
    return postebs;
}

void efi_cfg_pre_parse(efi_file_t *config)
{
    char *ptr = config->u.base, *end = ptr + config->size;
    bool start = true, comment = false;

    for ( ; ptr < end; ++ptr ) {
        if ( iscntrl(*ptr) ) {
            comment = false;
            start = true;
            *ptr = 0;
        }
        else if ( comment || (start && isspace(*ptr)) ) {
            *ptr = 0;
        }
        else if ( *ptr == '#' || (start && *ptr == ';') ) {
            comment = true;
            *ptr = 0;
        }
        else
            start = false;
    }
    if ( config->size && end[-1] )
         printk("No newline at end of config file last line will be ignored.\n");
}

char *efi_cfg_get_value(efi_file_t *config, const char *section,
                        const char *item)
{
    char *ptr, *end;
    size_t slen = section ? strlen(section) : 0, ilen = strlen(item);
    bool match = !slen;

    ptr = config->u.base;
    end = ptr + config->size;

    for ( ; ptr < end; ++ptr ) {
        switch ( *ptr ) {
        case 0:
            continue;
        case '[':
            if ( !slen )
                break;
            if ( match )
                return NULL;
            match = strncmp(++ptr, section, slen) == 0 && ptr[slen] == ']';
            break;
        default:
            if ( match && strncmp(ptr, item, ilen) == 0 && ptr[ilen] == '=' ) {
                ptr += ilen + 1;
                /* strip off any leading spaces */
                while ( *ptr && isspace(*ptr) )
                    ptr++;
                return ptr;
            }
            break;
        }
        ptr += strlen(ptr);
    }
    return NULL;
}
