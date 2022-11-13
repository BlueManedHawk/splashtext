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

/* This is the main file for the `splashtext` program. */

#include "splashtext.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
	enum splashtext flags = 0;
	enum { normal, version, list } request = normal;
	unsigned int retlen[2] = {[0] = 0, [1] = 1024};

	register int i = 1;
	/* The arguments to Splash Text always come before any pathnames; therefore, we start one loop to deal with the options and then start another one to deal with the pathnames once we reach them. */
	for (; i < argc; i++) {
		if (!strcmp(argv[i], "--")) {
			i++; /* so that the next loop doesn't treat the `--` as a filename */
			break;
		}
		if (!strcmp(argv[i], "--help")) {
			printf(
				"Splash Text - a unified system for splash text, freely usable by any program.\n"
				"\n"
				"Usage:  splashtext (--help|--version|--list|[--sequences] [--minlen <num>] [--maxlen <num>] --context <context>... [--content <content>] -- ([%%<percentage>] <pathname>)...)\n"
				"\n"
				"--help, --version, and --list override all other options.  --sequences says that it's okay if splashes contain Select Graphical Rendition sequences.  The default minimum length is 0, and the default maximum is 1024; these numbers include the header and terminating characters.\n"
				"\n"
				"Available contexts are `log`, `crash`, `subtitle`, `ominous`, `tips`, `quote`, and `other`.  Available contents are `sexual`, `graphic`, and `heavy`, with `humor` as a special parameter indicating humorous content based on these topic.\n"
				"\n"
				"The program will normally output a splash and return 0.  If --help, --version, or --list was specified, it will output the correspronding text and return 1.  If a splash can't be found, it will output nothing and return 2.  In the event of catastrophic failure, it will return 3 and nothing is guaranteed about the output.\n"
				"\n"
				"See the manpage splashtext(6) for exhaustive details on this program, including more details on parameters, the splash file file format, the format of the header, and the C language interface.\n"
			);
			exit(1);  /* Since --help overrides all other options, we don't need to bother checking them. */
		}
		if (argv[i][0] == '-' && argv[i][1] != '-') { /* i.e. invoker tried short options */
			printf(
				"Hi!  It looks like you're trying to invoke this program with short options.\n"
				"\n"
				"Look, i get that short options are nicer than long options in a lot of ways, and i get wanting to stick with them.  But they have the fundamental limitation of being only a single character long, so there's only so many short options a program can possibly have.  So while they're nice, they're unfortunately not practical enough for it to be worthwhile for this program to support them.\n"
				"\n"
				"Sorry.\n"
			);
			exit(1);
		}

		if (!strcmp(argv[i], "--sequences")) {
			flags |= splashtext$$sequences;
		} else if (!strcmp(argv[i], "--minlen")) {
			/* the minlen and maxlen checks could be a macro, but like. this is only repeated twice.  if it was more i'd consider it but just twice is fine i think */
			char endptr[strlen(argv[i+1])];
			signed long long minlen = strtoll(argv[i+1], endptr, 0);
			if (minlen < 0 || (minlen == LLONG_MAX && errno = ERANGE) || endptr[0] != '\0')
				fprintf(stderr, "warning: unrecognized argument to --minlen is ignored\n");
			else
				retlen[0] = minlen;
			i++;
		} else if (!strcmp(argv[i], "--maxlen")) {
			char endptr[strlen(argv[i+1])];
			signed long long maxlen = strtoll(argv[i+1], endptr, 0);
			if (minlen < 0 || (maxlen == LLONG_MAX && errno = ERANGE) || endptr[0] != '\0')
				fprintf(stderr, "warning: unrecognized argument to --maxlen `%s` is ignored\n", argv[i+1]);
			else
				retlen[1] = maxlen;
			i++;
		} else if (!strcmp(argv[i], "--context")) {
			for (; i < argc; i++) {
				#define ADD_CONTEXT_IF_SUPPLIED(teststr) if (!strcmp(argv[i], #teststr)) flags |= splashtext$$context$##teststr
				ADD_CONTEXT_IF_SUPPLIED(log);
				else ADD_CONTEXT_IF_SUPPLIED(crash);
				else ADD_CONTEXT_IF_SUPPLIED(subtitle);
				else ADD_CONTEXT_IF_SUPPLIED(ominous);
				else ADD_CONTEXT_IF_SUPPLIED(tips);
				else ADD_CONTEXT_IF_SUPPLIED(quote);
				else ADD_CONTEXT_IF_SUPPLIED(other);
				else if (argv[i][0] == '-') {
					i--; /* to allow the main for loop to check what kind of option this is that we've just come across */
					break;
				} else
					fprintf(stderr, "warning: unrecognized context `%s` is ignored\n", argv[i]);
				#undef ADD_CONTEXT_IF_SUPPLIED
			}
		} else if (!strcmp(argv[i], "--content")) {
			for (; i < argc; i++) {
				#define ADD_CONTENT_IF_SUPPLIED(teststr) if (!strcmp(argv[i], #teststr)) flags |= splashtext$$content$##teststr
				ADD_CONTENT_IF_SUPPLIED(sexual);
				else ADD_CONTENT_IF_SUPPLIED(graphic);
				else ADD_CONTENT_IF_SUPPLIED(heavy);
				else ADD_CONTENT_IF_SUPPLIED(humor);
				else if (argv[i][0] == '-') {
					i--; /* to allow the main for loop to check what kind of option this is that we've just come across*/
					break;
				}
				#undef ADD_CONTENT_IF_SUPPLIED
			}
		} else {
			fprintf(stderr, "warning: unrecognized option `%s` is ignored\nremark: options MUST come before pathnames and are separated from them by two dashes `--`\n", argv[i]);
		}
	}

	char * pathnames[argc - i];

	for (register int j = 0; i < argc; i++, j++)
		pathnames[j] = argv[i];

	char splash[retlen[1]] = splashtext(flags, retlen, pathnames, (size_t)(argc - i));
	puts(splash);
	return splash[0] == '\0' ? 2 : 0;
}