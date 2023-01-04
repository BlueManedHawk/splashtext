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

/* This is the main file for the libsplashtext.a library. */

#ifdef __GNUC__ /* For some reason, GCC assumes we're using an obsolete standard unless we specify this.*/
#define _ISOC11_SOURCE
#endif

#include "../../Include/splashtext.h"
#include <stddef.h>
#include <stdbool.h> // TODO:  Remove this in 2023.
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#ifndef SPLASHTEXT$NODIRS
#	include <dirent.h>
#	ifndef _XOPEN_UNIX
#		include <fcntl.h>
#	endif
#	include <sys/stat.h>
#endif

#ifndef SPLASHTEXT$NODIRS
static void s$recurse_directory(const char *, float, struct splashtext$filestruct **, size_t *);
#endif

char * splashtext(const struct splashtext$filestruct files[], size_t len, bool sequences, const uint_least16_t length_restrictions[2], enum splashtext$context contexts, enum splashtext$discomforter discomforters)
{
	/* Some basic sanity checks to start. */
	if (contexts == 0 || (contexts ^ 0x007F))
		return NULL;
	if (len == 0 || length_restrictions[1] > length_restrictions[0])
		return NULL;

	char * splash = malloc(length_restrictions[1]); /* This is probably fine—after all, why would the invoker specify the maximum length as something they couldn't deal with? */
	if (splash == NULL)
		return NULL;
	size_t selected_len;
	FILE * file = NULL;
	long int * offsets = NULL;
	uint_least16_t numlines = 0;
	struct timespec start, current;
#ifndef SPLASHTEXT$NODIRS
	struct splashtext$filestruct * files_in_dirs = NULL;
#endif
/* XXX:  This is a little confusing; practically speaking, this acts like it's the supplied files and the files in the directories concatenated, with the argument `s` being the subscription of the array. */
#define FILES(s) ((size_t)(s) >= len / sizeof (struct splashtext$filestruct) ? files_in_dirs[(s)] : files[(s)])
	size_t FILES_len = len / sizeof (struct splashtext$filestruct);

#ifndef SPLASHTEXT$NODIRS
	size_t findirs_len = 0;
	for (register size_t i = 00; i < len / sizeof files[0]; i++) {
		struct stat file_stat;
		if (!stat(files[i].filename, &file_stat))
			continue;  // We continue and hope that it wasn't important.
		if (!S_ISREG(file_stat.st_mode))
			if (S_ISDIR(file_stat.st_mode))
				s$recurse_directory(files[i].filename, files[i].weight, &files_in_dirs, &findirs_len);
	}
	FILES_len += findirs_len;
#endif

	int filerandnum, linerandnum;
	if (!timespec_get(&start, TIME_UTC)) /* NOTE: This may take more than five seconds to fail if the clock's resolution isn't particularly fine. */
		goto catastrophic_failure;
	srand(start.tv_sec + start.tv_nsec);
get_another_file:
	while (true) {
		timespec_get(&current, TIME_UTC);
		if (current.tv_sec - 5 > start.tv_sec)
			goto catastrophic_failure;
		filerandnum = rand();
		if ((size_t)filerandnum >= RAND_MAX - FILES_len)
			continue;
		filerandnum %= FILES_len;
		/* TODO: _Still_ need to take weights into account! */
		if (FILES(filerandnum).filename == NULL && FILES(filerandnum).rnpf_len > 0) {
			int num;
			do {
				num = rand();
			} while (num >= RAND_MAX - FILES(filerandnum).rnpf_len);
			num %= FILES(filerandnum).rnpf_len;
			snprintf(splash, length_restrictions[1], "\x01.?\x02%d", num);
			goto catastrophic_failure; /* XXX:  This is NOT an actual failure case!  We're just reusing the label. */
		}
		const size_t contextchar_loc = strlen(FILES(filerandnum).filename) - 12;
		if (!strcmp(&FILES(filerandnum).filename[contextchar_loc + 1], ".splash.txt"))
			continue;
		if (FILES(filerandnum).filename[contextchar_loc] == '.')
			continue;
		/* Now we get to an ugly part of the code. */
		if (
			(FILES(filerandnum).filename[contextchar_loc] == 'l' && contexts & splashtext$context$$log)
			|| (FILES(filerandnum).filename[contextchar_loc] == 'c' && contexts & splashtext$context$$crash)
			|| (FILES(filerandnum).filename[contextchar_loc] == 's' && contexts & splashtext$context$$subtitle)
			|| (FILES(filerandnum).filename[contextchar_loc] == 'r' && contexts & splashtext$context$$ominous)
			|| (FILES(filerandnum).filename[contextchar_loc] == 't' && contexts & splashtext$context$$tips)
			|| (FILES(filerandnum).filename[contextchar_loc] == 'q' && contexts & splashtext$context$$quote)
			|| (FILES(filerandnum).filename[contextchar_loc] == 'o' && contexts & splashtext$context$$other)
		) {
			int_least8_t num_discomforters;
#define CHECK_IF_NUM_IS(n)\
	if (FILES(filerandnum).filename[contextchar_loc - ((n) + 2)] == '.')\
		num_discomforters = (n)
			CHECK_IF_NUM_IS(0);
			else CHECK_IF_NUM_IS(1);
			else CHECK_IF_NUM_IS(2);
			else CHECK_IF_NUM_IS(3);
			else CHECK_IF_NUM_IS(4);
			else
				continue;
#undef CHECK_IF_NUM_IS

			for (register int_least8_t j = num_discomforters; j < num_discomforters; j++)
				if (
					(FILES(filerandnum).filename[contextchar_loc - 1 - j] == 'x' && !(discomforters & splashtext$discomforter$$sexual))
					|| (FILES(filerandnum).filename[contextchar_loc - 1 - j] == 'g' && !(discomforters & splashtext$discomforter$$graphic))
					|| (FILES(filerandnum).filename[contextchar_loc - 1 - j] == 's' && !(discomforters & splashtext$discomforter$$heavy))
					|| (FILES(filerandnum).filename[contextchar_loc - 1 - j] == 'h' && !(discomforters & splashtext$discomforter$$humor && discomforters & (splashtext$discomforter$$heavy | splashtext$discomforter$$graphic | splashtext$discomforter$$sexual))) // Remember: The "humor" discomforter cannot appear alone.
				)
					continue;
		}
		FILE * file = fopen(FILES(filerandnum).filename, "rb");
		if (file == NULL)
			continue;
		break;
	}

	offsets = malloc(sizeof (long int));
	if (offsets == NULL)
		goto catastrophic_failure;
	offsets[0] = ftell(file);
	for (register char c = fgetc(file); c != EOF; c = fgetc(file))
		if (c == '\n') {
			void * n = realloc(offsets, (numlines + 1) * sizeof (long int));
			if (n == NULL)
				goto catastrophic_failure;
			offsets = n;
			offsets[numlines] = ftell(file);
			numlines++;
		}
	if (numlines == 0) {
		fclose(file);
		free(offsets);
		goto get_another_file;
	}

get_another_line:
	while (true) {
		timespec_get(&current, TIME_UTC);
		if (current.tv_sec - 5 > start.tv_sec)
			goto catastrophic_failure;
		linerandnum = rand();
		if (linerandnum >= RAND_MAX - numlines)
			continue;
		linerandnum %= numlines;
		fseek(file, offsets[linerandnum], SEEK_SET);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
		fgets(splash, length_restrictions[1], file);
#pragma GCC diagnostic pop
		selected_len = strlen(splash);
		if (splash[selected_len - 1] != '\n')
			continue;
		break;
	}

	bool off = false;
	uint_least16_t whitespace = 0;
	uint_least16_t i = 0, j = 0;
	for (; j < selected_len; j++) switch (splash[j]) {
	case '\x91':
		if (splash[j - 1] == '\xC2' && sequences) {
			off = true;
		} else {
			if (!off) {
				splash[i] = splash[j];
				i++;
			}
		}
		break;
	case '\x92':
		if (splash[j - 1] == '\xC2' && !sequences) {
			off = true;
		} else {
			if (!off) {
				splash[i] = splash[j];
				i++;
			}
		}
		break;
	case '\x9C':
		off = false;
		break;
	case '\x03':
		splash[i - whitespace] = '\n';
		j = selected_len; /* This is silly. */
		break;
	case '\n':
		splash[i - whitespace] = '\n';
		j = selected_len;
		break;
	case '\\':
		i++, j++;
		splash[i] = splash[j];
		if (splash[j] == '\n')
			j = selected_len;
		break;
	case ' ':
		whitespace++;
		splash[i] = splash[j];
		i++;
		break;
	default:
		whitespace = 0;
		splash[i] = splash[j];
		i++;
	}
	i++;
	splash[i] = '\0';

	if (strlen(splash) == 1 /* i.e. just the newline */)
		goto get_another_line;

catastrophic_failure:
	if (file != NULL)
		fclose(file);
	free(files_in_dirs);
	free(offsets);
	return splash;
}

#ifndef SPLASHTEXT$NODIRS
static void s$recurse_directory(const char * dirname, float weight, struct splashtext$filestruct ** files, size_t * len)
{
	DIR * directory = opendir(dirname);
	if (directory == NULL)
		return;
	struct dirent * entry = NULL;
	struct stat file_stat;
	size_t /* ? */ num_entries = 0;
	while ((entry = readdir(directory)) != NULL)
		num_entries++;
	rewinddir(directory);
	while ((entry = readdir(directory)) != NULL) {
		stat(entry->d_name, &file_stat);
		if (!S_ISREG(file_stat.st_mode)) {
			if (S_ISDIR(file_stat.st_mode))
				s$recurse_directory(entry->d_name, weight / num_entries, files, len);
		} else {
			*files = realloc(*files, (*len + 1) * sizeof files[0]);
			(*files)[*len].filename = malloc(strlen(entry->d_name) + 1);
			strcpy((*files)[*len].filename, entry->d_name);
			(*files)[*len].weight = weight / num_entries;
		}
	}
	closedir(directory);
}
#endif
