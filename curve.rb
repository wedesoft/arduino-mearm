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
    @acceleration = options[:acceleration] || 1
  end
  def target value
    Quadratic.new @pos, target: value, acceleration: @acceleration
  end
  def get
    @pos
  end
  def reversal
    (Math.sqrt(2 * @speed ** 2 + 4 * @acceleration * (@target - @pos)) - 2 * @speed) / (2 * @acceleration)
  end
  def advance time
    pos_diff = @target - @pos
    sign = pos_diff <=> 0
    acceleration = sign * @acceleration
    pos_step = acceleration / 2 * time ** 2 + @speed * time
    speed_step = acceleration * time
    Quadratic.new @pos + pos_step, speed: @speed + speed_step, target: @target, acceleration: @acceleration
  end
end
