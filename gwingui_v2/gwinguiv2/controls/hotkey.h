#pragma once

namespace gwingui {

namespace hotkey {

/*
  Returns virtual keycode, 0 if no key selected
  hotkey_modifiers is a combo of
  HOTKEYF_ALT, HOTKEYF_CONTROL, HOTKEYF_EXT, HOTKEYF_SHIFT
*/
uint32_t GetCurrentHotkey( const HWND hotkey_handle,
                           uint32_t* hotkey_modifiers = nullptr );

void SetCurrentHotkey( const HWND hotkey_handle,
                       uint32_t virtual_keycode,
                       uint32_t hotkey_modifiers = 0 );

std::wstring VirtualKeycodeToString( const uint32_t virtual_keycode );

}  // namespace hotkey

}  // namespace gwingui