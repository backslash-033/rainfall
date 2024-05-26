#include <unistd.h>

void main(int argc,char **argv)
{
  N *my_class;
  void (*fnc_pointer)((*void)(), N*);
  
  if (argc < 2) {
    exit(1);
  }
  my_class = (N *)operator.new(0x6c);

  N::N(my_class, 5);

  fnc_pointer = new (void *);

  N::N(fnc_pointer, 6);
  N::setAnnotation(my_class, argv[1]);

  fnc_pointer(fnc_pointer, my_class);
  return;
}