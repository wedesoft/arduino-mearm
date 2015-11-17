#!/usr/bin/env ruby
require 'Qt'
require_relative 'curve'

class CurveWindow < Qt::Widget
  def initialize parent = nil
    super parent
    @arr = []
    @curve = Quadratic.new 0, acceleration: 0.02
    startTimer 20
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
    p.end
  end
  def mousePressEvent e
    @curve = @curve.target height - 1 - e.y
  end
  def timerEvent e
    @curve = @curve.advance 1
    @arr.push @curve.get
    update
  end
end
app = Qt::Application.new ARGV
window = CurveWindow.new
window.show
app.exec
