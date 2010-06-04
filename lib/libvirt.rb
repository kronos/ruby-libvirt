#
# libvirt.rb: main module for the ruby-libvirt bindings
#
# Copyright (C) 2007 Red Hat, Inc.
# Copyright (C) 2010 Oversun-Scalaxy.
#
# Distributed under the GNU Lesser General Public License v2.1 or later.
# See COPYING for details
#
# David Lutterkort <dlutter@redhat.com>
# Ivan Samsonov   <hronya@gmail.com>

require 'ffi'
require 'libvirt/ffi/libvirt'

module Libvirt
  def type=(type)
    @type = type
  end

  def type
    @type || "Xen"
  end

  # see readable_version method
  def version
    library_version = FFI::MemoryPointer.new(:pointer)
    type_version = FFI::MemoryPointer.new(:pointer)

    result = FFI::Libvirt.virGetVersion(library_version, @type, type_version)
    raise ArgumentError, "Failed get version for #{type} connection" if result < 0

    [library_version.get_ulong(0), type_version.get_ulong(0)]
  end
  module_function :version

  class Error < StandardError
  end

  class ConnectionError < Error
  end

  class DefinitionError < Error
  end

  class RetrieveErroror < Error
  end

  def readable_version
    return @version if @version
    library_version = version[0]

      major = library_version / 1_000_000
      minor = (library_version % 1_000_000) / 1_000
    release = library_version % 1_000

    @version = "#{major}.#{minor}.#{release}"
  end
  module_function :readable_version
end
