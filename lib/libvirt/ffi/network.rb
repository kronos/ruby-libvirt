require 'libvirt/ffi/libvirt'

module FFI::Libvirt::Network
  extend FFI::Library
  ffi_lib FFI::Libvirt::library_path

  if Libvirt::readable_version >= '0.2.0'
    # int	virConnectNumOfNetworks(virConnectPtr conn)
    attach_function :virConnectNumOfNetworks, [:pointer], :int
    # int	virConnectListNetworks(virConnectPtr conn, char ** const names, int maxnames)
    attach_function :virConnectListNetworks, [:pointer, :pointer, :int], :int
    # int	virConnectNumOfDefinedNetworks(virConnectPtr conn)
    attach_function :virConnectNumOfDefinedNetworks, [:pointer], :int
    # int	virConnectListDefinedNetworks(virConnectPtr conn, char ** const names, int maxnames)
    attach_function :virConnectListDefinedNetworks, [:pointer, :pointer, :int], :int
    # virNetworkPtr	virNetworkLookupByName(virConnectPtr conn, const char * name)
    attach_function :virNetworkLookupByName, [:pointer, :string], :pointer
    # virNetworkPtr	virNetworkLookupByUUIDString(virConnectPtr conn, const char * uuidstr)
    attach_function :virNetworkLookupByUUIDString, [:pointer, :string], :pointer
    # virNetworkPtr	virNetworkCreateXML(virConnectPtr conn, const char * xmlDesc)
    attach_function :virNetworkCreateXML, [:pointer, :string], :pointer
    # virNetworkPtr	virNetworkDefineXML	(virConnectPtr conn, const char * xml)
    attach_function :virNetworkDefineXML, [:pointer, :string], :pointer
    # int	virNetworkUndefine(virNetworkPtr network)
    attach_function :virNetworkUndefine, [:pointer], :int
    # int	virNetworkCreate(virNetworkPtr network)
    attach_function :virNetworkCreate, [:pointer], :int
    # int	virNetworkDestroy(virNetworkPtr network)
    attach_function :virNetworkDestroy, [:pointer], :int
    # const char *	virNetworkGetName(virNetworkPtr network)
    attach_function :virNetworkGetName, [:pointer], :int
    # int	virNetworkFree(virNetworkPtr network)
    attach_function :virNetworkFree, [:pointer], :int
    # int	virNetworkGetUUIDString(virNetworkPtr network, char * buf)
    attach_function :virNetworkGetUUIDString, [:pointer, :string], :int
    # char *	virNetworkGetXMLDesc(virNetworkPtr network, int flags)
    attach_function :virNetworkGetXMLDesc, [:pointer, :int], :string
    # char *	virNetworkGetBridgeName(virNetworkPtr network)
    attach_function :virNetworkGetBridgeName, [:pointer, :pointer], :int
  end

  if Libvirt::readable_version >= '0.2.1'
    # int	virNetworkGetAutostart		(virNetworkPtr network, int * autostart)
    attach_function :virNetworkGetAutostart, [:pointer, :pointer], :int
    # int	virNetworkSetAutostart(virNetworkPtr network, int autostart)
    attach_function :virNetworkSetAutostart, [:pointer, :int], :int
  end
end
