# struct virDomainInterfaceStatsStruct{
#   long long rx_bytes
#   long long rx_packets
#   long long rx_errs
#   long long rx_drop
#   long long tx_bytes
#   long long tx_packets
#   long long tx_errs
#   long long tx_drop
# }

class FFI::Libvirt::DomainInterfaceStats < FFI::Struct
  layout :rx_bytes,   :long_long,
         :rx_packets, :long_long,
         :rx_errs,    :long_long,
         :rx_drop,    :long_long,
         :tx_bytes,   :long_long,
         :tx_packets, :long_long,
         :tx_errs,    :long_long,
         :tx_drop,    :long_long
end
