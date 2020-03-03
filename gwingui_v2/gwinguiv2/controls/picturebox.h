#pragma once

#include <Windows.h>
#include <string>

namespace gwingui {

namespace picturecontrol {

// GetIcon returns a handle to the icon in the specified picture box
HICON GetIcon( const HWND picture_control_handle );

// SetIcon sets an icon for a picture control from a path
// returns false if failed
bool SetIcon( const HWND picture_control_handle,
              const std::wstring& icon_path );

// SetIcon sets an icon for a picture control from an id
// returns false if failed
bool SetIcon( const HWND picture_control_handle, const HINSTANCE instance, const WORD icon_id );

// SetIcon sets an icon for a picture control from a handle
// returns false if failed
void SetIcon( const HWND picture_control_handle, const HICON icon_handle );

// SetBitmap sets a bitmap for a picture control from handle
void SetBitmap( const HWND picture_control_handle,
                const HBITMAP bitmap_handle );

}  // namespace picturecontrol

}  // namespace gwingui