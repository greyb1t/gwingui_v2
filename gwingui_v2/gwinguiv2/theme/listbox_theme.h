#pragma once

constexpr auto ODA_HOVER = 0x0008;

namespace gwingui {

struct ListboxThemeData {
  ListboxThemeData()
      : prev_selected_item_index( -1 ),
        prev_hover_item_index( -1 ),
        has_control_status_changed( false ) {}

  int32_t prev_selected_item_index;
  int32_t prev_hover_item_index;
  bool has_control_status_changed;
};

namespace listboxtheme {

void InitializeListbox( HWND listbox_handle );

void OnOwnerDraw( const DRAWITEMSTRUCT* dis );

}  // namespace listboxtheme

}  // namespace gwingui