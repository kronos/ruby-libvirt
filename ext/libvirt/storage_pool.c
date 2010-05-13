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

#include "storage_pool.h"

VALUE c_storage_pool;
VALUE c_storage_pool_info;

/*
 * Call +virStoragePoolLookupByName+[http://www.libvirt.org/html/libvirt-libvirt.html#virStoragePoolLookupByName]
 */
VALUE libvirt_conn_lookup_pool_by_name(VALUE c, VALUE name)
{
  virStoragePoolPtr pool;
  virConnectPtr conn = connect_get(c);

  pool = virStoragePoolLookupByName(conn, StringValueCStr(name));
  _E(pool == NULL, create_error(e_RetrieveError, "virStoragePoolLookupByName", "", conn));

  return pool_new(pool, c);
}

/*
 * Call +virStoragePoolLookupByUUIDString+[http://www.libvirt.org/html/libvirt-libvirt.html#virStoragePoolLookupByUUIDString]
 */
VALUE libvirt_conn_lookup_pool_by_uuid(VALUE c, VALUE uuid)
{
  virStoragePoolPtr pool;
  virConnectPtr conn = connect_get(c);

  pool = virStoragePoolLookupByUUIDString(conn, StringValueCStr(uuid));
  _E(pool == NULL, create_error(e_RetrieveError, "virStoragePoolLookupByUUID", "", conn));

  return pool_new(pool, c);
}

/*
 * Call +virStoragePoolCreateXML+[http://www.libvirt.org/html/libvirt-libvirt.html#virStoragePoolCreateXML]
 */
VALUE libvirt_conn_create_pool_xml(int argc, VALUE *argv, VALUE c)
{
  virStoragePoolPtr pool;
  virConnectPtr conn = connect_get(c);
  char *xmlDesc;
  VALUE xml, flags;

  rb_scan_args(argc, argv, "11", &xml, &flags);

  if (NIL_P(flags))
    flags = INT2FIX(0);

  xmlDesc = StringValueCStr(xml);

  pool = virStoragePoolCreateXML(conn, xmlDesc, NUM2UINT(flags));
  _E(pool == NULL, create_error(e_Error, "virStoragePoolCreateXML", "", conn));

  return pool_new(pool, c);
}

/*
 * Call +virStoragePoolDefineXML+[http://www.libvirt.org/html/libvirt-libvirt.html#virStoragePoolDefineXML]
 */
VALUE libvirt_conn_define_pool_xml(int argc, VALUE *argv, VALUE c)
{
  virStoragePoolPtr pool;
  virConnectPtr conn = connect_get(c);
  VALUE xml, flags;

  rb_scan_args(argc, argv, "11", &xml, &flags);

  if (NIL_P(flags))
    flags = INT2FIX(0);

  pool = virStoragePoolDefineXML(conn, StringValueCStr(xml), NUM2UINT(flags));
  _E(pool == NULL, create_error(e_DefinitionError, "virStoragePoolDefineXML", "", conn));

  return pool_new(pool, c);
}

/*
 * Call +virConnectFindStoragePoolSources+[http://www.libvirt.org/html/libvirt-libvirt.html#virConnectFindStoragePoolSources]
 */
VALUE libvirt_conn_find_storage_pool_sources(int argc, VALUE *argv, VALUE c)
{
  virConnectPtr conn = connect_get(c);
  VALUE type, srcSpec_val, flags;
  const char *srcSpec;

  rb_scan_args(argc, argv, "12", &type, &srcSpec_val, &flags);

  srcSpec = get_string_or_nil(srcSpec_val);

  if (NIL_P(flags))
    flags = INT2FIX(0);

  gen_call_string(virConnectFindStoragePoolSources, conn, 1, conn, StringValueCStr(type), srcSpec, NUM2UINT(flags));
}

/*
 * Call +virStoragePoolBuild+[http://www.libvirt.org/html/libvirt-libvirt.html#virStoragePoolBuild]
 */
VALUE libvirt_pool_build(int argc, VALUE *argv, VALUE p)
{
  VALUE flags;

  rb_scan_args(argc, argv, "01", &flags);

  if (NIL_P(flags))
    flags = INT2FIX(0);

  gen_call_void(virStoragePoolBuild, conn(p), pool_get(p), NUM2UINT(flags));
}

/*
 * Call +virStoragePoolUndefine+[http://www.libvirt.org/html/libvirt-libvirt.html#virStoragePoolUndefine]
 */
VALUE libvirt_pool_undefine(VALUE p)
{
  gen_call_void(virStoragePoolUndefine, conn(p), pool_get(p));
}

/*
 * Call +virStoragePoolCreate+[http://www.libvirt.org/html/libvirt-libvirt.html#virStoragePoolCreate]
 */
VALUE libvirt_pool_create(int argc, VALUE *argv, VALUE p)
{
  VALUE flags;

  rb_scan_args(argc, argv, "01", &flags);

  if (NIL_P(flags))
    flags = INT2FIX(0);

  gen_call_void(virStoragePoolCreate, conn(p), pool_get(p), NUM2UINT(flags));
}

/*
 * Call +virStoragePoolDestroy+[http://www.libvirt.org/html/libvirt-libvirt.html#virStoragePoolDestroy]
 */
VALUE libvirt_pool_destroy(VALUE p)
{
  gen_call_void(virStoragePoolDestroy, conn(p), pool_get(p));
}

/*
 * Call +virStoragePoolDelete+[http://www.libvirt.org/html/libvirt-libvirt.html#virStoragePoolDelete]
 */
VALUE libvirt_pool_delete(int argc, VALUE *argv, VALUE p)
{
  VALUE flags;

  rb_scan_args(argc, argv, "01", &flags);

  if (NIL_P(flags))
    flags = INT2FIX(0);

  gen_call_void(virStoragePoolDelete, conn(p), pool_get(p), NUM2UINT(flags));
}

/*
 * Call +virStoragePoolRefresh+[http://www.libvirt.org/html/libvirt-libvirt.html#virStoragePoolRefresh]
 */
VALUE libvirt_pool_refresh(int argc, VALUE *argv, VALUE p)
{
  VALUE flags;

  rb_scan_args(argc, argv, "01", &flags);

  if (NIL_P(flags))
    flags = INT2FIX(0);

  gen_call_void(virStoragePoolRefresh, conn(p), pool_get(p), NUM2UINT(flags));
}

/*
 * Call +virStoragePoolGetName+[http://www.libvirt.org/html/libvirt-libvirt.html#virStoragePoolGetName]
 */
VALUE libvirt_pool_name(VALUE s)
{
  const char *name;

  name = virStoragePoolGetName(pool_get(s));
  _E(name == NULL, create_error(e_RetrieveError, "virStoragePoolGetName", "", conn(s)));

  return rb_str_new2(name);
}

/*
 * Call +virStoragePoolGetUUIDString+[http://www.libvirt.org/html/libvirt-libvirt.html#virStoragePoolGetUUIDString]
 */
VALUE libvirt_pool_uuid(VALUE s)
{
  char uuid[VIR_UUID_STRING_BUFLEN];
  int r;

  r = virStoragePoolGetUUIDString(pool_get(s), uuid);
  _E(r < 0, create_error(e_RetrieveError, "virStoragePoolGetUUIDString", "", conn(s)));

  return rb_str_new2((char *) uuid);
}

/*
 * Call +virStoragePoolGetInfo+[http://www.libvirt.org/html/libvirt-libvirt.html#virStoragePoolGetInfo]
 */
VALUE libvirt_pool_info(VALUE s)
{
  virStoragePoolInfo info;
  int r;
  VALUE result;

  r = virStoragePoolGetInfo(pool_get(s), &info);
  _E(r < 0, create_error(e_RetrieveError, "virStoragePoolGetInfo", "", conn(s)));

  result = rb_class_new_instance(0, NULL, c_storage_pool_info);
  rb_iv_set(result, "@state", INT2FIX(info.state));
  rb_iv_set(result, "@capacity", ULL2NUM(info.capacity));
  rb_iv_set(result, "@allocation", ULL2NUM(info.allocation));
  rb_iv_set(result, "@available", ULL2NUM(info.available));

  return result;
}

/*
 * Call +virStoragePoolGetXMLDesc+[http://www.libvirt.org/html/libvirt-libvirt.html#virStoragePoolGetXMLDesc]
 */
VALUE libvirt_pool_xml_desc(int argc, VALUE *argv, VALUE s)
{
  VALUE flags;

  rb_scan_args(argc, argv, "01", &flags);

  if (NIL_P(flags))
    flags = INT2FIX(0);

  gen_call_string(virStoragePoolGetXMLDesc, conn(s), 1, pool_get(s), NUM2UINT(flags));
}

/*
 * Call +virStoragePoolGetAutostart+[http://www.libvirt.org/html/libvirt-libvirt.html#virStoragePoolGetAutostart]
 */
VALUE libvirt_pool_autostart(VALUE s)
{
  int r, autostart;

  r = virStoragePoolGetAutostart(pool_get(s), &autostart);
  _E(r < 0, create_error(e_RetrieveError, "virStoragePoolGetAutostart", "", conn(s)));

  return autostart ? Qtrue : Qfalse;
}

/*
 * Call +virStoragePoolSetAutostart+[http://www.libvirt.org/html/libvirt-libvirt.html#virStoragePoolSetAutostart]
 */
VALUE libvirt_pool_autostart_set(VALUE s, VALUE autostart)
{
  gen_call_void(virStoragePoolSetAutostart, conn(s), pool_get(s), RTEST(autostart) ? 1 : 0);
}

/*
 * Call +virStoragePoolNumOfVolumes+[http://www.libvirt.org/html/libvirt-libvirt.html#virStoragePoolNumOfVolumes]
 */
VALUE libvirt_pool_num_of_volumes(VALUE s)
{
  int n = virStoragePoolNumOfVolumes(pool_get(s));
  _E(n < 0, create_error(e_RetrieveError, "virStoragePoolNumOfVolumes", "", conn(s)));

  return INT2FIX(n);
}

/*
 * Call +virStoragePoolListVolumes+[http://www.libvirt.org/html/libvirt-libvirt.html#virStoragePoolListVolumes]
 */
VALUE libvirt_pool_list_volumes(VALUE s)
{
  int i, r, num;
  char **names;
  virStoragePoolPtr pool = pool_get(s);
  VALUE result;

  num = virStoragePoolNumOfVolumes(pool);
  _E(num < 0, create_error(e_RetrieveError, "virStoragePoolNumOfVolumes", "", conn(s)));
  if (num == 0)
  {
    result = rb_ary_new2(num);
    return result;
  }

  names = ALLOC_N(char *, num);
  r = virStoragePoolListVolumes(pool, names, num);
  if (r < 0)
  {
    free(names);
    _E(r < 0, create_error(e_RetrieveError, "virStoragePoolListVolumes", "", conn(s)));
  }

  result = rb_ary_new2(num);
  for (i=0; i<num; i++)
  {
    rb_ary_push(result, rb_str_new2(names[i]));
    // FIXME: Should these really be freed ?
    free(names[i]);
  }
  free(names);
  return result;
}

/*
 * Call +virStoragePoolFree+[http://www.libvirt.org/html/libvirt-libvirt.html#virStoragePoolFree]
 */
VALUE libvirt_pool_free(VALUE s)
{
  gen_call_free(StoragePool, s);
}

/*
 * Call +virConnectListStoragePools+[http://www.libvirt.org/html/libvirt-libvirt.html#virConnectListStoragePools]
 */
VALUE libvirt_conn_list_storage_pools(VALUE s)
{
  gen_conn_list_names(s, StoragePools);
}

/*
 * Call +virConnectNumOfStoragePools+[http://www.libvirt.org/html/libvirt-libvirt.html#virConnectNumOfStoragePools]
 */
VALUE libvirt_conn_num_of_storage_pools(VALUE s)
{
  gen_conn_num_of(s, StoragePools);
}

/*
 * Call +virConnectListDefinedStoragePools+[http://www.libvirt.org/html/libvirt-libvirt.html#virConnectListDefinedStoragePools]
 */
VALUE libvirt_conn_list_defined_storage_pools(VALUE s)
{
  gen_conn_list_names(s, DefinedStoragePools);
}

/*
 * Call +virConnectNumOfDefinedStoragePools+[http://www.libvirt.org/html/libvirt-libvirt.html#virConnectNumOfDefinedStoragePools]
 */
VALUE libvirt_conn_num_of_defined_storage_pools(VALUE s)
{
  gen_conn_num_of(s, DefinedStoragePools);
}

static void pool_free(void *d)
{
  generic_free(StoragePool, d);
}

static virStoragePoolPtr pool_get(VALUE s)
{
  generic_get(StoragePool, s);
}

static VALUE pool_new(virStoragePoolPtr n, VALUE conn)
{
  return generic_new(c_storage_pool, n, conn, pool_free);
}

#define DEF_POOLCONST(name) rb_define_const(c_storage_pool, #name, INT2NUM(VIR_STORAGE_POOL_##name))

void init_storage_pool()
{
  c_storage_pool_info = rb_define_class_under(m_libvirt, "StoragePoolInfo", rb_cObject);
  rb_define_attr(c_storage_pool_info, "state", 1, 0);
  rb_define_attr(c_storage_pool_info, "capacity", 1, 0);
  rb_define_attr(c_storage_pool_info, "allocation", 1, 0);
  rb_define_attr(c_storage_pool_info, "available", 1, 0);

  c_storage_pool = rb_define_class_under(m_libvirt, "StoragePool", rb_cObject);

  /* virStoragePoolState */
  DEF_POOLCONST(INACTIVE);
  DEF_POOLCONST(BUILDING);
  DEF_POOLCONST(RUNNING);
  DEF_POOLCONST(DEGRADED);

  /* virStoragePoolBuildFlags */
  DEF_POOLCONST(BUILD_NEW);
  DEF_POOLCONST(BUILD_REPAIR);
  DEF_POOLCONST(BUILD_RESIZE);

  /* virStoragePoolDeleteFlags */
  DEF_POOLCONST(DELETE_NORMAL);
  DEF_POOLCONST(DELETE_ZEROED);

  /* Creating/destroying pools */
  rb_define_method(c_storage_pool, "build", libvirt_pool_build, -1);
  rb_define_method(c_storage_pool, "undefine", libvirt_pool_undefine, 0);
  rb_define_method(c_storage_pool, "create", libvirt_pool_create, -1);
  rb_define_method(c_storage_pool, "destroy", libvirt_pool_destroy, 0);
  rb_define_method(c_storage_pool, "delete", libvirt_pool_delete, -1);
  rb_define_method(c_storage_pool, "refresh", libvirt_pool_refresh, -1);

  /* StoragePool information */
  rb_define_method(c_storage_pool, "name", libvirt_pool_name, 0);
  rb_define_method(c_storage_pool, "uuid", libvirt_pool_uuid, 0);
  rb_define_method(c_storage_pool, "info", libvirt_pool_info, 0);
  rb_define_method(c_storage_pool, "xml_desc", libvirt_pool_xml_desc, -1);
  rb_define_method(c_storage_pool, "autostart", libvirt_pool_autostart, 0);
  rb_define_method(c_storage_pool, "autostart=", libvirt_pool_autostart_set, 1);

  /* List/lookup storage volumes within a pool */
  rb_define_method(c_storage_pool, "num_of_volumes", libvirt_pool_num_of_volumes, 0);
  rb_define_method(c_storage_pool, "list_volumes", libvirt_pool_list_volumes, 0);

  rb_define_method(c_storage_pool, "free", libvirt_pool_free, 0);

  rb_define_method(c_connect, "num_of_storage_pools", libvirt_conn_num_of_storage_pools, 0);
  rb_define_method(c_connect, "list_storage_pools", libvirt_conn_list_storage_pools, 0);
  rb_define_method(c_connect, "num_of_defined_storage_pools", libvirt_conn_num_of_defined_storage_pools, 0);
  rb_define_method(c_connect, "list_defined_storage_pools", libvirt_conn_list_defined_storage_pools, 0);
  rb_define_method(c_connect, "lookup_storage_pool_by_name", libvirt_conn_lookup_pool_by_name, 1);
  rb_define_method(c_connect, "lookup_storage_pool_by_uuid", libvirt_conn_lookup_pool_by_uuid, 1);
  rb_define_method(c_connect, "create_storage_pool_xml", libvirt_conn_create_pool_xml, -1);
  rb_define_method(c_connect, "define_storage_pool_xml", libvirt_conn_define_pool_xml, -1);
  rb_define_method(c_connect, "discover_storage_pool_sources", libvirt_conn_find_storage_pool_sources, -1);
}

#undef DEF_POOLCONST
