#!/usr/bin/env ruby
require 'Qt'
class CurveWindow < Qt::Widget
  def initialize parent = nil
    super parent
    @arr = []
    @target = 0
    @speed = 0
    @offset = 0
    @time = 0
    startTimer 10
  end
  def paintEvent e
    @arr = [0] + @arr while @arr.size < width
    @arr = @arr[1 .. -1] while @arr.size > width
    p = Qt::Painter.new self
    x, y = 0, 0
    for i in 0 ... width
      xs, ys = i, height - 1 - @arr[i]
      p.drawLine x, y, xs, ys
      x, y = xs, ys
    end
  end
  def mousePressEvent e
    current = value
    @target = height - 1 - e.y
    @speed = current < @target ? 1 : -1
    @offset = @time - (current - @target) / @speed
  end
  def value
    if @time < @offset
      @target + @speed * (@time - @offset)
    else
      @target
    end
  end
  def timerEvent e
    @time += 1
    @arr.push value
    update
  end
end
app = Qt::Application.new ARGV
window = CurveWindow.new
window.show
app.exec
