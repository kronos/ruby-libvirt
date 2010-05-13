require 'mkmf'

def have_libvirt_funcs(funcs)
  funcs.each { |f| have_func(f, "libvirt/libvirt.h") }
end

def have_libvirt_types(types)
  types.each { |t| have_type(t, "libvirt/libvirt.h") }
end

dir_config("libvirt")

unless pkg_config("libvirt")
  raise "libvirt not found"
end

libvirt_types = [ 'virNetworkPtr',
          'virStoragePoolPtr',
          'virStorageVolPtr' ]

have_libvirt_types(libvirt_types)

create_header
create_makefile("libvirt")
