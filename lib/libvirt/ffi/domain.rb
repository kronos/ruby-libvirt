require 'libvirt/ffi/structs/domain_interface_stats'

module FFI::Domain
  extend FFI::Library
  ffi_lib FFI::Libvirt::library_path

  # int	virDomainDestroy		(virDomainPtr domain)
  attach_function :virDomainDestroy,           [:pointer], :int
  # int	virDomainShutdown(virDomainPtr domain)
  attach_function :virDomainShutdown,          [:pointer], :int
  # int	virDomainReboot(virDomainPtr domain, unsigned int flags)
  attach_function :virDomainReboot,            [:pointer, :uint], :int
  # int	virDomainSuspend(virDomainPtr domain)
  attach_function :virDomainSuspend,           [:pointer], :int
  # int	virDomainResume(virDomainPtr domain)
  attach_function :virDomainResume,            [:pointer], :int
  # int	virDomainManagedSaveRemove(virDomainPtr dom, unsigned int flags)
  attach_function :virDomainManagedSaveRemove, [:pointer, :uint], :int
  # int	virDomainCoreDump(virDomainPtr domain, const char * to, int flags)
  attach_function :virDomainCoreDump,          [:pointer, :string, :int], :int
  # int	virDomainRestore(virConnectPtr conn, const char * from)
  attach_function :virDomainRestore,           [:pointer, :string], :int
  # int	virDomainGetInfo(virDomainPtr domain, virDomainInfoPtr info)
  attach_function :virDomainGetInfo,           [:pointer, :pointer], :int
  # int	virDomainInterfaceStats(virDomainPtr dom, const char * path, virDomainInterfaceStatsPtr stats, size_t size)
  attach_function :virDomainInterfaceStats,    [:pointer, :string, :pointer, :size_t], :int
  # const char *	virDomainGetName(virDomainPtr domain)
  attach_function :virDomainGetName,           [:pointer], :string
  # unsigned int	virDomainGetID(virDomainPtr domain)
  attach_function :virDomainGetID,             [:pointer], :uint
  # int	virDomainGetUUIDString(virDomainPtr domain, char * buf)
  attach_function :virDomainGetUUIDString,     [:pointer, :string], :int
  # char *	virDomainGetOSType(virDomainPtr domain)
  attach_function :virDomainGetOSType,         [:pointer], :string
  # unsigned long	virDomainGetMaxMemory(virDomainPtr domain)
  attach_function :virDomainGetMaxMemory,      [:pointer], :ulong
  # int	virDomainSetMaxMemory(virDomainPtr domain, unsigned long memory)
  attach_function :virDomainSetMaxMemory,      [:pointer, :ulong], :int
  # int	virDomainSetMemory(virDomainPtr domain, unsigned long memory)
  attach_function :virDomainSetMemory,         [:pointer, :ulong], :int
  # int	virDomainGetMaxVcpus(virDomainPtr domain)
  attach_function :virDomainGetMaxVcpus,       [:pointer], :int
  # int	virDomainSetVcpus(virDomainPtr domain, unsigned int nvcpus)
  attach_function :virDomainSetVcpus,          [:pointer, :uint], :int
  # int	virDomainPinVcpu(virDomainPtr domain, unsigned int vcpu, unsigned char * cpumap, int maplen)
  attach_function :virDomainPinVcpu,           [:pointer, :uint, :pointer, :int], :int
  # char *	virDomainGetXMLDesc(virDomainPtr domain, int flags)
  attach_function :virDomainGetXMLDesc,        [:pointer, :int], :string
  # int	virDomainUndefine(virDomainPtr domain)
  attach_function :virDomainUndefine,          [:pointer], :int
  # int	virDomainCreate(virDomainPtr domain)
  attach_function :virDomainCreate,            [:pointer], :int
  # int	virDomainGetAutostart(virDomainPtr domain, int * autostart)
  attach_function :virDomainGetAutostart,      [:pointer, :pointer], :int
  # int	virDomainSetAutostart(virDomainPtr domain, int autostart)
  attach_function :virDomainSetAutostart,      [:pointer, :int], :int
  # int	virDomainFree(virDomainPtr domain)
  attach_function :virDomainFree,              [:pointer], :int
  # int	virDomainAttachDevice(virDomainPtr domain, const char * xml)
  attach_function :virDomainAttachDevice,      [:pointer, :string], :int
  # int	virDomainDetachDevice(virDomainPtr domain, const char * xml)
  attach_function :virDomainDetachDevice,      [:pointer, :string], :int

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
end
