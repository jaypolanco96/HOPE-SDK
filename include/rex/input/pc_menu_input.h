#pragma once
#include <cstdint>

namespace rex::input {
struct PcMenuResult { uint16_t guest_buttons; bool open_menu; };
class PcMenuInput {
 public:
  PcMenuResult Update(uint16_t buttons, bool gameplay_or_host_menu, uint16_t chord, bool callback_available) {
    constexpr uint16_t start = 0x0010;
    const bool start_down = (buttons & start) != 0;
    const bool chord_down = chord != 0 && (buttons & chord) == chord;
    const bool open = !baseline_ && callback_available &&
        ((chord_down && !chord_down_) ||
         (gameplay_or_host_menu && start_down && !start_down_));
    start_down_ = start_down;
    chord_down_ = chord_down;
    baseline_ = false;
    // Consume immediately, before the deferred UI callback. Otherwise the
    // retail pause menu can open in the same frame underneath the PC menu.
    if (callback_available && (chord_down || (gameplay_or_host_menu && start_down))) buttons = 0;
    return {buttons, open};
  }
  void Reset() { start_down_ = chord_down_ = false; baseline_ = true; }
 private:
  bool start_down_ = false;
  bool chord_down_ = false;
  bool baseline_ = false;
};
}
