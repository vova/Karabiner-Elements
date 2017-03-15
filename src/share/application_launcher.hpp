#pragma once

#include <cstdlib>

namespace krbn {
class application_launcher final {
public:
  static void launch_event_viewer(void) {
    system("open '/Applications/Karabiner-EventViewer.app'");
  }

  static void launch_menu(void) {
    system("open -g '/Library/Application Support/org.pqrs/Karabiner-Elements/Karabiner-Menu.app'");
  }

  static void launch_preferences(void) {
    system("open '/Applications/Karabiner-Elements.app'");
  }
};
}
