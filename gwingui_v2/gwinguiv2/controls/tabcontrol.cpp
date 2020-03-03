#include "pch.h"
#include "tabcontrol.h"
#include "control.h"

namespace gwingui {

namespace tabcontrol {

LRESULT AddTab( const HWND tabcontrol_handle,
                const std::wstring& tab_name,
                const uint32_t index ) {
  if ( !tabcontrol_handle )
    throw std::runtime_error( "invalid tab control handle" );

  TCITEM tci = { 0 };
  tci.mask = TCIF_TEXT;
  tci.pszText = ( LPWSTR )tab_name.c_str();
  tci.cchTextMax = static_cast<int>( tab_name.size() ) * sizeof( wchar_t );

  const LRESULT result = SendMessage( tabcontrol_handle, TCM_INSERTITEM, index,
                                      reinterpret_cast<LPARAM>( &tci ) );

  return result;
}

void SetTabItemSize( const HWND tabcontrol_handle,
                     const uint32_t width,
                     const uint32_t height ) {
  if ( !tabcontrol_handle )
    throw std::runtime_error( "invalid tab control handle" );

  SendMessage( tabcontrol_handle, TCM_SETITEMSIZE, 0,
               MAKELPARAM( width, height ) );
}

bool DeleteAllItems( const HWND tabcontrol_handle ) {
  if ( !tabcontrol_handle )
    throw std::runtime_error( "invalid tab control handle" );

  LRESULT retVal = SendMessage( tabcontrol_handle, TCM_DELETEALLITEMS, 0, 0 );

  return !!retVal;
}

bool DeleteItem( const HWND tabcontrol_handle, const uint32_t index ) {
  if ( !tabcontrol_handle )
    throw std::runtime_error( "invalid tab control handle" );

  const LRESULT result =
      SendMessage( tabcontrol_handle, TCM_DELETEITEM, index, 0 );

  return !!result;
}

LRESULT GetSelectedTab( const HWND tabcontrol_handle ) {
  if ( !tabcontrol_handle )
    throw std::runtime_error( "invalid tab control handle" );

  return SendMessage( tabcontrol_handle, TCM_GETCURFOCUS, 0, 0 );
}

LRESULT GetTabCount( const HWND tabcontrol_handle ) {
  if ( !tabcontrol_handle )
    throw std::runtime_error( "invalid tab control handle" );

  return SendMessage( tabcontrol_handle, TCM_GETITEMCOUNT, 0, 0 );
}

void SetSelected( const HWND tabcontrol_handle, int index ) {
  if ( !tabcontrol_handle )
    throw std::runtime_error( "invalid tab control handle" );

  SendMessage( tabcontrol_handle, TCM_SETCURFOCUS, index, 0 );
}

void GetTabItemRect( const HWND tabcontrol_handle,
                     const uint32_t index,
                     RECT* rect ) {
  if ( !tabcontrol_handle )
    throw std::runtime_error( "invalid tab control handle" );

  SendMessage( tabcontrol_handle, TCM_GETITEMRECT, index,
               reinterpret_cast<LPARAM>( rect ) );
}

std::wstring GetTabItemText( const HWND tabcontrol_handle,
                             const uint32_t index ) {
  if ( !tabcontrol_handle )
    throw std::runtime_error( "invalid tab control handle" );

  wchar_t buf[ MAX_PATH ] = { 0 };

  TCITEM tci = { 0 };
  tci.mask = TCIF_TEXT;
  tci.cchTextMax = sizeof( buf );
  tci.pszText = reinterpret_cast<LPWSTR>( &buf );

  SendMessage( tabcontrol_handle, TCM_GETITEM, index,
               reinterpret_cast<LPARAM>( &tci ) );

  return buf;
}

}  // namespace tabcontrol

}  // namespace gwingui