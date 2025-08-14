# $Id: Makefile,v 1.9 2007/08/02 01:17:11 bsd Exp $

TARGET = servo

CPU = atmega128

SRCS = main.c

OBJS = main.o


# NOTE: if you enable external RAM, the diag_sram command will hang or
# reset the controller board

EXTERNAL_RAM = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x80ffff
EXTERNAL_RAM = 


OPTIMIZE = -O
CFLAGS   = -g -mmcu=${CPU} -Wall ${OPTIMIZE}
LDFLAGS  = ${EXTERNAL_RAM} -Wl,-u,vfprintf -lprintf_flt -lm


CC     = avr-gcc

all : depend
	make ${TARGET}

${TARGET} : ${TARGET}.hex

.c.s :
	avr-gcc -c ${CFLAGS} -S -o $@ $<

${TARGET}.hex : ${OBJS}
	avr-gcc -mmcu=${CPU} ${LDFLAGS} -o ${TARGET} ${OBJS} ${LIBDIRS} ${LIBS}
	avr-objcopy -O ihex ${TARGET} ${TARGET}.hex
	avr-objcopy -O srec ${TARGET} ${TARGET}.srec
	avr-objcopy -O binary ${TARGET} ${TARGET}.bin
	@ls -l ${TARGET}.bin

clean :
	rm -f *.o *.bin *.s *~ .depend Makefile.bak *.srec ${TARGET}

assembly :
	for i in ${OBJS}; do make $${i%%.*}.s; done

depend :
	@cp Makefile Makefile.bak
	@awk '/# .Id/,/^# DO NOT DELETE/' Makefile > Makefile.new
	@${CC} ${CFLAGS} -MM ${SRCS} >> Makefile.new
	@if ! diff Makefile Makefile.new > /dev/null 2>&1 ; then \
	  mv Makefile.new Makefile; \
	else \
	  rm Makefile.new; \
	  rm Makefile.bak; \
	fi

# DO NOT DELETE (Dependencies follow)
main.o: main.c
