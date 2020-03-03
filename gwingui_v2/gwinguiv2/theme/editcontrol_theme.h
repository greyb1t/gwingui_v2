#pragma once

#include <windows.h>
#include <stdint.h>

namespace gwingui {

namespace editcontroltheme {

void InitializeEditControl( const HWND edit_control_handle );

void DrawTextPart( const HDC hdc, const HWND edit_control_handle );

}

}