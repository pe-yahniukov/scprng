/**
 * MIT License
 *
 * Copyright 2024 PE Stanislav Yahniukov <pe@yahniukov.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the “Software”), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
*/

#include <stdlib.h>
#include <string.h>

#include "mbedtls/sha3.h"
#include "mbedtls/chacha20.h"

#include "scprng.h"

#define _RESEED_LIMIT_BYTES (512) // reseed every 512 bytes

static int _scprng_seed(uint8_t key[64],
                        uint8_t nonce[12],
                        uint8_t counter[4],
                        uint8_t encryption_key[32])
{
    uint8_t seed[48] = {0};
    memset(seed, 0, 48);

    int err = mbedtls_sha3(MBEDTLS_SHA3_384,
                           key, 64,
                           seed, 48);
    if (err)
        return -1;

    memcpy(encryption_key, seed, 32);
    memcpy(counter, seed + 44, 4);
    memcpy(nonce, seed + 32, 12);

    return 0;
}

int scprng_rand_numbers(uint32_t *numbers,
                        uint32_t numbers_count,
                        uint8_t key[64])
{
    int err = 0;
    uint32_t counter = 0;
    uint8_t rand[64] = {0};
    uint8_t nonce[12] = {0};
    uint8_t encryption_key[32] = {0};
    uint32_t idx = 0, idx_from_reseed = 0;

    if (!numbers || !numbers_count || !key)
        return -1;

    memset(nonce, 0, 12);
    memcpy(rand, key, 64);
    memset(encryption_key, 0, 32);
    if (_scprng_seed(key, nonce, (uint8_t *)&counter, encryption_key))
        return -1;

    while (idx < numbers_count)
    {
        err = mbedtls_chacha20_crypt((const unsigned char *)encryption_key,
                                     (const unsigned char *)nonce,
                                     counter, 64,
                                     (const unsigned char *)rand,
                                     (unsigned char *)rand);
        if (err)
            return -1;
        size_t copy_cnt = (idx + 16) < numbers_count ? 16 :
                                       numbers_count - idx;
        memcpy(&numbers[idx], rand, copy_cnt * sizeof(uint32_t));
        idx += copy_cnt;
        idx_from_reseed += copy_cnt;
        if ((idx_from_reseed * sizeof(uint32_t)) > _RESEED_LIMIT_BYTES)
        {
            if (_scprng_seed(rand, nonce, (uint8_t *)&counter, encryption_key))
                return -1;
            else
                idx_from_reseed = 0;
        }
        ++counter;
    }

    return 0;
}
