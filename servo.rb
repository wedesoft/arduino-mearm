#!/usr/bin/env ruby
require 'Qt'
class CurveWindow < Qt::Widget
  def initialize parent = nil
    super parent
    @arr = []
    @n = 0
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
    v = height - 1 - e.y
    @n = v
  end
  def timerEvent e
    value = @n
    @arr.push value
    update
  end
end
app = Qt::Application.new ARGV
window = CurveWindow.new
window.show
app.exec
