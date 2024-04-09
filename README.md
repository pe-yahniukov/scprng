# SCPRNG - Simple cryptographic pseudo-random number generator library with AES encryption

## Purposes

1. The generator should be deterministic - given the same inputs, always produces the same outputs, even on different platforms with different operating systems and architectures.
2. The seed is calculated from a secret key and secret initialization vector (IV). Thus, the algorithm generates the same number sequence on two platform-independent systems that have a shared secret key and secret IV.
3. The generator should pass most of the known statistical tests and should have very high entropy.
4. The generator should be small and fast, without external entropy sources that can slow down (or even temporarily block) number generation.

## Algorithm

See "doc/scprng_algo.pdf" for details.

## Standards

The generator doesn't comply with any national and international standards for cryptographic random number generators (CRNG) including "NIST Special Publication 800-90A Revision 1". Thus, if your application requires to be in line with some of these standards, the author of this repository doesn't garantee that this library or algorithm itself can meet this requirement. Use this library at your own risk (see "License" section).

## License

The algorithm is in the public domain. The source code of this repository has MIT License, see the "LICENSE" file.

## Library interface

See scprng.h

## Building

### Pre-requirements

* make
* [mbedtls](https://github.com/Mbed-TLS/mbedtls.git) project include directory and libmbedcrypto compiled library for your target OS and architecture. libmbedcrypto is used for AES encryption.

### Compiling

See Makefile arguments.

Example:
$ make BUILD_DIR=build CC=gcc AR=ar MBEDTLS_INCLUDE=./mbedtls/include MBEDTLS_LIBRARY=./mbedtls/build/x86_64/library

x86 Example:
$ make BUILD_DIR=build/x86 CC="gcc -m32" AR=ar MBEDTLS_INCLUDE=./mbedtls/include MBEDTLS_LIBRARY=./mbedtls/build/x86/library

MINGW Example:
$ make BUILD_DIR=build/win_amd64 CC=x86_64-w64-mingw32-gcc AR=x86_64-w64-mingw32-gcc-ar MBEDTLS_INCLUDE=./mbedtls/include MBEDTLS_LIBRARY=./mbedtls/build/win_amd64/library

AARCH64 Example:
$ make BUILD_DIR=build/arm64 CC=aarch64-linux-gnu-gcc AR=aarch64-linux-gnu-gcc-ar MBEDTLS_INCLUDE=./mbedtls/include MBEDTLS_LIBRARY=./mbedtls/build/arm64/library

Output in build directory:

* scprng.a (~3 KB)   - static library
* scprng.so (~30 KB) - dynamic library
* scprng_gen         - executable, generator utility

You may probably want to rename "scprng.so" to "scprng.dll" for Windows, and "scprng_gen" to "scprng_gen.exe".

## Testing

No automated tests were implemented. However, "test/test.sh" script generates big file using "scprng_gen" and run it through "ent" and "dieharder" utilities.

Pre-requirements:

* Install "scprng_gen" on the system:
  $ sudo make BUILD_DIR=build install
* Install "dieharder" on the system, e.g. on Ubuntu:
  $ sudo apt-get install dieharder
* Install "unzip" on the system, e.g. on Ubuntu:
  $ sudo apt-get install unzip
* "ent" utility will be downloaded by test script

Steps:

```console

$ cd test

$ ./test.sh 

	ENT BITS MODE (Test file without specifying key and IV)

Entropy = 1.000000 bits per bit.

Optimum compression would reduce the size
of this 300000000 bit file by 0 percent.

Chi square distribution for 300000000 samples is 3.07, and randomly
would exceed this value 7.99 percent of the times.

Arithmetic mean value of data bits is 0.4999 (0.5 = random).
Monte Carlo value for Pi is 3.142017920 (error 0.01 percent).
Serial correlation coefficient is -0.000116 (totally uncorrelated = 0.0).

	ENT NORMAL MODE (Test file without specifying key and IV)

Entropy = 7.999995 bits per byte.

Optimum compression would reduce the size
of this 37500000 byte file by 0 percent.

Chi square distribution for 37500000 samples is 262.71, and randomly
would exceed this value 35.66 percent of the times.

Arithmetic mean value of data bytes is 127.4822 (127.5 = random).
Monte Carlo value for Pi is 3.142017920 (error 0.01 percent).
Serial correlation coefficient is 0.000008 (totally uncorrelated = 0.0).

	DIEHARDER (Test file without specifying key and IV)

#=============================================================================#
#            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
#=============================================================================#
   rng_name    |           filename             |rands/second|
        mt19937|                       test.file|  2.17e+08  |
#=============================================================================#
        test_name   |ntup| tsamples |psamples|  p-value |Assessment
#=============================================================================#
   diehard_birthdays|   0|       100|     100|0.69508989|  PASSED  
      diehard_operm5|   0|   1000000|     100|0.44470626|  PASSED  
  diehard_rank_32x32|   0|     40000|     100|0.60699731|  PASSED  
    diehard_rank_6x8|   0|    100000|     100|0.94382242|  PASSED  
   diehard_bitstream|   0|   2097152|     100|0.45057573|  PASSED  
        diehard_opso|   0|   2097152|     100|0.68681942|  PASSED  
        diehard_oqso|   0|   2097152|     100|0.88372324|  PASSED  
         diehard_dna|   0|   2097152|     100|0.97967534|  PASSED  
diehard_count_1s_str|   0|    256000|     100|0.67302871|  PASSED  
diehard_count_1s_byt|   0|    256000|     100|0.66471522|  PASSED  
 diehard_parking_lot|   0|     12000|     100|0.33358085|  PASSED  
    diehard_2dsphere|   2|      8000|     100|0.48596530|  PASSED  
    diehard_3dsphere|   3|      4000|     100|0.39103388|  PASSED  
     diehard_squeeze|   0|    100000|     100|0.35697289|  PASSED  
        diehard_sums|   0|       100|     100|0.01899732|  PASSED  
        diehard_runs|   0|    100000|     100|0.98240929|  PASSED  
        diehard_runs|   0|    100000|     100|0.50796679|  PASSED  
       diehard_craps|   0|    200000|     100|0.19445983|  PASSED  
       diehard_craps|   0|    200000|     100|0.13919033|  PASSED  
 marsaglia_tsang_gcd|   0|  10000000|     100|0.18772799|  PASSED  
 marsaglia_tsang_gcd|   0|  10000000|     100|0.65609022|  PASSED  
         sts_monobit|   1|    100000|     100|0.42308331|  PASSED  
            sts_runs|   2|    100000|     100|0.99032413|  PASSED  
          sts_serial|   1|    100000|     100|0.11890123|  PASSED  
          sts_serial|   2|    100000|     100|0.89987944|  PASSED  
          sts_serial|   3|    100000|     100|0.91306310|  PASSED  
          sts_serial|   3|    100000|     100|0.61954118|  PASSED  
          sts_serial|   4|    100000|     100|0.92323056|  PASSED  
          sts_serial|   4|    100000|     100|0.57990674|  PASSED  
          sts_serial|   5|    100000|     100|0.61178693|  PASSED  
          sts_serial|   5|    100000|     100|0.56108292|  PASSED  
          sts_serial|   6|    100000|     100|0.46783005|  PASSED  
          sts_serial|   6|    100000|     100|0.46379197|  PASSED  
          sts_serial|   7|    100000|     100|0.87006053|  PASSED  
          sts_serial|   7|    100000|     100|0.60535486|  PASSED  
          sts_serial|   8|    100000|     100|0.85468262|  PASSED  
          sts_serial|   8|    100000|     100|0.38355868|  PASSED  
          sts_serial|   9|    100000|     100|0.48480347|  PASSED  
          sts_serial|   9|    100000|     100|0.51290358|  PASSED  
          sts_serial|  10|    100000|     100|0.07608163|  PASSED  
          sts_serial|  10|    100000|     100|0.09313152|  PASSED  
          sts_serial|  11|    100000|     100|0.42643045|  PASSED  
          sts_serial|  11|    100000|     100|0.14351646|  PASSED  
          sts_serial|  12|    100000|     100|0.63699309|  PASSED  
          sts_serial|  12|    100000|     100|0.38916364|  PASSED  
          sts_serial|  13|    100000|     100|0.88019766|  PASSED  
          sts_serial|  13|    100000|     100|0.84599907|  PASSED  
          sts_serial|  14|    100000|     100|0.58207015|  PASSED  
          sts_serial|  14|    100000|     100|0.94182138|  PASSED  
          sts_serial|  15|    100000|     100|0.97725441|  PASSED  
          sts_serial|  15|    100000|     100|0.75676768|  PASSED  
          sts_serial|  16|    100000|     100|0.68467610|  PASSED  
          sts_serial|  16|    100000|     100|0.34068000|  PASSED  
         rgb_bitdist|   1|    100000|     100|0.99677244|   WEAK   
         rgb_bitdist|   2|    100000|     100|0.64049929|  PASSED  
         rgb_bitdist|   3|    100000|     100|0.98066756|  PASSED  
         rgb_bitdist|   4|    100000|     100|0.76254677|  PASSED  
         rgb_bitdist|   5|    100000|     100|0.86189240|  PASSED  
         rgb_bitdist|   6|    100000|     100|0.81982660|  PASSED  
         rgb_bitdist|   7|    100000|     100|0.12190087|  PASSED  
         rgb_bitdist|   8|    100000|     100|0.27243023|  PASSED  
         rgb_bitdist|   9|    100000|     100|0.16091681|  PASSED  
         rgb_bitdist|  10|    100000|     100|0.80321645|  PASSED  
         rgb_bitdist|  11|    100000|     100|0.53994008|  PASSED  
         rgb_bitdist|  12|    100000|     100|0.64036985|  PASSED  
rgb_minimum_distance|   2|     10000|    1000|0.24571208|  PASSED  
rgb_minimum_distance|   3|     10000|    1000|0.73236020|  PASSED  
rgb_minimum_distance|   4|     10000|    1000|0.29470371|  PASSED  
rgb_minimum_distance|   5|     10000|    1000|0.93820647|  PASSED  
    rgb_permutations|   2|    100000|     100|0.90063402|  PASSED  
    rgb_permutations|   3|    100000|     100|0.31612917|  PASSED  
    rgb_permutations|   4|    100000|     100|0.05607427|  PASSED  
    rgb_permutations|   5|    100000|     100|0.29916168|  PASSED  
      rgb_lagged_sum|   0|   1000000|     100|0.32516136|  PASSED  
      rgb_lagged_sum|   1|   1000000|     100|0.91354348|  PASSED  
      rgb_lagged_sum|   2|   1000000|     100|0.65955428|  PASSED  
      rgb_lagged_sum|   3|   1000000|     100|0.49425778|  PASSED  
      rgb_lagged_sum|   4|   1000000|     100|0.40661633|  PASSED  
      rgb_lagged_sum|   5|   1000000|     100|0.38934918|  PASSED  
      rgb_lagged_sum|   6|   1000000|     100|0.81605303|  PASSED  
      rgb_lagged_sum|   7|   1000000|     100|0.62651024|  PASSED  
      rgb_lagged_sum|   8|   1000000|     100|0.69883130|  PASSED  
      rgb_lagged_sum|   9|   1000000|     100|0.84933854|  PASSED  
      rgb_lagged_sum|  10|   1000000|     100|0.04520335|  PASSED  
      rgb_lagged_sum|  11|   1000000|     100|0.12262259|  PASSED  
      rgb_lagged_sum|  12|   1000000|     100|0.98668560|  PASSED  
      rgb_lagged_sum|  13|   1000000|     100|0.82223960|  PASSED  
      rgb_lagged_sum|  14|   1000000|     100|0.13449306|  PASSED  
      rgb_lagged_sum|  15|   1000000|     100|0.45418775|  PASSED  
      rgb_lagged_sum|  16|   1000000|     100|0.64370907|  PASSED  
      rgb_lagged_sum|  17|   1000000|     100|0.42950331|  PASSED  
      rgb_lagged_sum|  18|   1000000|     100|0.95783543|  PASSED  
      rgb_lagged_sum|  19|   1000000|     100|0.86706848|  PASSED  
      rgb_lagged_sum|  20|   1000000|     100|0.67129802|  PASSED  
      rgb_lagged_sum|  21|   1000000|     100|0.72214191|  PASSED  
      rgb_lagged_sum|  22|   1000000|     100|0.91742890|  PASSED  
      rgb_lagged_sum|  23|   1000000|     100|0.27902265|  PASSED  
      rgb_lagged_sum|  24|   1000000|     100|0.91862357|  PASSED  
      rgb_lagged_sum|  25|   1000000|     100|0.11872426|  PASSED  
      rgb_lagged_sum|  26|   1000000|     100|0.97824836|  PASSED  
      rgb_lagged_sum|  27|   1000000|     100|0.85910377|  PASSED  
      rgb_lagged_sum|  28|   1000000|     100|0.61058224|  PASSED  
      rgb_lagged_sum|  29|   1000000|     100|0.53459173|  PASSED  
      rgb_lagged_sum|  30|   1000000|     100|0.94106448|  PASSED  
      rgb_lagged_sum|  31|   1000000|     100|0.58197252|  PASSED  
      rgb_lagged_sum|  32|   1000000|     100|0.04806957|  PASSED  
     rgb_kstest_test|   0|     10000|    1000|0.78002592|  PASSED  
     dab_bytedistrib|   0|  51200000|       1|0.03508698|  PASSED  
             dab_dct| 256|     50000|       1|0.36141357|  PASSED  
Preparing to run test 207.  ntuple = 0
        dab_filltree|  32|  15000000|       1|0.72886868|  PASSED  
        dab_filltree|  32|  15000000|       1|0.99286161|  PASSED  
Preparing to run test 208.  ntuple = 0
       dab_filltree2|   0|   5000000|       1|0.15055128|  PASSED  
       dab_filltree2|   1|   5000000|       1|0.15753949|  PASSED  
Preparing to run test 209.  ntuple = 0
        dab_monobit2|  12|  65000000|       1|0.02764910|  PASSED  

	ENT BITS MODE (Test file with specifying simple key and IV)

Entropy = 1.000000 bits per bit.

Optimum compression would reduce the size
of this 300000000 bit file by 0 percent.

Chi square distribution for 300000000 samples is 1.06, and randomly
would exceed this value 30.24 percent of the times.

Arithmetic mean value of data bits is 0.5000 (0.5 = random).
Monte Carlo value for Pi is 3.142164480 (error 0.02 percent).
Serial correlation coefficient is 0.000068 (totally uncorrelated = 0.0).

	ENT NORMAL MODE (Test file with specifying simple key and IV)

Entropy = 7.999994 bits per byte.

Optimum compression would reduce the size
of this 37500000 byte file by 0 percent.

Chi square distribution for 37500000 samples is 288.56, and randomly
would exceed this value 7.29 percent of the times.

Arithmetic mean value of data bytes is 127.4934 (127.5 = random).
Monte Carlo value for Pi is 3.142164480 (error 0.02 percent).
Serial correlation coefficient is -0.000099 (totally uncorrelated = 0.0).

	DIEHARDER (Test file with specifying simple key and IV)

#=============================================================================#
#            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
#=============================================================================#
   rng_name    |           filename             |rands/second|
        mt19937|                       test.file|  2.17e+08  |
#=============================================================================#
        test_name   |ntup| tsamples |psamples|  p-value |Assessment
#=============================================================================#
   diehard_birthdays|   0|       100|     100|0.65589335|  PASSED  
      diehard_operm5|   0|   1000000|     100|0.63249602|  PASSED  
  diehard_rank_32x32|   0|     40000|     100|0.86757227|  PASSED  
    diehard_rank_6x8|   0|    100000|     100|0.87868795|  PASSED  
   diehard_bitstream|   0|   2097152|     100|0.90111458|  PASSED  
        diehard_opso|   0|   2097152|     100|0.75638487|  PASSED  
        diehard_oqso|   0|   2097152|     100|0.60697236|  PASSED  
         diehard_dna|   0|   2097152|     100|0.67696893|  PASSED  
diehard_count_1s_str|   0|    256000|     100|0.90571550|  PASSED  
diehard_count_1s_byt|   0|    256000|     100|0.91740094|  PASSED  
 diehard_parking_lot|   0|     12000|     100|0.59664629|  PASSED  
    diehard_2dsphere|   2|      8000|     100|0.94792262|  PASSED  
    diehard_3dsphere|   3|      4000|     100|0.70344040|  PASSED  
     diehard_squeeze|   0|    100000|     100|0.76203078|  PASSED  
        diehard_sums|   0|       100|     100|0.18070288|  PASSED  
        diehard_runs|   0|    100000|     100|0.05943129|  PASSED  
        diehard_runs|   0|    100000|     100|0.49103284|  PASSED  
       diehard_craps|   0|    200000|     100|0.11396822|  PASSED  
       diehard_craps|   0|    200000|     100|0.84738434|  PASSED  
 marsaglia_tsang_gcd|   0|  10000000|     100|0.47324735|  PASSED  
 marsaglia_tsang_gcd|   0|  10000000|     100|0.55441159|  PASSED  
         sts_monobit|   1|    100000|     100|0.85006876|  PASSED  
            sts_runs|   2|    100000|     100|0.89555038|  PASSED  
          sts_serial|   1|    100000|     100|0.97696782|  PASSED  
          sts_serial|   2|    100000|     100|0.88739156|  PASSED  
          sts_serial|   3|    100000|     100|0.88572847|  PASSED  
          sts_serial|   3|    100000|     100|0.90907291|  PASSED  
          sts_serial|   4|    100000|     100|0.26742785|  PASSED  
          sts_serial|   4|    100000|     100|0.42905191|  PASSED  
          sts_serial|   5|    100000|     100|0.91664162|  PASSED  
          sts_serial|   5|    100000|     100|0.99300899|  PASSED  
          sts_serial|   6|    100000|     100|0.24764233|  PASSED  
          sts_serial|   6|    100000|     100|0.41829781|  PASSED  
          sts_serial|   7|    100000|     100|0.32936134|  PASSED  
          sts_serial|   7|    100000|     100|0.66819541|  PASSED  
          sts_serial|   8|    100000|     100|0.20453905|  PASSED  
          sts_serial|   8|    100000|     100|0.94845779|  PASSED  
          sts_serial|   9|    100000|     100|0.85580896|  PASSED  
          sts_serial|   9|    100000|     100|0.67559290|  PASSED  
          sts_serial|  10|    100000|     100|0.37511788|  PASSED  
          sts_serial|  10|    100000|     100|0.50559447|  PASSED  
          sts_serial|  11|    100000|     100|0.51639861|  PASSED  
          sts_serial|  11|    100000|     100|0.81088711|  PASSED  
          sts_serial|  12|    100000|     100|0.84092604|  PASSED  
          sts_serial|  12|    100000|     100|0.46995787|  PASSED  
          sts_serial|  13|    100000|     100|0.96795612|  PASSED  
          sts_serial|  13|    100000|     100|0.82973960|  PASSED  
          sts_serial|  14|    100000|     100|0.64496298|  PASSED  
          sts_serial|  14|    100000|     100|0.39055117|  PASSED  
          sts_serial|  15|    100000|     100|0.96957510|  PASSED  
          sts_serial|  15|    100000|     100|0.96259405|  PASSED  
          sts_serial|  16|    100000|     100|0.96065096|  PASSED  
          sts_serial|  16|    100000|     100|0.47803841|  PASSED  
         rgb_bitdist|   1|    100000|     100|0.81684775|  PASSED  
         rgb_bitdist|   2|    100000|     100|0.79365895|  PASSED  
         rgb_bitdist|   3|    100000|     100|0.24368745|  PASSED  
         rgb_bitdist|   4|    100000|     100|0.01493928|  PASSED  
         rgb_bitdist|   5|    100000|     100|0.66416575|  PASSED  
         rgb_bitdist|   6|    100000|     100|0.99968280|   WEAK   
         rgb_bitdist|   7|    100000|     100|0.40494693|  PASSED  
         rgb_bitdist|   8|    100000|     100|0.88065326|  PASSED  
         rgb_bitdist|   9|    100000|     100|0.37073617|  PASSED  
         rgb_bitdist|  10|    100000|     100|0.90359229|  PASSED  
         rgb_bitdist|  11|    100000|     100|0.76141236|  PASSED  
         rgb_bitdist|  12|    100000|     100|0.12934515|  PASSED  
rgb_minimum_distance|   2|     10000|    1000|0.39794417|  PASSED  
rgb_minimum_distance|   3|     10000|    1000|0.65175651|  PASSED  
rgb_minimum_distance|   4|     10000|    1000|0.50085082|  PASSED  
rgb_minimum_distance|   5|     10000|    1000|0.78261158|  PASSED  
    rgb_permutations|   2|    100000|     100|0.44175776|  PASSED  
    rgb_permutations|   3|    100000|     100|0.36372810|  PASSED  
    rgb_permutations|   4|    100000|     100|0.25997084|  PASSED  
    rgb_permutations|   5|    100000|     100|0.85189244|  PASSED  
      rgb_lagged_sum|   0|   1000000|     100|0.00295596|   WEAK   
      rgb_lagged_sum|   1|   1000000|     100|0.84676985|  PASSED  
      rgb_lagged_sum|   2|   1000000|     100|0.91982979|  PASSED  
      rgb_lagged_sum|   3|   1000000|     100|0.27779046|  PASSED  
      rgb_lagged_sum|   4|   1000000|     100|0.62602182|  PASSED  
      rgb_lagged_sum|   5|   1000000|     100|0.89477157|  PASSED  
      rgb_lagged_sum|   6|   1000000|     100|0.10925352|  PASSED  
      rgb_lagged_sum|   7|   1000000|     100|0.36046409|  PASSED  
      rgb_lagged_sum|   8|   1000000|     100|0.26214844|  PASSED  
      rgb_lagged_sum|   9|   1000000|     100|0.23193629|  PASSED  
      rgb_lagged_sum|  10|   1000000|     100|0.75702698|  PASSED  
      rgb_lagged_sum|  11|   1000000|     100|0.51886089|  PASSED  
      rgb_lagged_sum|  12|   1000000|     100|0.20129580|  PASSED  
      rgb_lagged_sum|  13|   1000000|     100|0.04134378|  PASSED  
      rgb_lagged_sum|  14|   1000000|     100|0.02621207|  PASSED  
      rgb_lagged_sum|  15|   1000000|     100|0.07479921|  PASSED  
      rgb_lagged_sum|  16|   1000000|     100|0.64061095|  PASSED  
      rgb_lagged_sum|  17|   1000000|     100|0.97998338|  PASSED  
      rgb_lagged_sum|  18|   1000000|     100|0.86135662|  PASSED  
      rgb_lagged_sum|  19|   1000000|     100|0.08835030|  PASSED  
      rgb_lagged_sum|  20|   1000000|     100|0.90535317|  PASSED  
      rgb_lagged_sum|  21|   1000000|     100|0.62470730|  PASSED  
      rgb_lagged_sum|  22|   1000000|     100|0.50351759|  PASSED  
      rgb_lagged_sum|  23|   1000000|     100|0.20242459|  PASSED  
      rgb_lagged_sum|  24|   1000000|     100|0.86717474|  PASSED  
      rgb_lagged_sum|  25|   1000000|     100|0.20829483|  PASSED  
      rgb_lagged_sum|  26|   1000000|     100|0.99859784|   WEAK   
      rgb_lagged_sum|  27|   1000000|     100|0.87637931|  PASSED  
      rgb_lagged_sum|  28|   1000000|     100|0.39062908|  PASSED  
      rgb_lagged_sum|  29|   1000000|     100|0.79208510|  PASSED  
      rgb_lagged_sum|  30|   1000000|     100|0.93617072|  PASSED  
      rgb_lagged_sum|  31|   1000000|     100|0.20207235|  PASSED  
      rgb_lagged_sum|  32|   1000000|     100|0.07093488|  PASSED  
     rgb_kstest_test|   0|     10000|    1000|0.26545882|  PASSED  
     dab_bytedistrib|   0|  51200000|       1|0.19234087|  PASSED  
             dab_dct| 256|     50000|       1|0.01003830|  PASSED  
Preparing to run test 207.  ntuple = 0
        dab_filltree|  32|  15000000|       1|0.10962207|  PASSED  
        dab_filltree|  32|  15000000|       1|0.44200324|  PASSED  
Preparing to run test 208.  ntuple = 0
       dab_filltree2|   0|   5000000|       1|0.09752927|  PASSED  
       dab_filltree2|   1|   5000000|       1|0.95087703|  PASSED  
Preparing to run test 209.  ntuple = 0
        dab_monobit2|  12|  65000000|       1|0.70547486|  PASSED  

	ENT BITS MODE (Test file with specifying complex key and IV)

Entropy = 1.000000 bits per bit.

Optimum compression would reduce the size
of this 300000000 bit file by 0 percent.

Chi square distribution for 300000000 samples is 5.51, and randomly
would exceed this value 1.89 percent of the times.

Arithmetic mean value of data bits is 0.5001 (0.5 = random).
Monte Carlo value for Pi is 3.140681600 (error 0.03 percent).
Serial correlation coefficient is -0.000069 (totally uncorrelated = 0.0).

	ENT NORMAL MODE (Test file with specifying complex key and IV)

Entropy = 7.999995 bits per byte.

Optimum compression would reduce the size
of this 37500000 byte file by 0 percent.

Chi square distribution for 37500000 samples is 279.44, and randomly
would exceed this value 14.04 percent of the times.

Arithmetic mean value of data bytes is 127.5265 (127.5 = random).
Monte Carlo value for Pi is 3.140681600 (error 0.03 percent).
Serial correlation coefficient is -0.000103 (totally uncorrelated = 0.0).

	DIEHARDER (Test file with specifying complex key and IV)

#=============================================================================#
#            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
#=============================================================================#
   rng_name    |           filename             |rands/second|
        mt19937|                       test.file|  2.17e+08  |
#=============================================================================#
        test_name   |ntup| tsamples |psamples|  p-value |Assessment
#=============================================================================#
   diehard_birthdays|   0|       100|     100|0.97636379|  PASSED  
      diehard_operm5|   0|   1000000|     100|0.75974377|  PASSED  
  diehard_rank_32x32|   0|     40000|     100|0.51581216|  PASSED  
    diehard_rank_6x8|   0|    100000|     100|0.98522063|  PASSED  
   diehard_bitstream|   0|   2097152|     100|0.52800335|  PASSED  
        diehard_opso|   0|   2097152|     100|0.72329200|  PASSED  
        diehard_oqso|   0|   2097152|     100|0.32995929|  PASSED  
         diehard_dna|   0|   2097152|     100|0.19213401|  PASSED  
diehard_count_1s_str|   0|    256000|     100|0.36202549|  PASSED  
diehard_count_1s_byt|   0|    256000|     100|0.92310390|  PASSED  
 diehard_parking_lot|   0|     12000|     100|0.43926798|  PASSED  
    diehard_2dsphere|   2|      8000|     100|0.01073134|  PASSED  
    diehard_3dsphere|   3|      4000|     100|0.62583313|  PASSED  
     diehard_squeeze|   0|    100000|     100|0.67894290|  PASSED  
        diehard_sums|   0|       100|     100|0.74355343|  PASSED  
        diehard_runs|   0|    100000|     100|0.79952161|  PASSED  
        diehard_runs|   0|    100000|     100|0.65285524|  PASSED  
       diehard_craps|   0|    200000|     100|0.46334170|  PASSED  
       diehard_craps|   0|    200000|     100|0.06649677|  PASSED  
 marsaglia_tsang_gcd|   0|  10000000|     100|0.04165852|  PASSED  
 marsaglia_tsang_gcd|   0|  10000000|     100|0.65631828|  PASSED  
         sts_monobit|   1|    100000|     100|0.85591997|  PASSED  
            sts_runs|   2|    100000|     100|0.96324661|  PASSED  
          sts_serial|   1|    100000|     100|0.95008701|  PASSED  
          sts_serial|   2|    100000|     100|0.92851440|  PASSED  
          sts_serial|   3|    100000|     100|0.79068340|  PASSED  
          sts_serial|   3|    100000|     100|0.66404235|  PASSED  
          sts_serial|   4|    100000|     100|0.93272792|  PASSED  
          sts_serial|   4|    100000|     100|0.98888563|  PASSED  
          sts_serial|   5|    100000|     100|0.53809531|  PASSED  
          sts_serial|   5|    100000|     100|0.81456600|  PASSED  
          sts_serial|   6|    100000|     100|0.87549174|  PASSED  
          sts_serial|   6|    100000|     100|0.12355299|  PASSED  
          sts_serial|   7|    100000|     100|0.25499231|  PASSED  
          sts_serial|   7|    100000|     100|0.23571799|  PASSED  
          sts_serial|   8|    100000|     100|0.99141721|  PASSED  
          sts_serial|   8|    100000|     100|0.35502219|  PASSED  
          sts_serial|   9|    100000|     100|0.28343792|  PASSED  
          sts_serial|   9|    100000|     100|0.29232140|  PASSED  
          sts_serial|  10|    100000|     100|0.58783960|  PASSED  
          sts_serial|  10|    100000|     100|0.45638111|  PASSED  
          sts_serial|  11|    100000|     100|0.23569289|  PASSED  
          sts_serial|  11|    100000|     100|0.68216197|  PASSED  
          sts_serial|  12|    100000|     100|0.77192342|  PASSED  
          sts_serial|  12|    100000|     100|0.56340514|  PASSED  
          sts_serial|  13|    100000|     100|0.74698124|  PASSED  
          sts_serial|  13|    100000|     100|0.29576507|  PASSED  
          sts_serial|  14|    100000|     100|0.60632258|  PASSED  
          sts_serial|  14|    100000|     100|0.47898279|  PASSED  
          sts_serial|  15|    100000|     100|0.94780374|  PASSED  
          sts_serial|  15|    100000|     100|0.51613264|  PASSED  
          sts_serial|  16|    100000|     100|0.99280221|  PASSED  
          sts_serial|  16|    100000|     100|0.83139431|  PASSED  
         rgb_bitdist|   1|    100000|     100|0.23223548|  PASSED  
         rgb_bitdist|   2|    100000|     100|0.61468693|  PASSED  
         rgb_bitdist|   3|    100000|     100|0.88603258|  PASSED  
         rgb_bitdist|   4|    100000|     100|0.37547691|  PASSED  
         rgb_bitdist|   5|    100000|     100|0.54601816|  PASSED  
         rgb_bitdist|   6|    100000|     100|0.11659585|  PASSED  
         rgb_bitdist|   7|    100000|     100|0.44503168|  PASSED  
         rgb_bitdist|   8|    100000|     100|0.77615610|  PASSED  
         rgb_bitdist|   9|    100000|     100|0.81701780|  PASSED  
         rgb_bitdist|  10|    100000|     100|0.90760496|  PASSED  
         rgb_bitdist|  11|    100000|     100|0.01895184|  PASSED  
         rgb_bitdist|  12|    100000|     100|0.04530498|  PASSED  
rgb_minimum_distance|   2|     10000|    1000|0.68693263|  PASSED  
rgb_minimum_distance|   3|     10000|    1000|0.73580818|  PASSED  
rgb_minimum_distance|   4|     10000|    1000|0.11851923|  PASSED  
rgb_minimum_distance|   5|     10000|    1000|0.50095408|  PASSED  
    rgb_permutations|   2|    100000|     100|0.17067531|  PASSED  
    rgb_permutations|   3|    100000|     100|0.96908058|  PASSED  
    rgb_permutations|   4|    100000|     100|0.30686031|  PASSED  
    rgb_permutations|   5|    100000|     100|0.60238853|  PASSED  
      rgb_lagged_sum|   0|   1000000|     100|0.29815321|  PASSED  
      rgb_lagged_sum|   1|   1000000|     100|0.05888328|  PASSED  
      rgb_lagged_sum|   2|   1000000|     100|0.38591346|  PASSED  
      rgb_lagged_sum|   3|   1000000|     100|0.99720537|   WEAK   
      rgb_lagged_sum|   4|   1000000|     100|0.56000320|  PASSED  
      rgb_lagged_sum|   5|   1000000|     100|0.59495219|  PASSED  
      rgb_lagged_sum|   6|   1000000|     100|0.57181183|  PASSED  
      rgb_lagged_sum|   7|   1000000|     100|0.97822465|  PASSED  
      rgb_lagged_sum|   8|   1000000|     100|0.33081657|  PASSED  
      rgb_lagged_sum|   9|   1000000|     100|0.21840503|  PASSED  
      rgb_lagged_sum|  10|   1000000|     100|0.38164870|  PASSED  
      rgb_lagged_sum|  11|   1000000|     100|0.26365058|  PASSED  
      rgb_lagged_sum|  12|   1000000|     100|0.46992255|  PASSED  
      rgb_lagged_sum|  13|   1000000|     100|0.81074416|  PASSED  
      rgb_lagged_sum|  14|   1000000|     100|0.70011030|  PASSED  
      rgb_lagged_sum|  15|   1000000|     100|0.93944357|  PASSED  
      rgb_lagged_sum|  16|   1000000|     100|0.59000862|  PASSED  
      rgb_lagged_sum|  17|   1000000|     100|0.17492244|  PASSED  
      rgb_lagged_sum|  18|   1000000|     100|0.99113748|  PASSED  
      rgb_lagged_sum|  19|   1000000|     100|0.02646569|  PASSED  
      rgb_lagged_sum|  20|   1000000|     100|0.99802592|   WEAK   
      rgb_lagged_sum|  21|   1000000|     100|0.24020339|  PASSED  
      rgb_lagged_sum|  22|   1000000|     100|0.31501205|  PASSED  
      rgb_lagged_sum|  23|   1000000|     100|0.00436336|   WEAK   
      rgb_lagged_sum|  24|   1000000|     100|0.19366485|  PASSED  
      rgb_lagged_sum|  25|   1000000|     100|0.96309059|  PASSED  
      rgb_lagged_sum|  26|   1000000|     100|0.74726130|  PASSED  
      rgb_lagged_sum|  27|   1000000|     100|0.91949279|  PASSED  
      rgb_lagged_sum|  28|   1000000|     100|0.98626739|  PASSED  
      rgb_lagged_sum|  29|   1000000|     100|0.29361452|  PASSED  
      rgb_lagged_sum|  30|   1000000|     100|0.29043267|  PASSED  
      rgb_lagged_sum|  31|   1000000|     100|0.40055467|  PASSED  
      rgb_lagged_sum|  32|   1000000|     100|0.69134587|  PASSED  
     rgb_kstest_test|   0|     10000|    1000|0.88917537|  PASSED  
     dab_bytedistrib|   0|  51200000|       1|0.14967301|  PASSED  
             dab_dct| 256|     50000|       1|0.08364921|  PASSED  
Preparing to run test 207.  ntuple = 0
        dab_filltree|  32|  15000000|       1|0.16804145|  PASSED  
        dab_filltree|  32|  15000000|       1|0.58875029|  PASSED  
Preparing to run test 208.  ntuple = 0
       dab_filltree2|   0|   5000000|       1|0.16228127|  PASSED  
       dab_filltree2|   1|   5000000|       1|0.36241840|  PASSED  
Preparing to run test 209.  ntuple = 0
        dab_monobit2|  12|  65000000|       1|0.17686632|  PASSED  

	ALL TESTS ARE DONE. PLEASE CHECK OUTPUT.
```
