require 'libvirt/ffi/libvirt'
require 'libvirt/ffi/structs/node_info'

module FFI::Connection
  extend FFI::Library
  ffi_lib FFI::Libvirt::library_path

  # virConnectPtr	virConnectOpen(const char * name)
  attach_function :virConnectOpen,                [:string], :pointer
  # int	virConnectClose(virConnectPtr conn)
  attach_function :virConnectClose,               [:pointer], :int
  # virConnectPtr	virConnectOpenReadOnly(const char * name)
  attach_function :virConnectOpenReadOnly,        [:string], :pointer
  # const char *	virConnectGetType(virConnectPtr conn)
  attach_function :virConnectGetType,             [:pointer], :string
  # int	virConnectGetVersion(virConnectPtr conn, unsigned long * hvVer)
  attach_function :virConnectGetVersion,          [:pointer, :pointer], :int
  # char *	virConnectGetHostname(virConnectPtr conn)
  attach_function :virConnectGetHostname,         [:pointer], :string
  # char *	virConnectGetURI(virConnectPtr conn)
  attach_function :virConnectGetURI,              [:pointer], :string
  # int	virConnectGetMaxVcpus(virConnectPtr conn, const char * type)
  attach_function :virConnectGetMaxVcpus,         [:pointer, :string], :int
  # int	virNodeGetInfo(virConnectPtr conn, virNodeInfoPtr info)
  attach_function :virNodeGetInfo,                [:pointer, :pointer], :int
  # char *	virConnectGetCapabilities(virConnectPtr conn)
  attach_function :virConnectGetCapabilities,     [:pointer], :string
  # int	virConnectNumOfDomains(virConnectPtr conn)
  attach_function :virConnectNumOfDomains,        [:pointer], :int
  # int	virConnectListDomains(virConnectPtr conn, int * ids, int maxids)
  attach_function :virConnectListDomains,         [:pointer, :pointer, :int], :int
  # int	virConnectNumOfDefinedDomains(virConnectPtr conn)
  attach_function :virConnectNumOfDefinedDomains, [:pointer], :int
  # int	virConnectListDefinedDomains(virConnectPtr conn, char ** const names, int maxnames)
  attach_function :virConnectListDefinedDomains,  [:pointer, :pointer, :int], :int
  # virDomainPtr	virDomainCreateLinux(virConnectPtr conn, const char * xmlDesc, unsigned int flags)
  attach_function :virDomainCreateLinux,          [:pointer, :string, :uint], :int
  # virDomainPtr	virDomainLookupByName(virConnectPtr conn, const char * name)
  attach_function :virDomainLookupByName,         [:pointer, :string], :pointer
  # virDomainPtr	virDomainLookupByID(virConnectPtr conn, int id)
  attach_function :virDomainLookupByID,           [:pointer, :int], :pointer
  # virDomainPtr	virDomainLookupByUUIDString(virConnectPtr conn, const char * uuidstr)
  attach_function :virDomainLookupByUUIDString,   [:pointer, :string], :pointer
  # virDomainPtr	virDomainDefineXML(virConnectPtr conn, const char * xml)
  attach_function :virDomainDefineXML,            [:pointer, :string], :pointer
end