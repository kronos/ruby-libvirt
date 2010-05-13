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

VALUE c_connect;
VALUE c_node_info;

/*
 * call-seq:
 *   Libvirt::open(url) -> Libvirt::Connect
 *
 * Open a connection to URL with virConnectOpen[http://www.libvirt.org/html/libvirt-libvirt.html#virConnectOpen]
 */
VALUE libvirt_open(VALUE m, VALUE url)
{
  char *str = NULL;

  if (url)
  {
    str = StringValueCStr(url);
    if (!str)
      rb_raise(rb_eTypeError, "expected string");
  }

  virConnectPtr ptr = virConnectOpen(str);

  if (!ptr)
  rb_raise(e_ConnectionError, "Failed to open %s", str);

  return connect_new(ptr);
}

/*
 * call-seq:
 *   Libvirt::openReadOnly(url) -> Libvirt::Connect
 *
 * Open a read-only connection to URL with
 * virConnectOpenReadOnly[http://www.libvirt.org/html/libvirt-libvirt.html#virConnectOpenReadOnly]
 */
VALUE libvirt_open_read_only(VALUE m, VALUE url)
{
  char *str = NULL;

  if (url)
  {
    str = StringValueCStr(url);
    if (!str)
      rb_raise(rb_eTypeError, "expected string");
  }

  virConnectPtr ptr = virConnectOpenReadOnly(str);

  if (!ptr)
  rb_raise(e_ConnectionError, "Failed to open %s readonly", str);

  return connect_new(ptr);
}

/*
 * call-seq:
 *   conn.close
 *
 * Close the connection
 */
VALUE libvirt_conn_close(VALUE s)
{
  virConnectPtr conn;
  Data_Get_Struct(s, virConnect, conn);

  if (conn)
  {
    connect_close(conn);
    DATA_PTR(s) = NULL;
  }

  return Qnil;
}

/*
 * call-seq:
 *   conn.closed?
 *
 * Return +true+ if the connection is closed, +false+ if it is open
 */
VALUE libvirt_conn_closed_p(VALUE s)
{
  virConnectPtr conn;

  Data_Get_Struct(s, virConnect, conn);

  return (conn==NULL) ? Qtrue : Qfalse;
}

/*
 * call-seq:
 *   conn.type -> string
 *
 * Call +virConnectGetType+[http://www.libvirt.org/html/libvirt-libvirt.html#virConnectGetType]
 */
VALUE libvirt_conn_type(VALUE s)
{
  gen_call_string(virConnectGetType, connect_get(s), 0, connect_get(s));
}

/*
 * call-seq:
 *   conn.version -> fixnum
 *
 * Call +virConnectGetVersion+[http://www.libvirt.org/html/libvirt-libvirt.html#virConnectGetVersion]
 */
VALUE libvirt_conn_version(VALUE s)
{
  int r;
  unsigned long v;
  virConnectPtr conn = connect_get(s);

  r = virConnectGetVersion(conn, &v);
  _E(r < 0, create_error(e_RetrieveError, "virConnectGetVersion", "", conn));

  return ULONG2NUM(v);
}

/*
 * call-seq:
 *   conn.hostname -> string
 *
 * Call +virConnectGetHostname+[http://www.libvirt.org/html/libvirt-libvirt.html#virConnectGetHostname]
 */
VALUE libvirt_conn_hostname(VALUE s) {
  gen_call_string(virConnectGetHostname, connect_get(s), 1, connect_get(s));
}

/*
 * Call +virConnectGetURI+[http://www.libvirt.org/html/libvirt-libvirt.html#virConnectGetURI]
 */
VALUE libvirt_conn_uri(VALUE s)
{
  virConnectPtr conn = connect_get(s);
  gen_call_string(virConnectGetURI, conn, 1,
          conn);
}

/*
 * Call +virConnectGetMaxVcpus+[http://www.libvirt.org/html/libvirt-libvirt.html#virConnectGetMaxVcpus]
 */
VALUE libvirt_conn_max_vcpus(VALUE s, VALUE type)
{
  int result;
  virConnectPtr conn = connect_get(s);

  result = virConnectGetMaxVcpus(conn, StringValueCStr(type));
  _E(result < 0, create_error(e_RetrieveError, "virConnectGetMaxVcpus", "", conn));

  return INT2NUM(result);
}

/*
 * Call +virNodeInfo+[http://www.libvirt.org/html/libvirt-libvirt.html#virNodeGetInfo]
 */
VALUE libvirt_conn_node_get_info(VALUE s)
{
  int r;
  virConnectPtr conn = connect_get(s);
  virNodeInfo nodeinfo;
  VALUE result;
  VALUE modelstr;

  r = virNodeGetInfo(conn, &nodeinfo);
  _E(r < 0, create_error(e_RetrieveError, "virNodeGetInfo", "", conn));

  modelstr = rb_str_new2(nodeinfo.model);

  result = rb_class_new_instance(0, NULL, c_node_info);
  rb_iv_set(result, "@model", modelstr);
  rb_iv_set(result, "@memory", ULONG2NUM(nodeinfo.memory));
  rb_iv_set(result, "@cpus", UINT2NUM(nodeinfo.cpus));
  rb_iv_set(result, "@mhz", UINT2NUM(nodeinfo.mhz));
  rb_iv_set(result, "@nodes", UINT2NUM(nodeinfo.nodes));
  rb_iv_set(result, "@sockets", UINT2NUM(nodeinfo.sockets));
  rb_iv_set(result, "@cores", UINT2NUM(nodeinfo.cores));
  rb_iv_set(result, "@threads", UINT2NUM(nodeinfo.threads));

  return result;
}

/*
 * Call +virConnectGetCapabilities+[http://www.libvirt.org/html/libvirt-libvirt.html#virConnectGetCapabilities]
 */
VALUE libvirt_conn_capabilities(VALUE s)
{
  virConnectPtr conn = connect_get(s);
  gen_call_string(virConnectGetCapabilities, conn, 1, conn);
}

/*
 * Call +virConnectNumOfDomains+[http://www.libvirt.org/html/libvirt-libvirt.html#virConnectNumOfDomains]
 */
VALUE libvirt_conn_num_of_domains(VALUE s)
{
  gen_conn_num_of(s, Domains);
}

/*
 * Call +virConnectListDomains+[http://www.libvirt.org/html/libvirt-libvirt.html#virConnectListDomains]
 */
VALUE libvirt_conn_list_domains(VALUE s)
{
  int i, r, num, *ids;
  virConnectPtr conn = connect_get(s);
  VALUE result;

  num = virConnectNumOfDomains(conn);
  _E(num < 0, create_error(e_RetrieveError, "virConnectNumOfDomains", "", conn));

  if (num == 0)
  {
    result = rb_ary_new2(num);
    return result;
  }

  ids = ALLOC_N(int, num);
  r = virConnectListDomains(conn, ids, num);
  if (r < 0)
  {
    free(ids);
    _E(r < 0, create_error(e_RetrieveError, "virConnectListDomains", "", conn));
  }

  result = rb_ary_new2(num);

  for (i=0; i<num; i++)
  {
    rb_ary_push(result, INT2NUM(ids[i]));
  }

  free(ids);
  return result;
}

/*
 * Call +virConnectNumOfDefinedDomains+[http://www.libvirt.org/html/libvirt-libvirt.html#virConnectNumOfDefinedDomains]
 */
VALUE libvirt_conn_num_of_defined_domains(VALUE s)
{
  gen_conn_num_of(s, DefinedDomains);
}

/*
 * Call +virConnectListDefinedDomains+[http://www.libvirt.org/html/libvirt-libvirt.html#virConnectListDefinedDomains]
 */
VALUE libvirt_conn_list_defined_domains(VALUE s)
{
  gen_conn_list_names(s, DefinedDomains);
}

/*
 * Call +virDomainCreateLinux+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainCreateLinux]
 */
VALUE libvirt_conn_create_linux(int argc, VALUE *argv, VALUE c)
{
  virDomainPtr dom;
  virConnectPtr conn = connect_get(c);
  char *xmlDesc;
  VALUE flags, xml;

  rb_scan_args(argc, argv, "11", &xml, &flags);

  if (NIL_P(flags))
    flags = INT2FIX(0);

  xmlDesc = StringValueCStr(xml);

  dom = virDomainCreateLinux(conn, xmlDesc, NUM2UINT(flags));
  _E(dom == NULL, create_error(e_Error, "virDomainCreateLinux", "", conn));

  return domain_new(dom, c);
}

/*
 * Call +virDomainLookupByName+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainLookupByName]
 */
VALUE libvirt_conn_lookup_domain_by_name(VALUE c, VALUE name)
{
  virDomainPtr dom;
  virConnectPtr conn = connect_get(c);

  dom = virDomainLookupByName(conn, StringValueCStr(name));
  _E(dom == NULL, create_error(e_RetrieveError, "virDomainLookupByName", "", conn));

  return domain_new(dom, c);
}

/*
 * Call +virDomainLookupByID+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainLookupByID]
 */
VALUE libvirt_conn_lookup_domain_by_id(VALUE c, VALUE id)
{
  virDomainPtr dom;
  virConnectPtr conn = connect_get(c);

  dom = virDomainLookupByID(conn, NUM2INT(id));
  _E(dom == NULL, create_error(e_RetrieveError, "virDomainLookupByID", "", conn));

  return domain_new(dom, c);
}

/*
 * Call +virDomainLookupByUUIDString+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainLookupByUUIDString]
 */
VALUE libvirt_conn_lookup_domain_by_uuid(VALUE c, VALUE uuid)
{
  virDomainPtr dom;
  virConnectPtr conn = connect_get(c);

  dom = virDomainLookupByUUIDString(conn, StringValueCStr(uuid));
  _E(dom == NULL, create_error(e_RetrieveError, "virDomainLookupByUUID", "", conn));

  return domain_new(dom, c);
}

/*
 * Call +virDomainDefineXML+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainDefineXML]
 */
VALUE libvirt_conn_define_domain_xml(VALUE c, VALUE xml)
{
  virDomainPtr dom;
  virConnectPtr conn = connect_get(c);

  dom = virDomainDefineXML(conn, StringValueCStr(xml));
  _E(dom == NULL, create_error(e_DefinitionError, "virDomainDefineXML", "", conn));

  return domain_new(dom, c);
}

///////////////////////////////////////////////

static VALUE connect_new(virConnectPtr p)
{
  return Data_Wrap_Struct(c_connect, NULL, connect_close, p);
}

static void connect_close(void *p)
{
  int r;

  if (!p)
  return;

  r = virConnectClose((virConnectPtr) p);
  _E(r < 0, create_error(rb_eSystemCallError, "connect_close", "Connection close failed", p));
}

static virConnectPtr connect_get(VALUE s)
{
  generic_get(Connect, s);
}

static VALUE conn_attr(VALUE s)
{
  if (rb_obj_is_instance_of(s, c_connect) != Qtrue)
  {
  s = rb_iv_get(s, "@connection");
  }

  if (rb_obj_is_instance_of(s, c_connect) != Qtrue)
  {
  rb_raise(rb_eArgError, "Expected Connection object");
  }

  return s;
}

static virConnectPtr conn(VALUE s)
{
  s = conn_attr(s);
  virConnectPtr conn;

  Data_Get_Struct(s, virConnect, conn);
  if (!conn)
  rb_raise(rb_eArgError, "Connection has been closed");

  return conn;
}

void init_connect()
{
	rb_define_module_function(m_libvirt, "open", libvirt_open, 1);
	rb_define_module_function(m_libvirt, "open_read_only", libvirt_open_read_only, 1);
  /*
   * Class Libvirt::Connect
   */
  c_connect = rb_define_class_under(m_libvirt, "Connect", rb_cObject);

  rb_define_method(c_connect, "close", libvirt_conn_close, 0);
  rb_define_method(c_connect, "closed?", libvirt_conn_closed_p, 0);
  rb_define_method(c_connect, "type", libvirt_conn_type, 0);
  rb_define_method(c_connect, "version", libvirt_conn_version, 0);
  rb_define_method(c_connect, "hostname", libvirt_conn_hostname, 0);
  rb_define_method(c_connect, "uri", libvirt_conn_uri, 0);
  rb_define_method(c_connect, "max_vcpus", libvirt_conn_max_vcpus, 1);
  rb_define_method(c_connect, "node_get_info", libvirt_conn_node_get_info, 0);
  rb_define_method(c_connect, "capabilities", libvirt_conn_capabilities, 0);
  rb_define_method(c_connect, "num_of_domains", libvirt_conn_num_of_domains, 0);
  rb_define_method(c_connect, "list_domains", libvirt_conn_list_domains, 0);
  rb_define_method(c_connect, "num_of_defined_domains", libvirt_conn_num_of_defined_domains, 0);
  rb_define_method(c_connect, "list_defined_domains", libvirt_conn_list_defined_domains, 0);


  // Domain creation/lookup
  rb_define_method(c_connect, "create_domain_linux", libvirt_conn_create_linux, -1);
  rb_define_method(c_connect, "lookup_domain_by_name", libvirt_conn_lookup_domain_by_name, 1);
  rb_define_method(c_connect, "lookup_domain_by_id", libvirt_conn_lookup_domain_by_id, 1);
  rb_define_method(c_connect, "lookup_domain_by_uuid", libvirt_conn_lookup_domain_by_uuid, 1);
  rb_define_method(c_connect, "define_domain_xml", libvirt_conn_define_domain_xml, 1);

  /*
   * Class Libvirt::Connect::Nodeinfo
   */
  c_node_info = rb_define_class_under(c_connect, "Nodeinfo", rb_cObject);

  rb_define_attr(c_node_info, "model", 1, 0);
  rb_define_attr(c_node_info, "memory", 1, 0);
  rb_define_attr(c_node_info, "cpus", 1, 0);
  rb_define_attr(c_node_info, "mhz", 1, 0);
  rb_define_attr(c_node_info, "nodes", 1, 0);
  rb_define_attr(c_node_info, "sockets", 1, 0);
  rb_define_attr(c_node_info, "cores", 1, 0);
  rb_define_attr(c_node_info, "threads", 1, 0);
}