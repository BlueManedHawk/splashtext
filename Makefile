# LICENSE
#
# Copyright © 2022 Blue-Maned_Hawk. All rights reserved.
#
# You may freely use this work for any purpose, freely make this work available to others by any means, freely modify this work in any way, and freely make works derived from this work available to others by any means.
#
# Should you choose to exercise any of these rights, you must give clear and conspicuous attribution to the original author, and you must not make it seem in any way like the author condones your act of exercising these rights in any way.
#
# Should you choose to exercise the second right listed above, you must make this license clearly and conspicuously available along with the original work, and you must clearly and conspicuously make the information necessary to reconstruct the work available along with the work.  #
# Should you choose to exercise the fourth right listed above, you must put any derived works you construct under a license that grants the same rights as this one under the same conditions and with the same restrictions, you must clearly and conspicuously make that license available alongside the work, you must clearly and conspicuously make the information necessary to reconstruct the work available alongside the work, you must clearly and conspicuously describe the changes which have been made from the original work, and you must not make it seem in any way like your derived works are the original work in any way.
#
# This license only applies to the copyright of this work, and does not apply to any other intellectual property rights, including but not limited to patent and trademark rights.
#
# THIS WORK COMES WITH ABSOLUTELY NO WARRANTY OF ANY KIND, IMPLIED OR EXPLICIT. THE AUTHOR DISCLAIMS ANY LIABILITY FOR ANY DAMAGES OF ANY KIND CAUSED DIRECTLY OR INDIRECTLY BY THIS WORK.
.POSIX:

CFLAGS = -Wall -Werror -Wextra -Wpedantic -g -Og\
	-Wno-error=unknown-pragmas

# See the `build` target below—we do this because POSIX doesn't define what happens when both macros are defined and a specific target is selected in the arguments to `make`.
bootstrap: $(TARGET)
	@if [ -z "$(TARGET)" ] ; then\
		make TARGET=help ; fi

.c.o:
	cd ` dirname $< ` ;\
	$(CC) -c $(CFLAGS) ` basename $< `

# See the `build` target below for where the `SRC` macro comes from.
splashtext: $(SRC:.c=.o)
	$(CC) $^ -L. -lsplashtext $(LDFLAGS) -o "splashtext"

libsplashtext.a: $(SRC:.c=.o)
	$(AR) $(ARFLAGS) libsplashtext.a $^

help:
	@echo "Please specify what you want to do.  Available options are \"build\", \"clean\", and \"install\" (that last one requires superuser priveleges)."

build:
	make TARGET=libsplashtext.a SRC=`ls ./Source/Library/ | awk -F ' ' '/.*\.c/ { printf "./Source/Library/" $$0 }' `
	make TARGET=splashtext SRC=` ls ./Source/Program/ | awk -F ' ' '/.*\.c/ { printf "./Source/Program/" $$0 }' `

clean:
	cd Source/Program; rm *.o
	cd Source/Library; rm *.o

# Note:  There has got to be a better way to get a failed symlinking from a preexisting file to not fail the whole installation.
install: splashtext libsplashtext.a
	@if [ $$USER = root ] ; then\
		cp splashtext /usr/local/games/splashtext ;\
		if [ ! -e /usr/local/share/splashtext/ ] ; then\
			mkdir -p /usr/local/share/splashtext/ ; fi ;\
		cp -r Assets/*.splash.txt /usr/local/share/splashtext/ ;\
		cp Include/splashtext.h /usr/local/include/splashtext.h ;\
		cp libsplashtext.a /usr/local/lib/libsplashtext.a ;\
		cp Documentation/splashtext.6 /usr/local/man/man6/splashtext.6 ;\
		true ` ln -s /usr/local/man/man6/splashtext.6 /usr/local/man/man7/splashtext.7 2> /dev/null ` ;\
		true ` ln -s /usr/local/man/man6/splashtext.6 /usr/local/man/man3/splashtext.3 2> /dev/null ` ;\
		true ` ln -s /usr/local/man/man6/splashtext.6 /usr/local/man/man5/splashtext.5 2> /dev/null ` ;\
		true ` ln -s /usr/local/man/man6/splashtext.6 /usr/local/man/man1/splashtext.1 2> /dev/null ` ;\
		echo "It seems like everything worked.  Splashtext should have been installed successfully." ;\
	else echo "You need superuser priveleges to install Splash Text." ; fi

.PHONY: build clean install help
