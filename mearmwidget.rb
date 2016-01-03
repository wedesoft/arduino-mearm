require 'Qt4'
require_relative 'ui_mearmwidget'

class MeArmWidget < Qt::Widget
  slots 'target()'
  attr_reader :ui
  def initialize client, parent = nil
    super parent
    @client = client
    @ui = Ui::MeArmWidget.new
    @ui.setupUi self
    @spin_boxes = [@ui.baseSpin, @ui.shoulderSpin, @ui.elbowSpin, @ui.gripperSpin]
    @spin_boxes.zip(client.pos, client.lower, client.upper).each do |spin_box, pos, lower, upper|
      spin_box.minimum = lower
      spin_box.maximum = upper
      spin_box.value = pos
      connect spin_box, SIGNAL('valueChanged(double)'), self, SLOT('target()')
    end
    @timer = nil
  end
  def timerEvent e
    pending if e.timerId == @timer
  end
  def values
    @spin_boxes.collect { |spin_box| spin_box.value }
  end
  def defer
    @timer = startTimer 0 unless @timer
  end
  def target
    if @client.ready?
      @client.target *values
    else
      defer
    end
  end
  def pending
    killTimer @timer
    @timer = nil
    target
  end
end
