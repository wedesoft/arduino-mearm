require 'serialport'

class Client
  def initialize device, baud
    @serial = SerialPort.new device, baud
    @serial.read_timeout = 2000
  end
  def time_required value
    write_serial "#{value}t"
    read_serial.to_f
  end
  def target *values
    write_serial "#{values.join " "}c"
  end
  def configuration c
    write_serial c
    read_serial.split(' ').collect &:to_f
  end
  def pos
    configuration 'c'
  end
  def lower
    configuration 'l'
  end
  def upper
    configuration 'u'
  end
  def ready?
    write_serial 'r'
    read_serial.to_i != 0
  end
  def stop
    write_serial 'x'
  end
  def write_serial str
    @serial.write str
    @serial.flush
  end
  def read_serial
    @serial.readline
  end
end

