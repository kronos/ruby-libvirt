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
  # major * 1_000_000 + minor * 1_000 + release
  def version(type="Xen")
    library_version = FFI::MemoryPointer.new(:pointer)
    type_version = FFI::MemoryPointer.new(:pointer)

    result = FFI::Libvirt.virGetVersion(library_version, type, type_version)
    raise ArgumentError, "Failed get version for #{type} connection" if result < 0

    [library_version.get_ulong(0), type_version.get_ulong(0)]
  end
  module_function :version
end
