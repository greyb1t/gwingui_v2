#pragma once

#include <Windows.h>

namespace gwingui {

namespace radiobutton {

// IsChecked returns true of false based on whether the radio button is checked
// or not
bool IsChecked( const HWND radiobutton_handle );

// SetCheck modifies the state of a radiobutton
void SetCheck( const HWND radiobutton_handle, const bool value );

}  // namespace radiobutton

}  // namespace gwingui