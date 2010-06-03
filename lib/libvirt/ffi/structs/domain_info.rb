# struct virDomainInfo{
#   unsigned char state  : the running state, one of virDomainState
#   unsigned long maxMem   : the maximum memory in KBytes allowed
#   unsigned long memory   : the memory in KBytes used by the domain
#   unsigned short  nrVirtCpu  : the number of virtual CPUs for the domain
#   unsigned long long  cpuTime  : the CPU time used in nanoseconds
# }

class FFI::Libvirt::NodeInfo < FFI::Struct
  layout :state,     :uchar,
         :maxMem,    :ulong,
         :memory,    :ulong,
         :nrVirtCpu, :ushort,
         :cpuTime,   :ulong_long
end
