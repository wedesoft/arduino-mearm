class Curve
  attr_reader :pos, :speed, :target
  def initialize pos, options = {}
    @pos = pos
    @speed = options[:speed] || 0.0
    @target = options[:target] || pos.to_f
    @acceleration = options[:acceleration] || 0
    @bound = options[:bound] || 1.0
    @state = options[:state] || :stop
  end
  def acceleration
    @acceleration
  end
  def retarget value
    acceleration = @bound * (value <=> stop_value)
    # test 0 ?
    Curve.new @pos, target: value.to_f, speed: @speed, acceleration: acceleration, bound: @bound, state: :accelerate
  end
  def reverse_time
    sign = @acceleration <=> 0
    (Math.sqrt(2 * @speed ** 2 + 4 * @acceleration * (@target - @pos)) - 2 * sign * @speed) / (2 * @bound)
  end
  def stop_time
    @speed.abs / @bound
  end
  def stop_value
    t = stop_time
    @pos + @speed * t - 0.5 * (@speed <=> 0) * @bound * t ** 2
  end
  def reverse
    Curve.new @pos, target: @target, speed: @speed, bound: @bound, state: :decelerate, acceleration: -@acceleration
  end
  def halt
    Curve.new @target, target: @target, speed: @speed, bound: @bound, state: :stop, acceleration: 0
  end
  def accelerate time
    pos = @pos + @acceleration / 2 * time ** 2 + @speed * time
    speed = @speed + @acceleration * time
    Curve.new pos, speed: speed, target: @target, bound: @bound, state: @state, acceleration: @acceleration
  end
  def advance time
    case @state
    when :accelerate
      reverse = reverse_time
      if time > reverse
        accelerate(reverse).reverse.advance time - reverse
      else
        accelerate time
      end
    when :decelerate
      stop = stop_time
      if time > stop
        accelerate(stop).halt
      else
        accelerate time
      end
    else
      self
    end
  end
end
