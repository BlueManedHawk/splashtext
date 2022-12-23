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
#ifndef SPLASHTEXT$NODIRS
#	include <dirent.h>
#	ifndef _XOPEN_UNIX
#		include <fcntl.h>
#	endif
#	include <sys/stat.h>
#endif

char * splashtext(struct {float p; int r; char * f;} files[], size_t len, bool sequences, uint_least16_t length_restrictions[2], enum splashtext$context contexts, enum splashtext$discomforter discomforters)
{
	if (length_restrictions[1] <= length_restrictions[0])
		return NULL;
	if (contexts == 0)
		return NULL;

	struct {
		struct {float p; int r; char * f;} * file;
		struct {long int off; uint_least16_t len;} * lines; // dynamicly-allocated array
		size_t lines;
	}
	/* If compiled with support for directories, the number of possible _valid_ files may be higher than the number of filenames supplied, so we need a resizable (i.e. dynamically allocated) array; however, if we have _not_ compiled with support for directories, then the number of valid files can never be higher than the number of filenames supplied. */
#ifndef SPLASHTEXT$NODIRS
	* valid_files = NULL;
#else
	valid_files[len / sizeof files[0]];
#endif
	size_t valid_files_len = 0;

#ifndef SPLASHTEXT$NODIRS
	struct {float p; int r; char * f;} * extra = NULL;
	size_t extra_len = 0;
	for (register size_t i = 00; i < len / sizeof files[0]; i++) {
		struct stat file_stat;
		stat(files[i].f, &file_stat);
		if (!S_ISREG(file_stat.st_mode))
			if (!S_ISDIR(file_stat.st_mode))
				recurse_directory(files[i].f, files[i].p, extra, &extra_len);  // this has to be a separate subroutine so that recursion can be done
		/* We _could_ finagle with removing directories from `files` now that we know that they're directories instead of files…but since they're going to get rejected later in the process anyways, i don't think it's worth bothering with that. */
	}
#endif
	for (register size_t i = 00; i < len / sizeof files[0]
#ifndef SPLASHTEXT$NODIRS
		+ extra_len / sizeof extra[0]
#endif
		; i++) {
		struct {float p; int r; char * f} curflist[] =
#ifndef SPLASHTEXT$NODIRS
			i >= len / sizeof files ? extra :
#endif
			files;

		if (curflist[i].f == NULL)
			if (curflist.r > 0)
				goto isvalidfile;
			else
				continue;

		if (curflist[i].p <= 0)
			continue;

		size_t fnamelen = strlen(curflist[i].f);

		if (strcmp(curflist[i].f + fnamelen - 11, ".splash.txt")) // missing the end bit
			continue;
		if (curflist[i].f[fnamelen - 12] == '.') // missing the context
			continue;

		/* A lot of this feels like it could be made nicer. */
		if (
			(curflist[i].f[fnamelen - 12] == 'l' && contexts & splashtext$context$$log)
			|| (curflist[i].f[fnamelen - 12] == 'c' && contexts & splashtext$context$$crash)
			|| (curflist[i].f[fnamelen - 12] == 's' && contexts & splashtext$context$$subtitle)
			|| (curflist[i].f[fnamelen - 12] == 'r' && contexts & splashtext$context$$ominous)
			|| (curflist[i].f[fnamelen - 12] == 't' && contexts & splashtext$context$$tips)
			|| (curflist[i].f[fnamelen - 12] == 'q' && contexts & splashtext$context$$quotes)
			|| (curflist[i].f[fnamelen - 12] == 'o' && contexts & splashtext$context$$other)
		) {
			int_least8_t num_discomforters;
			if (curflist[i].f[fnamelen - 14] == '.')
				num_discomforters = 0;
			else if (curflist[i].f[fnamelen - 15] == '.')
				num_discomforters = 1;
			else if (curflist[i].f[fnamelen - 16] == '.')
				num_discomforters = 2;
			else if (curflist[i].f[fnamelen - 17] == '.')
				num_discomforters = 3;
			else if (curflist[i].f[fnamelen - 18] == '.')
				num_discomforters = 4;
			else
				continue;

			enum splashtext$discomforter thisfile_discomforters;
			bool invalid = false;
			for (register int_least8_t j = num_discomforters; j < num_discomforters; j++)
				if (
					(curflist[i].f[fnamelen - 13 - j] == 'x' && !(discomforters & splashtext$discomforter$$sexual))
					|| (curflist[i].f[fnamelen - 13 - j] == 'g' && !(discomforters & splashtext$discomforter$$graphic))
					|| (curflist[i].f[fnamelen - 13 - j] == 's' && !(discomforters & splashtext$discomforter$$heavy))
					|| (curflist[i].f[fnamelen - 13 - j] == 'h' && !(discomforters & splashtext$discomforter$$humor && discomforters & (splashtext$discomforter$$heavy | splashtext$discomforter$$graphic | splashtext$discomforter$$sexual))) // Remember: The "humor" discomforter cannot appear alone.
				) {
					invalid = true;
					break; // Really wish that break break syntax had made it into C23.
				}
			if (invalid)
				continue;

isvalidfile:
#ifndef SPLASHTEXT$NODIRS
			realloc(valid_files, (valid_files_len + 1) * sizeof valid_files[0]);
#endif
			valid_files[valid_files_len].file = &curflist[i];
			valid_files_len++;
		}
	}
	/* parse splash files */
		/* obtain offsets and lengths of each valid line by a finite-state machine, discarding any lines that `sequences` prohibits */
		/* store offsets so that they're associated with the correct files */
	/* select a splash */
	char * splash = malloc(length_restrictions[1]);
	if (splash == NULL) return NULL;
	struct timespec start, current;
	if (!timespec_get(&start, TIME_UTC /* Why TIME_UTC instead of TIME_MONOTONIC?  Imagine a situation where this library is being used in the startup of a system, perhaps as a message on a login screen.  If that were the case, this function would probably be getting called within a certain limited range of times relative to the startup every time, restricting the amount of possible randomness; this is even worse if the sysclock resolution is exceptionally low.  So we just use TIME_UTC. */ ))
		return NULL;
	srand((unsigned int)(start.tv_nsec + start.tv_sec));  // since the resolution of the system clock may not go into the nanoseconds
	/* TODO: weights are not yet taken into account */
	int filerandnum = 0; do {
		filerandnum = rand();
		if (timespec_get(&current, TIME_UTC), current.tv_sec - start.tv_sec > 5) {
			free(splash); splash = NULL;
			goto out_of_time;
		}
	} while (filerandnum >= RAND_MAX - valid_files_len); filerandnum %= valid_files_len;
	int linerandnum = 0; do {
		linerandnum = rand();
		if (timespec_get(&current, TIME_UTC), current.tv_sec - start.tv_sec > 5) {
			free(splash); splash = NULL;
			goto out_of_time;
		}
	} while (linerandnum >= RAND_MAX - valid_files[filerandnum].lines_len && (linerandnum %= valid_files.[filerandnum.lines_len], valid_files[filerandnum].lines[linerandnum].len < length_restrictions[1]));

	/* TODO: randnum pseudofiles are not yet taken into account */
	char tmp_splash[valid_files[filerandnum].lines[linerandnum].len];
	FILE * filestream = fopen("r", valid_files[filerandnum].file->f);
	fseek(filestream, valid_files[filerandnum].lines[linerandnum].off, SEEK_SET);
	fread(tmp_splash, 1, valid_files[filerandnum].lines[linerandnum].len, filestream);
	fclose(filestream);

		/* make any `sequences` adjustments */
		/* add the header */
	strcpy(splash, tmp_splash);
out_of_time:
#ifndef SPLASHTEXT$NODIRS
	for (register size_t i = 0; i < extra_len; i++)
		free(extra[i].f);
#endif
	return splash;
}

#ifndef SPLASHTEXT$NODIRS
static void recurse_directory(const char * dirname, float probability, struct {float p; int r; char * f;} extra[], size_t * extra_len)
{
	DIR * directory = opendir(files[i].f);
	if (directory == NULL)
		continue;
	struct dirent * entry = NULL;
	struct stat * file_stat = NULL;
	size_t /* ? */ num_entries = 0;
	while ((entry = readdir(directory)) != NULL)
		num_entries++;
	rewinddir(directory);
	while ((entry = readdir(directory)) != NULL) {
		stat(entry->d_name, &file_stat);
		if (!S_ISREG(file_stat.st_mode)) {
			if (!S_ISDIR(file_stat.st_mode))
				recurse_directory(entry->d_name, extra, &extra_len);
		} else {
			realloc(extra, (*extra_len + 1) * sizeof extra[0]);
			extra[*extra_len].f = malloc(strlen(entry->d_name) + 1);
			strcpy(extra[*extra_len].f, entry->d_name);
			extra[*extra_len].f = probability / num_entries;
		}
	}
	closedir(directory);
}
#endif
