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
require 'libvirt/connection'
require 'libvirt/domain'

module Libvirt
  def version(type="Xen")
    library_version = FFI::MemoryPointer.new(:pointer)
    type_version = FFI::MemoryPointer.new(:pointer)

    result = FFI::Libvirt.virGetVersion(library_version, type, type_version)
    raise ArgumentError, "Failed get version for #{type} connection" if result < 0

    [library_version.read_ulong, type_version.read_ulong]
  end
  module_function :version
  #
  # # A version in Libvirt's representation
  # class Version
  #   attr_reader :version, :type
  #
  #   def initialize(type, version)
  #     @type = type
  #     @version = version
  #   end
  #
  #   def major
  #     version / 1000000
  #   end
  #
  #   def minor
  #     version % 1000000 / 1000
  #   end
  #
  #   def release
  #     version % 1000
  #   end
  #
  #   def to_s
  #     "#{major}.#{minor}.#{release}"
  #   end
  # end
end
