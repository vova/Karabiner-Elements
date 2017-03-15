#pragma once

#include "types.hpp"
#include <CoreGraphics/CoreGraphics.h>
#include <thread>
#include <vector>

namespace krbn {
namespace manipulator {
class modifier_flag_manager final {
public:
  modifier_flag_manager(void) {
    states_.resize(static_cast<size_t>(modifier_flag::prepared_modifier_flag_end_));

    states_[static_cast<size_t>(modifier_flag::caps_lock)] = std::make_unique<state>("caps lock", "⇪");
    states_[static_cast<size_t>(modifier_flag::left_control)] = std::make_unique<state>("control", "⌃");
    states_[static_cast<size_t>(modifier_flag::left_shift)] = std::make_unique<state>("shift", "⇧");
    states_[static_cast<size_t>(modifier_flag::left_option)] = std::make_unique<state>("option", "⌥");
    states_[static_cast<size_t>(modifier_flag::left_command)] = std::make_unique<state>("command", "⌘");
    states_[static_cast<size_t>(modifier_flag::right_control)] = std::make_unique<state>("control", "⌃");
    states_[static_cast<size_t>(modifier_flag::right_shift)] = std::make_unique<state>("shift", "⇧");
    states_[static_cast<size_t>(modifier_flag::right_option)] = std::make_unique<state>("option", "⌥");
    states_[static_cast<size_t>(modifier_flag::right_command)] = std::make_unique<state>("command", "⌘");
    states_[static_cast<size_t>(modifier_flag::fn)] = std::make_unique<state>("fn", "fn");
  }

  void reset(void) {
    for (const auto& s : states_) {
      if (s) {
        s->reset();
      }
    }
  }

  void unlock(void) {
    for (const auto& s : states_) {
      if (s) {
        s->unlock();
      }
    }
  }

  enum class operation {
    increase,
    decrease,
    lock,
    unlock,
    toggle_lock,
  };

  void manipulate(modifier_flag k, operation operation) {
    auto i = static_cast<size_t>(k);
    if (states_[i]) {
      switch (operation) {
      case operation::increase:
        states_[i]->increase();
        break;
      case operation::decrease:
        states_[i]->decrease();
        break;
      case operation::lock:
        states_[i]->lock();
        break;
      case operation::unlock:
        states_[i]->unlock();
        break;
      case operation::toggle_lock:
        states_[i]->toggle_lock();
        break;
      }
    }
  }

  bool pressed(modifier_flag m) const {
    if (m == modifier_flag::zero) {
      return true;
    }

    auto i = static_cast<size_t>(m);
    if (i < states_.size() && states_[i]) {
      return states_[i]->pressed();
    }
    return false;
  }

  bool pressed(const std::vector<modifier_flag>& modifier_flags) const {
    // return true if all modifier flags are pressed.
    for (const auto& m : modifier_flags) {
      if (!pressed(m)) {
        return false;
      }
    }
    return true;
  }

  uint8_t get_hid_report_bits(void) const {
    uint8_t bits = 0;

    if (pressed(modifier_flag::left_control)) {
      bits |= (0x1 << 0);
    }
    if (pressed(modifier_flag::left_shift)) {
      bits |= (0x1 << 1);
    }
    if (pressed(modifier_flag::left_option)) {
      bits |= (0x1 << 2);
    }
    if (pressed(modifier_flag::left_command)) {
      bits |= (0x1 << 3);
    }
    if (pressed(modifier_flag::right_control)) {
      bits |= (0x1 << 4);
    }
    if (pressed(modifier_flag::right_shift)) {
      bits |= (0x1 << 5);
    }
    if (pressed(modifier_flag::right_option)) {
      bits |= (0x1 << 6);
    }
    if (pressed(modifier_flag::right_command)) {
      bits |= (0x1 << 7);
    }

    return bits;
  }

  IOOptionBits get_io_option_bits(key_code key_code) const {
    // OS X requires to set NX_NONCOALSESCEDMASK.
    IOOptionBits bits = NX_NONCOALSESCEDMASK;

    if (pressed(modifier_flag::caps_lock)) {
      bits |= NX_ALPHASHIFTMASK;
    }
    if (pressed(modifier_flag::left_control)) {
      bits |= NX_CONTROLMASK | NX_DEVICELCTLKEYMASK;
    }
    if (pressed(modifier_flag::right_control)) {
      bits |= NX_CONTROLMASK | NX_DEVICERCTLKEYMASK;
    }
    if (pressed(modifier_flag::left_shift)) {
      bits |= NX_SHIFTMASK | NX_DEVICELSHIFTKEYMASK;
    }
    if (pressed(modifier_flag::right_shift)) {
      bits |= NX_SHIFTMASK | NX_DEVICERSHIFTKEYMASK;
    }
    if (pressed(modifier_flag::left_option)) {
      bits |= NX_ALTERNATEMASK | NX_DEVICELALTKEYMASK;
    }
    if (pressed(modifier_flag::right_option)) {
      bits |= NX_ALTERNATEMASK | NX_DEVICERALTKEYMASK;
    }
    if (pressed(modifier_flag::left_command)) {
      bits |= NX_COMMANDMASK | NX_DEVICELCMDKEYMASK;
    }
    if (pressed(modifier_flag::right_command)) {
      bits |= NX_COMMANDMASK | NX_DEVICERCMDKEYMASK;
    }
    if (pressed(modifier_flag::fn)) {
      bits |= NX_SECONDARYFNMASK;
    }

    // Add NX_NUMERICPADMASK, NX_SECONDARYFNMASK by key_code.
    //
    // Note:
    //   Microsoft Remote Client will fail to treat shift-arrow keys unless these flags.
    //   Alfred will fail f16 key as trigger key unless fn flag.

    switch (key_code) {
    case key_code::keypad_slash:
    case key_code::keypad_asterisk:
    case key_code::keypad_hyphen:
    case key_code::keypad_plus:
    case key_code::keypad_enter:
    case key_code::keypad_1:
    case key_code::keypad_2:
    case key_code::keypad_3:
    case key_code::keypad_4:
    case key_code::keypad_5:
    case key_code::keypad_6:
    case key_code::keypad_7:
    case key_code::keypad_8:
    case key_code::keypad_9:
    case key_code::keypad_0:
    case key_code::keypad_period:
    case key_code::keypad_equal_sign:
    case key_code::keypad_comma:
      bits |= NX_NUMERICPADMASK;
      break;

    case key_code::right_arrow:
    case key_code::left_arrow:
    case key_code::down_arrow:
    case key_code::up_arrow:
      bits |= NX_NUMERICPADMASK | NX_SECONDARYFNMASK;
      break;

    case key_code::f1:
    case key_code::f2:
    case key_code::f3:
    case key_code::f4:
    case key_code::f5:
    case key_code::f6:
    case key_code::f7:
    case key_code::f8:
    case key_code::f9:
    case key_code::f10:
    case key_code::f11:
    case key_code::f12:
    case key_code::f13:
    case key_code::f14:
    case key_code::f15:
    case key_code::f16:
    case key_code::f17:
    case key_code::f18:
    case key_code::f19:
    case key_code::f20:
    case key_code::f21:
    case key_code::f22:
    case key_code::f23:
    case key_code::f24:
      bits |= NX_SECONDARYFNMASK;
      break;

    default:
      break;
    }

    return bits;
  }

  CGEventFlags get_cg_event_flags_for_mouse_events(void) const {
    // The CGEventFlags and IOOptionBits are same for now.
    return get_io_option_bits(key_code::vk_none);
  }

private:
  class state final {
  public:
    state(const std::string& name, const std::string& symbol) : name_(name),
                                                                symbol_(symbol),
                                                                count_(0),
                                                                lock_count_(0) {}

    const std::string& get_name(void) const { return name_; }
    const std::string& get_symbol(void) const { return symbol_; }

    bool pressed(void) {
      std::lock_guard<std::mutex> guard(mutex_);

      return (count_ + lock_count_) > 0;
    }

    void reset(void) {
      std::lock_guard<std::mutex> guard(mutex_);

      count_ = 0;
    }

    void increase(void) {
      std::lock_guard<std::mutex> guard(mutex_);

      ++count_;
    }

    void decrease(void) {
      std::lock_guard<std::mutex> guard(mutex_);

      --count_;
    }

    void lock(void) {
      std::lock_guard<std::mutex> guard(mutex_);

      lock_count_ = 1;
    }

    void unlock(void) {
      std::lock_guard<std::mutex> guard(mutex_);

      lock_count_ = 0;
    }

    void toggle_lock(void) {
      std::lock_guard<std::mutex> guard(mutex_);

      lock_count_ = lock_count_ == 0 ? 1 : 0;
    }

  private:
    std::string name_;
    std::string symbol_;
    int count_;
    int lock_count_;

    std::mutex mutex_;
  };

  std::vector<std::unique_ptr<state>> states_;
};
}
}
