# 
# find the position of words based on the 
# clock display
#
# Note that hours words are searched beginning at the first letter
# whereas minutes words use a reverse search starting at the last character
#
# the code generate C code 
# to fill a structure like  struct S_Word { byte pos, byte len }

HORLOGE =   <<~EOF
IL#ESTRUNELDIX 
MINUITDEUXSEPT 
QUATREMIDICING 
HUIT-TROISNEUF 
SIXONZE+HEURES 
TRENTEQUARANTE 
VINGTCINQUANTE 
DIXQUATRESEPTI 
UNE#TROISSEIZE 
SIXEPILE!DEUXF 
ONZELNEUFCHUIT 
UEDOUZEACINQUE 
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
    [   "PILE",    "",    "",      ],  
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
HORLOGE = HORLOGE.split.join("")
puts HORLOGE

puts "struct s_HourCode tHourWords[] = "
puts "{"
Heures.each do |words|
  print "  { " 
  words.each do |word|
    pos = HORLOGE.index(word)
    print "  {  %3d,  %2d  }, " % [pos, word.length ]
  end
  print " },     // Words -%s %s-" % words
  puts
end
puts "};"

# find words starting at the 5th line 
puts "struct s_MinuteCode tMinuteWords[] = "
puts "{"
Minutes.each do |words|
  print "  { " 
  words.each do |word|
    pos = HORLOGE.rindex(word)
    print "  {  %3d,  %2d  }, " % [pos, word.length ]
  end
  print " },     // Words -%s %s %s-" % words
  puts
end
puts "};"

