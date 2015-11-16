class Linear
  SPEED = 10
  def initialize pos, target
    @pos = pos
    @target = target
  end
  def get
    @pos
  end
  def advance time
    difference = @target - @pos
    Linear.new @pos + (difference <=> 0) * [difference.abs, time * SPEED].min, @target
  end
end
