module FFI::Libvirt
  extend FFI::Library
  ffi_lib FFI::Libvirt::library_path

  def self.library_path
    'libvirt.so.0'
  end

  #int	virGetVersion(unsigned long * libVer, const char * type, unsigned long * typeVer)
  attach_function :virGetVersion, [:pointer, :string, :pointer], :int
  # int	virInitialize(void)
  attach_function :virInitialize, [], :int
end
