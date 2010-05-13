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

#include "storage_vol.h"

VALUE c_storage_vol;
VALUE c_storage_vol_info;


/*
 * Call +virStorageVolLookupByName+[http://www.libvirt.org/html/libvirt-libvirt.html#virStorageVolLookupByName]
 */
VALUE libvirt_pool_lookup_vol_by_name(VALUE p, VALUE name)
{
  virStorageVolPtr vol;

  vol = virStorageVolLookupByName(pool_get(p), StringValueCStr(name));
  _E(vol == NULL, create_error(e_RetrieveError, "virStorageVolLookupByName", "", conn(p)));

  return vol_new(vol, conn_attr(p));
}

/*
 * Call +virStorageVolLookupByKey+[http://www.libvirt.org/html/libvirt-libvirt.html#virStorageVolLookupByKey]
 */
VALUE libvirt_pool_lookup_vol_by_key(VALUE p, VALUE key)
{
  virStorageVolPtr vol;

  // FIXME: Why does this take a connection, not a pool ?
  vol = virStorageVolLookupByKey(conn(p), StringValueCStr(key));
  _E(vol == NULL, create_error(e_RetrieveError, "virStorageVolLookupByKey", "", conn(p)));

  return vol_new(vol, conn_attr(p));
}

/*
 * Call +virStorageVolLookupByPath+[http://www.libvirt.org/html/libvirt-libvirt.html#virStorageVolLookupByPath]
 */
VALUE libvirt_pool_lookup_vol_by_path(VALUE p, VALUE path)
{
  virStorageVolPtr vol;

  // FIXME: Why does this take a connection, not a pool ?
  vol = virStorageVolLookupByPath(conn(p), StringValueCStr(path));
  _E(vol == NULL, create_error(e_RetrieveError, "virStorageVolLookupByPath", "", conn(p)));

  return vol_new(vol, conn_attr(p));
}

/*
 * Call +virStoragePoolLookupByVolume+[http://www.libvirt.org/html/libvirt-libvirt.html#virStoragePoolLookupByVolume]
 */
VALUE libvirt_vol_get_pool(VALUE v)
{
  virStoragePoolPtr pool;

  pool = virStoragePoolLookupByVolume(vol_get(v));
  _E(pool == NULL, create_error(e_RetrieveError, "virStoragePoolLookupByVolume", "", conn(v)));

  return pool_new(pool, conn_attr(v));
}

/*
 * Call +virStorageVolGetName+[http://www.libvirt.org/html/libvirt-libvirt.html#virStorageVolGetName]
 */
VALUE libvirt_vol_name(VALUE v)
{
  gen_call_string(virStorageVolGetName, conn(v), 0, vol_get(v));
}

/*
 * Call +virStorageVolGetKey+[http://www.libvirt.org/html/libvirt-libvirt.html#virStorageVolGetKey]
 */
VALUE libvirt_vol_key(VALUE v)
{
  gen_call_string(virStorageVolGetKey, conn(v), 0, vol_get(v));
}

/*
 * Call +virStorageVolCreateXML+[http://www.libvirt.org/html/libvirt-libvirt.html#virStorageVolCreateXML]
 */
VALUE libvirt_pool_vol_create_xml(int argc, VALUE *argv, VALUE p)
{
  virStorageVolPtr vol;
  virConnectPtr c = conn(p);
  char *xmlDesc;
  VALUE xml, flags;

  rb_scan_args(argc, argv, "11", &xml, &flags);

  if (NIL_P(flags))
    flags = INT2FIX(0);

  xmlDesc = StringValueCStr(xml);

  vol = virStorageVolCreateXML(pool_get(p), xmlDesc, NUM2UINT(flags));
  _E(vol == NULL, create_error(e_Error, "virNetworkCreateXML", "", c));

  return vol_new(vol, conn_attr(p));
}

/*
 * Call +virStorageVolDelete+[http://www.libvirt.org/html/libvirt-libvirt.html#virStorageVolDelete]
 */
VALUE libvirt_vol_delete(int argc, VALUE *argv, VALUE v)
{
  VALUE flags;

  rb_scan_args(argc, argv, "01", &flags);

  if (NIL_P(flags))
    flags = INT2FIX(0);

  gen_call_void(virStorageVolDelete, conn(v), vol_get(v), NUM2UINT(flags));
}

/*
 * Call +virStorageVolGetInfo+[http://www.libvirt.org/html/libvirt-libvirt.html#virStorageVolGetInfo]
 */
VALUE libvirt_vol_info(VALUE v)
{
  virStorageVolInfo info;
  int r;
  VALUE result;

  r = virStorageVolGetInfo(vol_get(v), &info);
  _E(r < 0, create_error(e_RetrieveError, "virStorageVolGetInfo", "", conn(v)));

  result = rb_class_new_instance(0, NULL, c_storage_vol_info);
  rb_iv_set(result, "@type", INT2NUM(info.type));
  rb_iv_set(result, "@capacity", ULL2NUM(info.capacity));
  rb_iv_set(result, "@allocation", ULL2NUM(info.allocation));

  return result;
}

/*
 * Call +virStorageVolGetXMLDesc+[http://www.libvirt.org/html/libvirt-libvirt.html#virStorageVolGetXMLDesc]
 */
VALUE libvirt_vol_xml_desc(int argc, VALUE *argv, VALUE v)
{
  VALUE flags;

  rb_scan_args(argc, argv, "01", &flags);

  if (NIL_P(flags))
    flags = INT2FIX(0);

  gen_call_string(virStorageVolGetXMLDesc, conn(v), 1, vol_get(v), NUM2UINT(flags));
}

/*
 * Call +virStorageVolGetPath+[http://www.libvirt.org/html/libvirt-libvirt.html#virStorageVolGetPath]
 */
VALUE libvirt_vol_path(VALUE v)
{
  gen_call_string(virStorageVolGetPath, conn(v), 1, vol_get(v));
}

/*
 * Call +virStorageVolFree+[http://www.libvirt.org/html/libvirt-libvirt.html#virStorageVolFree]
 */
VALUE libvirt_vol_free(VALUE s)
{
  gen_call_free(StorageVol, s);
}

static void vol_free(void *d)
{
  generic_free(StorageVol, d);
}

static virStorageVolPtr vol_get(VALUE s)
{
  generic_get(StorageVol, s);
}

static VALUE vol_new(virStorageVolPtr n, VALUE conn)
{
  return generic_new(c_storage_vol, n, conn, vol_free);
}



#define DEF_VOLCONST(name) rb_define_const(c_storage_vol, #name, INT2NUM(VIR_STORAGE_VOL_##name))

void init_storage_vol()
{
  c_storage_vol_info = rb_define_class_under(m_libvirt, "StorageVolInfo", rb_cObject);
  rb_define_attr(c_storage_vol_info, "type", 1, 0);
  rb_define_attr(c_storage_vol_info, "capacity", 1, 0);
  rb_define_attr(c_storage_vol_info, "allocation", 1, 0);

  c_storage_vol = rb_define_class_under(m_libvirt, "StorageVol", rb_cObject);

  /* virStorageVolType */
  DEF_VOLCONST(FILE);
  DEF_VOLCONST(BLOCK);

  /* virStorageVolDeleteFlags */
  DEF_VOLCONST(DELETE_NORMAL);
  DEF_VOLCONST(DELETE_ZEROED);

  rb_define_method(c_storage_vol, "pool", libvirt_vol_get_pool, 0);
  rb_define_method(c_storage_vol, "name", libvirt_vol_name, 0);
  rb_define_method(c_storage_vol, "key", libvirt_vol_key, 0);
  rb_define_method(c_storage_vol, "delete", libvirt_vol_delete, -1);
  rb_define_method(c_storage_vol, "info", libvirt_vol_info, 0);
  rb_define_method(c_storage_vol, "xml_desc", libvirt_vol_xml_desc, -1);
  rb_define_method(c_storage_vol, "path", libvirt_vol_path, 0);
  rb_define_method(c_storage_vol, "free", libvirt_vol_free, 0);  

  /* Lookup volumes based on various attributes */
  rb_define_method(c_storage_pool, "lookup_volume_by_name", libvirt_pool_lookup_vol_by_name, 1);
  rb_define_method(c_storage_pool, "lookup_volume_by_key", libvirt_pool_lookup_vol_by_key, 1);
  rb_define_method(c_storage_pool, "lookup_volume_by_path", libvirt_pool_lookup_vol_by_path, 1);
  rb_define_method(c_storage_pool, "create_vol_xml", libvirt_pool_vol_create_xml, -1);
}

#undef DEF_VOLCONST
