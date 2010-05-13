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

#include "libvirt.h"

#if HAVE_TYPE_VIRNETWORKPTR
#  include "network.h"
#endif
#if HAVE_TYPE_VIRSTORAGEPOOLPTR
#  include "storage_pool.h"
#endif
#if HAVE_TYPE_VIRSTORAGEVOLPTR
#  include "storage_vol.h"
#endif

VALUE m_libvirt;
VALUE c_libvirt_version;

// define additional errors here
VALUE e_Error;               // Error - generic error
VALUE e_ConnectionError;     // ConnectionError - error durring connection establishment
VALUE e_DefinitionError;     // DefinitionError - error during data definition
VALUE e_RetrieveError;       // RetrievalError - error during data retrieval


/*
 * call-seq:
 *   Libvirt::version(type) -> [ libvirt_version, type_version ]
 *
 * Call
 * +virGetVersion+[http://www.libvirt.org/html/libvirt-libvirt.html#virGetVersion]
 * to get the version of libvirt and of the hypervisor TYPE. Returns an
 * array with two entries of type Libvirt::Version.
 *
 */
VALUE libvirt_version(VALUE m, VALUE t)
{
  unsigned long libVer;
  const char *type = NULL;
  unsigned long typeVer;
  int r;
  VALUE result, argv[2];

  type = StringValueCStr(t);
  r = virGetVersion(&libVer, type, &typeVer);
  if (r < 0)
    rb_raise(rb_eArgError, "Failed to get version for %s", type);

  result = rb_ary_new2(2);

  argv[0] = rb_str_new2("libvirt");
  argv[1] = ULONG2NUM(libVer);
  rb_ary_push(result, rb_class_new_instance(2, argv, c_libvirt_version));

  argv[0] = t;
  argv[1] = ULONG2NUM(typeVer);
  rb_ary_push(result, rb_class_new_instance(2, argv, c_libvirt_version));

  return result;
}

static void vir_error(VALUE exception)
{
  rb_exc_raise(exception);
}

static char *get_string_or_nil(VALUE arg)
{
  if (TYPE(arg) == T_NIL)
    return NULL;
  else if (TYPE(arg) == T_STRING)
    return StringValueCStr(arg);
  else
    rb_raise(rb_eTypeError, "wrong argument type (expected String or nil)");  return NULL;
}

static VALUE create_error(VALUE error, const char* method, const char* msg, virConnectPtr conn)
{
  VALUE ruby_errinfo;
  virErrorPtr err;

  if (msg == NULL || strlen(msg) == 0)
  {
    char *defmsg;
    size_t len;
    len = snprintf(NULL, 0, "Call to function %s failed", method) + 1;
    defmsg = ALLOC_N(char, len);
    snprintf(defmsg, len, "Call to function %s failed", method);
    ruby_errinfo = rb_exc_new2(error, defmsg);
    free(defmsg);
  }
  else
  {
    ruby_errinfo = rb_exc_new2(error, msg);
  }

  rb_iv_set(ruby_errinfo, "@libvirt_function_name", rb_str_new2(method));

  if (conn == NULL)
    err = virGetLastError();
  else
    err = virConnGetLastError(conn);

  if (err != NULL && err->message != NULL)
  {
    rb_iv_set(ruby_errinfo, "@libvirt_message", rb_str_new2(err->message));
  }

  return ruby_errinfo;
};

static void domain_free(void *d)
{
  generic_free(Domain, d);
}

static virDomainPtr domain_get(VALUE s)
{
  generic_get(Domain, s);
}

static VALUE domain_new(virDomainPtr d, VALUE conn)
{
  return generic_new(c_domain, d, conn, domain_free);
}

static VALUE generic_new(VALUE klass, void *ptr, VALUE conn, RUBY_DATA_FUNC free_func)
{
  VALUE result;
  result = Data_Wrap_Struct(klass, NULL, free_func, ptr);
  rb_iv_set(result, "@connection", conn);
  return result;
}

void Init_main()
{
  int r;

  m_libvirt = rb_define_module("Libvirt");
  c_libvirt_version = rb_define_class_under(m_libvirt, "Version", rb_cObject);

  rb_define_module_function(m_libvirt, "version", libvirt_version, 1);

  /*
   * Libvirt Errors
   */
  e_Error = rb_define_class_under(m_libvirt, "Error", rb_eStandardError);
  e_ConnectionError = rb_define_class_under(m_libvirt, "ConnectionError", e_Error);
  e_DefinitionError = rb_define_class_under(m_libvirt, "DefinitionError", e_Error);
  e_RetrieveError = rb_define_class_under(m_libvirt, "RetrieveError", e_Error);

  // create 'libvirt_function_name' and 'vir_connect_ptr' attributes on e_Error class
  rb_define_attr(e_Error, "libvirt_function_name", 1, 0);
  rb_define_attr(e_Error, "libvirt_message", 1, 0);

  /*
   * Class Libvirt::Connect and Libvirt::Connect::Nodeinfo
   */  
  init_connect();

  #if HAVE_TYPE_VIRSTORAGEPOOLPTR
  init_storage_pool();
  #endif

  /*
   * Class Libvirt::Domain, Libvirt::Domain::Info and Libvirt::Domain::InterfaceInfo
   */  
  init_domain();

  /*
   * Class Libvirt::Network
   */
  #if HAVE_TYPE_VIRNETWORKPTR
  init_libvirt_network();
  #endif

  /*
   * Class Libvirt::StoragePool and Libvirt::StoragePoolInfo
   */
  #if HAVE_TYPE_VIRSTORAGEPOOLPTR
  init_storage_pool();
  #endif

  /*
   * Class Libvirt::StorageVol and Libvirt::StorageVolInfo
   */
  #if HAVE_TYPE_VIRSTORAGEVOLPTR
  init_storage_vol();
  #endif

  r = virInitialize();
  if (r < 0)
  rb_raise(rb_eSystemCallError, "virInitialize failed");
}
