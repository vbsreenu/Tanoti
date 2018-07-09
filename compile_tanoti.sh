#!/bin/bash

# Script for compiling tanoti source code. 
# Sreenu Vattipally

cc tanoti_assembler_1.c -o TANOTI_ASSEMBLER_1
cc tanoti_assembler_2.c -o TANOTI_ASSEMBLER_2
cc tanoti_postprocessor.c -o TANOTI_POSTPROCESSOR
cc tanoti_preprocessor.c -o TANOTI_PREPROCESSOR

cd ..

if [ ! -d bin ]; then 
	mkdir bin
fi

mv src/TANOTI* bin/. 
ln src/tanoti bin/.
ln src/blastall bin/.
ln src/formatdb bin/.
