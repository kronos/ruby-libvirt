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

VALUE c_domain;
VALUE c_domain_info;
VALUE c_domain_ifinfo;

VALUE libvirt_dom_migrate(int argc, VALUE *argv, VALUE s)
{
  VALUE dconn, flags, dname_val, uri_val, bandwidth;
  virDomainPtr ddom = NULL;
  const char *dname;
  const char *uri;

  rb_scan_args(argc, argv, "23", &dconn, &flags, &dname_val, &uri_val, &bandwidth);

  dname = get_string_or_nil(dname_val);
  uri = get_string_or_nil(uri_val);

  if (NIL_P(bandwidth))
    bandwidth = INT2FIX(0);

  ddom = virDomainMigrate(domain_get(s), conn(dconn), NUM2UINT(flags), dname, uri, NUM2UINT(bandwidth));

  _E(ddom == NULL, create_error(e_Error, "virDomainMigrate", "", conn(dconn)));

  return domain_new(ddom, dconn);
}

/*
 * Call +virDomainShutdown+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainShutdown]
 */
VALUE libvirt_dom_shutdown(VALUE s)
{
  gen_call_void(virDomainShutdown, conn(s), domain_get(s));
}

/*
 * Call +virDomainReboot+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainReboot]
 */
VALUE libvirt_dom_reboot(int argc, VALUE *argv, VALUE s)
{
  VALUE flags;

  rb_scan_args(argc, argv, "01", &flags);

  if (NIL_P(flags))
    flags = INT2FIX(0);

  gen_call_void(virDomainReboot, conn(s), domain_get(s), NUM2UINT(flags));
}

/*
 * Call +virDomainDestroy+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainDestroy]
 */
VALUE libvirt_dom_destroy(VALUE s)
{
  gen_call_void(virDomainDestroy, conn(s),
          domain_get(s));
}

/*
 * Call +virDomainSuspend+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainSuspend]
 */
VALUE libvirt_dom_suspend(VALUE s)
{
  gen_call_void(virDomainSuspend, conn(s),
          domain_get(s));
}

/*
 * Call +virDomainResume+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainResume]
 */
VALUE libvirt_dom_resume(VALUE s)
{
  gen_call_void(virDomainResume, conn(s),
          domain_get(s));
}

/*
 * Call +virDomainSave+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainSave]
 */
VALUE libvirt_dom_save(VALUE s, VALUE to)
{
  gen_call_void(virDomainSave, conn(s),
          domain_get(s), StringValueCStr(to));
}

/*
 * Call +virDomainCoreDump+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainCoreDump]
 */
VALUE libvirt_dom_core_dump(int argc, VALUE *argv, VALUE s)
{
  VALUE to, flags;

  rb_scan_args(argc, argv, "11", &to, &flags);

  if (NIL_P(flags))
    flags = INT2FIX(0);

  gen_call_void(virDomainCoreDump, conn(s),
          domain_get(s), StringValueCStr(to), NUM2UINT(flags));
}

/*
 * Call +virDomainRestore+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainRestore]
 */
VALUE libvirt_dom_s_restore(VALUE klass, VALUE c, VALUE from)
{
  gen_call_void(virDomainRestore, connect_get(c),
          connect_get(c), StringValueCStr(from));
}

/*
 * call-seq:
 *   domain.info -> Libvirt::Domain::Info
 *
 * Call +virDomainGetInfo+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainGetInfo]
 */
VALUE libvirt_dom_info(VALUE s)
{
  virDomainPtr dom = domain_get(s);
  virDomainInfo info;
  int r;
  VALUE result;

  r = virDomainGetInfo(dom, &info);
  _E(r < 0, create_error(e_RetrieveError, "virDomainGetInfo", "", conn(s)));

  result = rb_class_new_instance(0, NULL, c_domain_info);
  rb_iv_set(result, "@state", CHR2FIX(info.state));
  rb_iv_set(result, "@max_mem", ULONG2NUM(info.maxMem));
  rb_iv_set(result, "@memory", ULONG2NUM(info.memory));
  rb_iv_set(result, "@nr_virt_cpu", INT2FIX((int) info.nrVirtCpu));
  rb_iv_set(result, "@cpu_time", ULL2NUM(info.cpuTime));
  return result;
}

/*
 * call-seq:
 *   domain.info -> Libvirt::Domain::IfInfo
 *
 * Call +virDomainInterfaceStats+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainGetInfo]
 */
VALUE libvirt_dom_if_stats(VALUE s, VALUE sif)
{
  virDomainPtr dom = domain_get(s);
  char *ifname = get_string_or_nil(sif);
  virDomainInterfaceStatsStruct ifinfo;
  int r;
  VALUE result;

  if(ifname)
  {
    r = virDomainInterfaceStats(dom, ifname, &ifinfo, sizeof(virDomainInterfaceStatsStruct));
    _E(r < 0, create_error(e_RetrieveError, "virDomainInterfaceStats", "", conn(s)));

    result = rb_class_new_instance(0, NULL, c_domain_ifinfo);
    rb_iv_set(result, "@rx_bytes", LL2NUM(ifinfo.rx_bytes));
    rb_iv_set(result, "@rx_packets", LL2NUM(ifinfo.rx_packets));
    rb_iv_set(result, "@rx_errs", LL2NUM(ifinfo.rx_errs));
    rb_iv_set(result, "@rx_drop", LL2NUM(ifinfo.rx_drop));
    rb_iv_set(result, "@tx_bytes", LL2NUM(ifinfo.tx_bytes));
    rb_iv_set(result, "@tx_packets", LL2NUM(ifinfo.tx_packets));
    rb_iv_set(result, "@tx_errs", LL2NUM(ifinfo.tx_errs));
    rb_iv_set(result, "@tx_drop", LL2NUM(ifinfo.tx_drop));
  }
  return result;
}

/*
 * Call +virDomainGetName+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainGetName]
 */
VALUE libvirt_dom_name(VALUE s)
{
  gen_call_string(virDomainGetName, conn(s), 0, domain_get(s));
}

/*
 * Call +virDomainGetID+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainGetID]
 */
VALUE libvirt_dom_id(VALUE s)
{
  virDomainPtr dom = domain_get(s);
  unsigned int id;

  id = virDomainGetID(dom);
  _E(id < 0, create_error(e_RetrieveError, "virDomainGetID", "", conn(s)));

  return UINT2NUM(id);
}

/*
 * Call +virDomainGetUUIDString+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainGetUUIDString]
 */
VALUE libvirt_dom_uuid(VALUE s)
{
  virDomainPtr dom = domain_get(s);
  char uuid[VIR_UUID_STRING_BUFLEN];
  int r;

  r = virDomainGetUUIDString(dom, uuid);
  _E(r < 0, create_error(e_RetrieveError, "virDomainGetUUIDString", "", conn(s)));

  return rb_str_new2((char *) uuid);
}

/*
 * Call +virDomainGetOSType+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainGetOSType]
 */
VALUE libvirt_dom_os_type(VALUE s)
{
  gen_call_string(virDomainGetOSType, conn(s), 1, domain_get(s));
}

/*
 * Call +virDomainGetMaxMemory+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainGetMaxMemory]
 */
VALUE libvirt_dom_max_memory(VALUE s)
{
  virDomainPtr dom = domain_get(s);
  unsigned long max_memory;

  max_memory = virDomainGetMaxMemory(dom);
  _E(max_memory == 0, create_error(e_RetrieveError, "virDomainGetMaxMemory", "", conn(s)));

  return ULONG2NUM(max_memory);
}

/*
 * Call +virDomainSetMaxMemory+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainSetMaxMemory]
 */
VALUE libvirt_dom_max_memory_set(VALUE s, VALUE max_memory)
{
  virDomainPtr dom = domain_get(s);
  int r;

  r = virDomainSetMaxMemory(dom, NUM2ULONG(max_memory));
  _E(r < 0, create_error(e_DefinitionError, "virDomainSetMaxMemory", "", conn(s)));

  return ULONG2NUM(max_memory);
}

/*
 * Call +virDomainSetMemory+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainSetMemory]
 */
VALUE libvirt_dom_memory_set(VALUE s, VALUE memory)
{
  virDomainPtr dom = domain_get(s);
  int r;

  r = virDomainSetMemory(dom, NUM2ULONG(memory));
  _E(r < 0, create_error(e_DefinitionError, "virDomainSetMemory", "", conn(s)));

  return ULONG2NUM(memory);
}

/*
 * Call +virDomainGetMaxVcpus+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainGetMaxVcpus]
 */
VALUE libvirt_dom_max_vcpus(VALUE s)
{
  virDomainPtr dom = domain_get(s);
  int vcpus;

  vcpus = virDomainGetMaxVcpus(dom);
  _E(vcpus < 0, create_error(e_RetrieveError, "virDomainGetMaxVcpus", "", conn(s)));

  return INT2NUM(vcpus);
}


/*
 * Call +virDomainSetVcpus+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainSetVcpus]
 */
VALUE libvirt_dom_vcpus_set(VALUE s, VALUE nvcpus)
{
  virDomainPtr dom = domain_get(s);
  int r;

  r = virDomainSetVcpus(dom, NUM2UINT(nvcpus));
  _E(r < 0, create_error(e_DefinitionError, "virDomainSetVcpus", "", conn(s)));

  return r;
}

/*
 * Call +virDomainPinVcpu+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainPinVcpu]
 */
VALUE libvirt_dom_pin_vcpu(VALUE s, VALUE vcpu, VALUE cpulist)
{
  virDomainPtr dom = domain_get(s);
  int r, i, len, maplen;
  unsigned char *cpumap;
  virNodeInfo nodeinfo;
  virConnectPtr c = conn(s);

  r = virNodeGetInfo(c, &nodeinfo);
  _E(r < 0, create_error(e_RetrieveError, "virNodeGetInfo", "", c));

  maplen = VIR_CPU_MAPLEN(nodeinfo.cpus);
  cpumap = ALLOC_N(unsigned char, maplen);
  MEMZERO(cpumap, unsigned char, maplen);

  len = RARRAY_LEN(cpulist);
  for(i = 0; i < len; i++)
  {
    VALUE e = rb_ary_entry(cpulist, i);
    VIR_USE_CPU(cpumap, NUM2UINT(e));
  }

  r = virDomainPinVcpu(dom, NUM2UINT(vcpu), cpumap, maplen);
  free(cpumap);
  _E(r < 0, create_error(e_RetrieveError, "virDomainPinVcpu", "", c));

  return r;
}


/*
 * Call +virDomainGetXMLDesc+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainGetXMLDesc]
 */
VALUE libvirt_dom_xml_desc(int argc, VALUE *argv, VALUE s)
{
  VALUE flags;

  rb_scan_args(argc, argv, "01", &flags);

  if (NIL_P(flags))
    flags = INT2FIX(0);

  gen_call_string(virDomainGetXMLDesc, conn(s), 1, domain_get(s), 0);
}

/*
 * Call +virDomainUndefine+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainUndefine]
 */
VALUE libvirt_dom_undefine(VALUE s)
{
  gen_call_void(virDomainUndefine, conn(s), domain_get(s));
}

/*
 * Call +virDomainCreate+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainCreate]
 */
VALUE libvirt_dom_create(VALUE s)
{
  gen_call_void(virDomainCreate, conn(s), domain_get(s));
}

/*
 * Call +virDomainGetAutostart+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainGetAutostart]
 */
VALUE libvirt_dom_autostart(VALUE s)
{
  virDomainPtr dom = domain_get(s);
  int r, autostart;

  r = virDomainGetAutostart(dom, &autostart);
  _E(r < 0, create_error(e_RetrieveError, "virDomainAutostart", "", conn(s)));

  return autostart ? Qtrue : Qfalse;
}

/*
 * Call +virDomainSetAutostart+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainSetAutostart]
 */
VALUE libvirt_dom_autostart_set(VALUE s, VALUE autostart)
{
  gen_call_void(virDomainSetAutostart, conn(s), domain_get(s), RTEST(autostart) ? 1 : 0);
}

/*
 * Call +virDomainFree+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainFree]
 */
VALUE libvirt_dom_free(VALUE s)
{
  gen_call_free(Domain, s);
}

/*
 * Call +virDomainAttachDevice+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainAttachDevice]
 */
VALUE libvirt_dom_attach_device(VALUE s, VALUE xml)
{
  gen_call_void(virDomainAttachDevice, conn(s), domain_get(s), StringValueCStr(xml));
}

/*
 * Call +virDomainDetachDevice+[http://www.libvirt.org/html/libvirt-libvirt.html#virDomainDetachDevice]
 */
VALUE libvirt_dom_detach_device(VALUE s, VALUE xml)
{
  gen_call_void(virDomainDetachDevice, conn(s), domain_get(s), StringValueCStr(xml));
}


#define DEF_DOMSTATE(name) rb_define_const(c_domain, #name, INT2NUM(VIR_DOMAIN_##name))

void init_domain()
{
  c_domain = rb_define_class_under(m_libvirt, "Domain", rb_cObject);

  /* virDomainState */
  DEF_DOMSTATE(NOSTATE);
  DEF_DOMSTATE(RUNNING);
  DEF_DOMSTATE(BLOCKED);
  DEF_DOMSTATE(PAUSED);
  DEF_DOMSTATE(SHUTDOWN);
  DEF_DOMSTATE(SHUTOFF);
  DEF_DOMSTATE(CRASHED);

  /* virDomainMigrateFlags */
  rb_define_const(c_domain, "MIGRATE_LIVE", INT2NUM(VIR_MIGRATE_LIVE));

  rb_define_method(c_domain, "migrate", libvirt_dom_migrate, -1);
  rb_define_attr(c_domain, "connection", 1, 0);
  rb_define_method(c_domain, "shutdown", libvirt_dom_shutdown, 0);
  rb_define_method(c_domain, "reboot", libvirt_dom_reboot, -1);
  rb_define_method(c_domain, "destroy", libvirt_dom_destroy, 0);
  rb_define_method(c_domain, "suspend", libvirt_dom_suspend, 0);
  rb_define_method(c_domain, "resume", libvirt_dom_resume, 0);
  rb_define_method(c_domain, "save", libvirt_dom_save, 1);
  rb_define_singleton_method(c_domain, "restore", libvirt_dom_s_restore, 2);
  rb_define_method(c_domain, "core_dump", libvirt_dom_core_dump, -1);
  rb_define_method(c_domain, "info", libvirt_dom_info, 0);
  rb_define_method(c_domain, "ifinfo", libvirt_dom_if_stats, 1);
  rb_define_method(c_domain, "name", libvirt_dom_name, 0);
  rb_define_method(c_domain, "id", libvirt_dom_id, 0);
  rb_define_method(c_domain, "uuid", libvirt_dom_uuid, 0);
  rb_define_method(c_domain, "os_type", libvirt_dom_os_type, 0);
  rb_define_method(c_domain, "max_memory", libvirt_dom_max_memory, 0);
  rb_define_method(c_domain, "max_memory=", libvirt_dom_max_memory_set, 1);
  rb_define_method(c_domain, "memory=", libvirt_dom_memory_set, 1);
  rb_define_method(c_domain, "max_vcpus", libvirt_dom_max_vcpus, 0);
  rb_define_method(c_domain, "vcpus=", libvirt_dom_vcpus_set, 1);
  rb_define_method(c_domain, "pin_vcpu", libvirt_dom_pin_vcpu, 2);
  rb_define_method(c_domain, "xml_desc", libvirt_dom_xml_desc, -1);
  rb_define_method(c_domain, "undefine", libvirt_dom_undefine, 0);
  rb_define_method(c_domain, "create", libvirt_dom_create, 0);
  rb_define_method(c_domain, "autostart", libvirt_dom_autostart, 0);
  rb_define_method(c_domain, "autostart=", libvirt_dom_autostart_set, 1);
  rb_define_method(c_domain, "free", libvirt_dom_free, 0);
  rb_define_method(c_domain, "attach_device", libvirt_dom_attach_device, 1);
  rb_define_method(c_domain, "detach_device", libvirt_dom_detach_device, 1);

  /*
   * Class Libvirt::Domain::Info
   */
  c_domain_info = rb_define_class_under(c_domain, "Info", rb_cObject);
  rb_define_attr(c_domain_info, "state", 1, 0);
  rb_define_attr(c_domain_info, "max_mem", 1, 0);
  rb_define_attr(c_domain_info, "memory", 1, 0);
  rb_define_attr(c_domain_info, "nr_virt_cpu", 1, 0);
  rb_define_attr(c_domain_info, "cpu_time", 1, 0);

  /*
   * Class Libvirt::Domain::InterfaceInfo
   */
  c_domain_ifinfo = rb_define_class_under(c_domain, "InterfaceInfo", rb_cObject);
  rb_define_attr(c_domain_ifinfo, "rx_bytes", 1, 0);
  rb_define_attr(c_domain_ifinfo, "rx_packets", 1, 0);
  rb_define_attr(c_domain_ifinfo, "rx_errs", 1, 0);
  rb_define_attr(c_domain_ifinfo, "rx_drop", 1, 0);
  rb_define_attr(c_domain_ifinfo, "tx_bytes", 1, 0);
  rb_define_attr(c_domain_ifinfo, "tx_packets", 1, 0);
  rb_define_attr(c_domain_ifinfo, "tx_errs", 1, 0);
  rb_define_attr(c_domain_ifinfo, "tx_drop", 1, 0);
}

#undef DEF_DOMSTATE