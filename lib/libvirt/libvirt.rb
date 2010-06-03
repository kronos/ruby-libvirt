class Libvirt  
  def self.open
    result = FFI::Libvirt.virInitialize
    if result < 0
      raise SystemCallError, "Libvirt initializaion failed"
    end

    true
  end

end