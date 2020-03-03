#pragma once

namespace gwingui {

namespace tabcontroltheme {

// TODO: Add support to disable the tab control

struct TabControlData {
  HWND prev_window_handle;
  int32_t hovering_index;  // if -1, does mean no tab item is being hovered
};

static WNDPROC default_tabcontrol_proc_;

void Initialize( const HWND tabcontrol_handle );

LRESULT CALLBACK TabControlThemeProc( HWND tabcontrol_handle,
                                      uint32_t message,
                                      WPARAM wparam,
                                      LPARAM lparam );

}  // namespace tabcontroltheme

}  // namespace gwingui