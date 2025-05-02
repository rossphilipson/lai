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
#ifndef __BTFE64_H__
#define __BTFE64_H__

#define BTFE64_MAX_IMAGE_MEM 0xfffff000

/* define uuid_t here in case uuid.h wasn't pre-included */
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
#ifdef NO_BTFE64_LOGLVL
#define BTFE64_NONE
#define BTFE64_ERR
#define BTFE64_WARN
#define BTFE64_INFO
#define BTFE64_DETA
#define BTFE64_ALL
#else /* NO_BTFE64_LOGLVL */
#define BTFE64_NONE       "<0>"
#define BTFE64_ERR        "<1>"
#define BTFE64_WARN       "<2>"
#define BTFE64_INFO       "<3>"
#define BTFE64_DETA       "<4>"
#define BTFE64_ALL        "<5>"
#endif /* NO_BTFE64_LOGLVL */


#define SHUTDOWN_REBOOT      0
#define SHUTDOWN_S5          1
#define SHUTDOWN_S4          2
#define SHUTDOWN_S3          3
#define SHUTDOWN_HALT        4
#define SHUTDOWN_WFS         5

#define BTFE64_MEM_LOG_SIZE   0x8000

/*
 * used to log printk output
 */
typedef struct {
    uuid_t     uuid;
    bool       is_init;
    uint16_t   max_size;
    uint16_t   curr_pos;
    char       buf[BTFE64_MEM_LOG_SIZE];
} log_t;

/* {C0192526-6B30-4db4-844C-A3E953B88174} */
#define BTFE64_LOG_UUID   {0xc0192526, 0x6b30, 0x4db4, 0x844c, \
                             {0xa3, 0xe9, 0x53, 0xb8, 0x81, 0x74 }}

#endif /* __BTFE64_H__ */

/*
 * Local variables:
 * mode: C
 * c-set-style: "BSD"
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
