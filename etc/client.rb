#!/usr/bin/env ruby
require 'serialport'
s = SerialPort.new '/dev/ttyUSB0', 115200
s.write 't'
print s.readline
10.times do
  s.write '-45g'
  sleep 1
  s.write '45g'
  sleep 1
end
s.write '0g'
