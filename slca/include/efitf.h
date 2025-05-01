/*
 * Copyright (c) 2006-2010, Intel Corporation
 * All rights reserved.
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
 *
 */
#ifndef __EFITF_H__
#define __EFITF_H__

#define EFITF_MAX_IMAGE_MEM 0xfffff000

/* define uuid_t here in case uuid.h wasn't pre-included */
/* (i.e. so tboot.h can be self-sufficient) */
#ifndef __UUID_H__
typedef struct __packed {
    uint32_t    data1;
    uint16_t    data2;
    uint16_t    data3;
    uint16_t    data4;
    uint8_t     data5[6];
} uuid_t;
#endif

/* Log level */
#ifdef NO_TBOOT_LOGLVL
#define TBOOT_NONE
#define TBOOT_ERR
#define TBOOT_WARN
#define TBOOT_INFO
#define TBOOT_DETA
#define TBOOT_ALL
#else /* NO_TBOOT_LOGLVL */
#define TBOOT_NONE       "<0>"
#define TBOOT_ERR        "<1>"
#define TBOOT_WARN       "<2>"
#define TBOOT_INFO       "<3>"
#define TBOOT_DETA       "<4>"
#define TBOOT_ALL        "<5>"
#endif /* NO_TBOOT_LOGLVL */


#define TB_SHUTDOWN_REBOOT      0
#define TB_SHUTDOWN_S5          1
#define TB_SHUTDOWN_S4          2
#define TB_SHUTDOWN_S3          3
#define TB_SHUTDOWN_HALT        4
#define TB_SHUTDOWN_WFS         5

#define TBOOT_MEM_LOG_SIZE   0x8000

/*
 * used to log tboot printk output
 */
typedef struct {
    uuid_t     uuid;
    bool       is_init;
    uint16_t   max_size;
    uint16_t   curr_pos;
    char       buf[TBOOT_MEM_LOG_SIZE];
} tboot_log_t;

/* {C0192526-6B30-4db4-844C-A3E953B88174} */
#define TBOOT_LOG_UUID   {0xc0192526, 0x6b30, 0x4db4, 0x844c, \
                             {0xa3, 0xe9, 0x53, 0xb8, 0x81, 0x74 }}

#endif /* __EFITF_H__ */

/*
 * Local variables:
 * mode: C
 * c-set-style: "BSD"
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
