freertos-avr-test
=============

A basic example of running FreeRTOS on an ATmega328p

I *highly* recommend using the MinSizeRel configuration in CMake, otherwise the binary is huge.

Building
--------
```
git clone --recurse-submodules https://github.com/teknoman117/avr
cd avr
# Switch into FreeRTOS examples
cmake -GNinja -DCMAKE_BUILD_TYPE=MinSizeRel -B build .
cmake --build build
```

Upload
------
Uploading to an Arduino Uno R3
```
avrdude -v -p atmega328p -c arduino -P /dev/ttyACM0 -b 115200 -D -U flash:w:build/src/uart-test.hex:i
```

Output
------
Use your favorite serial monitor, mine is picocom
```
picocom -b 115200 /dev/ttyACM0
```

Output should look something like this:
```
[0] I am task 1, I send every 333 ms
[0] I am task 2, I send every 500 ms
[0] I am task 3, I send every 1000 ms
[20] I am task 1, I send every 333 ms
[31] I am task 2, I send every 500 ms
[40] I am task 1, I send every 333 ms
[60] I am task 1, I send every 333 ms
[62] I am task 3, I send every 1000 ms
[62] I am task 2, I send every 500 ms
[80] I am task 1, I send every 333 ms
[93] I am task 2, I send every 500 ms
[100] I am task 1, I send every 333 ms
[120] I am task 1, I send every 333 ms
[124] I am task 3, I send every 1000 ms
[124] I am task 2, I send every 500 ms
[140] I am task 1, I send every 333 ms
[155] I am task 2, I send every 500 ms
[160] I am task 1, I send every 333 ms
[180] I am task 1, I send every 333 ms
[186] I am task 3, I send every 1000 ms
```
