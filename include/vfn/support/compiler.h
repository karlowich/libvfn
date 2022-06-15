/* SPDX-License-Identifier: LGPL-2.1-or-later or MIT */

/*
 * This file is part of libvfn.
 *
 * Copyright (C) 2022 The libvfn Authors. All Rights Reserved.
 *
 * This library (libvfn) is dual licensed under the GNU Lesser General
 * Public License version 2.1 or later or the MIT license. See the
 * COPYING and LICENSE files for more information.
 */

#ifndef LIBVFN_SUPPORT_COMPILER_H
#define LIBVFN_SUPPORT_COMPILER_H

#define __glue(x, y) x ## y
#define glue(x, y) __glue(x, y)

/*
 * glib-style auto pointer
 *
 * The __autoptr() provides a general way of "cleaning up" when going out of
 * scope. Inspired by glib, but simplified a lot (at the expence of
 * flexibility).
 */

#define __AUTOPTR_CLEANUP(t) __autoptr_cleanup_##t
#define __AUTOPTR_T(t) __autoptr_##t

#define DEFINE_AUTOPTR(t, cleanup) \
	typedef t *__AUTOPTR_T(t); \
	\
	static inline void __AUTOPTR_CLEANUP(t) (t **p) \
	{ \
		if (*p) \
			(cleanup)(*p); \
	}

#define __autoptr(t) __attribute__((cleanup(__AUTOPTR_CLEANUP(t)))) __AUTOPTR_T(t)

#define barrier() asm volatile("" ::: "memory")

#if defined(__aarch64__)
# define rmb() asm volatile("dsb ld" ::: "memory")
# define wmb() asm volatile("dsb st" ::: "memory")
# define mb()  asm volatile("dsb sy" ::: "memory")
#elif defined(__x86_64__)
# define rmb() asm volatile("lfence" ::: "memory")
# define wmb() asm volatile("sfence" ::: "memory")
# define mb()  asm volatile("mfence" ::: "memory")
#else
# error unsupported architecture
#endif

/**
 * LOAD - force volatile load
 * @v: variable to load
 *
 * Force a volatile load, detering the compiler from doing a range of optimizations.
 *
 * Return: The value of the variable.
 */
#define LOAD(v) (*(const volatile typeof(v) *)&(v))

/**
 * STORE - force volatile store
 * @v: variable to store into
 * @val: value to store into @v
 *
 * Force a volatile store, detering the compiler from doing a range of optimizations.
 */
#define STORE(v, val) (*(volatile typeof(v) *)&(v) = (val))

#define likely(cond) __builtin_expect(!!(cond), 1)
#define unlikely(cond) __builtin_expect(!!(cond), 0)

#ifdef __CHECKER__
# ifndef __bitwise
#  define __bitwise	__attribute__((bitwise))
# endif
# define __force	__attribute__((force))
#else
# ifndef __bitwise
#  define __bitwise
# endif
# define __force
#endif

#endif /* LIBVFN_SUPPORT_COMPILER_H */
