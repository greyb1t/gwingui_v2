#include "pch.h"
#include "listbox.h"
#include "control.h"

namespace gwingui {

namespace listbox {

LRESULT AddString( const HWND listbox_handle, const std::wstring& text ) {
  if ( !listbox_handle )
    throw std::runtime_error( "invalid listbox handle" );

  const LRESULT result =
      SendMessage( listbox_handle, LB_ADDSTRING, 0, ( LPARAM )text.c_str() );

  return result;
}

LRESULT DeleteItem( const HWND listbox_handle, const uint32_t index ) {
  if ( !listbox_handle )
    throw std::runtime_error( "invalid listbox handle" );

  const LRESULT result =
      SendMessage( listbox_handle, LB_DELETESTRING, index, 0 );

  return result;
}

void SetSelectedItem( const HWND listbox_handle, const uint32_t index ) {
  if ( !listbox_handle )
    throw std::runtime_error( "invalid listbox handle" );

  SendMessage( listbox_handle, LB_SETCURSEL, index, 0 );
}

bool SetItemText( const HWND listbox_handle,
                  const uint32_t index,
                  const std::wstring& text ) {
  if ( !listbox_handle )
    throw std::runtime_error( "invalid listbox handle" );

  const LRESULT result = SendMessage( listbox_handle, LB_SETITEMDATA, index,
                                      ( LPARAM )text.c_str() );

  return result != LB_ERR;
}

std::wstring GetText( const HWND listbox_handle, const uint32_t index ) {
  if ( !listbox_handle )
    throw std::runtime_error( "invalid listbox handle" );

  const LRESULT text_length =
      SendMessage( listbox_handle, LB_GETTEXTLEN, index, 0 );

  if ( text_length < 0 )
    return TEXT( "" );

  std::wstring str_buf;
  str_buf.resize( text_length );

  SendMessage( listbox_handle, LB_GETTEXT, index,
               reinterpret_cast<LPARAM>( &str_buf[ 0 ] ) );

  return str_buf;
}

std::wstring GetSelectedString( const HWND listbox_handle ) {
  if ( !listbox_handle )
    throw std::runtime_error( "invalid listbox handle" );

  const int32_t selected_index = GetSelectedIndex( listbox_handle );

  if ( selected_index < 0 )
    return TEXT( "" );

  return GetText( listbox_handle, selected_index );
}

// Returns -1 if none is selected
LRESULT GetSelectedIndex( const HWND listbox_handle ) {
  if ( !listbox_handle )
    throw std::runtime_error( "invalid listbox handle" );

  return SendMessage( listbox_handle, LB_GETCURSEL, 0, 0 );
}

LRESULT GetCount( const HWND listbox_handle ) {
  if ( !listbox_handle )
    throw std::runtime_error( "invalid listbox handle" );

  const LRESULT count = SendMessage( listbox_handle, LB_GETCOUNT, NULL, NULL );

  return count;
}

LRESULT FindString( const HWND listbox_handle,
                    const uint32_t start_index,
                    const std::wstring& text ) {
  if ( !listbox_handle )
    throw std::runtime_error( "invalid listbox handle" );

  return SendMessage( listbox_handle, LB_FINDSTRING, start_index,
                      reinterpret_cast<LPARAM>( text.c_str() ) );
}

void ClearItems( const HWND listbox_handle ) {
  if ( !listbox_handle )
    throw std::runtime_error( "invalid listbox handle" );

  SendMessage( listbox_handle, LB_RESETCONTENT, 0, 0 );
}

}  // namespace listbox

}  // namespace gwingui