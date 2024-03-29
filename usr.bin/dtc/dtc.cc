/*-
 * Copyright (c) 2013 David Chisnall
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract (FA8750-10-C-0237)
 * ("CTSRD"), as part of the DARPA CRASH research programme.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD: release/10.1.0/usr.bin/dtc/dtc.cc 254522 2013-08-19 12:37:13Z theraven $
 */

#include <sys/resource.h>
#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


#include "fdt.hh"
#include "checking.hh"

using namespace dtc;

/**
 * The current major version of the tool.
 */
int version_major = 0;
/**
 * The current minor version of the tool.
 */
int version_minor = 4;
/**
 * The current patch level of the tool.
 */
int version_patch = 0;

static void usage(const char* argv0)
{
	fprintf(stderr, "Usage:\n"
		"\t%s\t[-fhsv] [-b boot_cpu_id] [-d dependency_file]"
			"[-E [no-]checker_name]\n"
		"\t\t[-H phandle_format] [-I input_format]"
			"[-O output_format]\n"
		"\t\t[-o output_file] [-R entries] [-S bytes] [-p bytes]"
			"[-V blob_version]\n"
		"\t\t-W [no-]checker_name] input_file\n", basename(argv0));
}

/**
 * Prints the current version of this program..
 */
static void version(const char* progname)
{
	fprintf(stderr, "Version: %s %d.%d.%d\n", progname, version_major,
			version_minor, version_patch);
}

using fdt::device_tree;

int
main(int argc, char **argv)
{
	int ch;
	int outfile = fileno(stdout);
	const char *outfile_name = "-";
	const char *in_file = "-";
	FILE *depfile = 0;
	bool debug_mode = false;
	void (device_tree::*write_fn)(int) = &device_tree::write_binary;
	void (device_tree::*read_fn)(const char*, FILE*) =
		&device_tree::parse_dts;
	uint32_t boot_cpu;
	bool boot_cpu_specified = false;
	bool keep_going = false;
	bool sort = false;
	clock_t c0 = clock();
	class device_tree tree;
	fdt::checking::check_manager checks;
	const char *options = "hqI:O:o:V:d:R:S:p:b:fisvH:W:E:DP:";

	// Don't forget to update the man page if any more options are added.
	while ((ch = getopt(argc, argv, options)) != -1)
	{
		switch (ch)
		{
		case 'h':
			usage(argv[0]);
			return EXIT_SUCCESS;
		case 'v':
			version(argv[0]);
			return EXIT_SUCCESS;
		case 'I':
		{
			string arg = string(optarg);
			if (arg == string("dtb"))
			{
				read_fn = &device_tree::parse_dtb;
			}
			else if (arg == string("dts"))
			{
				read_fn = &device_tree::parse_dts;
			}
			else
			{
				fprintf(stderr, "Unknown input format: %s\n", optarg);
				return EXIT_FAILURE;
			}
			break;
		}
		case 'O':
		{
			string arg = string(optarg);
			if (arg == string("dtb"))
			{
				write_fn = &device_tree::write_binary;
			}
			else if (arg == string("asm"))
			{
				write_fn = &device_tree::write_asm;
			}
			else if (arg == string("dts"))
			{
				write_fn = &device_tree::write_dts;
			}
			else
			{
				fprintf(stderr, "Unknown output format: %s\n", optarg);
				return EXIT_FAILURE;
			}
			break;
		}
		case 'o':
		{
			outfile_name = optarg;
			outfile = open(optarg, O_CREAT | O_TRUNC | O_WRONLY, 0666);
			if (outfile == -1)
			{
				perror("Unable to open output file");
				return EXIT_FAILURE;
			}
			break;
		}
		case 'D':
			debug_mode = true;
			break;
		case 'V':
			if (string(optarg) != string("17"))
			{
				fprintf(stderr, "Unknown output format version: %s\n", optarg);
				return EXIT_FAILURE;
			}
			break;
		case 'd':
		{
			if (depfile != 0)
			{
				fclose(depfile);
			}
			if (string(optarg) == string("-"))
			{
				depfile = stdout;
			}
			else
			{
				depfile = fdopen(open(optarg, O_CREAT | O_TRUNC | O_WRONLY, 0666), "w");
				if (depfile == 0)
				{
					perror("Unable to open dependency file");
					return EXIT_FAILURE;
				}
			}
			break;
		}
		case 'H':
		{
			string arg = string(optarg);
			if (arg == string("both"))
			{
				tree.set_phandle_format(device_tree::BOTH);
			}
			else if (arg == string("epapr"))
			{
				tree.set_phandle_format(device_tree::EPAPR);
			}
			else if (arg == string("linux"))
			{
				tree.set_phandle_format(device_tree::LINUX);
			}
			else
			{
				fprintf(stderr, "Unknown phandle format: %s\n", optarg);
				return EXIT_FAILURE;
			}
			break;
		}
		case 'b':
			// Don't bother to check if strtoll fails, just
			// use the 0 it returns.
			boot_cpu = (uint32_t)strtoll(optarg, 0, 10);
			boot_cpu_specified = true;
			break;
		case 'f':
			keep_going = true;
			break;
		case 'W':
		case 'E':
		{
			string arg = string(optarg);
			if ((arg.size() > 3) && (strncmp(optarg, "no-", 3) == 0))
			{
				arg = string(optarg+3);
				if (!checks.disable_checker(arg))
				{
					fprintf(stderr, "Checker %s either does not exist or is already disabled\n", optarg+3);
				}
				break;
			}
			if (!checks.enable_checker(arg))
			{
				fprintf(stderr, "Checker %s either does not exist or is already enabled\n", optarg);
			}
			break;
		}
		case 's':
		{
			sort = true;
			break;
		}
		case 'i':
		{
			tree.add_include_path(optarg);
			break;
		}
		// Should quiet warnings, but for now is silently ignored.
		case 'q':
			break;
		case 'R':
			tree.set_empty_reserve_map_entries(strtoll(optarg, 0, 10));
			break;
		case 'S':
			tree.set_blob_minimum_size(strtoll(optarg, 0, 10));
			break;
		case 'p':
			tree.set_blob_padding(strtoll(optarg, 0, 10));
			break;
		case 'P':
			if (!tree.parse_define(optarg))
			{
				fprintf(stderr, "Invalid predefine value %s\n",
				        optarg);
			}
			break;
		default:
			fprintf(stderr, "Unknown option %c\n", ch);
			return EXIT_FAILURE;
		}
	}
	if (optind < argc)
	{
		in_file = argv[optind];
	}
	if (depfile != 0)
	{
		fputs(outfile_name, depfile);
		fputs(": ", depfile);
		fputs(in_file, depfile);
	}
	clock_t c1 = clock();
	(tree.*read_fn)(in_file, depfile);
	// Override the boot CPU found in the header, if we're loading from dtb
	if (boot_cpu_specified)
	{
		tree.set_boot_cpu(boot_cpu);
	}
	if (sort)
	{
		tree.sort();
	}
	if (depfile != 0)
	{
		putc('\n', depfile);
		fclose(depfile);
	}
	if (!(tree.is_valid() || keep_going))
	{
		fprintf(stderr, "Failed to parse tree.  Unhappy face!\n");
		return EXIT_FAILURE;
	}
	clock_t c2 = clock();
	if (!(checks.run_checks(&tree, true) || keep_going))
	{
		return EXIT_FAILURE;
	}
	clock_t c3 = clock();
	(tree.*write_fn)(outfile);
	close(outfile);
	clock_t c4 = clock();

	if (debug_mode)
	{
		struct rusage r;

		getrusage(RUSAGE_SELF, &r);
		fprintf(stderr, "Peak memory usage: %ld bytes\n", r.ru_maxrss);
		fprintf(stderr, "Setup and option parsing took %f seconds\n",
				((double)(c1-c0))/CLOCKS_PER_SEC);
		fprintf(stderr, "Parsing took %f seconds\n",
				((double)(c2-c1))/CLOCKS_PER_SEC);
		fprintf(stderr, "Checking took %f seconds\n",
				((double)(c3-c2))/CLOCKS_PER_SEC);
		fprintf(stderr, "Generating output took %f seconds\n",
				((double)(c4-c3))/CLOCKS_PER_SEC);
		fprintf(stderr, "Total time: %f seconds\n",
				((double)(c4-c0))/CLOCKS_PER_SEC);
		// This is not needed, but keeps valgrind quiet.
		fclose(stdin);
	}
	return EXIT_SUCCESS;
}

