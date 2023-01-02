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

/* This is the header file for libsplashtext.  See the manpage `splashtext(6)` for documentation. */

#ifndef SPLASHTEXT_H
#define SPLASHTEXT_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#pragma clang attribute push (__attribute__ ((__flag_enum__)), apply_to = enum) /* Only Clang has the __flag_enum__ attribute, so we don't need to worry about how to apply the attribute when GCC is used. */
enum splashtext$context {
	splashtext$context$$log = 0x0001,
	splashtext$context$$crash = 0x0002,
	splashtext$context$$subtitle = 0x0004,
	splashtext$context$$ominous = 0x0008,
	splashtext$context$$tips = 0x0010,
	splashtext$context$$quote = 0x0020,
	splashtext$context$$other = 0x0040
};

enum splashtext$discomforter {
	splashtext$discomforter$$sexual = 0x0001,
	splashtext$discomforter$$graphic = 0x0002,
	splashtext$discomforter$$heavy = 0x0004,
	splashtext$discomforter$$humor = 0x0008
};
#pragma clang attribute pop

struct splashtext$filestruct {
	float weight;
	int rnpf_len; // `rnpf` for "*r*and*n*um *p*seudo*f*ile""
	char * filename;
};

extern char * splashtext(const struct splashtext$filestruct[], size_t, bool, const uint_least16_t[2], enum splashtext$context, enum splashtext$discomforter);

#endif/*ndef SPLASHTEXT_H*/
