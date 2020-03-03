#include "pch.h"
#include "button.h"
#include "control.h"

namespace gwingui {

namespace button {

bool SetImage( const HWND button_handle,
               const ButtonImageType image_type,
               const HINSTANCE instance,
               const WORD image_id ) {
  if ( !button_handle )
    throw std::runtime_error( "Invalid button handle" );

  WPARAM image_type_native;

  switch ( image_type ) {
    case ButtonImageType::Icon:
      image_type_native = IMAGE_ICON;
      break;

    case ButtonImageType::Bitmap:
      image_type_native = IMAGE_BITMAP;
      break;

    default:
      throw std::runtime_error( "Unknown image bitmap" );
      break;
  }

  const HICON icon_handle = LoadIcon( instance, MAKEINTRESOURCE( image_id ) );

  if ( !icon_handle )
    return false;

  return !!SendMessage( button_handle, BM_SETIMAGE, image_type_native,
                        reinterpret_cast<LPARAM>( icon_handle ) );
}

}  // namespace button

}  // namespace gwingui