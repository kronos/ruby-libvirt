# struct virNodeInfo{
#   char[32] model  : string indicating the CPU model
#   unsigned long memory   : memory size in kilobytes
#   unsigned int  cpus   : the number of active CPUs
#   unsigned int  mhz  : expected CPU frequency
#   unsigned int  nodes  : the number of NUMA cell, 1 for uniform mem access
#   unsigned int  sockets  : number of CPU socket per node
#   unsigned int  cores  : number of core per socket
#   unsigned int  threads  : number of threads per core
# }

class FFI::Libvirt::NodeInfo < FFI::Struct
  layout :model,   [:char, 32],
         :memory,  :ulong,
         :cpus,    :uint,
         :mhz,     :uint,
         :nodes,   :uint,
         :sockets, :uint,
         :cores,   :uint,
         :threads, :uint
end
