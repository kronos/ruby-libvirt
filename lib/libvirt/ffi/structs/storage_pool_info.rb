# struct virStoragePoolInfo{
#   int state  : virStoragePoolState flags
#   unsigned long long  capacity   : Logical size bytes
#   unsigned long long  allocation   : Current allocation bytes
#   unsigned long long  available  : Remaining free space bytes
# }
class FFI::Libvirt::StoragePoolInfo < FFI::Struct
  layout :state,      :int,
         :capacity,   :ulong_long,
         :allocation, :ulong_long,
         :available,  :ulong_long
end
