#!/bin/bash
cd dev
diff -ru gcc-4.4.0-orig gcc-4.4.0 | grep -v "Only in gcc-4.4.0" > ../gcc-4.4.0-tms9900-1.26.patch
diff -ru binutils-2.19.1-orig binutils-2.19.1 | grep -v "Only in binutils-2.19.1" > ../binutils-2.19.1-tms9900-1.10.patch
cd ..

