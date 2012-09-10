#!/bin/bash

for dir in $(find . -type d); do
echo "$dir/*.hex"
rm -rvf "$dir/.DS_Store"
rm -rvf "$dir/.dep"
rm -rvf "$dir/*.o"
rm -rvf "$dir/*.hex"
rm -rvf "$dir/*.d"
rm -rvf "$dir/*.lst"
rm -rvf "$dir/*.eep"
rm -rvf "$dir/*.elf"
rm -rvf "$dir/*.lss"
rm -rvf "$dir/*.map"
rm -rvf "$dir/*.sym"
rm -rvf "$dir/*.a"
done

