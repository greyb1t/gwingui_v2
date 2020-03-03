#include "pch.h"
#include "message_box.h"

std::wstring GetLastErrorString() {
  wchar_t buf[ 256 ] = { 0 };
  constexpr auto kMaxLength = sizeof( buf ) / sizeof( buf[ 0 ] );

  FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(),
                 MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), buf, kMaxLength,
                 NULL );

  std::transform( buf, buf + kMaxLength, buf, []( wchar_t d ) -> wchar_t {
    if ( d == '\n' || d == '\r' )
      return '\0';

    return d;
  } );

  return buf;
}

void gwingui::messagebox::Error( const std::wstring& message,
                                 const std::wstring& title,
                                 const HWND parent_handle ) {
  std::wstring error_message = message + TEXT( "\n\nLast error code: " ) +
                               std::to_wstring( GetLastError() ) +
                               TEXT( "\nError message: \"" ) +
                               GetLastErrorString() + TEXT( "\"" );

  MessageBox( parent_handle, error_message.c_str(), title.c_str(),
              MB_OK | MB_ICONERROR );
}

void gwingui::messagebox::Warning( const std::wstring& message,
                                   const std::wstring& title,
                                   const HWND parent_handle ) {
  MessageBox( parent_handle, message.c_str(), title.c_str(),
              MB_OK | MB_ICONWARNING );
}
