require 'serialport'

class Client
  def initialize device, baud
    @serial = SerialPort.new device, baud: baud
  end
  def time_required value
    write_serial "#{value}t"
    read_serial.to_f
  end
  def target value
    write_serial "#{value}c"
  end
  def ready?
    write_serial 'r'
    read_serial.to_i != 0
  end
  def write_serial str
    @serial.write str
  end
  def read_serial
    @serial.readline
  end
end

