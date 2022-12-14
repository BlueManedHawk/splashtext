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

#include "../../Include/splashtext.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>

int main(int argc, char * argv[]) {
	enum splashtext$context contexts = 0;
	enum splashtext$discomforter discomforters = 0;
	bool sequences = false;
	uint_least16_t retlen[2] = {[0] = 0, [1] = 1024};

	register int i = 1;
	/* The arguments to Splash Text always come before any pathnames; therefore, we start one loop to deal with the options and then start another one to deal with the pathnames once we reach them. */
	for (; i < argc; i++) {
		if (!strcmp(argv[i], "--")) {
			i++; /* so that the next loop doesn't treat the `--` as a filename */
			break;
		}
		if (!strcmp(argv[i], "--help")) {
			printf(
				"Splash Text - a unified system for splash text, freely usable by any program.  This is the command-line wrapper; for details on the C language interface, see the manpage splashtext(6).\n"
				"\n"
				"Usage:  splashtext (--help|--version|[--sequences] [--minlen <num>] [--maxlen <num>] --contexts <context>... [--discomforters <discomforter>...] -- ([%%<weight>] <pathname>)...)\n"
				"\n"
				"--help, --version, and --list override all other options.  --sequences says that it's okay if splashes contain Select Graphical Rendition sequences.  The default minimum length is 0, and the default maximum is 1024; these numbers include the header and terminating characters.\n"
				"\n"
				"Available contexts are `log`, `crash`, `subtitle`, `ominous`, `tips`, `quote`, and `other`.  Available discomforters are `sexual`, `graphic`, and `heavy`, with `humor` as a special parameter indicating humorous content based on the topic.\n"
				"\n"
				"The program will normally output a splash to stdout and exit with code 0.  If --help or --version was specified (or you tried to use a short option), it will output the correspronding text and exit with code 1.  If a splash can't be found, it will output nothing and exit with code 2.  In the event of catastrophic failure, it will exit with some other code and nothing is guaranteed about the output.\n"
			);
			exit(1);  /* Since --help overrides all other options, we don't need to bother checking them. */
		}
		if (!strcmp(argv[i], "--version")) {
			printf(
				"This is Splash Text v0.0.0.\n"
			);
			exit(1);
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
			sequences = true;
		} else if (!strcmp(argv[i], "--minlen")) {
			/* the minlen and maxlen checks could be a macro, but like. this is only repeated twice.  if it was more i'd consider it but just twice is fine i think */
			char * endptr[strlen(argv[i + 1]) + 1];
			memset(endptr, 0, strlen(argv[i + 1] + 1));
			signed long minlen = strtol(argv[i+1], endptr /* This should not work. */, 0);
			if ((*endptr)[0] != '\0' /* we avoid translating anything that isn't just a number to be safe */)
				fprintf(stderr, "error: argument `%s` to --minlen is possibly not a number\n", argv[i + 1]);
			else if (minlen < 0)
				fprintf(stderr, "error: argument `%s` to --minlen is a negative number\n", argv[i + 1]);
			else if (minlen > UINT_LEAST16_MAX)
				fprintf(stderr, "error: argument `%s` to --minlen exceeds your C implementation's UINT_LEAST16_MAX of %d\n", argv[i + 1], UINT_LEAST16_MAX);
			else
				retlen[0] = minlen;
			if (retlen[0] != minlen)
				exit(3);
			i++;
		} else if (!strcmp(argv[i], "--maxlen")) {
			char * endptr[strlen(argv[i + 1]) + 1];
			memset(endptr, 0, strlen(argv[i + 1] + 1));
			signed long maxlen = strtol(argv[i+1], endptr, 0);
			if ((*endptr)[0] != '\0' /* see above */)
				fprintf(stderr, "error: argument `%s` to --maxlen is possibly not a number\n", argv[i + 1]);
			else if (maxlen < 0)
				fprintf(stderr, "error: argument `%s` to --maxlen is a negative number\n", argv[i + 1]);
			else if (maxlen > UINT_LEAST16_MAX)
				fprintf(stderr, "error: argument `%s` to --maxlen exceeds your C implementation's UINT_LEAST16_MAX of %d\n", argv[i + 1], UINT_LEAST16_MAX);
			else
				retlen[0] = maxlen;
			if (retlen[0] != maxlen)
				exit(3);
			i++;
		} else if (!strcmp(argv[i], "--contexts")) {
			i++;
			for (; i < argc; i++) {
#define ADD_CONTEXT_IF_SUPPLIED(teststr)\
	if (!strcmp(argv[i], #teststr))\
		contexts |= splashtext$context$$##teststr
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
				} else {
					fprintf(stderr, "error: context `%s` is invalid\n", argv[i]);
					exit(3);
				}
#undef ADD_CONTEXT_IF_SUPPLIED
			}
		} else if (!strcmp(argv[i], "--discomforters")) {
			i++;
			for (; i < argc; i++) {
#define ADD_DISCOMFORTER_IF_SUPPLIED(teststr)\
	if (!strcmp(argv[i], #teststr))\
		discomforters |= splashtext$discomforter$$##teststr
				ADD_DISCOMFORTER_IF_SUPPLIED(sexual);
				else ADD_DISCOMFORTER_IF_SUPPLIED(graphic);
				else ADD_DISCOMFORTER_IF_SUPPLIED(heavy);
				else ADD_DISCOMFORTER_IF_SUPPLIED(humor);
				else if (argv[i][0] == '-') {
					i--; /* see above */
					break;
				} else {
					fprintf(stderr, "error: discomforter `%s` is invalid\n", argv[i]);
					exit(3);
				}
#undef ADD_DISCOMFORTER_IF_SUPPLIED
			}
		} else {
			fprintf(stderr,
				"error: unrecognized option `%s` is ignored\n"
				"remark: options MUST come before pathnames and are separated from them by two dashes `--`\n"
			, argv[i]);
			exit(3);
		}
	}

	struct splashtext$filestruct pathnames[argc - i];
	memset(pathnames, 0, (size_t)(argc - i) * sizeof (struct splashtext$filestruct));

	register int j = 0;
	for (; i < argc; i++, j++) {
		if (argv[i][0] == '#') {
			pathnames[j].filename = NULL;
			char * endptr[strlen(argv[i]) + 1];
			signed long val = strtol(argv[i] + 1, endptr, 0);
			if (val < 0)
				fprintf(stderr, "error: numerical psuedofile argument `%s` is a negative number\n", argv[i]);
			else if (val > RAND_MAX)
				fprintf(stderr, "error: numerical psuedofile argument `%s` exceeds your C implementation's RAND_MAX of %d\n", argv[i], RAND_MAX);
			else if (*endptr[0] != '\0' /* see above */)
				fprintf(stderr, "error: numerical psuedofile argument `%s` is probably not a number\n", argv[i]);
			else
				pathnames[j].rnpf_len = val;
			if (pathnames[j].rnpf_len != val)
				exit(3);
			pathnames[j].weight = pathnames[j].weight == 0 ? 1 : pathnames[j].weight;
		} else if (argv[i][0] == '%') {
			char * endptr[strlen(argv[i]) + 1];
			float val = strtod(argv[i] + 1, endptr);
			if (*endptr[0] != '\0')
				fprintf(stderr, "error: value for weight in argument `%s` is probably not a number\n", argv[i]);
			else if (val == HUGE_VALF)
				fprintf(stderr, "error: value for weight in argument `%s` exceeds your C implementation's HUGE_VALF of %f\n", argv[i], HUGE_VALF);
			else if (val < 0)
				fprintf(stderr, "error: value for weight in argument `%s` is a negative number\n", argv[i]);
			else
				pathnames[j].weight = val;
			if (pathnames[j].weight != val)
				exit(3);
			j--;
		} else if (argv[i][0] == '\\') {
			pathnames[j].filename = &argv[i][1];
			pathnames[j].weight = pathnames[j].weight == 0 ? 1 : pathnames[j].weight;
		} else {
			pathnames[j].filename = argv[i];
			pathnames[j].weight = pathnames[j].weight == 0 ? 1 : pathnames[j].weight;
		}
	}

	char * splash = splashtext(pathnames, (size_t)j * sizeof (struct splashtext$filestruct), sequences, retlen, contexts, discomforters);
	if (splash == NULL)
		return 2;

	puts(splash);
	free(splash);
	return 0;
}
