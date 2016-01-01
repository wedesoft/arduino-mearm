require 'Qt4'
require 'serialport'
require_relative 'ui_mearmwidget'

BASE = 0

class Client
  LETTER = ['b', 's', 'e', 'g']
  def initialize device, baud
    @serial = SerialPort.new device, baud: baud
  end
  def target_angle drive, value
    @serial.write "#{value}#{LETTER[drive]}"
  end
end

class MeArmWidget < Qt::Widget
  slots 'baseChange(int)'
  attr_reader :ui
  def initialize client, parent = nil
    super parent
    @client = client
    @ui = Ui::MeArmWidget.new
    @ui.setupUi self
    connect @ui.baseSlider, SIGNAL('valueChanged(int)'), self, SLOT('baseChange(int)')
  end
  def baseChange value
    @client.target_angle BASE, value
  end
end
