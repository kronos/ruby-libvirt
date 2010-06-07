require 'libvirt/ffi/domain'
require 'libvirt/ffi/structs/domain_info'
require 'libvirt/ffi/structs/domain_interface_stats'

module Libvirt
  class Domain
    def initialize(domain)
      @domain = domain
    end

    def destroy
      result = FFI::Libvirt::Domain.virDomainDestroy(@domain)
      raise Libvirt::Error, "Cannot free domain object" if result < 0
      true
    end

    def shutdown
      result = FFI::Libvirt::Domain.virDomainShutdown(@domain)
      raise Libvirt::Error, "Cannot shutdown domain" if result < 0
      true
    end

    def reboot(flags = 0)
      result = FFI::Libvirt::Domain.virDomainShutdown(@domain, flags)
      raise Libvirt::Error, "Cannot reboot domain" if result < 0
      true
    end

    def suspend
      result = FFI::Libvirt::Domain.virDomainSuspend(@domain)
      raise Libvirt::Error, "Cannot suspend domain" if result < 0
      true
    end

    def resume
      result = FFI::Libvirt::Domain.virDomainResume(@domain)
      raise Libvirt::Error, "Cannot resume domain" if result < 0
      true
    end

    def managed_save_remove(flags = 0)
      result = FFI::Libvirt::Domain.virDomainManagedSaveRemove(@domain, flags)
      raise Libvirt::Error, "Cannot managed save remove domain" if result < 0
      true
    end

    def core_dump(path, flags = 0)
      result = FFI::Libvirt::Domain.virDomainCoreDump(@domain, path, flags)
      raise Libvirt::Error, "Cannot perform core dump domain" if result < 0
      true
    end

    def info
      domain_info_ptr = FFI::MemoryPointer.new(:pointer)
      result = FFI::Libvirt::Domain.virDomainGetInfo(@domain, domain_info_ptr)
      raise Libvirt::Error, "Cannot retrieve domain info" if result < 0
      FFI::Libvirt::NodeInfo.new(domain_info_ptr)
    end

    def interface_stats(path)
      interface_stats_ptr = FFI::MemoryPointer.new(:pointer)
      result = FFI::Libvirt::Domain.virDomainInterfaceStats(@domain, path, interface_stats_ptr, FFI::Libvirt::DomainInterfaceStats.size)
      raise Libvirt::Error, "Cannot retrtive domain's interface stats for path='#{path}'" if result < 0
      FFI::Libvirt::DomainInterfaceStats.new(interface_stats_ptr)
    end

    def name
      name = FFI::Libvirt::Domain.virDomainGetName(@domain)
      raise Libvirt::Error, "Cannot retrieve domain's name" unless name.kind_of? String
      name
    end

    def id
      id = FFI::Libvirt::Domain.virDomainGetID(@domain)      # FIXME: the formula right only for MRI
      raise Libvirt::Error, "Cannot retrieve domain's id" if id >= (1 << (8*0.size - 2))-1
      id
    end

    def uuid
      uuid = MemoryPointer.new(:char, 36)
      result = FFI::Libvirt::Domain.virDomainGetUUIDString(@domain, uuid)
      raise Libvirt::Error, "Cannot retrieve domain's uuid" if result < 0
      uuid.read_string
    end

    def os_type
      os_type = FFI::Libvirt::Domain.virDomainGetOSType(@domain)
      raise Libvirt::Error, "Cannot retrieve domain's uuid" unless os_type.kind_of? String
      os_type
    end

    def max_memory
      memory_in_kilobytes = FFI::Libvirt::Domain.virDomainGetMaxMemory(@domain)
      raise Libvirt::Error, "Cannot retrieve domain's max memory value" if memory_in_kilobytes == 0
      memory_in_kilobytes * 1024
    end

    def max_memory=(memory)
      result = FFI::Libvirt::Domain.virDomainSetMaxMemory(@domain, memory)
      raise Libvirt::Error, "Cannot set domain's max memory size value" if memory < 0
      true
    end

    def memory=(memory_in_bytes)
      memory_in_kilobytes = memory_in_bytes / 1024
      result = FFI::Libvirt::Domain.virDomainSetMemory(@domain, memory_in_kilobytes)
      raise Libvirt::Error, "Cannot set domain's memory size value" if result < 0
      true
    end

    def max_vcpus
      vcpus = FFI::Libvirt::Domain.virDomainGetMaxVcpus(@domain)
      raise Libvirt::Error, "Cannot retrieve domain's max vcpus value" if vcpus < 0
      vcpus
    end

    def vcpus=(vcpus)
      vcpus = FFI::Libvirt::Domain.virDomainSetVcpus(@domain, vcpus)
      raise Libvirt::Error, "Cannot set domain's vcpus value" if vcpus < 0
      vcpus
    end

    def pin_vcpu
      raise 'unimplemented'
    end

    def to_xml(flags = 0)
      xml_description = FFI::Libvirt::Domain.virDomainGetXMLDesc(@domain, flags)
      raise Libvirt::Error, "Cannot retrieve domain's xml description" unless xml_description.kind_of? String
      xml_description
    end

    def undefine
      result = FFI::Libvirt::Domain.virDomainUndefine(@domain)
      raise Libvirt::Error, "Cannot undefine domain" if result < 0
      true
    end

    def create
      result = FFI::Libvirt::Domain.virDomainCreate(@domain)
      raise Libvirt::Error, "Cannot create a defined domain" if result < 0
      true
    end

    def autostart
      autostart = FFI::MemoryPointer.new(:int)
      result = FFI::Libvirt::Domain.virDomainGetAutostart(@domain, autostart)
      raise Libvirt::Error, "Cannot retrieve domain's autostart options" if result < 0
      autostart.read_int
    end

    def autostart=(autostart)
      result = FFI::Libvirt::Domain.virDomainSetAutostart(@domain, autostart)
      raise Libvirt::Error, "Cannot set domain's autostart options" if result < 0
      true
    end

    def free
      result = FFI::Libvirt::Domain.virDomainFree(@domain)
      raise Libvirt::Error, "Cannot free domain object" if result < 0
      true
    end

    def attach_device(xml)
      result = FFI::Libvirt::Domain.virDomainAttachDevice(@domain, xml)
      raise Libvirt::Error, "Cannot attach device to domain" if result < 0
      true
    end

    def detach_device(xml, flags = 0)
      result = FFI::Libvirt::Domain.virDomainAttachDevice(@domain, xml, flags)
      raise Libvirt::Error, "Cannot deattach device to domain" if result < 0
      true
    end

    def save(path)
      result = FFI::Libvirt::Domain.virDomainSave(@domain, path)
      raise Libvirt::Error, "Cannot save domain" if result < 0
      true
    end
  end
end
