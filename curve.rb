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
    @speed = options[:speed] || 0.0
    @target = options[:target] || pos.to_f
    @acceleration = options[:acceleration] || 1.0
  end
  def target value
    Quadratic.new @pos, target: value.to_f, speed: @speed, acceleration: @acceleration
  end
  def get
    @pos
  end
  def reversal
    (Math.sqrt(2 * @speed ** 2 + 4 * @acceleration * (@target - @pos).abs) - 2 * @speed.abs) / (2 * @acceleration)
  end
  def accelerate acceleration, time
    pos_step = acceleration / 2 * time ** 2 + @speed * time
    speed_step = acceleration * time
    Quadratic.new @pos + pos_step, speed: @speed + speed_step, target: @target, acceleration: @acceleration
  end
  def advance time
    t1 = [time, reversal].min
    t2 = time - t1
    sign = @target <=> @pos
    accelerate(sign * @acceleration, t1).accelerate -sign * @acceleration, t2
  end
end
