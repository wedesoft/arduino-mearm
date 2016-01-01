require 'Qt4'
require_relative 'ui_mearmwidget'

class MeArmWidget < Qt::Widget
  slots 'baseChange(int)'
  attr_reader :ui
  def initialize client, parent = nil
    super parent
    @client = client
    @ui = Ui::MeArmWidget.new
    @ui.setupUi self
    connect @ui.baseSlider, SIGNAL('valueChanged(int)'), self, SLOT('baseChange(int)')
    @timer = nil
  end
  def timerEvent e
    pending if e.timerId == @timer
  end
  def value
    @ui.baseSlider.value
  end
  def defer
    @timer = startTimer 0 unless @timer
  end
  def target
    if @client.ready?
      @client.target value
    else
      defer
    end
  end
  def pending
    killTimer @timer
    @timer = nil
    target
  end
  def baseChange value
    target
  end
end
