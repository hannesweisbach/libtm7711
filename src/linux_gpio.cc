#include <gpiod.hpp>

#include "bitbang.h"

auto find_line_or_throw(const std::string &name) {
  auto line = gpiod::find_line(name);
  if (!line) {
    throw std::runtime_error("GPIO line " + dout + " not found.");
  }
  return line;
}

class LinuxGPIOBitbang : public Bitbang {
private:
  gpiod::line dout_;
  gpiod::line pclk_;

public:
  LinuxGPIOBitbang(const std::string &dout, const std::string &pclk)
      dout_(find_line_or_throw(dout)),
      pclk_(find_line_or_throw(pclk)) {
    dout_.request({"TM7711.dout", ::gpiod::line_request::DIRECTION_INPUT, 0});
    pclk_.request({"TM711.pclk", ::gpiod::line_request::DIRECTION_OUTPUT, 0},
                  1);
  }
  ~LinuxGPIOBitbang() = default;

  void set_clock(int val) { pclk_.set_value(!!val); }

  int clock_bit() {
    pclk_.set_value(1);
    pclk_.set_value(0);
    return dout_.get_value();
  }
};

std::unique_ptr<Bitbang> make_linuxgpio_interface(const std::string &dout,
                                                  const std::string &pclk) {
  return std::make_unique<LinuxGPIOBitBang>(dout, pclk);
}
