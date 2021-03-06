/*
 * Copyright 2014 Mentor Graphics Corporation.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2 of the
 * License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef VDSOTEST_UTIL_H
#define VDSOTEST_UTIL_H

#include <search.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "compiler.h"

#define NSEC_PER_SEC 1000000000L

#define ADDR_SPACE_END UINTPTR_MAX

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

void *xmalloc(size_t sz);
void *xzmalloc(size_t sz);
void *xrealloc(void *ptr, size_t sz);
void xfree(void *ptr);
int xasprintf(char **strp, const char *fmt, ...) __printf(2, 3);

struct hashtable {
	struct hsearch_data *htab;
};

void *hashtable_lookup(struct hashtable *ht, const char *key);
void hashtable_add(struct hashtable *ht, const char *key, const void *data);

static inline uint64_t timespec_to_nsec(const struct timespec *ts)
{
	uint64_t res;

	res = NSEC_PER_SEC * (unsigned long long)ts->tv_sec;
	res += ts->tv_nsec;

	return res;
}

static inline struct timespec nsec_to_timespec(uint64_t nsec)
{
	struct timespec ret;

	ret = (struct timespec) {
		.tv_sec = nsec / NSEC_PER_SEC,
		.tv_nsec = nsec % NSEC_PER_SEC,
	};

	return ret;
}

static inline uint64_t
timespec_delta_nsec(const struct timespec *before_ts,
		    const struct timespec *after_ts)
{
	uint64_t before;
	uint64_t after;

	before = timespec_to_nsec(before_ts);
	after = timespec_to_nsec(after_ts);

	return after - before;
}

static inline struct timespec
timespec_delta(const struct timespec *before_ts,
	       const struct timespec *after_ts)
{
	return nsec_to_timespec(timespec_delta_nsec(before_ts, after_ts));
}

void *alloc_page(int prot);
void free_page(void *page);

struct signal_set {
	uint64_t mask;
};

#define SIGNO_TO_BIT(n) (1 << (n))

static inline bool signal_in_set(const struct signal_set *set, int sig)
{
	return set->mask & SIGNO_TO_BIT(sig);
}

void *get_vdso_sym(const char *name);

#endif
