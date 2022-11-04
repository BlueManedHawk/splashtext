# LICENSE
#
# Copyright © 2022 Blue-Maned_Hawk. All rights reserved.
#
# You may freely use this work for any purpose, freely make this work available to others by any means, freely modify this work in any way, and freely make works derived from this work available to others by any means.
#
# Should you choose to exercise any of these rights, you must give clear and conspicuous attribution to the original author, and you must not make it seem in any way like the author condones your act of exercising these rights in any way.
#
# Should you choose to exercise the second right listed above, you must make this license clearly and conspicuously available along with the original work, and you must clearly and conspicuously make the information necessary to reconstruct the work available along with the work.
#
# Should you choose to exercise the fourth right listed above, you must put any derived works you construct under a license that grants the same rights as this one under the same conditions and with the same restrictions, you must clearly and conspicuously make that license available alongside the work, you must clearly and conspicuously make the information necessary to reconstruct the work available alongside the work, you must clearly and conspicuously describe the changes which have been made from the original work, and you must not make it seem in any way like your derived works are the original work in any way.
#
# This license only applies to the copyright of this work, and does not apply to any other intellectual property rights, including but not limited to patent and trademark rights.
#
# THIS WORK COMES WITH ABSOLUTELY NO WARRANTY OF ANY KIND, IMPLIED OR EXPLICIT. THE AUTHOR DISCLAIMS ANY LIABILITY FOR ANY DAMAGES OF ANY KIND CAUSED DIRECTLY OR INDIRECTLY BY THIS WORK.

CFLAGS = -Wall -Werror -Wextra -Wpedantic -g -Og
SRC_CLI = $(wildcard Source/Program/*.c)
OBJ_CLI = $(SRC_CLI:.c=.o)
SRC_LIB = $(wildcard Source/Library/*.c)
OBJ_LIB = $(SRC_LIB:.c=.o)

help:
	@echo "Please specify what you want to do.  Available options are \"build\", \"clean\", and \"install\" (that last one requires superuser priveleges)."

%.o: %.c
	$(CC) -c $(CFLAGS) $^

splashtext: $(OBJ_CLI)
	$(CC) $^ -o "splashtext"

libsplashtext.a: $(OBJ_LIB)
	$(AR) $(ARFLAGS) libsplashtext.a $^

build: splashtext libsplashtext.a

clean:
	cd Source/Program; rm *.o
	cd Source/Library; rm *.o

install: build
	@if [ $$USER = root ] ; then\
		cp splashtext /usr/local/games/splashtext;\
		if [ ! -e /usr/local/share/splashtext/ ]; then\
			mkdir -p /usr/local/share/splashtext/; fi\
		cp -r Assets/*.splash.txt /usr/local/share/splashtext/;\
		cp splashtext.h /usr/local/include/splashtext.h;\
		cp libsplashtext.a /usr/local/lib/libsplashtext.a;\
		cp Documentation/splashtext.6 /usr/local/man/man6/splashtext.6; gzip -9f /usr/local/man/man6/splashtext.6;\
		ls -s /usr/local/man/man7/splashtext.7 /usr/local/man/man6/splashtext.6;\
		ls -s /usr/local/man/man3/splashtext.3 /usr/local/man/man6/splashtext.6;\
		ls -s /usr/local/man/man5/splashtext.5 /usr/local/man/man6/splashtext.6;\
		ls -s /usr/local/man/man1/splashtext.1 /usr/local/man/man6/splashtext.6;\
	else echo "You need superuser priveleges to install Splash Text."; fi

.PHONY: build clean install
