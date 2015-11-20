#!/usr/bin/env ruby
require 'Qt'
require_relative 'curve'

class CurveWindow < Qt::Widget
  def initialize parent = nil
    super parent
    @target = []
    @acceleration = []
    @speed = []
    @pos = []
    @curve = Curve.new 0, bound: 0.02
    startTimer 20
  end
  def enlarge arr, len
    retval = arr
    retval = [0] + retval while retval.size < width
    retval = retval[1 .. -1] while retval.size > width
    retval
  end
  def draw painter, arr, color
    arr = enlarge arr, width
    painter.setPen Qt::Color.new(color)
    x, y = 0, 0
    for i in 0 ... width
      xs, ys = i, height / 2 - arr[i].round
      painter.drawLine x, y, xs, ys
      x, y = xs, ys
    end
    arr
  end
  def paintEvent e
    painter = Qt::Painter.new self
    @target = draw painter, @target, Qt.red
    @acceleration = draw painter, @acceleration, Qt.blue
    @speed = draw painter, @speed, Qt.green
    @pos = draw painter, @pos, Qt.black
    painter.end
  end
  def mousePressEvent e
    @curve = @curve.retarget height / 2 - e.y
  end
  def timerEvent e
    @curve = @curve.advance 1
    @target.push @curve.target
    @acceleration.push @curve.acceleration * 2000
    @speed.push @curve.speed * 50
    @pos.push @curve.pos
    update
  end
end
app = Qt::Application.new ARGV
window = CurveWindow.new
window.show
app.exec
