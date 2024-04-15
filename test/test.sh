#!/bin/bash

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

#
# Test Plan:
# 0. Download "ent" utility: https://www.fourmilab.ch/random/
# 1. Generate file with length of 37500000 bytes without specifying key and IV
#    $ ../build/scprng_gen -c 9375000 -o test.file -u 4294967295
# 2. Print "ent" result in bits mode and normal mode
# 3. Print "dieharder" result with ALL tests
# 4. Generate file with length of 37500000 bytes with specifying simple key and IV
#    $ ../build/scprng_gen -c 9375000 -o test.file -u 4294967295 -k 123 -v 123
# 5. Print "ent" result in bits mode and normal mode
# 6. Print "dieharder" result with ALL tests
# 4. Generate file with length of 37500000 bytes with specifying complex key and IV
#    $ ../build/scprng_gen -c 9375000 -o test.file -u 4294967295 -k $(../build/scprng_gen -c 8 -u 4294967295) -v $(../build/scprng_gen -c 4 -u 4294967295)
# 5. Print "ent" result in bits mode and normal mode
# 6. Print "dieharder" result with ALL tests
#

if [ -z $(ls $(pwd) | grep test.sh) ]; then
    echo "ERROR: Please execute this script in the directory with this script"
    exit 1
fi
if [[ ! -z $(type ../build/scprng_gen 2>&1 | grep "not found") ]]; then
    echo "ERROR: Please build '../build/scprng_gen' before launching this script"
    echo "See 'Building' section in README.md"
    exit 1
fi
if [[ ! -z $(type dieharder 2>&1 | grep "not found") ]]; then
    echo "ERROR: Please install 'dieharder' before launching this script"
    echo "E.g., the command for Ubuntu is:"
    echo "$ sudo apt-get install dieharder"
    exit 1
fi
if [[ ! -z $(type unzip 2>&1 | grep "not found") ]]; then
    echo "ERROR: Please install 'unzip' before launching this script"
    echo "E.g., the command for Ubuntu is:"
    echo "$ sudo apt-get install unzip"
    exit 1
fi
if [[ ! -z $(type make 2>&1 | grep "not found") ]]; then
    echo "ERROR: Please install 'make' before launching this script"
    echo "E.g., the command for Ubuntu is:"
    echo "$ sudo apt-get install build-essential"
    exit 1
fi

#
# Step 0 - Download "ent" utility
#

if [ ! -d ./random ]; then
    if [ ! -f ./random.zip ]; then
        wget https://www.fourmilab.ch/random/random.zip
    fi
    if [ ! -f ./random.zip ]; then
        echo "ERROR: Unable to download 'ent' utility. Do it manually: https://www.fourmilab.ch/random/"
        exit 1
    else
        mkdir random && mv random.zip random && cd random
        unzip random.zip && rm random.zip && make && cd ..
    fi
fi

#
# Step 1 - Generate file with length of 37500000 bytes without specifying key and IV
#

rm -f test.file
../build/scprng_gen -c 9375000 -o test.file -u 4294967295
if [ ! -f test.file ]; then
    echo "ERROR: Failed to generate test file"
    exit 1
fi

#
# Step 2 - Print "ent" result in bits mode and normal mode
#

echo
echo -e "\tENT BITS MODE (Test file without specifying key and IV)"
echo
./random/ent -b test.file
echo
echo -e "\tENT NORMAL MODE (Test file without specifying key and IV)"
echo
./random/ent test.file

#
# Step 3 - Print "dieharder" result with ALL tests
#

echo
echo -e "\tDIEHARDER (Test file without specifying key and IV)"
echo
dieharder -a -f test.file

#
# Step 4 - Generate file with length of 37500000 bytes with specifying simple key and IV
#

rm -f test.file
../build/scprng_gen -c 9375000 -o test.file -u 4294967295 -k 123 -v 123
if [ ! -f test.file ]; then
    echo "ERROR: Failed to generate test file"
    exit 1
fi

#
# Step 5 - Print "ent" result in bits mode and normal mode
#

echo
echo -e "\tENT BITS MODE (Test file with specifying simple key and IV)"
echo
./random/ent -b test.file
echo
echo -e "\tENT NORMAL MODE (Test file with specifying simple key and IV)"
echo
./random/ent test.file

#
# Step 6 - Print "dieharder" result with ALL tests
#

echo
echo -e "\tDIEHARDER (Test file with specifying simple key and IV)"
echo
dieharder -a -f test.file

#
# Step 7 - Generate file with length of 37500000 bytes with specifying complex key and IV
#

rm -f test.file
../build/scprng_gen -c 9375000 -o test.file -u 4294967295 -k $(../build/scprng_gen -c 8 -u 4294967295) -v $(../build/scprng_gen -c 4 -u 4294967295)
if [ ! -f test.file ]; then
    echo "ERROR: Failed to generate test file"
    exit 1
fi

#
# Step 8 - Print "ent" result in bits mode and normal mode
#

echo
echo -e "\tENT BITS MODE (Test file with specifying complex key and IV)"
echo
./random/ent -b test.file
echo
echo -e "\tENT NORMAL MODE (Test file with specifying complex key and IV)"
echo
./random/ent test.file

#
# Step 9 - Print "dieharder" result with ALL tests
#

echo
echo -e "\tDIEHARDER (Test file with specifying complex key and IV)"
echo
dieharder -a -f test.file

#
# Exit
#

# Uncomment if cleaning is necessary. These files are added to gitignore.
# rm -rf random test.file

echo
echo -e "\tALL TESTS ARE DONE. PLEASE CHECK OUTPUT."
echo

exit 0
