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

#pragma once

#include <stdint.h>

#if defined(_WIN32) && _WIN32
    #define SCPRNG_EXPORT __declspec(dllexport)
#elif defined(WIN32) && WIN32
    #define SCPRNG_EXPORT __declspec(dllexport)
#else
    #define SCPRNG_EXPORT
#endif

/**
 * @brief      Generate a particular amount of pseudo-random numbers
 *
 * @param[out] numbers         Output numbers buffer. Should be allocated.
 * @param[in]  numbers_count   Count of numbers that the function should generate.
 * @param[in]  upper_limit     The upper limit for each number.
 * @param[in]  encryption_key  (Optional) 32-byte secret encryption key.
 *                             If not specified, the algorithm will use 32 zeros.
 * @param[in]  encryption_iv   (Optional) 16-byte secret encryption initialization vector.
 *                             If not specified, the algorithm will use 16 zeros.
 *
 * @return     0 on success
 */
SCPRNG_EXPORT
int scprng_rand_numbers(uint32_t *numbers,
                        uint32_t numbers_count,
                        uint32_t upper_limit,
                        uint8_t encryption_key[32],
                        uint8_t encryption_iv[16]);
