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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#include "scprng.h"

static void help(void);
static int args_parse(int argc, char **argv);

static int       numbers_count      = 0;
static FILE     *output_file        = NULL;
static uint32_t  upper_limit        = RAND_MAX;
static uint8_t   encryption_iv[16]  = {0};
static uint8_t   encryption_key[32] = {0};

int main(int argc, char **argv)
{
    uint32_t *numbers = NULL;

    int ret = args_parse(argc, argv);
    if (ret < 0)
        goto scprng_gen_exit;

    numbers = (uint32_t *)calloc(numbers_count, sizeof(uint32_t));
    if (numbers == NULL)
    {
        fprintf(stderr, "ERROR: Failed to allocate enough memory\n");
        ret = -1;
        goto scprng_gen_exit;
    }

    ret = scprng_rand_numbers(numbers,
                    (uint32_t)numbers_count,
                              upper_limit,
                              encryption_key,
                              encryption_iv);
    if (ret)
    {
        fprintf(stderr, "ERROR: Failed to perform operation\n");
        goto scprng_gen_exit;
    }

    if (output_file)
    {
        ret = fwrite((const void *)numbers,
                     sizeof(uint32_t),
                     (size_t)numbers_count,
                     output_file);
        if (ret != numbers_count)
        {
            fprintf(stderr, "ERROR: Failed to write the result to the output file\n");
            ret = -1;
            goto scprng_gen_exit;
        }
    }
    else
    {
        for (int i = 0; i < numbers_count; ++i)
            fprintf(stdout, "%08X", numbers[i]);
        fprintf(stdout, "\n");
    }

scprng_gen_exit:

    if (output_file)
        fclose(output_file);
    if (numbers)
        free(numbers);

    return ret;
}

static void help(void)
{
    fprintf(stdout, "scprng_gen -c <numbers_count> [-o <output_file>] [-u <upper_limit>]\n"); 
    fprintf(stdout, "                              [-k <encryption_key>] [-v <encryption_iv>]\n\n");
    fprintf(stdout, "  Arguments:\n");
    fprintf(stdout, "   -c <numbers_count>    - Count of numbers that the program should generate. Range = [1; 2147483647].\n\n");
    fprintf(stdout, "  Optional arguments:\n");
    fprintf(stdout, "   -o <output_file>      - Path to the file in which generated numbers will be written.\n");
    fprintf(stdout, "                           If not specified, the numbers will be printed to STDOUT\n");
    fprintf(stdout, "                           in hexadecimal format.\n");
    fprintf(stdout, "   -u <upper_limit>      - The upper limit for each generated number. Range = [1; 4294967295].\n");
    fprintf(stdout, "                           If not specified, RAND_MAX will be used.\n");
    fprintf(stdout, "   -k <encryption_key>   - 32-byte secret encryption key. If not specified, zeros will be used.\n");
    fprintf(stdout, "                           If the specified string length is less than 32, zeros will be added.\n");
    fprintf(stdout, "                           If the specified string length is more than 32, the string will be truncated.\n");
    fprintf(stdout, "   -v <encryption_iv>    - 16-byte secret encryption initialization vector. If not specified, zeros will be used.\n");
    fprintf(stdout, "                           If the specified string length is less than 16, zeros will be added.\n");
    fprintf(stdout, "                           If the specified string length is more than 16, the string will be truncated.\n\n");
    fprintf(stdout, "  See 'scprng_algo.pdf' for the algorithm description of this cryptographic pseudo-random generator.\n\n");
    fprintf(stdout, "  MIT License. Copyright (C) 2024 PE Stanislav Yahniukov <pe@yahniukov.com>.\n\n");
}

static long int str_to_int(char *str)
{
    long int result = strtol((const char *)str, NULL, 10);
    if (errno)
        return -1;
    if (result < INT_MIN || result > INT_MAX)
        return -1;
    if (!result && strcmp(str, "0"))
        return -1;
    return result;
}

static long int str_to_uint(char *str)
{
    long int result = strtol((const char *)str, NULL, 10);
    if (errno)
        return -1;
    if (result < 0 || result > UINT_MAX)
        return -1;
    if (!result && strcmp(str, "0"))
        return -1;
    return result;
}

static int args_parse(int argc, char **argv)
{
    long int ret = 0;

    if (argc < 3)
    {
        fprintf(stderr, "ERROR: too few arguments\n");
        help();
        return -1;
    }

    memset(encryption_iv,  0, 16);
    memset(encryption_key, 0, 32);

    for (int i = 1; i < argc; i += 2)
    {
        if (!strcmp(argv[i], "-c"))
        {
            ret = str_to_int(argv[i + 1]);
            if (ret < 0)
            {
                fprintf(stderr, "ERROR: Failed to convert string to number in '-c' option\n");
                return -1;
            }
            if (ret == 0)
            {
                fprintf(stderr, "ERROR: Value for '-c' option should be more than 0 (zero)\n");
                return -1;
            }
            numbers_count = (int)ret;
        }
        if (!strcmp(argv[i], "-o"))
        {
            output_file = fopen(argv[i + 1], "wb");
            if (output_file == NULL)
            {
                fprintf(stderr, "ERROR: Failed to write to '%s' file\n", argv[i + 1]);
                return -1;
            }
        }
        if (!strcmp(argv[i], "-u"))
        {
            ret = str_to_uint(argv[i + 1]);
            if (ret < 0)
            {
                fprintf(stderr, "ERROR: Failed to convert string to unsigned number in '-u' option\n");
                return -1;
            }
            if (ret == 0)
            {
                fprintf(stderr, "ERROR: Value for '-u' option should be more than 0 (zero)\n");
                return -1;
            }
            upper_limit = (uint32_t)ret;
        }
        if (!strcmp(argv[i], "-k"))
        {
            memcpy(encryption_key, argv[i + 1], strlen(argv[i + 1]) > 32 ? 32 : strlen(argv[i + 1]));
        }
        if (!strcmp(argv[i], "-v"))
        {
            memcpy(encryption_iv, argv[i + 1], strlen(argv[i + 1]) > 16 ? 16 : strlen(argv[i + 1]));
        }
    }

    if (!numbers_count)
    {
        fprintf(stderr, "ERROR: Parameter '-c' is required. Please specify it.\n");
        return -1;
    }

    return 0;
}
