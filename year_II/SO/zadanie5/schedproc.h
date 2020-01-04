/* This table has one slot per process.  It contains scheduling information
 * for each process.
 */
#include <limits.h>
#include <inttypes.h>

#include <minix/bitmap.h>

/* EXTERN should be extern except in main.c, where we want to keep the struct */
#ifdef _MAIN
#undef EXTERN
#define EXTERN
#endif

#ifndef CONFIG_SMP
#define CONFIG_MAX_CPUS 1
#endif

/* Overflow is impossible in practice */
typedef int64_t Token;

/**
 * We might later want to add more information to this table, such as the
 * process owner, process group or cpumask.
 */

EXTERN struct schedproc {
	endpoint_t endpoint;	/* process endpoint id */
	endpoint_t parent;	/* parent endpoint id */
	unsigned flags;		/* flag bits */

	/* User space scheduling */
	unsigned max_priority;	/* this process' highest allowed priority */
	unsigned priority;		/* the process' current priority */
	unsigned time_slice;		/* this process's time slice */
	unsigned cpu;		/* what CPU is the process running on */
	bitchunk_t cpu_mask[BITMAP_CHUNKS(CONFIG_MAX_CPUS)]; /* what CPUs is the
								process allowed
								to run on */
  Token tok_sys_time;
  clock_t used_sys_time;
} schedproc[NR_PROCS];

/* Flag values */
#define IN_USE		0x00001	/* set when 'schedproc' slot in use */

#ifndef DEFAULT_USER_TIME_SLICE
#define DEFAULT_USER_TIME_SLICE 200
#endif

#define MAX_TOKENS 8
#define SCHED_FACTOR 0.9
