/*
 * ruby-libvirt: Ruby bindings for libvirt
 *
 * Copyright (C) 2007 Red Hat Inc.
 * Copyright (C) 2010 Oversun-Scalaxy.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 *
 * Author: David Lutterkort <dlutter@redhat.com>
 * Author: Ivan Samsonov  <hronya@gmail.com>
 */

#ifndef RUBY_LIBVIRT_MAIN
#define RUBY_LIBVIRT_MAIN

#include <ruby.h>
#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>
#include "extconf.h"
#include "connect.h"
#include "domain.h"

#if HAVE_TYPE_VIRNETWORKPTR
#  include "network.h"
#endif
#if HAVE_TYPE_VIRSTORAGEPOOLPTR
#  include "storage_pool.h"
#endif
#if HAVE_TYPE_VIRSTORAGEVOLPTR
#  include "storage_vol.h"
#endif


// Ruby 1.9 compability
#ifndef RARRAY_LEN
#  define RARRAY_LEN(ary) RARRAY(ary)->len
#endif

#define _E(cond, excep) do { if (cond) vir_error(excep); } while(0)

/*
 * Generate a call to a virConnectNumOf... function. C is the Ruby VALUE
 * holding the connection and OBJS is a token indicating what objects to
 * get the number of, e.g. 'Domains'
 */
#define gen_conn_num_of(c, objs)                                                      \
  do {                                                                                \
    int result;                                                                       \
    virConnectPtr conn = connect_get(c);                                              \
                                                                                      \
    result = virConnectNumOf##objs(conn);                                             \
    _E(result < 0, create_error(e_RetrieveError, "virConnectNumOf" # objs, "", conn));\
                                                                                      \
    return INT2NUM(result);                                                           \
  } while(0)

/*
 * Generate a call to a virConnectList... function. S is the Ruby VALUE
 * holding the connection and OBJS is a token indicating what objects to
 * get the number of, e.g. 'Domains' The list function must return an array
 * of strings, which is returned as a Ruby array
 */
#define gen_conn_list_names(s, objs)                                                \
  do {                                                                              \
    int i, r, num;                                                                  \
    char **names;                                                                   \
    virConnectPtr conn = connect_get(s);                                            \
    VALUE result;                                                                   \
                                                                                    \
    num = virConnectNumOf##objs(conn);                                              \
    _E(num < 0, create_error(e_RetrieveError, "virConnectNumOf" # objs, "", conn)); \
    if (num == 0) {                                                                 \
      /* if num is 0, don't call virConnectList* function */                        \
      result = rb_ary_new2(num);                                                    \
      return result;                                                                \
    }                                                                               \
    names = ALLOC_N(char *, num);                                                   \
    r = virConnectList##objs(conn, names, num);                                     \
    if (r < 0) {                                                                    \
      free(names);                                                                  \
      _E(r < 0, create_error(e_RetrieveError, "virConnectList" # objs, "", conn));  \
    }                                                                               \
                                                                                    \
    result = rb_ary_new2(num);                                                      \
    for (i=0; i<num; i++) {                                                         \
      rb_ary_push(result, rb_str_new2(names[i]));                                   \
      free(names[i]);                                                               \
    }                                                                               \
    free(names);                                                                    \
    return result;                                                                  \
  } while(0)

/* Generate a call to a function FUNC which returns an int error, where -1
 * indicates error and 0 success. The Ruby function will return Qnil on
 * success and throw an exception on error.
 */
#define gen_call_void(func, conn, args...)                    \
  do {                                                        \
    int _r_##func;                                            \
    _r_##func = func(args);                                   \
    _E(_r_##func < 0, create_error(e_Error, #func, "", conn));\
    return Qnil;                                              \
  } while(0)

/* Generate a call to a function FUNC which returns a string. The Ruby
 * function will return the string on success and throw an exception on
 * error. The string returned by FUNC is freed if dealloc is true.
 */
#define gen_call_string(func, conn, dealloc, args...)         \
  do {                                                        \
    const char *str;                                          \
    VALUE result;                                             \
                                                              \
    str = func(args);                                         \
    _E(str == NULL, create_error(e_Error, # func, "", conn)); \
                                                              \
    result = rb_str_new2(str);                                \
    if (dealloc)                                              \
      free((void *) str);                                     \
    return result;                                            \
  } while(0)

/* Generate a call to vir##KIND##Free and return Qnil. Set the the embedded
 * vir##KIND##Ptr to NULL. If that pointer is already NULL, do nothing.
 */
#define gen_call_free(kind, s)                                           \
  do {                                                                   \
    vir##kind##Ptr ptr;                                                  \
    Data_Get_Struct(s, vir##kind, ptr);                                  \
    if (ptr != NULL) {                                                   \
      int r = vir##kind##Free(ptr);                                      \
      _E(r < 0, create_error(e_Error, "vir" #kind "Free", "", conn(s))); \
      DATA_PTR(s) = NULL;                                                \
    }                                                                    \
    return Qnil;                                                         \
  } while (0)

#define generic_free(kind, p)                              \
  do {                                                     \
    int r;                                                 \
    r = vir##kind##Free((vir##kind##Ptr) p);               \
    if (r < 0)                                             \
      rb_raise(rb_eSystemCallError, # kind " free failed");\
  } while(0);

#define generic_get(kind, v)                          \
  do {                                                \
    vir##kind##Ptr ptr;                               \
    Data_Get_Struct(v, vir##kind, ptr);               \
    if (!ptr)                                         \
      rb_raise(rb_eArgError, #kind " has been freed");\
    return ptr;                                       \
  } while (0);


NORETURN(void vir_error(VALUE exception));
char *get_string_or_nil(VALUE arg);
VALUE create_error(VALUE error, const char* method, const char* msg, virConnectPtr conn);

void domain_free(void *d);
virDomainPtr domain_get(VALUE s);
VALUE domain_new(virDomainPtr d, VALUE conn);

VALUE generic_new(VALUE klass, void *ptr, VALUE conn, RUBY_DATA_FUNC free_func);



extern VALUE m_libvirt;
extern VALUE c_libvirt_version;

extern VALUE e_Error;
extern VALUE e_ConnectionError;
extern VALUE e_DefinitionError;
extern VALUE e_RetrieveError;

#endif
