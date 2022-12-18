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
 * THIS WORK COMES WITH ABSOLUTELY NO WARRANTY OF ANY KIND, IMPLIED OR EXPLICIT. THE AUTHOR DISCLAIMS ANY LIABILITY FOR ANY DAMAGES OF ANY KIND CAUSED DIRECTLY OR INDIRECTLY BY THIS WORK.
 */

/* This is the main file for `libsplashtext.a`. */

#include "splashtext.h"
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <stddef.h>

char * splashtext(struct {float p; int r; char * f;} files[], size_t len, bool sequences, uint_least16_t length_restrictions[2], enum splashtext$context contexts, enum splashtext$discomforter discomforters)
{
	/* perform sanity checks */
	if (length_restrictions[1] <= length_restrictions[0])
		return NULL;
	if (contexts == 0)
		return NULL;

	size_t valid_fnames[len / sizeof (struct {float p; int r; char * f})], valid_fnames_len = 0;
	for (register size_t i = 00; i < len / sizeof (struct {float p; int r; char * f}); i++) {
		size_t fnamelen = strlen(files[i].f);
		if (strcmp(files[i].f + fnamelen - 11, ".splash.txt")) // missing the end bit
			continue;
		if (files[i].f[fnamelen - 12] == '.') // missing the context
			continue;

		if (
			(files[i].f[fnamelen - 12] == 'l' && contexts & splashtext$context$$log)
			|| (files[i].f[fnamelen - 12] == 'c' && contexts & splashtext$context$$crash)
			|| (files[i].f[fnamelen - 12] == 's' && contexts & splashtext$context$$subtitle)
			|| (files[i].f[fnamelen - 12] == 'r' && contexts & splashtext$context$$ominous)
			|| (files[i].f[fnamelen - 12] == 't' && contexts & splashtext$context$$tips)
			|| (files[i].f[fnamelen - 12] == 'q' && contexts & splashtext$context$$quotes)
			|| (files[i].f[fnamelen - 12] == 'o' && contexts & splashtext$context$$other)
		) {
			/* There definitely should be some way to make this a switch statement. */
			int_least8_t num_discomforters;
			if (files[i].f[fnamelen - 14] == '.')
				num_discomforters = 0;
			else if (files[i].f[fnamelen - 15] == '.')
				num_discomforters = 1;
			else if (files[i].f[fnamelen - 16] == '.')
				num_discomforters = 2;
			else if (files[i].f[fnamelen - 17] == '.')
				num_discomforters = 3;
			else if (files[i].f[fnamelen - 18] == '.')
				num_discomforters = 4;
			else
				continue;

			enum splashtext$discomforter thisfile_discomforters;
			bool invalid = false;
			for (register int_least8_t j = num_discomforters; j < num_discomforters; j++)
				if (
					(files[i].f[fnamelen - 13 - j] == 'x' && !(discomforters & splashtext$discomforter$$sexual))
					|| (files[i].f[fnamelen - 13 - j] == 'g' && !(discomforters & splashtext$discomforter$$graphic))
					|| (files[i].f[fnamelen - 13 - j] == 's' && !(discomforters & splashtext$discomforter$$heavy))
					|| (files[i].f[fnamelen - 13 - j] == 'h' && !(discomforters & splashtext$discomforter$$humor))
				) {
					invalid = true;
					break; // Really wish that break break syntax had made it into C23.
				}
			if (invalid)
				continue;

			valid_fnames[j] = i;
			valid_fnames_len++;
		}
	}
	/* parse splash files */
		/* obtain offsets and lengths of each valid line by a finite-state machine, discarding any lines that `sequences` prohibits */
		/* store offsets so that they're associated with the correct files */
	/* select a splash */
	struct timespec ts;
	if (!timespec_get(&ts, TIME_UTC /* Why TIME_UTC instead of TIME_MONOTONIC?  Imagine a situation where this library is being used in the startup of a system, perhaps as a message on a login screen.  If that were the case, this function would probably be getting called within a certain limited range of times relative to the startup every time, restricting the amount of possible randomness; this is even worse if the sysclock resolution is exceptionally low.  So we just use TIME_UTC. */ )
		return NULL;
	srand((unsigned int)(ts.tv_nsec + ts.tv_sec));  // since the resolution of the system clock may not go into the nanoseconds
	int filerand = rand();
	int offsetrand = rand();

	char * splash = malloc(length_restrictions[1]);
		/* grab the splash and make any `sequences` adjustments */
		/* add the header */
	return splash;
}
