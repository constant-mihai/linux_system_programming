#!/bin/bash
###########################################
# File Name : block_copies.sh
#
# Creation Date : 14-09-2018
#
# Last Modified :
#
# Created By : Mihai Constantin constant.mihai@googlemail.com
###########################################

# The book has the following table:
# Block size  |  Real time  |  User Time  |  System Time
#-------------------------------------------------------
# 1 byte      |  18.707 s   |  1.118 s    |  17.549 s
#-------------------------------------------------------
# 1024 bytes  |  0.025  s   |  0.002 s    |  0.023 s
#-------------------------------------------------------
# 1130 bytes  |  0.035 s   |  0.002 s    |  0.027 s

# Different computers will have different execution times.

echo "dd if=random.txt of=copy.txt bs=1 count=2097152"
time dd if=random.txt of=copy.txt bs=1 count=2097152
echo "dd if=random.txt of=copy.txt bs=1024 count=2048"
time dd if=random.txt of=copy.txt bs=1024 count=2048
echo "dd if=random.txt of=copy.txt bs=1130 count=2048"
time dd if=random.txt of=copy.txt bs=1130 count=1856 # 2097152 (nr of bytes we want to copy) / 1130 (copy block)
