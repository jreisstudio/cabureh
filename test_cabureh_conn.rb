require 'net/http'
uri   = URI  'http://192.168.1.101/digital/13/1'
uri_b = URI  'http://192.168.1.101/digital/13/0'
puts "turning on the led on pin 13 "
res = Net::HTTP.get(uri)
sleep 3.5
puts "turning off the led on pin 13 "
res_b = Net::HTTP.get(uri_b)
puts "Nice...that's ok!"
