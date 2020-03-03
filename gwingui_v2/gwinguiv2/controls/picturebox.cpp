#include "pch.h"
#include "picturebox.h"
#include "control.h"

namespace gwingui {

namespace picturecontrol {

HICON GetIcon( const HWND picture_control_handle ) {
  if ( !picture_control_handle )
    throw std::runtime_error( "invalid picture control handle" );

  return reinterpret_cast<HICON>(
      SendMessage( picture_control_handle, STM_GETIMAGE, IMAGE_ICON, 0 ) );
}

bool SetIcon( const HWND picture_control_handle,
              const std::wstring& icon_path ) {
  if ( !picture_control_handle )
    throw std::runtime_error( "invalid picture control handle" );

  const HICON icon_handle = reinterpret_cast<HICON>(
      LoadImage( NULL, icon_path.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE ) );

  if ( !icon_handle )
    return false;

  SetIcon( picture_control_handle, icon_handle );

  return true;
}

bool SetIcon( const HWND picture_control_handle,
              const HINSTANCE instance,
              const WORD icon_id ) {
  if ( !picture_control_handle )
    throw std::runtime_error( "invalid picture control handle" );

  const HICON icon_handle = LoadIcon( instance, MAKEINTRESOURCE( icon_id ) );

  if ( !icon_handle )
    return false;

  SetIcon( picture_control_handle, icon_handle );

  return true;
}

void SetIcon( const HWND picture_control_handle, const HICON icon_handle ) {
  if ( !picture_control_handle )
    throw std::runtime_error( "invalid picture control handle" );

  SendMessage( picture_control_handle, STM_SETIMAGE, IMAGE_ICON,
               reinterpret_cast<LPARAM>( icon_handle ) );
}

void SetBitmap( const HWND picture_control_handle,
                const HBITMAP bitmap_handle ) {
  if ( !picture_control_handle )
    throw std::runtime_error( "invalid picture control handle" );

  SendMessage( picture_control_handle, STM_SETIMAGE, IMAGE_BITMAP,
               reinterpret_cast<LPARAM>( bitmap_handle ) );
}

}  // namespace picturecontrol

}  // namespace gwingui