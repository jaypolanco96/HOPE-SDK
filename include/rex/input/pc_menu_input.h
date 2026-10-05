#pragma once
#include <cstdint>

namespace rex::input {
struct PcMenuResult { uint16_t guest_buttons; bool open_menu; };
class PcMenuInput {
 public:
  PcMenuResult Update(uint16_t buttons, uint16_t chord, bool callback_available) {
    const bool chord_down = chord != 0 && (buttons & chord) == chord;
    const bool open = !baseline_ && callback_available &&
        chord_down && !chord_down_;
    chord_down_ = chord_down;
    baseline_ = false;
    // Consume immediately, before the deferred UI callback. Otherwise the
    // retail pause menu can open in the same frame underneath the PC menu.
    if (callback_available && chord_down) buttons = 0;
    return {buttons, open};
  }
  void Reset() { chord_down_ = false; baseline_ = true; }
 private:
  bool chord_down_ = false;
  bool baseline_ = false;
};
}
