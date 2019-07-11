#pragma once

#include <memory>

class Bitbang {
  virtual ~Bitbang() = 0;
  virtual void set_clock(int value) = 0;
  virtual int clock_bit() = 0;
}

std::unique_ptr<Bitbang> make_linuxgpio_interface(const std::string &dout,
                                                  const std::string &pclk);
