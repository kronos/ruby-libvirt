# struct virStorageVolInfo{
#   int type   : virStorageVolType flags
#   unsigned long long  capacity   : Logical size bytes
#   unsigned long long  allocation   : Current allocation bytes
# }

class FFI::Libvirt::StorageVolInfo < FFI::Struct
  layout :type,       :int,
         :capacity,   :ulong_long,
         :allocation, :ulong_long
end
