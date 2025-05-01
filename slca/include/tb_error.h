/*
 * tb_error.h: error code definitions
 *
 * Copyright (c) 2006-2007, Intel Corporation
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

#ifndef __ERROR_H__
#define __ERROR_H__

typedef enum {
    ERR_NONE                = 0,         /* succeed */
    ERR_FIXED               = 1,         /* previous error has been fixed */

    ERR_GENERIC,                         /* non-fatal generic error */

    ERR_TPM_NOT_READY,                   /* tpm not ready */
    ERR_SMX_NOT_SUPPORTED,               /* smx not supported */
    ERR_VMX_NOT_SUPPORTED,               /* vmx not supported */
    ERR_TXT_NOT_SUPPORTED,               /* txt not supported */

    ERR_MODULE_VERIFICATION_FAILED,      /* module failed to verify against
                                               policy */
    ERR_MODULES_NOT_IN_POLICY,           /* modules in mbi but not in
                                               policy */
    ERR_POLICY_INVALID,                  /* policy is invalid */
    ERR_POLICY_NOT_PRESENT,              /* no policy in TPM NV */

    ERR_SINIT_NOT_PRESENT,               /* SINIT ACM not provided */
    ERR_ACMOD_VERIFY_FAILED,             /* verifying AC module failed */

    ERR_POST_LAUNCH_VERIFICATION,        /* verification of post-launch
                                               failed */
    ERR_S3_INTEGRITY,                    /* creation or verification of
                                               S3 integrity measurements
                                               failed */

    ERR_FATAL,                           /* generic fatal error */
    ERR_NV_VERIFICATION_FAILED,          /* NV failed to verify against
                                               policy */
    ERR_MAX
} tb_error_t;


void print_tb_error_msg(tb_error_t error);
bool read_tb_error_code(tb_error_t *error);
bool write_tb_error_code(tb_error_t error);
bool was_last_boot_error(void);


#endif /* __ERROR_H__ */

/*
 * Local variables:
 * mode: C
 * c-set-style: "BSD"
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
