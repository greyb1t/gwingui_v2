#pragma once

#include <Windows.h>

namespace gwingui {

enum class ButtonImageType { Icon, Bitmap };

namespace button {

// To display both icon and text in button, make sure to have BS_ICON or
// BS_BITMAP not set.

// Returns false if failed
bool SetImage( const HWND button_handle,
               const ButtonImageType image_type,
  const HINSTANCE instance,
               const WORD image_id );

}  // namespace button

}  // namespace gwingui