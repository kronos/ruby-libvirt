require 'libvirt/ffi/connection'
require 'libvirt/domain'

module Libvirt
  class Connection
    def initialize(url)
      @url = url
      @connection = FFI::Pointer.new(0)
    end

    def open
      @connection = FFI::Libvirt::Connection.virConnectOpen(@url)
      raise Libvirt::ConnectionError, "Failed to open #{@url}" if @connection.null?
      true
    end

    def open_read_only
      @connection = FFI::Libvirt::Connection.virConnectOpenReadOnly(@url)
      raise Libvirt::ConnectionError, "Failed to read-only open #{@url}" if @connection.null?
      true
    end

    def closed?
      @connection.null?
    end

    def type
      type = FFI::Libvirt::Connection.virConnectGetType(@connection)
      raise Libvirt::RetrieveError, "Couldn't revtrieve connection type" unless type.kind_of? String
      type
    end

    def version
      version_ptr = FFI::MemoryPointer.new(:ulong)
      result = FFI::Libvirt::Connection.virConnectGetVersion(@connection, version_ptr)
      raise Libvirt::RetrieveError, "Couldn't revtrieve connection version" if result < 0
      version_ptr.get_ulong(0)
    end

    def hostname
      hostname = FFI::Libvirt::Connection.virConnectGetHostname(@connection)
      raise Libvirt::RetrieveError, "Couldn't revtrieve connection hostname" unless hostname.kind_of? String
      hostname
    end

    def uri
      uri = FFI::Libvirt::Connection.virConnectGetURI(@connection)
      raise Libvirt::RetrieveError, "Couldn't revtrieve connection hostname" unless uri.kind_of? String
      uri
    end

    def max_vcpus(hypervisor_type = 'xen')
      vcpus = FFI::Libvirt::Connection.virConnectGetMaxVcpus(@connection, hypervisor_type)
      raise Libvirt::RetrieveError, "Couldn't revtrieve max vcpus value" unless vcpus.kind_of? Numeric
      vcpus
    end

    def get_node_info
      node_info_ptr = FFI::MemoryPointer.new(:pointer)
      result = FFI::Libvirt::Connection.virNodeGetInfo(@connection, node_info_ptr)
      raise Libvirt::RetrieveError, "Couldn't revtrieve connection node info" if result < 0
      FFI::Libvirt::NodeInfo.new(node_info_ptr)
    end

    def capabilities
      connection_capabilities = FFI::Libvirt::Connection.virConnectGetCapabilities(@connection)
      raise Libvirt::RetrieveError, "Couldn't revtrieve connection node info" unless connection_capabilities.kind_of? String
      connection_capabilities
    end

    def num_of_domains
      result = FFI::Libvirt::Connection.virConnectNumOfDomains(@connection)
      raise Libvirt::RetrieveError, "Couldn't revtrieve connection num of domains" if result < 0
      result
    end

    def list_domains
      domains_count = 0
      domains_count = num_of_domains
      return [] if domains_count == 0

      array_names_ptr = FFI::MemoryPointer.new(:pointer, domains_count)
      result = FFI::Libvirt::Connection.virConnectListDomains(@connection, array_names_ptr, domains_count)
      raise Libvirt::RetrieveError, "Couldn't revtrieve connection list of domain names" if result < 0

      string_ptr = array_names_ptr.read_pointer
      string_ptr.null? ? [] : string_ptr.get_array_of_string(0, domains_count).compact
    end

    def num_of_defined_domains
      result = FFI::Libvirt::Connection.virConnectNumOfDefinedDomains(@connection)
      raise Libvirt::RetrieveError, "Couldn't revtrieve connection num of defined domains" if result < 0
      result
    end

    def list_defined_domains
      domains_count = 0
      domains_count = num_of_defined_domains
      return [] if domains_count == 0

      array_names_ptr = FFI::MemoryPointer.new(:pointer, domains_count)
      result = FFI::Libvirt::Connection.virConnectListDefinedDomains(@connection, array_names_ptr, domains_count)
      raise Libvirt::RetrieveError, "Couldn't revtrieve connection list of defined domain names" if result < 0

      string_ptr = array_names_ptr.read_pointer
      string_ptr.null? ? [] : string_ptr.get_array_of_string(0, domains_count).compact
    end

    def migrate_domain(domain, flags = FFI::Libvirt::Domain::VIR_MIGRATE_LIVE, bandwidth = 0, dname = "", uri = "")
      new_domain_ptr = FFI::Libvirt::Domain.virDomainMigrate(domain, @connection, flags, dname, uri, bandwidth)
      raise Libvirt::Error, "Cannot migrate domain" if new_domain_ptr.null?
      Libvirt::Domain.new(new_domain_ptr)
    end

    def restore_domain(path)
      result = FFI::Libvirt::Domain.virDomainRestore(@domain, path)
      raise Libvirt::Error, "Cannot perform restore domain from core dump" if result < 0
      true
    end

    def create_domain(xml)
      domain = if Libvirt.readable_version >= '0.4.6'
        FFI::Libvirt::Connection.virDomainCreateXML(@connection, xml, 0)
      else
        FFI::Libvirt::Connection.virDomainCreateLinux(@connection, xml, 0)
      end
      raise Libvirt::Error, "Couldn't create domain" if domain.null?
      Libvirt::Domain.new(domain)
    end

    def get_domain_by_name(name)
      domain = FFI::Libvirt::Connection.virDomainLookupByName(@connection, name)
      raise Libvirt::RetrieveError, "Can not find domain with name '#{name}'" if domain.null?
      Libvirt::Domain.new(domain)
    end

    def get_domain_by_id(id)
      domain = FFI::Libvirt::Connection.virDomainLookupByID(@connection, id)
      raise Libvirt::RetrieveError, "Can not find domain with id '#{id}'" if domain.null?
      Libvirt::Domain.new(domain)
    end

    def get_domain_by_uuid(uuid)
      domain = FFI::Libvirt::Connection.virDomainLookupByID(@connection, uuid)
      raise Libvirt::RetrieveError, "Can not find domain with uuid '#{uuid}'" if domain.null?
      Libvirt::Domain.new(domain)
    end

    def define_domain_xml(xml)
      domain = FFI::Libvirt::Connection.virDomainDefineXML(@connection, xml)
      raise Libvirt::DefinitionError, "Can not define domain with xml:\n#{xml}" if domain.null?
      Libvirt::Domain.new(domain)
    end

    def close
      result = FFI::Libvirt::Connection.virConnectClose(@connection)
      raise Libvirt::SystemCallError, "Connection closing failed" if result < 0
    ensure
      @connection = FFI::Pointer.new(0)
    end
  end
end