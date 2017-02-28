/*
 * Copyright (c) 2017, Circonus, Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *    * Neither the name Circonus, Inc. nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MTEV_MAYBE_ALLOC_H
#define MTEV_MAYBE_ALLOC_H

#include <stdlib.h>

#define MTEV_MAYBE_DECL_TYPE(type, type_name, cnt) \
  struct type_name {                               \
    size_t sz;                                     \
    type   static_buff[cnt];                       \
    type   *data;                                  \
    type   *pos;                                   \
  }

/*! \fn MTEV_MAYBE_DECL_VARS(type, name, cnt)
    \brief C Macro for declaring a "maybe" buffer.
    \param type A C type (e.g. char)
    \param name The name of the C variable to declare.
    \param cnt The number of type elements initially declared.
 */
#define MTEV_MAYBE_DECL_VARS(type, name, cnt) \
  MTEV_MAYBE_DECL_TYPE(type, ##name##_t, cnt) name;

/*! \fn MTEV_MAYBE_INIT_VARS(name)
    \brief C Macro for initializing a "maybe" buffer
    \param name The name of "maybe" buffer.
 */
#define MTEV_MAYBE_INIT_VARS(name)              \
  (name).sz = sizeof((name).static_buff);       \
  (name).data = (name).static_buff;             \
  (name).pos = (name).data

/*! \fn MTEV_MAYBE_DECL(type, name, cnt)
    \brief C Macro for declaring a "maybe" buffer.
    \param type A C type (e.g. char)
    \param name The name of the C variable to declare.
    \param cnt The number of type elements initially declared.

    A "maybe" buffer is a buffer that is allocated on-stack, but
    if more space is required can be reallocated off stack (malloc).
    One should always call `MTEV_MAYBE_FREE` on any allocated
    maybe buffer.
 */
#define MTEV_MAYBE_DECL(type, name, cnt) \
  MTEV_MAYBE_DECL_VARS(type, name, cnt); \
  MTEV_MAYBE_INIT_VARS(name)

/*! \fn MTEV_MAYBE_IZE(name)
    \brief C Macro for number of bytes available in this buffer.
    \param name The name of the "maybe" buffer.
 */
#define MTEV_MAYBE_SIZE(name) ((name).sz + 0)

/*! \fn MTEV_MAYBE_USED(name)
    \brief C Macro for number of bytes used in this buffer.
    \param name The name of the "maybe" buffer.
 */
#define MTEV_MAYBE_USED(name) ((name).pos - (name).data)

/*! \fn MTEV_MAYBE_AVAIL(name)
    \brief C Macro for number of bytes available at the end of this buffer
    \param name The name of the "maybe" buffer.
 */
#define MTEV_MAYBE_AVAIL(name) ((name).sz - (name).pos - (name).data)

/*! \fn MTEV_MAYBE_REALLOC(name, cnt)
    \brief C Macro to ensure a maybe buffer has at least cnt elements allocated.
    \param name The name of the "maybe" buffer.
    \param cnt The total number of elements expected in the allocation.

    This macro will never reduce the size and is a noop if a size smaller
    than or equal to the current allocation size is specified.  It is safe
    to simply run this macro prior to each write to the buffer.
 */
#define MTEV_MAYBE_REALLOC(name, cnt)           \
  do {                                              \
    if((name).sz < (cnt) * sizeof(*((name).data))) {  \
      size_t prevsz = (name).sz;                      \
      (name).sz = (cnt) * sizeof(*((name).data));     \
      ptrdiff_t diff = (name).pos - (name).data;      \
      if((name).data != (name).static_buff) {         \
        (name).data = realloc((name).data, (name).sz);  \
        (name).pos = (name).data + diff;                \
      } else {                                          \
        (name).data = malloc((name).sz);                  \
        memcpy((name).data, (name).static_buff, prevsz);  \
        (name).pos = (name).data + diff;                  \
      }                                                   \
    }                                                     \
  } while(0)

#define MTEV_MAYBE_DATA(name) (name).data
#define MTEV_MAYBE_POS(name) (name).pos

/*! \fn MTEV_MAYBE_FREE(name)
    \brief C Macro to free any heap space associated with a "maybe" buffer.
    \param name The name of the "maybe" buffer.
 */
#define MTEV_MAYBE_FREE(name) \
  do { if((name).data != (name).static_buff) free((name).data); } while(0)

#endif
