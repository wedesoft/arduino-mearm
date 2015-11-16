class Linear
  def initialize pos, options = {}
    @pos = pos
    @target = options[:target] || pos
    @speed =  options[:speed] || 1
  end
  def target value
    Linear.new @pos, speed: @speed, target: value
  end
  def get
    @pos
  end
  def advance time
    difference = @target - @pos
    sign = difference <=> 0
    step = [difference.abs, time * @speed].min
    Linear.new @pos + sign * step, speed: @speed, target: @target
  end
end

class Quadratic
  def initialize pos, options = {}
    @pos = pos
    @speed = options[:speed] || 0
    @target = options[:target] || pos
  end
  def target value
    Quadratic.new @pos, target: value
  end
  def get
    @pos
  end
  def advance time
    Quadratic.new @target > @pos ? @pos + 5 * time ** 2 + @speed * time : 100, speed: @speed + 10 * time, target: @target
  end
end
