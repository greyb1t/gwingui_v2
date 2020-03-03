#include "pch.h"
#include "combobox.h"

namespace gwingui {

namespace combobox {

LRESULT AddString( const HWND combobox_handle, const std::wstring& text ) {
  if ( !combobox_handle )
    throw std::runtime_error( "Invalid combobox handle" );

  // Receives zero-based index, CB_ERR if failed or CB_ERRSPACE if insufficient
  // space
  const LRESULT result =
      SendMessage( combobox_handle, CB_ADDSTRING, 0,
                   reinterpret_cast<LPARAM>( text.c_str() ) );

  return result;
}

LRESULT DeleteItem( const HWND combobox_handle, const uint32_t index ) {
  if ( !combobox_handle )
    throw std::runtime_error( "Invalid combobox handle" );

  const LRESULT result =
      SendMessage( combobox_handle, CB_DELETESTRING, index, 0 );

  return result;
}

LRESULT SetSelectedIndex( const HWND combobox_handle, const uint32_t index ) {
  if ( !combobox_handle )
    throw std::runtime_error( "Invalid combobox handle" );

  const LRESULT result = SendMessage( combobox_handle, CB_SETCURSEL, index, 0 );

  return result;
}

LRESULT GetSelectedIndex( const HWND combobox_handle ) {
  if ( !combobox_handle )
    throw std::runtime_error( "Invalid combobox handle" );

  return SendMessage( combobox_handle, CB_GETCURSEL, 0, 0 );
}

LRESULT SetItemText( const HWND combobox_handle,
                     const uint32_t index,
                     const std::wstring& text ) {
  if ( !combobox_handle )
    throw std::runtime_error( "Invalid combobox handle" );

  const LRESULT result =
      SendMessage( combobox_handle, CB_SETITEMDATA, index,
                   reinterpret_cast<LPARAM>( text.c_str() ) );

  return result;
}

std::wstring GetString( const HWND combobox_handle, const uint32_t index ) {
  if ( !combobox_handle )
    throw std::runtime_error( "Invalid combobox handle" );

  if ( index < 0 )
    throw std::runtime_error( "Invalid index" );

  const LRESULT text_length =
      SendMessage( combobox_handle, CB_GETLBTEXTLEN, index, 0 );

  if ( text_length < 0 )
    return TEXT( "" );

  std::wstring str_buf;
  str_buf.resize( text_length );

  SendMessage( combobox_handle, CB_GETLBTEXT, index,
               reinterpret_cast<LPARAM>( &str_buf[ 0 ] ) );

  return str_buf;
}

std::wstring GetSelectedString( const HWND combobox_handle ) {
  if ( !combobox_handle )
    throw std::runtime_error( "Invalid combobox handle" );

  const auto sel_index = GetSelectedIndex( combobox_handle );

  if ( sel_index == CB_ERR )
    return std::wstring();

  return GetString( combobox_handle, sel_index );
}

LRESULT GetCount( const HWND combobox_handle ) {
  if ( !combobox_handle )
    throw std::runtime_error( "Invalid combobox handle" );

  const LRESULT count = SendMessage( combobox_handle, CB_GETCOUNT, 0, 0 );

  return count;
}

LRESULT FindString( const HWND combobox_handle,
                    const uint32_t startIndex,
                    const std::wstring& text ) {
  if ( !combobox_handle )
    throw std::runtime_error( "Invalid combobox handle" );

  const LRESULT result =
      SendMessage( combobox_handle, CB_FINDSTRING, startIndex,
                   reinterpret_cast<LPARAM>( text.c_str() ) );

  return result;
}

LRESULT SetItemHeight( const HWND combobox_handle,
                       const uint32_t height,
                       const uint32_t index ) {
  if ( !combobox_handle )
    throw std::runtime_error( "Invalid combobox handle" );

  const LRESULT result =
      SendMessage( combobox_handle, CB_SETITEMHEIGHT, index, height );

  return result;
}

LRESULT GetItemHeight( const HWND combobox_handle, const uint32_t index ) {
  if ( !combobox_handle )
    throw std::runtime_error( "Invalid combobox handle" );

  const LRESULT result =
      SendMessage( combobox_handle, CB_GETITEMHEIGHT, index, 0 );

  return result;
}

}  // namespace combobox

}  // namespace gwingui