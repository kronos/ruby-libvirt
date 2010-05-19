require 'structs/storage_pool_info'

module FFI::StoragePool
  extend FFI::Library
  ffi_lib FFI::Libvirt::library_path

  # virStoragePoolPtr	virStoragePoolLookupByName(virConnectPtr conn, const char * name
  attach_function :virStoragePoolLookupByName, [:pointer, :string], :pointer
  # virStoragePoolPtr	virStoragePoolLookupByUUIDString(virConnectPtr conn, const char * uuidstr)
  attach_function :virStoragePoolLookupByUUIDString, [:pointer, :string], :pointer
  # virStoragePoolPtr	virStoragePoolCreateXML(virConnectPtr conn, const char * xmlDesc, unsigned int flags)
  attach_function :virStoragePoolCreateXML, [:pointer, :string, :uint], :pointer
  # virStoragePoolPtr	virStoragePoolDefineXML(virConnectPtr conn, const char * xml, unsigned int flags)
  attach_function :virStoragePoolDefineXML, [:pointer, :string, :uint], :pointer
  # char *	virConnectFindStoragePoolSources(virConnectPtr conn, const char * type, const char * srcSpec, 
  #                                            unsigned int flags)
  attach_function :virConnectFindStoragePoolSources, [:pointer, :string, :string, :uint], :string
  # int	virStoragePoolBuild(virStoragePoolPtr pool, unsigned int flags)
  attach_function :virStoragePoolBuild, [:pointer, :uint], :int
  # int	virStoragePoolUndefine(virStoragePoolPtr pool)
  attach_function :virStoragePoolUndefine, [:pointer], :int
  # int	virStoragePoolCreate(virStoragePoolPtr pool, unsigned int flags)
  attach_function :virStoragePoolCreate, [:pointer, :uint], :int
  # int	virStoragePoolDestroy(virStoragePoolPtr pool)
  attach_function :virStoragePoolDestroy, [:pointer], :int
  # int	virStoragePoolDelete(virStoragePoolPtr pool, unsigned int flags)
  attach_function :virStoragePoolDelete, [:pointer, :uint], :int
  # int	virStoragePoolRefresh(virStoragePoolPtr pool, unsigned int flags)
  attach_function :virStoragePoolRefresh, [:pointer, :uint], :int
  # const char *	virStoragePoolGetName(virStoragePoolPtr pool)
  attach_function :virStoragePoolGetName, [:pointer], :string
  # int	virStoragePoolGetUUIDString(virStoragePoolPtr pool, char * buf)
  attach_function :virStoragePoolGetUUIDString, [:pointer, :string], :int
  # int	virStoragePoolGetInfo(virStoragePoolPtr pool, virStoragePoolInfoPtr info)
  attach_function :virStoragePoolGetInfo, [:pointer, :pointer], :int
  # char *	virStoragePoolGetXMLDesc(virStoragePoolPtr pool, unsigned int flags)
  attach_function :virStoragePoolGetXMLDesc, [:pointer, :uint], :string
  # int	virStoragePoolGetAutostart(virStoragePoolPtr pool, int * autostart)
  attach_function :virStoragePoolGetAutostart, [:pointer, :pointer], :int
  # int	virStoragePoolSetAutostart(virStoragePoolPtr pool, int autostart)
  attach_function :virStoragePoolSetAutostart, [:pointer, :int], :int
  # int	virStoragePoolNumOfVolumes(virStoragePoolPtr pool)
  attach_function :virStoragePoolNumOfVolumes, [:pointer], :int
  # int	virStoragePoolListVolumes(virStoragePoolPtr pool, char ** const names, int maxnames)
  attach_function :virStoragePoolListVolumes, [:pointer, :pointer, :int], :int
  # int	virStoragePoolFree(virStoragePoolPtr pool)
  attach_function :virStoragePoolFree, [:pointer], :int
  # int	virConnectListStoragePools	(virConnectPtr conn, char ** const names, int maxnames)
  attach_function :virConnectListStoragePools, [:pointer, :pointer, :int], :int
  # int	virConnectNumOfStoragePools	(virConnectPtr conn)
  attach_function :virConnectNumOfStoragePools, [:pointer], :int
  # int	virConnectListDefinedStoragePools	(virConnectPtr conn, char ** const names, int maxnames)
  attach_function :virConnectListDefinedStoragePools, [:pointer, :pointer, :int], :int
  # int	virConnectNumOfDefinedStoragePools	(virConnectPtr conn)
  attach_function :virConnectNumOfDefinedStoragePools, [:pointer], :int
  
  # enum virStoragePoolState {
  #   VIR_STORAGE_POOL_INACTIVE  =  0  : Not running
  #   VIR_STORAGE_POOL_BUILDING  =  1  : Initializing pool, not available
  #   VIR_STORAGE_POOL_RUNNING   =  2  : Running normally
  #   VIR_STORAGE_POOL_DEGRADED  =  3  : Running degraded
  # }
  enum :virStoragePoolState, [:VIR_STORAGE_POOL_INACTIVE, 0,
                              :VIR_STORAGE_POOL_BUILDING,
                              :VIR_STORAGE_POOL_RUNNING,
                              :VIR_STORAGE_POOL_DEGRADED]

  # enum virStoragePoolBuildFlags {
  #   VIR_STORAGE_POOL_BUILD_NEW   =  0  : Regular build from scratch
  #   VIR_STORAGE_POOL_BUILD_REPAIR  =  1  : Repair / reinitialize
  #   VIR_STORAGE_POOL_BUILD_RESIZE  =  2  : Extend existing pool
  # }
  enum :virStoragePoolBuildFlags, [:VIR_STORAGE_POOL_BUILD_NEW, 0,
                                   :VIR_STORAGE_POOL_BUILD_REPAIR,
                                   :VIR_STORAGE_POOL_BUILD_RESIZE]


  # enum virStoragePoolDeleteFlags {
  #   VIR_STORAGE_POOL_DELETE_NORMAL   =  0  : Delete metadata only (fast)
  #   VIR_STORAGE_POOL_DELETE_ZEROED   =  1  : Clear all data to zeros (slow)
  # }
  enum :virStoragePoolBuildFlags, [:VIR_STORAGE_POOL_DELETE_NORMAL, 0,
                                   :VIR_STORAGE_POOL_DELETE_ZEROED]
end
