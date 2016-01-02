#!/usr/bin/env ruby
require 'serialport'
s = SerialPort.new '/dev/ttyUSB0', 115200
s.read_timeout = 2000
10.times do
  s.write 'r'
  s.flush
  puts s.readline
end
