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
    @state = options[:state] || :stop
  end
  def target value
    Quadratic.new @pos, target: value.to_f, speed: @speed, acceleration: @acceleration, state: :accel
  end
  def get
    @pos
  end
  def reversal
    (Math.sqrt(2 * @speed ** 2 + 4 * @acceleration * (@target - @pos).abs) - 2 * @speed.abs) / (2 * @acceleration)
  end
  def stop
    @speed.abs / @acceleration
  end
  def state value
    Quadratic.new @pos, target: @target, speed: @speed, acceleration: @acceleration, state: value
  end
  def accelerate acceleration, time
    pos_step = acceleration / 2 * time ** 2 + @speed * time
    speed_step = acceleration * time
    Quadratic.new @pos + pos_step, speed: @speed + speed_step, target: @target, acceleration: @acceleration, state: @state
  end
  def advance time
    sign = @target <=> @pos
    case @state
    when :accel
      rev = reversal
      if time > rev
        accelerate(sign * @acceleration, rev).state(:decel).advance time - rev
      else
        accelerate sign * @acceleration, time
      end
    when :decel
      st = stop
      if time > st
        accelerate(-sign * @acceleration, st).state :stop
      else
        accelerate -sign * @acceleration, time
      end
    else
      return self
    end
  end
end
