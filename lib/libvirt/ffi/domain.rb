module FFI::Libvirt::Domain
  extend FFI::Library
  ffi_lib FFI::Libvirt::library_path

  if Libvirt.readable_version >= '0.0.3'
    # int	virDomainDestroy		(virDomainPtr domain)
    attach_function :virDomainDestroy,           [:pointer], :int
    # int	virDomainFree(virDomainPtr domain)
    attach_function :virDomainFree,              [:pointer], :int
    # unsigned int	virDomainGetID(virDomainPtr domain)
    attach_function :virDomainGetID,             [:pointer], :uint
    # int	virDomainGetInfo(virDomainPtr domain, virDomainInfoPtr info)
    attach_function :virDomainGetInfo,           [:pointer, :pointer], :int
    # unsigned long	virDomainGetMaxMemory(virDomainPtr domain)
    attach_function :virDomainGetMaxMemory,      [:pointer], :ulong
    # char *	virDomainGetOSType(virDomainPtr domain)
    attach_function :virDomainGetOSType,         [:pointer], :string
    # char *	virDomainGetXMLDesc(virDomainPtr domain, int flags)
    attach_function :virDomainGetXMLDesc,        [:pointer, :int], :string
    # int	virDomainResume(virDomainPtr domain)
    attach_function :virDomainResume,            [:pointer], :int
    # int	virDomainSave(virDomainPtr domain, const char * to)
    attach_function :virDomainSave,              [:pointer, :string], :int
    # int	virDomainSetMaxMemory(virDomainPtr domain, unsigned long memory)
    attach_function :virDomainSetMaxMemory,      [:pointer, :ulong], :int
    # int	virDomainShutdown(virDomainPtr domain)
    attach_function :virDomainShutdown,          [:pointer], :int
    # const char *	virDomainGetName(virDomainPtr domain)
    attach_function :virDomainGetName,           [:pointer], :string
    # int	virDomainSuspend(virDomainPtr domain)
    attach_function :virDomainSuspend,           [:pointer], :int
  end

  if Libvirt.readable_version >= '0.1.0'
    # int	virDomainReboot(virDomainPtr domain, unsigned int flags)
    attach_function :virDomainReboot,            [:pointer, :uint], :int
  end

  if Libvirt.readable_version >= '0.1.1'
    # int	virDomainCreate(virDomainPtr domain)
    attach_function :virDomainCreate,            [:pointer], :int
    # int	virDomainGetUUIDString(virDomainPtr domain, char * buf)
    attach_function :virDomainGetUUIDString,     [:pointer, :string], :int
    # int	virDomainSetMemory(virDomainPtr domain, unsigned long memory)
    attach_function :virDomainSetMemory,         [:pointer, :ulong], :int
    # int	virDomainUndefine(virDomainPtr domain)
    attach_function :virDomainUndefine,          [:pointer], :int
  end

  if Libvirt.readable_version >= '0.1.4'
    # TODO: virDomainGetVcpus
    # int virDomainGetVcpus (virDomainPtr domain, virVcpuInfoPtr info, int maxinfo,
    #     unsigned char * cpumaps, int maplen)
    attach_function :virDomainGetVcpus, [:pointer, :pointer, :int, :pointer, :int], :int

    # int	virDomainSetVcpus(virDomainPtr domain, unsigned int nvcpus)
    attach_function :virDomainSetVcpus,          [:pointer, :uint], :int
    # int	virDomainPinVcpu(virDomainPtr domain, unsigned int vcpu, unsigned char * cpumap, int maplen)
    attach_function :virDomainPinVcpu,           [:pointer, :uint, :pointer, :int], :int
  end

  if Libvirt.readable_version >= '0.1.9'
    # int	virDomainAttachDevice(virDomainPtr domain, const char * xml)
    attach_function :virDomainAttachDevice,      [:pointer, :string], :int
    # int	virDomainCoreDump(virDomainPtr domain, const char * to, int flags)
    attach_function :virDomainCoreDump,          [:pointer, :string, :int], :int
    # int	virDomainDetachDevice(virDomainPtr domain, const char * xml)
    attach_function :virDomainDetachDevice,      [:pointer, :string], :int
  end

  if Libvirt.readable_version >= '0.2.1'
    # int	virDomainGetAutostart(virDomainPtr domain, int * autostart)
    attach_function :virDomainGetAutostart,      [:pointer, :pointer], :int
    # int	virDomainGetMaxVcpus(virDomainPtr domain)
    attach_function :virDomainGetMaxVcpus,       [:pointer], :int
    # int	virDomainSetAutostart(virDomainPtr domain, int autostart)
    attach_function :virDomainSetAutostart,      [:pointer, :int], :int
  end

  if Libvirt.readable_version >= '0.3.2'
    # int	virDomainInterfaceStats(virDomainPtr dom, const char * path, virDomainInterfaceStatsPtr stats, size_t size)
    attach_function :virDomainInterfaceStats,    [:pointer, :string, :pointer, :size_t], :int
  end

  if Libvirt.readable_version >= '0.8.0'
    # int	virDomainManagedSaveRemove(virDomainPtr dom, unsigned int flags)
    attach_function :virDomainManagedSaveRemove, [:pointer, :uint], :int
  end

  # enum virDomainState {
  #   VIR_DOMAIN_NOSTATE   =  0  : no state
  #   VIR_DOMAIN_RUNNING   =  1  : the domain is running
  #   VIR_DOMAIN_BLOCKED   =  2  : the domain is blocked on resource
  #   VIR_DOMAIN_PAUSED    =  3  : the domain is paused by user
  #   VIR_DOMAIN_SHUTDOWN  =  4  : the domain is being shut down
  #   VIR_DOMAIN_SHUTOFF   =  5  : the domain is shut off
  #   VIR_DOMAIN_CRASHED   =  6  : the domain is crashed
  # }
  enum :virDomainState, [:VIR_DOMAIN_NOSTATE, 0,
                         :VIR_DOMAIN_RUNNING,
                         :VIR_DOMAIN_BLOCKED,
                         :VIR_DOMAIN_PAUSED,
                         :VIR_DOMAIN_SHUTDOWN,
                         :VIR_DOMAIN_SHUTOFF,
                         :VIR_DOMAIN_CRASHED]

  # enum virDomainMigrateFlags {
  #   VIR_MIGRATE_LIVE  =  1  : live migration
  #   VIR_MIGRATE_PEER2PEER   =  2  : direct source -> dest host control channel Note the less-common spelling that we're stuck with: VIR_MIGRATE_TUNNELLED should be VIR_MIGRATE_TUNNELED
  #   VIR_MIGRATE_TUNNELLED   =  4  : tunnel migration data over libvirtd connection
  #   VIR_MIGRATE_PERSIST_DEST  =  8  : persist the VM on the destination
  #   VIR_MIGRATE_UNDEFINE_SOURCE   =  16   : undefine the VM on the source
  #   VIR_MIGRATE_PAUSED  =  32   : pause on remote side
  #   VIR_MIGRATE_NON_SHARED_DISK   =  64   : migration with non-shared storage with full disk copy
  #   VIR_MIGRATE_NON_SHARED_INC  =  128  : migration with non-shared storage with incremental copy (same base image shared between source and destination)
  # }
  enum :virDomainState, [:VIR_MIGRATE_LIVE,             1,
                         :VIR_MIGRATE_PEER2PEER,        2,
                         :VIR_MIGRATE_TUNNELLED,        4,
                         :VIR_MIGRATE_PERSIST_DEST,     8,
                         :VIR_MIGRATE_UNDEFINE_SOURCE, 16,
                         :VIR_MIGRATE_PAUSED,          32,
                         :VIR_MIGRATE_NON_SHARED_DISK, 64,
                         :VIR_MIGRATE_NON_SHARED_INC, 128]

  # enum virDomainXMLFlags {
  #   VIR_DOMAIN_XML_SECURE  =  1  : dump security sensitive information too
  #   VIR_DOMAIN_XML_INACTIVE  =  2  : dump inactive domain information
  #   VIR_DOMAIN_XML_UPDATE_CPU  =  4  : update guest CPU requirements according to host CPU
  # }
  enum :virDomainXMLFlags, [:VIR_DOMAIN_XML_SECURE,     1,
                            :VIR_DOMAIN_XML_INACTIVE,   2,
                            :VIR_DOMAIN_XML_UPDATE_CPU, 4]

  # TODO: find right place to put the enum in
  # enum virDomainDeviceModifyFlags {
  #   VIR_DOMAIN_DEVICE_MODIFY_CURRENT   =  0  : Modify device allocation based on current domain state
  #   VIR_DOMAIN_DEVICE_MODIFY_LIVE  =  1  : Modify live device allocation
  #   VIR_DOMAIN_DEVICE_MODIFY_CONFIG  =  2  : Modify persisted device allocation
  # }
  enum :virDomainDeviceModifyFlags, [:VIR_DOMAIN_DEVICE_MODIFY_CURRENT, 0,
                                     :VIR_DOMAIN_DEVICE_MODIFY_LIVE,
                                     :VIR_DOMAIN_DEVICE_MODIFY_CONFIG]
end
