class Linear
  def initialize pos, target, options = {}
    @pos = pos
    @target = target
    @speed =  options[:speed] || 1
  end
  def get
    @pos
  end
  def advance time
    difference = @target - @pos
    sign = difference <=> 0
    step = [difference.abs, time * @speed].min
    Linear.new @pos + sign * step, @target
  end
end
