
require 'net/http'
require 'json'

http = Net::HTTP.new('192.168.1.102', 80)
puts "Turning off all Leds."

http.send_request('PUT', '/digital/11/0')
http.send_request('PUT', '/digital/12/0')
http.send_request('PUT', '/digital/13/0')

puts "Getting info from a LDR that is into analogic pin 0 "

while true

  r2 = http.send_request('GET', '/analogic/00')
  pin_value = JSON.parse(r2.body)["pin_value"]

  if (pin_value > 25)
    puts "It's clear...The light is green!"
    http.send_request('PUT', '/digital/11/1')
    #Just one light can be shinning..
    http.send_request('PUT', '/digital/12/0')
    http.send_request('PUT', '/digital/13/0')

  elsif (pin_value.between?(12,25))
    puts "It's a little dark, let's shine the yellow light!"
    http.send_request('PUT', '/digital/12/1')
    #Just one light can be shinning..
    http.send_request('PUT', '/digital/13/0')
    http.send_request('PUT', '/digital/11/0')

  else
    puts "It's completly in  darkness, let's shine the red light!"
    http.send_request('PUT', '/digital/13/1')
    #Just one light can be shinning..
    http.send_request('PUT', '/digital/11/0')
    http.send_request('PUT', '/digital/12/0')
  end

  sleep(3) 

end
