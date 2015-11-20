class Curve
  attr_reader :pos, :speed, :target
  def initialize pos, options = {}
    @pos = pos
    @speed = options[:speed] || 0.0
    @target = options[:target] || pos.to_f
    @acceleration = options[:acceleration] || 1.0
    @sign = options[:sign] || 0
    @state = options[:state] || :stop
  end
  def acceleration
    @acceleration * @sign
  end
  def retarget value
    Curve.new @pos, target: value.to_f, speed: @speed, acceleration: @acceleration, state: :accel, sign: value <=> stop_value
  end
  def reverse_time
    acceleration = @sign * @acceleration
    (Math.sqrt(2 * @speed ** 2 + 4 * acceleration * (@target - @pos)) - 2 * @sign * @speed) / (2 * @acceleration)
  end
  def stop_time
    @speed.abs / @acceleration
  end
  def stop_value
    t = stop_time
    @pos + @speed * t - 0.5 * (@speed <=> 0) * @acceleration * t ** 2
  end
  def state value
    Curve.new @pos, target: @target, speed: @speed, acceleration: @acceleration, state: value, sign: @sign
  end
  def sign value
    Curve.new @pos, target: @target, speed: @speed, acceleration: @acceleration, state: @state, sign: value
  end
  def accelerate acceleration, time
    pos = @pos + acceleration / 2 * time ** 2 + @speed * time
    speed = @speed + acceleration * time
    Curve.new pos, speed: speed, target: @target, acceleration: @acceleration, state: @state, sign: @sign
  end
  def advance time
    case @state
    when :accel
      reverse = [reverse_time, 0].max
      if time > reverse
        accelerate(@sign * @acceleration, reverse).state(:decel).sign(-@sign).advance time - reverse
      else
        accelerate @sign * @acceleration, time
      end
    when :decel
      stop = stop_time
      if time > stop
        accelerate(@sign * @acceleration, stop).state(:stop).sign 0
      else
        accelerate @sign * @acceleration, time
      end
    else
      self
    end
  end
end
