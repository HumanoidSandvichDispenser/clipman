PREFIX ?= /usr/local

x11_bsd_flags = -I/usr/X11R6/include -L/usr/X11R6/lib

CC = gcc
CXX = g++
CFLAGS = -g -lX11 -lXfixes
CXXFLAGS = -g -lX11 -lXfixes -lssl -lcrypto
OUT = clipman

all: $(OUT)

$(OUT): main.o clipboard.o utils.o
	${CXX} ${CXXFLAGS} ${LDFLAGS} -o $(OUT) $(x11_bsd_flags) $^

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c $^ -o $@ $(x11_bsd_flags)

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f $(OUT) ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/$(OUT)

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/$(OUT)

clean:
	rm -f *.o *~ $(OUT)
