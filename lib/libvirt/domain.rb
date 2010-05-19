class Domain
  def intialize(domain)
    @domain = domain
  end

  def release
    @domain.free
  end
end