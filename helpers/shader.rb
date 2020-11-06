#
# Construct the purple to orange shading
#
# Output C code for big endian processor
# directly usable by the uint32_t color parameter 
#

COTE = 14

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

HEADERS = "
struct S_Degrade
{
   byte r;
   byte g;
   byte b;
   byte padding;
};

const PROGMEM struct S_Degrade tColors[] =
"
TRAILERS = "};"

if __FILE__ == $0
   STDERR.puts HEADERS
   Shader()
   STDERR.puts TRAILERS
end   