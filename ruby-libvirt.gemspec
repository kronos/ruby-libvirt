Gem::Specification.new do |s|
  s.name = %q{ruby-libvirt}
  s.version = "0.0"

  s.required_rubygems_version = Gem::Requirement.new(">= 1.2") if s.respond_to? :required_rubygems_version=
  s.authors = ["David Lutterkort", "Ivan Samsonov"]
  s.date = %q{2010-05-19}
  s.description = %q{libvirt interface via FFI}
  s.email = %q{hronya@gmail.com}
  s.extra_rdoc_files = Dir.glob('lib/**/*.rb')
  s.files = ["AUTHORS", "CHANGELOG", "COPYING", "doc", "doc/site", "doc/site/index.html", "lib", "lib/libvirt", "lib/libvirt/connection.rb", 
             "lib/libvirt/domain.rb", "lib/libvirt/ffi", "lib/libvirt/ffi/connection.rb", "lib/libvirt/ffi/domain.rb", "lib/libvirt/ffi/libvirt.rb", 
             "lib/libvirt/ffi/network.rb", "lib/libvirt/ffi/storage_pool.rb", "lib/libvirt/ffi/storage_vol.rb", "lib/libvirt/ffi/structs", 
             "lib/libvirt/ffi/structs/domain_interface_stats.rb", "lib/libvirt/ffi/structs/node_info.rb", "lib/libvirt/ffi/structs/storage_pool_info.rb", 
             "lib/libvirt/ffi/structs/storage_vol_info.rb", "lib/libvirt/libvirt.rb", "lib/libvirt.rb", "Rakefile", "README", "README.rdoc", 
             "ruby-libvirt.gemspec", "sketch.rb", "tests", "tests/node.xml", "tests/tc_connect.rb"]
  s.homepage = %q{http://github.com/scalaxy/ruby-libvirt}
  s.rdoc_options = ["--line-numbers", "--inline-source", "--title", "libvirt", "--main", "README.rdoc"]
  s.require_paths = ["lib"]
  # s.rubyforge_project = %q{tidy-ffi}
  # s.rubygems_version = %q{1.3.5}
  s.summary = %q{libvirt interface via FFI}
  s.test_files = ["tests/tc_connect.rb"]

  if s.respond_to? :specification_version then
    current_version = Gem::Specification::CURRENT_SPECIFICATION_VERSION
    s.specification_version = 3

    if Gem::Version.new(Gem::RubyGemsVersion) >= Gem::Version.new('1.2.0') then
      s.add_runtime_dependency(%q<ffi>, [">= 0.3.5"])
    else
      s.add_dependency(%q<ffi>, [">= 0.3.5"])
    end
  else
    s.add_dependency(%q<ffi>, [">= 0.3.5"])
  end
end
