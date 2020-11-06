#
# display the shader into a web mosaic
# 

require 'sinatra'
# require 'sinatra/reloader'

set :protection, :except => :frame_options
set :bind, '0.0.0.0'
# set :port, 8080

def Shader
  colors = []
  (0...COTE*COTE).each_with_index do   |idx, i|
        red =  (  128 - idx * ( 256 + 128 ) / (COTE*COTE) ).abs
        gre =  256 - (  256 - idx * ( 128 + 256 ) / (COTE*COTE) ).abs
        blu = 255 - red
        colors  <<   {
                           r:  red,
                           g:  gre,
                           b:  blu 
                      }

          STDERR.print "   { %3d, %3d, %3d,  0 },  " % [blu, gre, red]
          STDERR.puts if i%7==6
          STDERR.puts if i%14==13
   end  
   return colors
end



get '/'   do 
  LUM = 128
  colors = Shader ()
  # print colors
  erb :degrade, :locals => { :size => COTE, :colors => colors.map { |c|  { r: c[:r]/2, g: c[:g]/2, b: c[:b]/2 } }
  }
end
