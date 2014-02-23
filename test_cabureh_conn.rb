require 'net/http'

puts "turning on the led on pin 13 "
http = Net::HTTP.new('192.168.1.102', 80)
response = http.send_request('PUT', '/digital/13/1')
sleep 3.5
puts "turning off the led on pin 13 "
response = http.send_request('PUT', '/digital/13/0')
puts "Nice...that's ok!"
