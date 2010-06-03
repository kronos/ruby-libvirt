require 'libvirt/ffi/libvirt'
require 'libvirt/ffi/structs/stroage_vol_info'

module FFI::Libvirt::StorageVol
  extend FFI::Library
  ffi_lib FFI::Libvirt::library_path

  # virStorageVolPtr	virStorageVolLookupByName(virStoragePoolPtr pool, const char * name)
  attach_function :virStorageVolLookupByName, [:pointer, :string], :pointer
  # virStorageVolPtr	virStorageVolLookupByKey(virConnectPtr conn, const char * key)
  attach_function :virStorageVolLookupByKey, [:pointer, :string], :pointer
  # virStorageVolPtr	virStorageVolLookupByPath(virConnectPtr conn, const char * path)
  attach_function :virStorageVolLookupByPath, [:pointer, :string], :pointer
  # virStoragePoolPtr	virStoragePoolLookupByVolume(virStorageVolPtr vol)
  attach_function :virStoragePoolLookupByVolume, [:pointer], :pointer
  # const char *	virStorageVolGetName(virStorageVolPtr vol)
  attach_function :virStorageVolGetName, [:pointer], :string
  # const char *	virStorageVolGetKey(virStorageVolPtr vol)
  attach_function :virStorageVolGetName, [:pointer], :string
  # virStorageVolPtr	virStorageVolCreateXML(virStoragePoolPtr pool, const char * xmldesc, unsigned int flags)
  attach_function :virStorageVolCreateXML, [:pointer, :string, :uint], :pointer
  # int	virStorageVolDelete(virStorageVolPtr vol, unsigned int flags)
  attach_function :virStorageVolDelete, [:pointer, :uint], :int
  # int	virStorageVolGetInfo(virStorageVolPtr vol, virStorageVolInfoPtr info)
  attach_function :virStorageVolGetInfo, [:pointer, :pointer], :int
  # char *	virStorageVolGetXMLDesc(virStorageVolPtr vol, unsigned int flags
  attach_function :virStorageVolGetXMLDesc, [:pointer, :uint], :string
  # char *	virStorageVolGetPath		(virStorageVolPtr vol)
  attach_function :virStorageVolGetPath, [:pointer], :string
  # int	virStorageVolFree		(virStorageVolPtr vol)
  attach_function :virStorageVolFree, [:pointer], :int
  
  # enum virStorageVolType {
  #   VIR_STORAGE_VOL_FILE   =  0  : Regular file based volumes
  #   VIR_STORAGE_VOL_BLOCK  =  1  : Block based volumes
  # }
  enum :virStorageVolType, [:VIR_STORAGE_VOL_FILE, 0,
                            :VIR_STORAGE_VOL_BLOCK]
  
  # enum virStorageVolDeleteFlags {
  #   VIR_STORAGE_VOL_DELETE_NORMAL  =  0  : Delete metadata only (fast)
  #   VIR_STORAGE_VOL_DELETE_ZEROED  =  1  : Clear all data to zeros (slow)
  # }
  enum :virStorageVolDeleteFlags, [:VIR_STORAGE_VOL_DELETE_NORMAL, 0,
                                   :VIR_STORAGE_VOL_DELETE_ZEROED]
end
