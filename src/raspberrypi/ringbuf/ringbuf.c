/*
 * Copyright (c) 2016-2017 Mindaugas Rasiukevicius <rmind at noxt eu>
 * All rights reserved.
 *
 * Use is subject to license terms, as specified in the LICENSE file.
 */

/*
 * Atomic multi-producer single-consumer ring buffer, which supports
 * contiguous range operations and which can be conveniently used for
 * message passing.
 *
 * There are three offsets -- think of clock hands:
 * - NEXT: marks the beginning of the available space,
 * - WRITTEN: the point up to which the data is actually written.
 * - Observed READY: point up to which data is ready to be written.
 *
 * Producers
 *
 *	Observe and save the 'next' offset, then request N bytes from
 *	the ring buffer by atomically advancing the 'next' offset.  Once
 *	the data is written into the "reserved" buffer space, the thread
 *	clears the saved value; these observed values are used to compute
 *	the 'ready' offset.
 *
 * Consumer
 *
 *	Writes the data between 'written' and 'ready' offsets and updates
 *	the 'written' value.  The consumer thread scans for the lowest
 *	seen value by the producers.
 *
 * Key invariant
 *
 *	Producers cannot go beyond the 'written' offset; producers are
 *	also not allowed to catch up with the consumer.  Only the consumer
 *	is allowed to catch up with the producer i.e. set the 'written'
 *	offset to be equal to the 'next' offset.
 *
 * Wrap-around
 *
 *	If the producer cannot acquire the requested length due to little
 *	available space at the end of the buffer, then it will wraparound.
 *	WRAP_LOCK_BIT in 'next' offset is used to lock the 'end' offset.
 *
 *	There is an ABA problem if one producer stalls while a pair of
 *	producer and consumer would both successfully wrap-around and set
 *	the 'next' offset to the stale value of the first producer, thus
 *	letting it to perform a successful CAS violating the invariant.
 *	A counter in the 'next' offset (masked by WRAP_COUNTER) is used
 *	to prevent from this problem.  It is incremented on wraparounds.
 *
 *	The same ABA problem could also cause a stale 'ready' offset,
 *	which could be observed by the consumer.  We set WRAP_LOCK_BIT in
 *	the 'seen' value before advancing the 'next' and clear this bit
 *	after the successful advancing; this ensures that only the stable
 *	'ready' is observed by the consumer.
 */

#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#include "ringbuf.h"
#include "utils.h"

#define	RBUF_OFF_MASK	(0x00000000ffffffffUL)
#define	WRAP_LOCK_BIT	(0x8000000000000000UL)
#define	RBUF_OFF_MAX	(UINT64_MAX & ~WRAP_LOCK_BIT)

#define	WRAP_COUNTER	(0x7fffffff00000000UL)
#define	WRAP_INCR(x)	(((x) + 0x100000000UL) & WRAP_COUNTER)

typedef uint64_t	ringbuf_off_t;

struct ringbuf_worker {
	volatile ringbuf_off_t	seen_off;
	int			registered;
};

struct ringbuf {
	/* Ring buffer space. */
	size_t			space;

	/*
	 * The NEXT hand is atomically updated by the producer.
	 * WRAP_LOCK_BIT is set in case of wrap-around; in such case,
	 * the producer can update the 'end' offset.
	 */
	volatile ringbuf_off_t	next;
	ringbuf_off_t		end;

	/* The following are updated by the consumer. */
	ringbuf_off_t		written;
	unsigned		nworkers;
	ringbuf_worker_t	workers[];
};

/*
 * ringbuf_setup: initialise a new ring buffer of a given length.
 */
int
ringbuf_setup(ringbuf_t *rbuf, unsigned nworkers, size_t length)
{
	if (length >= RBUF_OFF_MASK) {
		errno = EINVAL;
		return -1;
	}
	memset(rbuf, 0, offsetof(ringbuf_t, workers[nworkers]));
	rbuf->space = length;
	rbuf->end = RBUF_OFF_MAX;
	rbuf->nworkers = nworkers;
	return 0;
}

/*
 * ringbuf_get_sizes: return the sizes of the ringbuf_t and ringbuf_worker_t.
 */
void
ringbuf_get_sizes(unsigned nworkers,
    size_t *ringbuf_size, size_t *ringbuf_worker_size)
{
	if (ringbuf_size)
		*ringbuf_size = offsetof(ringbuf_t, workers[nworkers]);
	if (ringbuf_worker_size)
		*ringbuf_worker_size = sizeof(ringbuf_worker_t);
}

/*
 * ringbuf_register: register the worker (thread/process) as a producer
 * and pass the pointer to its local store.
 */
ringbuf_worker_t *
ringbuf_register(ringbuf_t *rbuf, unsigned i)
{
	ringbuf_worker_t *w = &rbuf->workers[i];

	w->seen_off = RBUF_OFF_MAX;
	atomic_store_explicit(&w->registered, true, memory_order_release);
	return w;
}

void
ringbuf_unregister(ringbuf_t *rbuf, ringbuf_worker_t *w)
{
	w->registered = false;
	(void)rbuf;
}

/*
 * stable_nextoff: capture and return a stable value of the 'next' offset.
 */
static inline ringbuf_off_t
stable_nextoff(ringbuf_t *rbuf)
{
	unsigned count = SPINLOCK_BACKOFF_MIN;
	ringbuf_off_t next;
retry:
	next = atomic_load_explicit(&rbuf->next, memory_order_acquire);
	if (next & WRAP_LOCK_BIT) {
		SPINLOCK_BACKOFF(count);
		goto retry;
	}
	ASSERT((next & RBUF_OFF_MASK) < rbuf->space);
	return next;
}

/*
 * stable_seenoff: capture and return a stable value of the 'seen' offset.
 */
static inline ringbuf_off_t
stable_seenoff(ringbuf_worker_t *w)
{
	unsigned count = SPINLOCK_BACKOFF_MIN;
	ringbuf_off_t seen_off;
retry:
	seen_off = atomic_load_explicit(&w->seen_off, memory_order_acquire);
	if (seen_off & WRAP_LOCK_BIT) {
		SPINLOCK_BACKOFF(count);
		goto retry;
	}
	return seen_off;
}

/*
 * ringbuf_acquire: request a space of a given length in the ring buffer.
 *
 * => On success: returns the offset at which the space is available.
 * => On failure: returns -1.
 */
ssize_t
ringbuf_acquire(ringbuf_t *rbuf, ringbuf_worker_t *w, size_t len)
{
	ringbuf_off_t seen, next, target;

	ASSERT(len > 0 && len <= rbuf->space);
	ASSERT(w->seen_off == RBUF_OFF_MAX);

	do {
		ringbuf_off_t written;

		/*
		 * Get the stable 'next' offset.  Save the observed 'next'
		 * value (i.e. the 'seen' offset), but mark the value as
		 * unstable (set WRAP_LOCK_BIT).
		 *
		 * Note: CAS will issue a memory_order_release for us and
		 * thus ensures that it reaches global visibility together
		 * with new 'next'.
		 */
		seen = stable_nextoff(rbuf);
		next = seen & RBUF_OFF_MASK;
		ASSERT(next < rbuf->space);
		atomic_store_explicit(&w->seen_off, next | WRAP_LOCK_BIT,
		    memory_order_relaxed);

		/*
		 * Compute the target offset.  Key invariant: we cannot
		 * go beyond the WRITTEN offset or catch up with it.
		 */
		target = next + len;
		written = rbuf->written;
		if (__predict_false(next < written && target >= written)) {
			/* The producer must wait. */
			atomic_store_explicit(&w->seen_off,
			    RBUF_OFF_MAX, memory_order_release);
			return -1;
		}

		if (__predict_false(target >= rbuf->space)) {
			const bool exceed = target > rbuf->space;

			/*
			 * Wrap-around and start from the beginning.
			 *
			 * If we would exceed the buffer, then attempt to
			 * acquire the WRAP_LOCK_BIT and use the space in
			 * the beginning.  If we used all space exactly to
			 * the end, then reset to 0.
			 *
			 * Check the invariant again.
			 */
			target = exceed ? (WRAP_LOCK_BIT | len) : 0;
			if ((target & RBUF_OFF_MASK) >= written) {
				atomic_store_explicit(&w->seen_off,
				    RBUF_OFF_MAX, memory_order_release);
				return -1;
			}
			/* Increment the wrap-around counter. */
			target |= WRAP_INCR(seen & WRAP_COUNTER);
		} else {
			/* Preserve the wrap-around counter. */
			target |= seen & WRAP_COUNTER;
		}
	} while (!atomic_compare_exchange_weak(&rbuf->next, &seen, target));

	/*
	 * Acquired the range.  Clear WRAP_LOCK_BIT in the 'seen' value
	 * thus indicating that it is stable now.
	 *
	 * No need for memory_order_release, since CAS issued a fence.
	 */
	atomic_store_explicit(&w->seen_off, w->seen_off & ~WRAP_LOCK_BIT,
	    memory_order_relaxed);

	/*
	 * If we set the WRAP_LOCK_BIT in the 'next' (because we exceed
	 * the remaining space and need to wrap-around), then save the
	 * 'end' offset and release the lock.
	 */
	if (__predict_false(target & WRAP_LOCK_BIT)) {
		/* Cannot wrap-around again if consumer did not catch-up. */
		ASSERT(rbuf->written <= next);
		ASSERT(rbuf->end == RBUF_OFF_MAX);
		rbuf->end = next;
		next = 0;

		/*
		 * Unlock: ensure the 'end' offset reaches global
		 * visibility before the lock is released.
		 */
		atomic_store_explicit(&rbuf->next,
		    (target & ~WRAP_LOCK_BIT), memory_order_release);
	}
	ASSERT((target & RBUF_OFF_MASK) <= rbuf->space);
	return (ssize_t)next;
}

/*
 * ringbuf_produce: indicate the acquired range in the buffer is produced
 * and is ready to be consumed.
 */
void
ringbuf_produce(ringbuf_t *rbuf, ringbuf_worker_t *w)
{
	(void)rbuf;
	ASSERT(w->registered);
	ASSERT(w->seen_off != RBUF_OFF_MAX);
	atomic_store_explicit(&w->seen_off, RBUF_OFF_MAX, memory_order_release);
}

/*
 * ringbuf_consume: get a contiguous range which is ready to be consumed.
 */
size_t
ringbuf_consume(ringbuf_t *rbuf, size_t *offset)
{
	ringbuf_off_t written = rbuf->written, next, ready;
	size_t towrite;
retry:
	/*
	 * Get the stable 'next' offset.  Note: stable_nextoff() issued
	 * a load memory barrier.  The area between the 'written' offset
	 * and the 'next' offset will be the *preliminary* target buffer
	 * area to be consumed.
	 */
	next = stable_nextoff(rbuf) & RBUF_OFF_MASK;
	if (written == next) {
		/* If producers did not advance, then nothing to do. */
		return 0;
	}

	/*
	 * Observe the 'ready' offset of each producer.
	 *
	 * At this point, some producer might have already triggered the
	 * wrap-around and some (or all) seen 'ready' values might be in
	 * the range between 0 and 'written'.  We have to skip them.
	 */
	ready = RBUF_OFF_MAX;

	for (unsigned i = 0; i < rbuf->nworkers; i++) {
		ringbuf_worker_t *w = &rbuf->workers[i];
		ringbuf_off_t seen_off;

		/*
		 * Skip if the worker has not registered.
		 *
		 * Get a stable 'seen' value.  This is necessary since we
		 * want to discard the stale 'seen' values.
		 */
		if (!atomic_load_explicit(&w->registered, memory_order_relaxed))
			continue;
		seen_off = stable_seenoff(w);

		/*
		 * Ignore the offsets after the possible wrap-around.
		 * We are interested in the smallest seen offset that is
		 * not behind the 'written' offset.
		 */
		if (seen_off >= written) {
			ready = MIN(seen_off, ready);
		}
		ASSERT(ready >= written);
	}

	/*
	 * Finally, we need to determine whether wrap-around occurred
	 * and deduct the safe 'ready' offset.
	 */
	if (next < written) {
		const ringbuf_off_t end = MIN(rbuf->space, rbuf->end);

		/*
		 * Wrap-around case.  Check for the cut off first.
		 *
		 * Reset the 'written' offset if it reached the end of
		 * the buffer or the 'end' offset (if set by a producer).
		 * However, we must check that the producer is actually
		 * done (the observed 'ready' offsets are clear).
		 */
		if (ready == RBUF_OFF_MAX && written == end) {
			/*
			 * Clear the 'end' offset if was set.
			 */
			if (rbuf->end != RBUF_OFF_MAX) {
				rbuf->end = RBUF_OFF_MAX;
			}

			/*
			 * Wrap-around the consumer and start from zero.
			 */
			written = 0;
			atomic_store_explicit(&rbuf->written,
			    written, memory_order_release);
			goto retry;
		}

		/*
		 * We cannot wrap-around yet; there is data to consume at
		 * the end.  The ready range is smallest of the observed
		 * 'ready' or the 'end' offset.  If neither is set, then
		 * the actual end of the buffer.
		 */
		ASSERT(ready > next);
		ready = MIN(ready, end);
		ASSERT(ready >= written);
	} else {
		/*
		 * Regular case.  Up to the observed 'ready' (if set)
		 * or the 'next' offset.
		 */
		ready = MIN(ready, next);
	}
	towrite = ready - written;
	*offset = written;

	ASSERT(ready >= written);
	ASSERT(towrite <= rbuf->space);
	return towrite;
}

/*
 * ringbuf_release: indicate that the consumed range can now be released.
 */
void
ringbuf_release(ringbuf_t *rbuf, size_t nbytes)
{
	const size_t nwritten = rbuf->written + nbytes;

	ASSERT(rbuf->written <= rbuf->space);
	ASSERT(rbuf->written <= rbuf->end);
	ASSERT(nwritten <= rbuf->space);

	rbuf->written = (nwritten == rbuf->space) ? 0 : nwritten;
}
