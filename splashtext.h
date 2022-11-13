/* LICENSE
 *
 * Copyright © 2022 Blue-Maned_Hawk. All rights reserved.
 *
 * You may freely use this work for any purpose, freely make this work available to others by any means, freely modify this work in any way, and freely make works derived from this work available to others by any means.
 *
 * Should you choose to exercise any of these rights, you must give clear and conspicuous attribution to the original author, and you must not make it seem in any way like the author condones your act of exercising these rights in any way.
 *
 * Should you choose to exercise the second right listed above, you must make this license clearly and conspicuously available along with the original work, and you must clearly and conspicuously make the information necessary to reconstruct the work available along with the work.
 *
 * Should you choose to exercise the fourth right listed above, you must put any derived works you construct under a license that grants the same rights as this one under the same conditions and with the same restrictions, you must clearly and conspicuously make that license available alongside the work, you must clearly and conspicuously make the information necessary to reconstruct the work available alongside the work, you must clearly and conspicuously describe the changes which have been made from the original work, and you must not make it seem in any way like your derived works are the original work in any way.
 *
 * This license only applies to the copyright of this work, and does not apply to any other intellectual property rights, including but not limited to patent and trademark rights.
 *
 * THIS WORK COMES WITH ABSOLUTELY NO WARRANTY OF ANY KIND, IMPLIED OR EXPLICIT. THE AUTHOR DISCLAIMS ANY LIABILITY FOR ANY DAMAGES OF ANY KIND CAUSED DIRECTLY OR INDIRECTLY BY THIS WORK. */

/* This is the header file for libsplashtext.  See the manpage `splashtext(6)` for information on the C language API of Splash Text. */

#ifndef SPLASHTEXT_H
#define SPLASHTEXT_H

#include <stddef.h>

#ifdef __clang__
__attribute__ ((flag_enum))
#endif
enum splashtext {
	splashtext$$version = 0x0001,
	splashtext$$list = 0x0002,
	splashtext$$sequences = 0x0004,
	/* Where is 0x0008? */

	splashtext$$context$log = 0x0010,
	splashtext$$context$crash = 0x0020,
	splashtext$$context$subtitle = 0x0040,
	splashtext$$context$ominous = 0x0080,
	splashtext$$context$tips = 0x0100,
	splashtext$$context$quote = 0x0200,
	splashtext$$context$other = 0x0400,

	splashtext$$content$sexual = 0x0800,
	splashtext$$content$graphic = 0x1000,
	splashtext$$content$heavy = 0x2000,
	splashtext$$content$humor = 0x4000
}

extern char * splashtext(enum splashtext, int[2], char *[], size_t); 

#endif/*ndef SPLASHTEXT_H*/
