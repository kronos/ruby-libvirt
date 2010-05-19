class Libvirt  
  def self.open
    result = FFI::Libvirt.virInitialize
    if result < 0
      raise SystemCallError, "Libvirt initializaion failed"
    end

    true
  end

  class Error < StandardError
  end

  class ConnectionError < Error
  end

  class DefinitionError < Error
  end

  class ErrRetrieveErroror < Error
  end
end