#include "tm7711.h"
#include "bitbang.h"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    return;
  }
  auto iface = make_linuxgpio_interface(argv[1], argv[2]);
}
