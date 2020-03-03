#include "pch.h"
#include "listview.h"

namespace gwingui {

namespace listview {

void EnableFullRowSelect( const HWND listview_handle ) {
  if ( !listview_handle )
    throw std::runtime_error( "invalid listview handle" );

  SendMessage( listview_handle, LVM_SETEXTENDEDLISTVIEWSTYLE, 0,
               LVS_EX_FULLROWSELECT );
}

bool ClearItems( const HWND listview_handle ) {
  if ( !listview_handle )
    throw std::runtime_error( "invalid listview handle" );

  LRESULT result = SendMessage( listview_handle, LVM_DELETEALLITEMS, 0, 0 );

  return !!result;
}

LRESULT GetSelectedColumn( const HWND listview_handle ) {
  if ( !listview_handle )
    throw std::runtime_error( "invalid listview handle" );

  LRESULT sel_column_index =
      SendMessage( listview_handle, LVM_GETSELECTEDCOLUMN, 0, 0 );
  return sel_column_index;
}

LRESULT GetSelectedItem( const HWND listview_handle ) {
  if ( !listview_handle )
    throw std::runtime_error( "invalid listview handle" );

  return SendMessage( listview_handle, LVM_GETNEXTITEM, ( WPARAM )-1,
                      LVNI_FOCUSED );
}

LRESULT GetItemCount( const HWND listview_handle ) {
  if ( !listview_handle )
    throw std::runtime_error( "invalid listview handle" );

  return SendMessage( listview_handle, LVM_GETITEMCOUNT, 0, 0 );
}

LRESULT GetSelectedItemCount( const HWND listview_handle ) {
  if ( !listview_handle )
    throw std::runtime_error( "invalid listview handle" );

  return SendMessage( listview_handle, LVM_GETSELECTEDCOUNT, 0, 0 );
}

LRESULT GetColumnCount( const HWND listview_handle ) {
  if ( !listview_handle )
    throw std::runtime_error( "invalid listview handle" );

  HWND header_handle = reinterpret_cast<HWND>(
      SendMessage( listview_handle, LVM_GETHEADER, 0, 0 ) );

  LRESULT count = SendMessage( header_handle, HDM_GETITEMCOUNT, 0, 0 );

  return count;
}

std::vector<std::wstring> GetItemRowText( const HWND listview_handle,
                                          const uint32_t index ) {
  if ( !listview_handle )
    throw std::runtime_error( "invalid listview handle" );

  const int32_t column_count = GetColumnCount( listview_handle );

  std::vector<std::wstring> item_row;

  // Add the items from each column
  for ( int i = 0; i < column_count; ++i )
    item_row.push_back( GetItemText( listview_handle, index, i ) );

  return std::move( item_row );
}

int AddColumn( const HWND listview_handle,
               const std::wstring& column_name,
               const uint32_t column_index ) {
  if ( !listview_handle )
    throw std::runtime_error( "invalid listview handle" );

  LVCOLUMN column = { 0 };
  column.mask = LVCF_TEXT | LVCF_WIDTH;
  column.pszText = ( LPWSTR )( column_name.c_str() );
  column.cx = 70;  // make this an argument maybe

  const LRESULT result =
      SendMessage( listview_handle, LVM_INSERTCOLUMN, column_index,
                   reinterpret_cast<LPARAM>( &column ) );

  return result;
}

int AddItem( const HWND listview_handle,
             const std::wstring& name,
             const uint32_t index ) {
  if ( !listview_handle )
    throw std::runtime_error( "invalid listview handle" );

  LVITEM item = { 0 };
  item.mask = LVIF_TEXT;
  item.cchTextMax = name.size();
  item.iItem = index;
  item.pszText = ( LPWSTR )( name.c_str() );

  LRESULT result = SendMessage( listview_handle, LVM_INSERTITEM, 0,
                                reinterpret_cast<LPARAM>( &item ) );

  return result;
}

bool AddSubitem( const HWND listview_handle,
                 const std::wstring& name,
                 const uint32_t item_index,
                 const uint32_t subitem_index ) {
  if ( !listview_handle )
    throw std::runtime_error( "invalid listview handle" );

  LVITEM lvi = { 0 };
  lvi.mask = LVIF_TEXT;
  lvi.iItem = item_index;
  lvi.iSubItem = subitem_index - 1;
  lvi.pszText = ( LPWSTR )( name.c_str() );

  LRESULT result = SendMessage( listview_handle, LVM_SETITEM, 0,
                                reinterpret_cast<LPARAM>( &lvi ) );

  return !!result;
}

bool SetItemText( const HWND listview_handle,
                  const uint32_t index,
                  const uint32_t subitem_index,
                  const std::wstring& text ) {
  if ( !listview_handle )
    throw std::runtime_error( "invalid listview handle" );

  LVITEM lvi = { 0 };
  lvi.mask = LVIF_TEXT;
  lvi.iItem = index;
  lvi.iSubItem = subitem_index;
  lvi.pszText = ( LPWSTR )( text.c_str() );

  const LRESULT result = SendMessage( listview_handle, LVM_SETITEM, 0,
                                      reinterpret_cast<LPARAM>( &lvi ) );

  return !!result;
}

bool SetSelectedItemText( const HWND listview_handle,
                          const std::wstring& text ) {
  if ( !listview_handle )
    throw std::runtime_error( "invalid listview handle" );

  return SetItemText( listview_handle, GetSelectedItem( listview_handle ), 0,
                      text );
}

void SetSelectedItem( const HWND listview_handle, const uint32_t index ) {
  if ( !listview_handle )
    throw std::runtime_error( "invalid listview handle" );

  ListView_SetItemState( listview_handle, index, LVIS_SELECTED | LVIS_FOCUSED,
                         LVIS_SELECTED | LVIS_FOCUSED );
}

bool RemoveItem( const HWND listview_handle, const uint32_t index ) {
  if ( !listview_handle )
    throw std::runtime_error( "invalid listview handle" );

  const LRESULT result =
      SendMessage( listview_handle, LVM_DELETEITEM, index, 0 );

  return !!result;
}

bool RemoveSelectedItem( const HWND listview_handle ) {
  if ( !listview_handle )
    throw std::runtime_error( "invalid listview handle" );

  const int32_t selected_index = GetSelectedItem( listview_handle );
  return RemoveItem( listview_handle, selected_index );
}

std::wstring GetItemText( const HWND listview_handle,
                          int index,
                          int subItemIndex ) {
  if ( !listview_handle )
    throw std::runtime_error( "invalid listview handle" );

  wchar_t buf[ MAX_PATH ] = { 0 };

  ListView_GetItemText( listview_handle, index, subItemIndex, buf, MAX_PATH );

  return buf;
}

std::vector<std::wstring> GetSelectedItemRowText( const HWND listview_handle ) {
  if ( !listview_handle )
    throw std::runtime_error( "invalid listview handle" );

  const int32_t selected_index = GetSelectedItem( listview_handle );
  return std::move( GetItemRowText( listview_handle, selected_index ) );
}

int FindItem( const HWND listview_handle, const std::wstring& name ) {
  if ( !listview_handle )
    throw std::runtime_error( "invalid listview handle" );

  LVFINDINFO lvfind = { 0 };
  lvfind.flags = LVFI_PARTIAL | LVFI_STRING | LVFI_SUBSTRING;
  lvfind.psz = name.c_str();

  const LRESULT result =
      SendMessage( listview_handle, LVM_FINDITEM, ( WPARAM )-1,
                   reinterpret_cast<LPARAM>( &lvfind ) );

  if ( result < 0 )
    return result;

  SendMessage( listview_handle, LVM_ENSUREVISIBLE, result, FALSE );

  SetSelectedItem( listview_handle, result );

  return result;
}

bool SetColumnWidthAutoSize( const HWND listview_handle,
                             const uint32_t column_index ) {
  if ( !listview_handle )
    throw std::runtime_error( "invalid listview handle" );

  const LRESULT result = SendMessage( listview_handle, LVM_SETCOLUMNWIDTH,
                                      column_index, LVSCW_AUTOSIZE_USEHEADER );

  return !!result;
}

bool SetColumnWidth( const HWND listview_handle,
                     const uint32_t index,
                     const uint32_t width ) {
  if ( !listview_handle )
    throw std::runtime_error( "invalid listview handle" );

  const LRESULT result =
      SendMessage( listview_handle, LVM_SETCOLUMNWIDTH, index, width );

  return !!result;
}

uint32_t GetColumnWidth( const HWND listview_handle, const uint32_t index ) {
  if ( !listview_handle )
    throw std::runtime_error( "invalid listview handle" );

  const LRESULT result =
      SendMessage( listview_handle, LVM_GETCOLUMNWIDTH, index, 0 );

  return static_cast<uint32_t>( result );
}

}  // namespace listview

}  // namespace gwingui