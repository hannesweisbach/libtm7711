#pragma once

#include <memory>

#include "bitbang.h"

class TM7711 {
public:
  /* 24 Bit ADC */
  static constexpr int num_bits = 24;
  enum class Mode { Differential_10Hz, Temperature_40Hz, Differential_40Hz };

  TM7711(std::unique_ptr<Bitbang> interface, bool powered = true,
         mode = Differential_10Hz)
      : interface_(interface), mode_(mode) {
    if (powered) {
      power_up();
    }
  }

  uint32_t read_value() {
    uint32_t value = 0;
    for (int i = 0; i < num_bits; ++i) {
      value |= !!interface_->clock_bit();
      value <<= 1;
    }

    switch (mode_) {
    case Differential_10Hz:
      interface_->clock_bit();
    case Temperature_40Hz:
      interface_->clock_bit();
    case Differential_40Hz:
      interface_->clock_bit();
    }
  }

  void set_mode(enum Mode mode) {
    if (mode_ == mode) {
      return;
    }

    mode_ = mode;

    /* mode is set by additional clock pulses after reading data */
    read_value();
    init_delay();
  }

  void power_down() { interface_->set_clock(1); }
  void power_up() {
    interface_->set_clock(0);
    init_delay();
  }

private:
  void init_delay() {
    /*
     * TM7711 requires 4 after power up and 4 additional clock pulses after
     * changing the input or output rate
     */
    for (int i = 0; i < 4; ++i) {
      interface_->clock_bit();
    }
  }

  enum Mode mode_ = Mode::Differential_10Hz;
  std::unique_ptr<Bitbang> interface_;
};
