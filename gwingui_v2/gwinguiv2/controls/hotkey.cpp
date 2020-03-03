#include "pch.h"
#include "hotkey.h"

uint32_t gwingui::hotkey::GetCurrentHotkey( const HWND hotkey_handle,
                                            uint32_t* hotkey_modifiers ) {
  if ( !hotkey_handle )
    throw std::runtime_error( "invalid hotkey control handle" );

  const auto result = SendMessage( hotkey_handle, HKM_GETHOTKEY, 0, 0 );
  const auto low_result = LOWORD( result );

  const auto virtual_key_code = LOBYTE( low_result );

  const auto modifiers = HIBYTE( low_result );

  if ( hotkey_modifiers )
    *hotkey_modifiers = modifiers;

  return virtual_key_code;
}

void gwingui::hotkey::SetCurrentHotkey( const HWND hotkey_handle,
                                        uint32_t virtual_keycode,
                                        uint32_t hotkey_modifiers ) {
  if ( !hotkey_handle )
    throw std::runtime_error( "invalid hotkey control handle" );

  const auto wparam =
      MAKEWPARAM( LOBYTE( virtual_keycode ) | HIBYTE( hotkey_modifiers ), 0 );

  SendMessage( hotkey_handle, HKM_SETHOTKEY, wparam, 0 );
}

std::wstring gwingui::hotkey::VirtualKeycodeToString(
    const uint32_t virtual_keycode ) {
  const LPARAM repeatCount = 1;
  const LPARAM scanCode = MapVirtualKey( virtual_keycode, MAPVK_VK_TO_VSC );
  const LPARAM extended = 0;  // fExtended (spy++)
  const LPARAM context = 0;  // fAltDown (spy++)
  const LPARAM previousState = 0;  // fRepeat (spy++)
  const LPARAM transition = 0;  // fUp (spy++)

  const LPARAM key_data = repeatCount | ( scanCode << 16 ) |
                          ( extended << 24 ) | ( context << 29 ) |
                          ( previousState << 30 ) | ( transition << 31 );

  wchar_t buf[ 255 ] = { 0 };
  if ( GetKeyNameText( static_cast<LONG>( key_data ), buf, 255 ) ) {
    return buf;
  } else {
    return TEXT( "Unknown key" );
  }

  throw std::runtime_error( "Should never occur" );
}
