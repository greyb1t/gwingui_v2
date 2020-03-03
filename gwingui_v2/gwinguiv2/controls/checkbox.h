#pragma once

#include <Windows.h>

namespace gwingui {

namespace checkbox {

// IsChecked returns true of false based on whether the checkbox is checked or
// not
bool IsChecked( const HWND checkbox_handle );

// SetCheck modifes the state of the checkbox to either unchecked or checked
void SetCheck( const HWND checkbox_handle, const bool value );

}  // namespace checkbox

}  // namespace gwingui