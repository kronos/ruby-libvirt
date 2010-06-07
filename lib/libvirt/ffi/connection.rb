if Libvirt.readable_version >= '0.1.0'
  require 'libvirt/ffi/structs/node_info'
end

module FFI::Libvirt::Connection
  extend FFI::Library
  ffi_lib FFI::Libvirt::library_path

  if Libvirt.readable_version >= '0.0.3'
    # int	virConnectClose(virConnectPtr conn)
    attach_function :virConnectClose,               [:pointer], :int
    # const char *	virConnectGetType(virConnectPtr conn)
    attach_function :virConnectGetType,             [:pointer], :string
    # int	virConnectGetVersion(virConnectPtr conn, unsigned long * hvVer)
    attach_function :virConnectGetVersion,          [:pointer, :pointer], :int
    # int	virConnectListDomains(virConnectPtr conn, int * ids, int maxids)
    attach_function :virConnectListDomains,         [:pointer, :pointer, :int], :int
    # int	virConnectNumOfDomains(virConnectPtr conn)
    attach_function :virConnectNumOfDomains,        [:pointer], :int
    # virConnectPtr	virConnectOpen(const char * name)
    attach_function :virConnectOpen,                [:string], :pointer
    # virConnectPtr	virConnectOpenReadOnly(const char * name)
    attach_function :virConnectOpenReadOnly,        [:string], :pointer

    if Libvirt.readable_version < '0.5.0'
      # virDomainPtr	virDomainCreateLinux(virConnectPtr conn, const char * xmlDesc, unsigned int flags)
      attach_function :virDomainCreateLinux,          [:pointer, :string, :uint], :pointer
    end

    # virDomainPtr	virDomainLookupByID(virConnectPtr conn, int id)
    attach_function :virDomainLookupByID,           [:pointer, :int], :pointer
    # virDomainPtr	virDomainLookupByName(virConnectPtr conn, const char * name)
    attach_function :virDomainLookupByName,         [:pointer, :string], :pointer
    # int	virDomainRestore(virConnectPtr conn, const char * from)
    attach_function :virDomainRestore,              [:pointer, :string], :int
  end

  if Libvirt.readable_version >= '0.1.0'
    # int	virNodeGetInfo(virConnectPtr conn, virNodeInfoPtr info)
    attach_function :virNodeGetInfo,                [:pointer, :pointer], :int
  end

  if Libvirt.readable_version >= '0.1.1'
    # virDomainPtr	virDomainDefineXML(virConnectPtr conn, const char * xml)
    attach_function :virDomainDefineXML,            [:pointer, :string], :pointer
    # int	virConnectListDefinedDomains(virConnectPtr conn, char ** const names, int maxnames)
    attach_function :virConnectListDefinedDomains,  [:pointer, :pointer, :int], :int
    # virDomainPtr	virDomainLookupByUUIDString(virConnectPtr conn, const char * uuidstr)
    attach_function :virDomainLookupByUUIDString,   [:pointer, :string], :pointer
  end

  if Libvirt.readable_version >= '0.1.5'
    # int	virConnectNumOfDefinedDomains(virConnectPtr conn)
    attach_function :virConnectNumOfDefinedDomains, [:pointer], :int
  end

  if Libvirt.readable_version >= '0.2.1'
    # char *	virConnectGetCapabilities(virConnectPtr conn)
    attach_function :virConnectGetCapabilities,     [:pointer], :string
    # int	virConnectGetMaxVcpus(virConnectPtr conn, const char * type)
    attach_function :virConnectGetMaxVcpus,         [:pointer, :string], :int
  end

  if Libvirt.readable_version >= '0.3.0'
    # char *	virConnectGetHostname(virConnectPtr conn)
    attach_function :virConnectGetHostname,         [:pointer], :string
    # char *	virConnectGetURI(virConnectPtr conn)
    attach_function :virConnectGetURI,              [:pointer], :string
  end

  if Libvirt.readable_version >= '0.3.2'
    # virDomainPtr	virDomainMigrate	(virDomainPtr domain, virConnectPtr dconn, unsigned long flags, const char * dname, const char * uri, unsigned long bandwidth)
    attach_function :virDomainMigrate,              [:pointer, :pointer, :ulong, :string, :string, :ulong], :pointer
  end

  if Libvirt.readable_version >= '0.5.0'
    # virDomainPtr	virDomainCreateXML(virConnectPtr conn, const char * xmlDesc, unsigned int flags)
    attach_function :virDomainCreateXML,          [:pointer, :string, :uint], :pointer
  end
end
