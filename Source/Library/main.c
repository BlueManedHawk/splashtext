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

struct splashtext$static$counted_line {
	long int off;
	uint_least16_t len;
};

/* TODO:  There are a lot of situations where it would make more sense to use `typeof` than explicitly specifying the type.  Once 2023 comes about, let's do that. */
char * splashtext(const struct splashtext$filestruct files[], size_t len, bool sequences, const uint_least16_t length_restrictions[2], enum splashtext$context contexts, enum splashtext$discomforter discomforters)
{
	if (length_restrictions[1] <= length_restrictions[0])
		return NULL;
	if (contexts == 0)
		return NULL;

	struct {
		struct splashtext$filestruct * file;
		struct splashtext$static$counted_line * lines; // dynamicly-allocated array
		size_t lines_len;
	}
	/* If compiled with support for directories, the number of possible _valid_ files may be higher than the number of filenames supplied, so we need a resizable (i.e. dynamically allocated) array; however, if we have _not_ compiled with support for directories, then the number of valid files can never be higher than the number of filenames supplied. */
#ifndef SPLASHTEXT$NODIRS
	* valid_files = NULL;
#else
	valid_files[len / sizeof files[0]];
#endif
	size_t valid_files_len = 0;

#ifndef SPLASHTEXT$NODIRS
	struct splashtext$filestruct * extra = NULL;
	size_t extra_len = 0;
	for (register size_t i = 00; i < len / sizeof files[0]; i++) {
		struct stat file_stat;
		if (!stat(files[i].f, &file_stat))
			continue;  // We continue and hope that it wasn't important.
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
		struct splashtext$filestruct curflist[] =
#ifndef SPLASHTEXT$NODIRS
			i >= len / sizeof files ? extra :
#endif
			files;

		if (curflist[i].filename == NULL)
			if (curflist.rnpf_len > 0)
				goto isvalidfile;
			else
				continue;
		if (curflist[i].weight <= 0)
			continue;

		const size_t contextchar_loc = strlen(curflist[i].f) - 12;
		if (strcmp(curflist[i].filename + contextchar_loc + 1, ".splash.txt")) // missing the end bit
			continue;
		if (curflist[i].filename[contextchar_loc] == '.') // missing the context
			continue;

		/* A lot of this feels like it could be made nicer. */
		if (
			(curflist[i].filename[contextchar_loc] == 'l' && contexts & splashtext$context$$log)
			|| (curflist[i].filename[contextchar_loc] == 'c' && contexts & splashtext$context$$crash)
			|| (curflist[i].filename[contextchar_loc] == 's' && contexts & splashtext$context$$subtitle)
			|| (curflist[i].filename[contextchar_loc] == 'r' && contexts & splashtext$context$$ominous)
			|| (curflist[i].filename[contextchar_loc] == 't' && contexts & splashtext$context$$tips)
			|| (curflist[i].filename[contextchar_loc] == 'q' && contexts & splashtext$context$$quotes)
			|| (curflist[i].filename[contextchar_loc] == 'o' && contexts & splashtext$context$$other)
		) {
			int_least8_t num_discomforters;
			if (curflist[i].filename[contextchar_loc - 2] == '.')
				num_discomforters = 0;
			else if (curflist[i].filename[contextchar_loc - 3] == '.')
				num_discomforters = 1;
			else if (curflist[i].filename[contextchar_loc - 4] == '.')
				num_discomforters = 2;
			else if (curflist[i].filename[contextchar_loc - 5] == '.')
				num_discomforters = 3;
			else if (curflist[i].filename[contextchar_loc - 6] == '.')
				num_discomforters = 4;
			else
				continue;

			enum splashtext$discomforter thisfile_discomforters;
			bool invalid = false;
			for (register int_least8_t j = num_discomforters; j < num_discomforters; j++)
				if (
					(curflist[i].filename[contextchar_loc - 1 - j] == 'x' && !(discomforters & splashtext$discomforter$$sexual))
					|| (curflist[i].filename[contextchar_loc - 1 - j] == 'g' && !(discomforters & splashtext$discomforter$$graphic))
					|| (curflist[i].filename[contextchar_loc - 1 - j] == 's' && !(discomforters & splashtext$discomforter$$heavy))
					|| (curflist[i].filename[contextchar_loc - 1 - j] == 'h' && !(discomforters & splashtext$discomforter$$humor && discomforters & (splashtext$discomforter$$heavy | splashtext$discomforter$$graphic | splashtext$discomforter$$sexual))) // Remember: The "humor" discomforter cannot appear alone.
				) {
					invalid = true;
					break; // Really wish that break break syntax had made it into C23.
				}
			if (invalid)
				continue;

isvalidfile:
#ifndef SPLASHTEXT$NODIRS
			void * n = realloc(valid_files, (valid_files_len + 1) * sizeof valid_files[0]);
			if (n == NULL)
				goto exit_failurously;
			else
				valid_files = n;
#endif
			valid_files[valid_files_len].file = &curflist[i];
			valid_files_len++;
		}
	}

	/* parse splash files */
	enum { // NOTE:  Once 2023 comes about, this has got to be a uint_least8_t.  XXX:  This stack gets used later on in the code.
		stack$$comment,
		stack$$sequences,
		stack$$nosequences
	} stack[] = NULL;
	size_t stack_len = 0;
#define PUSHSTACK(n) do {\
	stack_len++;\
	void * n = realloc(stack, stack_len * sizeof stack[0]);\
	if (n == NULL) {\
		goto exit_failurously;\
	}\
	stack[stack_len - 1] = n;\
} while (false)
#define POPSTACK() do {\
	stack_len--;\
	if (stack_len == 0) {\
		/* since reallocing to 0 is undefined… */\
		free(stack);\
		stack = NULL;\
	} else {\
		void * n = realloc(stack, stack_len * sizeof stack[0]);\
		if (n == NULL)\
			goto exit_failurously;\
	}\
} while (false)
#define STACKTOP stack[stack_len - 1]

	for (register size_t i = 0; i < valid_files_len; i++) {
		FILE * file = fopen(valid_files[i].file->filename, "rb");
		bool invalid_file = false;
		uint_least16_t llen = 0;
		bool potentially_sequence_prohibited = false;
		bool just_saw_newline = true;
		bool whitespace_at_head = true;
		uint_least16_t whitespace_amount = 0;
		uint_least16_t lineno = 0;

		valid_files[i].lines = NULL;
		/* It is at this point that i REALLY wish that FILE *s weren't an opaque type. */
		for (register char c; !invalid_file && (c = fgetc(file)) != EOF;) switch (c) {
		case '\xE1':
			char tmp[3];
			llen++;
			if (!strncmp(fgets(tmp, 2, file), 2, "\x82\x8E") || !strcmp(tmp, 2, "\x9A\x80"))
				goto space; // TO SPAAAAAAAAACE!!!!!!
			goto boring_character;
		case '\xE2':
			c = fgetc(file);
			llen++;
			switch (c) {
			case '\x80':
				c = fgetc(file);
				switch (c) {
				case '\xA9':
				case '\xA8':
					goto newline;

				case '\x80':
				case '\x81':
				case '\x82':
				case '\x83':
				case '\x84':
				case '\x85':
				case '\x86':
				case '\x87':
				case '\x88':
				case '\x89':
				case '\x8A':
				case '\x8B':
				case '\x8C':
				case '\x8D':
				case '\xAF':
					goto space;

				goto boring_character;
				}
			case '\x81':
				c = fgetc(file);
				if (c == '\x9F' || c == '\xA0')
					goto space;
				goto boring_character;
			case '\x85':
				if ((c = fgetc(file)) == '\x9f' || c == '\xA0') //hooray for short-circuiting!
					goto space;
				goto boring_character;
			case '\xA0':
				if ((c = fgetc(file)) == '\x80')
					goto space;
				goto boring_character;
			}
		case '\xE3':
			char tmp[3];
			llen++;
			if (!strncmp(fgets(tmp, 2, file), 2, "\x80\x80") || !strcmp(tmp, 2, "\x85\xA4"))
				goto space;
			goto boring_character;
		case '\xEF':
			char tmp[3];
			llen++;
			if (!strncmp(fgets(tmp, 2, file), 2, "\xBB\xBF") || !strcmp(tmp, 2, "\xBE\xA0"))
				goto space;
			goto boring_character;

		case '\xC2':
			c = fgetc(file);
			switch (c) {
			case '\x85':
				goto newline;
			case '\xA0':
				goto space;

			case '\x91':
				llen++;
				if ((c = fgetc(file)) == '\xC2' && (llen++, c = fgetc(file)) == '\x98') {
					PUSHSTACK(stack$$sequences);
					if (!sequences && len == 2)
						potentially_sequence_prohibited = true;
				}
				continue;
			case '\x92':
				llen++;
				if ((c = fgetc(file)) == '\xC2' && (llen++, c = fgetc(file)) == '\x98') {
					PUSHSTACK(stack$$nosequences);
					if (sequences && len == 2)
						potentially_sequence_prohibited = true;
				}
				continue;
			case '\x98':
				llen++;
				PUSHSTACK(stack$$comment);
				continue;
			case '\x9C':
				llen++;
				if (stack != NULL)
					POPSTACK();
				else
					invalid_file = true;
				continue;
			}

		case '\n':
		case '\r':
		case '\f':
		case '\v':
newline:
			if (STACKTOP == stack$$sequences || STACKTOP == stack$$nosequences) {
				invalid_file = true;
				continue;
			}
			if (!just_saw_newline) {
				just_saw_newline = true;
				valid_files[i].lines[lineno].len = potentially_whitespace_prohibited ? 0 : llen - whitespace_amount;
				llen = 0;
				c = fgetc(file);
				lineno++;
				whitespace_at_head = true;
				whitespace_amount = 0;
			}
			continue;

		case ' ':
		case '\t':
space:
			whitespace_amount++;
			continue;

		case '\\':
			c = fgetc(file);
			if (c >= '\xC2' && c <= '\xDF') {
				c = fgetc(file);
				llen += 2;
				continue;
			} else if (c >= '\xE0' && c <= '\xEF') {
				c = fgetc(file);
				c = fgetc(file);
				llen += 3;
				continue;
			} else if (c >= '\xF0' && c <= '\xF4') {
				c = fgetc(file);
				c = fgetc(file);
				c = fgetc(file);
				llen += 4;
				continue;
			}

		default:
			llen++;
boring_character:
			if (whitespace_at_head) {
				whitespace_at_head = false;
				valid_files[i].lines[lineno].off = ftell(file);
			}
			fseek(file, -2, SEEK_CUR);
			char tmp[3];
			potentially_sequence_prohibited = strncmp(fgets(tmp, 2, file), 2, "\xC2\x9C") ? potentially_sequence_prohibited : false;
		}

		if (stack != NULL) {
			invalid_file = true;
			while (stack != NULL)
				POPSTACK();
		}

		if (invalid_file) {
			valid_files[i].file = NULL;
		}

		fclose(file);
	}

	/* select a splash */
	char * splash = malloc(length_restrictions[1]);
	if (splash == NULL)
		goto exit_failurously;

	struct timespec start, current;
	if (!timespec_get(&start, TIME_UTC /* Why TIME_UTC instead of TIME_MONOTONIC?  Imagine a situation where this library is being used in the startup of a system, perhaps as a message on a login screen.  If that were the case, this function would probably be getting called within a certain limited range of times relative to the startup every time, restricting the amount of possible randomness; this is even worse if the sysclock resolution is exceptionally low.  So we just use TIME_UTC. */ )) {
		goto exit_failurously;
	}
	srand((unsigned int)(start.tv_nsec + start.tv_sec));  // since the resolution of the system clock may not go into the nanoseconds

	/* TODO: weights are not yet taken into account */
	int filerandnum, linerandnum;
	do {
		filerandnum = rand();
		// Depending on the resolution of the system clock, this may take more than five seconds to fail.
		if (!timespec_get(&current, TIME_UTC))
			goto exit_failurously;
		if (current.tv_sec - start.tv_sec > 5)
			goto exit_failurously;
	} while (filerandnum >= RAND_MAX - valid_files_len && valid_files[(filerandnum %= valid_files_len)].file != NULL);
	const struct {
		struct splashtext$filestruct * file;
		struct splashtext$static$counted_line * lines;
		size_t lines_len;
	} * selected_file = &valid_files[filerandnum];
	do {
		linerandnum = rand();
		if (!timespec_get(&current, TIME_UTC))
			goto exit_failurously;
		if (current.tv_sec - start.tv_sec > 5)
			goto exit_failurously;
	} while (linerandnum >= RAND_MAX - selected_file->lines_len && selected_file->lines[(linerandnum %= selected_file->lines_len)].len < length_restrictions[1] + 8 /* for the header */ && selected_file->lines[linerandnum].len != 0);
	const struct splashtext$static$counted_line * selected_line = &selected_file->lines[linerandnum]; // this is actually a pointer this time

	char tmp_splash[selected_line->len + 8 /* see above */];
	if (selected_file->file->filename == NULL){
		int r;
		do {
			r = rand();
		} while (r >= RAND_MAX - selected_file->file->rnpf_len);
		r %= selected_file->file->rnpf_len;
		sprintf(tmp_splash, "%d", r);

		sprintf(splash, "\x01.?\x02");
	} else {
		FILE * filestream = fopen("r", selected_file->file->filename);
		fseek(filestream, selected_line->off, SEEK_SET);
		fread(tmp_splash, 1, selected_line->len, filestream);
		fclose(filestream);

		splash[0] = '\x01';
		const size_t contextchar_loc = strlen(selected_file->file->filename) - 12;
		int_least8_t num_discomforters;
		if (selected_file->file->filename[contextchar_loc - 2] == '.')
			num_discomforters = 0;
		else if (selected_file->file->filename[contextchar_loc - 3] == '.')
			num_discomforters = 1;
		else if (selected_file->file->filename[contextchar_loc - 4] == '.')
			num_discomforters = 2;
		else if (selected_file->file->filename[contextchar_loc - 5] == '.')
			num_discomforters = 3;
		else
			num_discomforters = 4;
		snprintf(&splash[1], num_discomforters + 2, &selected_file->file->filename[contextchar_loc - num_discomforters - 1]);
		splash[num_discomforters + 2] = '\x02';
		splash[num_discomforters + 3] = '\0';
	}

	register size_t i_final = strlen(splash);
	for (register size_t i_tmp = 0; i_tmp < selected_line->len; i_tmp++, i_final++) switch (tmp_splash[i_tmp]) {
	case '\\':
		if (STACKTOP == stack$$comment || (STACKTOP == stack$$sequences && !sequences) || (STACKTOP == stack$$nosequences && sequences)) {
			if (!strncmp(&tmp_splash[i_tmp + 1], "\xC2\x9C", 2)) {
				i_tmp += 3;  i_final += 2;
				continue;
			}
		}
		if (!strncmp(tmp_splash[i_tmp + 1], "\xC2\x91", 2) || !strncmp(tmp_splash[i_tmp + 1], "\xC2\x92", 2)) { // Special handling for the PU1-SOS and PU2-SOS sequences; otherwise, they'd be interpreted as a comment that just happens to be preceeded by a private-use control character.
			strncat(splash, &tmp_splash[i_tmp + 1], 2);
			i_tmp += 3;  i_final += 2;
			continue;
		}
		goto basecase;
	case '\xC2':
		switch (tmp_splash[i_tmp + 1]) {
		case '\x9B':
isseq:
			register int j = i_tmp + 2;
			for (; tmp_splash[j] < '\x40'; j++);
			if (tmp_splash[j] != 'm' || !sequences)
				goto basecase;
			strncpy(&splash[i_final], &tmp_splash[i_tmp], j - i_tmp);
			i_final += j - i_tmp;
			i_tmp += j - i_tmp;
			continue;
		case '\x91':
			if (!strncmp(&tmp_splash[i_tmp + 2], "\xC2\x98", 2))
				PUSHSTACK(stack$$nosequences);
			i_tmp += 4;
			continue;
		case '\x92':
			if (!strncmp(&tmp_splash[i_tmp + 2], "\xC2\x98", 2))
				PUSHSTACK(stack$$sequences);
			i_tmp += 4;
			continue;
		case '\x98':
			PUSHSTACK(stack$$comment);
			i_tmp += 2;
			continue;
		case '\x9C':
			POPSTACK();
		}
	case '\x1b':
		if (tmp_splash[i_tmp + 1] == '[') goto isseq;
		/* FALL THRU */  //TODO:  Once 2023 comes about, replace this with the attribute.
	default:
basecase:
		if (stack[stack_len - 1] == stack$$comment || (stack[stack_len - 1] == stack$$sequences && !sequences) || (stack[stack_len - 1] == stack$$nosequences && sequences))
			continue;
		else
			splash[i_final] = tmp_splash[i_tmp];
	}
#undef PUSHSTACK
#undef POPSTACK
#undef STACKTOP

if (false) { // This is unbelievably stupid.
exit_failurously:  // This is definitely a real word.
	free(splash);
	splash = NULL;
}
#ifndef SPLASHTEXT$NODIRS
	free(stack);
	free(valid_files);
	for (register size_t i = 0; i < extra_len; i++)
		free(extra[i].f);
#endif
	return splash;
}

#ifndef SPLASHTEXT$NODIRS
static void splashtext$static$recurse_directory(const char * dirname, float weight, struct {float p; int r; char * f;} extra[], size_t * extra_len)
{
	DIR * directory = opendir(files[i].filename);
	if (directory == NULL)
		return;
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
				recurse_directory(entry->d_name, weight / num_entries, extra, &extra_len);
		} else {
			realloc(extra, (*extra_len + 1) * sizeof extra[0]);
			extra[*extra_len].filename = malloc(strlen(entry->d_name) + 1);
			strcpy(extra[*extra_len].filename, entry->d_name);
			extra[*extra_len].filename = weight / num_entries;
		}
	}
	closedir(directory);
}
#endif
