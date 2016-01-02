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
    @sliders = [@ui.baseSlider, @ui.shoulderSlider, @ui.elbowSlider, @ui.gripperSlider]
    @sliders.each do |slider|
      connect slider, SIGNAL('valueChanged(int)'), self, SLOT('target()')
    end
    @timer = nil
  end
  def timerEvent e
    pending if e.timerId == @timer
  end
  def values
    @sliders.collect { |slider| slider.value }
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
