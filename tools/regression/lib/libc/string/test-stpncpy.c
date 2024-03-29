/*-
 * Copyright (c) 2009 David Schultz <das@FreeBSD.org>
 * All rights reserved.
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
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: release/10.1.0/tools/regression/lib/libc/string/test-stpncpy.c 189140 2009-02-28 06:34:04Z das $");

#include <sys/mman.h>
#include <sys/param.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *
makebuf(size_t len, int guard_at_end)
{
	char *buf;
	size_t alloc_size = roundup2(len, PAGE_SIZE) + PAGE_SIZE;

	buf = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_ANON, -1, 0);
	assert(buf);
	if (guard_at_end) {
		assert(munmap(buf + alloc_size - PAGE_SIZE, PAGE_SIZE) == 0);
		return (buf + alloc_size - PAGE_SIZE - len);
	} else {
		assert(munmap(buf, PAGE_SIZE) == 0);
		return (buf + PAGE_SIZE);
	}
}

static void
test_stpncpy(const char *s)
{
	char *src, *dst;
	size_t size, len, bufsize, x;
	int i, j;

	size = strlen(s) + 1;
	for (i = 0; i <= 1; i++) {
		for (j = 0; j <= 1; j++) {
			for (bufsize = 0; bufsize <= size + 10; bufsize++) {
				src = makebuf(size, i);
				memcpy(src, s, size);
				dst = makebuf(bufsize, j);
				memset(dst, 'X', bufsize);
				len = (bufsize < size) ? bufsize : size - 1;
				assert(stpncpy(dst, src, bufsize) == dst+len);
				assert(memcmp(src, dst, len) == 0);
				for (x = len; x < bufsize; x++)
					assert(dst[x] == '\0');
			}
		}
	}
}

int
main(int argc, char *argv[])
{

	printf("1..3\n");

	test_stpncpy("");
	printf("ok 1 - stpncpy\n");
	test_stpncpy("foo");
	printf("ok 2 - stpncpy\n");
	test_stpncpy("glorp");
	printf("ok 3 - stpncpy\n");

	exit(0);
}
