require 'libvirt/ffi/connection'
require 'libvirt/domain'

class Connection
  def initialize(url)
    @url = url
    @connection = FFI::Pointer.new(0)
  end

  def open
    @connection = FFI::Connection.virConnectOpen(@url)
    raise(Libvirt::ConnectionError, "Failed to open #{@url}") if @connection.null?
    true
  end

  def closed?
    @connection.null?
  end

  def type
    type = FFI::Connect.virConnectGetType(@connection)
    raise(RetrieveError, "Could'n revtrieve connection type") unless type
    type
  end

  def version
    version_ptr = MemoryPointer.new(:ulong)
    result = FFI::Connect.virConnectGetVersion(@connection, connection_version)

    raise(RetrieveError, "Could'n revtrieve connection version") if result < 0

    version = version_ptr.get_ulong(0)
  end

  def hostname
    hostname = FFI::Connect.virConnectGetHostname(@connection)
    raise(RetrieveError, "Could'n revtrieve connection hostname") unless hostname
    type
  end

  def uri
    uri = FFI::Connect.virConnectGetURI(@connection)
    raise(RetrieveError, "Could'n revtrieve connection hostname") unless uri
    uri
  end

  def max_vcpus(hypervisor_type = 'xen')
    vcpus = FFI::Connect.virConnectGetURI(@connection, hypervisor_type)
    raise(RetrieveError, "Could'n revtrieve connection hostname") unless vcpus
    vcpus
  end

  def get_node_info
    node_info_ptr = FFI::MemoryPointer.new(:char, NodeInfo.size)
    result = FFI::Connection.virNodeGetInfo(@connection, node_info_ptr)
    raise(RetrieveError, "Couldn't revtrieve connection node info") if result < 0
    NodeInfo.new(node_info_ptr)
  end

  def capabilities
    connection_capabilities = FFI::Connection.virConnectGetCapabilities(@connection)
    raise(RetrieveError, "Couldn't revtrieve connection node info") unless connection_capabilities
    connection_capabilities
  end

  def num_of_domains
    result = FFI::Connection.virConnectNumOfDomains(@connection)
    raise(RetrieveError, "Couldn't revtrieve connection num of domains") if result < 0
    result
  end

  def list_domains
    domains_count = 0
    domains_count = num_of_domains
    return [] if domains_count == 0

    array_names_ptr = FFI::MemoryPointer.new(:pointer, domains_count)
    result = FFI::Connection.virConnectListDomains(@connection, array_names_ptr, domains_count)
    raise(RetrieveError, "Couldn't revtrieve connection list of domain names") if result < 0

    string_ptr = array_names_ptr.read_pointer
    string_ptr.null? ? [] : string_ptr.get_array_of_string(0, domains_count).compact
  end

  def num_of_defined_domains
    result = FFI::Connection.virConnectNumOfDefinedDomains(@connection)
    raise(RetrieveError, "Couldn't revtrieve connection num of defined domains") if result < 0
    result    
  end

  def list_defined_domains
    domains_count = 0
    domains_count = num_of_defined_domains
    return [] if domains_count == 0

    array_names_ptr = FFI::MemoryPointer.new(:pointer, domains_count)
    result = FFI::Connection.virConnectListDefinedDomains(@connection, array_names_ptr, domains_count)
    raise(RetrieveError, "Couldn't revtrieve connection list of defined domain names") if result < 0

    string_ptr = array_names_ptr.read_pointer
    string_ptr.null? ? [] : string_ptr.get_array_of_string(0, domains_count).compact
  end

  def create_domain_linux(xml)
    domain = FFI::Connection.virConnectListDefinedDomains(@connection, xml, 0)
    raise(Error, "Couldn't create linux domain") if result < 0
    Domain.new(domain.read_pointer)
  end

  def get_domain_by_name(name)
    domain = FFI::Connection.virDomainLookupByName(@connection, name)
    raise(RetrieveError, "Can not find domain with name '#{name}'") if domain.null?
    Domain.new(domain.read_pointer)
  end

  def get_domain_by_id(id)
    domain = FFI::Connection.virDomainLookupByID(@connection, id)
    raise(RetrieveError, "Can not find domain with id '#{id}'") if domain.null?
    Domain.new(domain.read_pointer)
  end

  def get_domain_by_uuid(uuid)
    domain = FFI::Connection.virDomainLookupByID(@connection, uuid)
    raise(RetrieveError, "Can not find domain with uuid '#{uuid}'") if domain.null?
    Domain.new(domain.read_pointer)
  end

  def define_domain_xml(xml)
    domain = FFI::Connection.virDomainDefineXML(@connection, xml)
    raise(DefinitionError, "Can not define domain with xml:\n#{xml}") if domain.null?
    Domain.new(domain.read_pointer)
  end

  def close
    result = Connect.virConnectClose(@connection)
    raise(Libvirt::SystemCallError, "Connection close failed") if result < 0
  end
end
