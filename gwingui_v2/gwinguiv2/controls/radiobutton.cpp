#include "pch.h"
#include "radiobutton.h"
#include "checkbox.h"

namespace gwingui {

namespace radiobutton {

bool IsChecked( const HWND radiobutton_handle ) {
  return checkbox::IsChecked( radiobutton_handle );
}

void SetCheck( const HWND radiobutton_handle, const bool value ) {
  checkbox::SetCheck( radiobutton_handle, value );
}

}  // namespace radiobutton

}  // namespace gwingui