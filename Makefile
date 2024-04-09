#
# MIT License
#
# Copyright 2024 PE Stanislav Yahniukov <pe@yahniukov.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this
# software and associated documentation files (the “Software”), to deal in the Software
# without restriction, including without limitation the rights to use, copy, modify, merge,
# publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
# to whom the Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
# PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
# FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#

.PHONY: all util lib clean install uninstall

# Makefile arguments
BUILD_DIR       ?= ./
CC              ?= gcc
AR              ?= ar
MBEDTLS_INCLUDE ?= ./mbedtls/include
MBEDTLS_LIBRARY ?= ./mbedtls/build/library

CFLAGS  = -Wall -Werror -Wpedantic -Wno-unused-variable
LDFLAGS = -s -O3

all: util

util: dir $(BUILD_DIR)/scprng_gen

lib: dir $(BUILD_DIR)/scprng.a $(BUILD_DIR)/scprng.so

clean:
	rm -f $(BUILD_DIR)/scprng.o $(BUILD_DIR)/scprng.a $(BUILD_DIR)/scprng.so $(BUILD_DIR)/scprng_gen

dir:
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/scprng_gen: scprng_gen.c lib
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) -L$(BUILD_DIR) -l:scprng.a -L$(MBEDTLS_LIBRARY) -lmbedcrypto

$(BUILD_DIR)/scprng.a: $(BUILD_DIR)/scprng.o
	$(AR) rsc $@ $<

$(BUILD_DIR)/scprng.so: $(BUILD_DIR)/scprng.o
	$(CC) -shared -fPIC -o $@ $< $(LDFLAGS) -L$(MBEDTLS_LIBRARY) -lmbedcrypto

$(BUILD_DIR)/scprng.o: scprng.c scprng.h
	$(CC) -c -o $@ $(CFLAGS) -I$(MBEDTLS_INCLUDE) $<

install: all
	@cp ./scprng.h /usr/include
	@cp $(BUILD_DIR)/scprng.so /usr/lib
	@cp $(BUILD_DIR)/scprng_gen /usr/bin

uninstall:
	@rm -f /usr/include/scprng.h /usr/lib/scprng.so /usr/bin/scprng_gen
