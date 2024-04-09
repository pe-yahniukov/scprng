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

#include "mbedtls/aes.h"

#include "scprng.h"

static int _scprng_aes_128_ecb_encrypt(uint8_t buf[16], uint8_t key[16]);

static inline uint32_t _scprng_rand(uint64_t *seed, uint32_t upper_limit)
{
#define PRNG_INCREMENT  (12345)
#define PRNG_MULTIPLIER (1103515245)

    *seed = (*seed) * PRNG_MULTIPLIER + PRNG_INCREMENT;
    return (uint32_t)((*seed) / (upper_limit * 2)) % upper_limit;
}

static uint32_t _scprng_get_seed(uint8_t encryption_key[32], uint8_t encryption_iv[16])
{
#define DEFAULT_SEED (1)

    int result = 0;
    uint8_t seed_key[16] = {0}, seed_buf[16] = {0};
    uint32_t *seed1 = NULL, *seed2 = NULL, *seed3 = NULL, *seed4 = NULL;

    memset(seed_key, 0, 16);
    memset(seed_buf, 0, 16);

    if (encryption_iv)
        memcpy(seed_key, encryption_iv, 16);
    if (encryption_key)
        memcpy(seed_buf, encryption_key, 16);

    result = _scprng_aes_128_ecb_encrypt(seed_buf, seed_key);
    if (result)
        return DEFAULT_SEED;

    if (encryption_key)
        memcpy(seed_key, encryption_key + 16, 16);

    result = _scprng_aes_128_ecb_encrypt(seed_buf, seed_key);
    if (result)
        return DEFAULT_SEED;

    seed1 = (uint32_t *)(seed_buf + 0);
    seed2 = (uint32_t *)(seed_buf + 4);
    seed3 = (uint32_t *)(seed_buf + 8);
    seed4 = (uint32_t *)(seed_buf + 12);

    return (uint32_t)(*(seed1) ^ *(seed2) ^ *(seed3) ^ *(seed4));
}

static int _scprng_aes_128_ecb_encrypt(uint8_t buf[16], uint8_t key[16])
{
    int result = 0;
    mbedtls_aes_context ctx;
    uint8_t encrypt_buf[16] = {0};

    memset(encrypt_buf, 0, 16);

    mbedtls_aes_init(&ctx);

    result = mbedtls_aes_setkey_enc(&ctx, (const unsigned char *)key, 128);
    if (result)
        goto _scprng_aes_128_ecb_encrypt_exit;

    result = mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_ENCRYPT,
                                  (const unsigned char *)buf,
                                  (unsigned char *)encrypt_buf);
    if (result)
        goto _scprng_aes_128_ecb_encrypt_exit;

    memcpy(buf, encrypt_buf, 16);

_scprng_aes_128_ecb_encrypt_exit:

    mbedtls_aes_free(&ctx);

    return result;
}

int scprng_rand_numbers(uint32_t *numbers,
                        uint32_t numbers_count,
                        uint32_t upper_limit,
                        uint8_t encryption_key[32],
                        uint8_t encryption_iv[16])
{
    uint64_t seed = 0;

    if (!numbers || !upper_limit)
        return -1;

    seed = _scprng_get_seed(encryption_key, encryption_iv);

    for (uint32_t i = 0; i < numbers_count; ++i)
        numbers[i] = _scprng_rand(&seed, upper_limit);

    return 0;
}
