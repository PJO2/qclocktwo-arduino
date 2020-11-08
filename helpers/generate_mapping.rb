# 
# find the position of words based on the 
# clock display
#
# Note that hours words are searched beginning at the first letter
# whereas minutes words use a reverse search starting at the last character
#
# the code generate C code 
# to fill a structure like  struct S_Word { byte pos, byte len }
# The order is reversed to be read by pgm_read_word_near

HORLOGE =   <<~EOF
IL#ESTRUNELDIX 
MINUITDEUXSEPT 
QUATREMIDICINQ 
HUIT-TROISNEUF 
SIXONZE+HEURES 
TRENTEQUARANTE 
VINGTCINQUANTE 
DIXQUATRETCINQ 
UNE-TROISSEIZE 
SIXEPILE!DEUXF 
ONZELNEUFCHUIT 
UEDOUZEASEPTUE 
QUATORZETREIZE 
CQUINZEADEGRES 
EOF


Heures = [
    [ "MINUIT",   ""       ],
    [ "UNE",     "HEURE",   ],
    [ "DEUX",    "HEURES",  ],  
    [ "TROIS",   "HEURES",  ], 
    [ "QUATRE",  "HEURES",  ], 
    [ "CINQ",    "HEURES",  ],  
    [ "SIX",     "HEURES",  ], 
    [ "SEPT",    "HEURES",  ], 
    [ "HUIT",    "HEURES",  ],  
    [ "NEUF",    "HEURES",  ], 
    [ "DIX",     "HEURES",  ], 
    [ "ONZE",    "HEURES",  ],
    [ "MIDI",     "",     ],
    [ "UNE",     "HEURE",   ],
    [ "DEUX",    "HEURES",  ],  
    [ "TROIS",   "HEURES",  ], 
    [ "QUATRE",  "HEURES",  ], 
    [ "CINQ",    "HEURES",  ],  
    [ "SIX",     "HEURES",  ], 
    [ "SEPT",    "HEURES",  ], 
    [ "HUIT",    "HEURES",  ],  
    [ "NEUF",    "HEURES",  ],
    [ "DIX",     "HEURES",  ],  
    [ "ONZE",    "HEURES",  ],
]               
Minutes = [
    [   "PILE!",   "",    "",      ],  
    [   "UNE",     "",    "",      ],  
    [   "DEUX",    "",    "",      ],  
    [   "TROIS",   "",    "",      ],  
    [   "QUATRE",  "",    "",      ],  
    [   "CINQ",    "",    "",      ],  
    [   "SIX",     "",    "",      ],  
    [   "SEPT",    "",    "",      ],  
    [   "HUIT",    "",    "",      ],  
     [   "NEUF",    "",    "",      ],  
     [   "DIX",     "",    "",      ],  
     [   "ONZE",    "",    "",      ],  
     [   "DOUZE",   "",    "",      ],  
     [   "TREIZE",  "",    "",      ],  
     [   "QUATORZE","",    "",      ],  
     [   "QUINZE",  "",    "",      ],  
     [   "SEIZE",   "",    "",      ],  
     [   "DIX",     "-",  "SEPT",   ],  
     [   "DIX",     "-",  "HUIT",   ],  
     [   "DIX",     "-",  "NEUF",   ],  
     [   "VINGT",   "",    "",      ],  
     [   "VINGT",   "ET",   "UN",   ],  
     [   "VINGT",   "DEUX",  "",    ],  
     [   "VINGT",   "TROIS", "",    ],  
     [   "VINGT",   "QUATRE","",    ],  
     [   "VINGT",   "CINQ",  "",    ],  
     [   "VINGT",   "SIX",   "",    ],  
     [   "VINGT",   "SEPT",  "",    ],  
     [   "VINGT",   "HUIT",  "",    ],  
     [   "VINGT",   "NEUF",  "",    ],  
     [   "TRENTE",  "",    "",      ],  
     [   "TRENTE",  "ET",   "UN",   ],  
     [   "TRENTE",  "DEUX",   "",   ],  
     [   "TRENTE",  "TROIS",  "",   ],  
     [   "TRENTE",  "QUATRE", "",   ],  
     [   "TRENTE",  "CINQ",   "",   ],  
     [   "TRENTE",  "SIX",    "",   ],  
     [   "TRENTE",  "SEPT",   "",   ],  
     [   "TRENTE",  "HUIT",   "",   ],  
     [   "TRENTE",  "NEUF",   "",   ],  
     [   "QUARANTE","",    "",      ],  
     [   "QUARANTE","ET",   "UN",   ],  
     [   "QUARANTE","DEUX",  "",    ],  
     [   "QUARANTE","TROIS", "",    ],  
     [   "QUARANTE","QUATRE","",    ],  
     [   "QUARANTE","CINQ",  "",    ],  
     [   "QUARANTE","SIX",   "",    ],  
     [   "QUARANTE","SEPT",  "",    ],  
     [   "QUARANTE","HUIT",  "",    ],  
     [   "QUARANTE","NEUF",  "",    ],  
     [   "CINQUANTE","",    "",     ],  
     [   "CINQUANTE","ET",   "UN",   ],  
     [   "CINQUANTE","DEUX",   "",   ],  
     [   "CINQUANTE","TROIS",  "",   ],  
     [   "CINQUANTE","QUATRE", "",   ],  
     [   "CINQUANTE","CINQ",   "",   ],  
     [   "CINQUANTE","SIX",    "",   ],  
     [   "CINQUANTE","SEPT",   "",   ],  
     [   "CINQUANTE","HUIT",   "",   ],   
     [   "CINQUANTE","NEUF",   "",   ],  
]

COTE = 14
MATRIX = HORLOGE.split.join("")
MINUTES_off = 5*COTE    # minutes start at row 6, skip 5 rows
puts MATRIX

puts
puts "/*"
puts " * Code based under the following word clock"
puts HORLOGE
puts "*/"  
puts
puts "const PROGMEM struct s_HourCode tHourWords[] = "
puts "{"
Heures.each do |words|
  print "  { " 
  words.each do |word|
    pos = MATRIX.index(word)
    # reverse odd rows according to the wiring
    if ( (pos / COTE) % 2 == 1) 
           # puts "word is #{word}, pos is #{pos}/#{word.length}"
          pos =    (pos / COTE) * COTE +
                   (COTE - (pos % COTE + word.length) )
      end
    print "  { %3d,  %2d  }, " % [ pos, word.length ]
  end
  print " },     // Words \"%s\"" % words.join(' ')
  puts
end
puts "};"

# find words starting at the 5th line 
puts "const PROGMEM struct s_MinuteCode tMinuteWords[] = "
puts "{"
Minutes.each do |words|
  print "  { " 
  words.each do |word|
    if (word=="CINQ")   # avoid CINQUANTE
        pos = MATRIX.rindex(word) 
    else
        pos = MATRIX.index(word, MINUTES_off)
    end
    # reverse odd rows according to the wiring
    if ( (pos / COTE) % 2 == 0) 
          pos =    (pos / COTE) * COTE +
                   (COTE - (pos % COTE + word.length) )
      end
    print "  { %3d,  %2d  }, " % [ pos - MINUTES_off, word.length ]
  end
  print " },     // Words \"%s\"" % words.join(' ')
  puts
end
puts "};"
