AVRcam Release 1.4
1/16/2005

Whats New in v1.4:
-Greatly improved the issue where the AVRcam would potentially
lock up in particular situations (especially after lots of frame
dumps). This was due to a combination of bugs, which have all been fixed:
   -turned off interrupts in certain critical sections that were missed
    previously
   -only allow PCLK to be piped into timer1 when needed (i.e., when dumping
    a line or tracking a frame).
   -allowed the main executive to check both the fast event bitmask as well
    as the normal event queue on each pass of the main executive

I'll be curious to hear if this has completely fixed the issue (I haven't
seen it happen after ~500 frame dumps, but the real test is when
others try it  :-).  Also note:  an input voltage below 6 volts will
cause the system to work sometimes and lock up others.  The input voltage
regulator really wants to see a 6V minimum.  I had drained
8 AA NiMH batteries down to where they were only providing ~4.8 V at the
input.  The system worked for a little while, and then started locking
up.  I immediately changed batteries and everything was fine.

-Improved how the colorMap gets written to EEPROM.  Now, before
each byte in the colorMap is written to EEPROM, a check of the
current value in EEPROM is made to see if the old value matches
the new value.  If so, the write is skipped.  Also, after a value
is written, it is read back to ensure that it is correct, and 
will re-write up to three times if needed.

-Moved the UART rx ISR to the UartInterface.c file, instead of having
it in its own UartInterfaceAsm.S file.  The ISR is in C now.

-Added a revision history to each source file so that people can
see the changes on a per-version basis.


---Misc Info about AVRcam---

* The AVRcam source files were built using the WinAVR distribution
(version 3.3.1 of GCC).  I haven't tested other versions of GCC,
but they should compile without too much difficulty.  The makefile
used to build the source is included.

* Currently, the AVRcam only works at a baud rate of 115.2 kbps.  I
am in the process of working on autobaud routines that will set the
baud rate automatically at start up.  I'm pretty sure the system 
should be able to track 8 objects all the way down to a baud rate
of 38.4 kbps.  Check back to www.jrobot.net for more info about the
AVRcam.

* Different portions of the code are #ifdef'ed in or out to show
the various debugging that can be turned on in the system.  The
debugging mechanisms are typically for dumping additional information
out to a serial terminal for viewing, and are not intended to work
with the AVRcamVIEW PC application.  The code, as released, doesn't
have any debugging features turned on.

* For more info about the AVRcam, check out http://www.jrobot.net

John Orlando
January 16, 2005