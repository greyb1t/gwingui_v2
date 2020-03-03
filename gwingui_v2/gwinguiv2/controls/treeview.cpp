#include "pch.h"
#include "treeview.h"
#include "control.h"

namespace gwingui {

namespace Treeview {

HTREEITEM AddItemRoot( const HWND treeview_handle, std::wstring rootName ) {
  if ( !treeview_handle )
    throw std::runtime_error( "invalid treeview handle" );

  return _AddItem( treeview_handle, rootName, NULL );
}

HTREEITEM AddItemChild( const HWND treeview_handle,
                        HTREEITEM parentHandle,
                        std::wstring childName ) {
  if ( !treeview_handle )
    throw std::runtime_error( "invalid treeview handle" );

  TVITEM tvi = { 0 };
  tvi = GetItemAttr( treeview_handle, parentHandle );
  tvi.cChildren = 1;

  SetItemAttr( treeview_handle, tvi, TVIF_TEXT | TVIF_CHILDREN, 1 );
  delete[] tvi.pszText;
  return _AddItem( treeview_handle, childName, parentHandle );
}

HTREEITEM AddItemSelectedChild( const HWND treeview_handle,
                                std::wstring childName ) {
  HTREEITEM selectedHandle = GetSelectedHandle( treeview_handle );

  TVITEM tvi = { 0 };
  tvi = GetItemAttr( treeview_handle, selectedHandle );
  tvi.cChildren = 1;

  SetItemAttr( treeview_handle, tvi, TVIF_TEXT | TVIF_CHILDREN, 1 );

  delete[] tvi.pszText;

  return _AddItem( treeview_handle, childName, selectedHandle );
}

void DeleteItem( const HWND treeview_handle, HTREEITEM itemHandle ) {
  if ( !treeview_handle )
    throw std::runtime_error( "invalid treeview handle" );

  LRESULT deleteTreeViewItem =
      SendMessage( treeview_handle, TVM_DELETEITEM, 0, ( LPARAM )itemHandle );
}

void DeleteAll( const HWND treeview_handle ) {
  DeleteItem( treeview_handle, NULL );
}

void Expand( const HWND treeview_handle, HTREEITEM parentHandle ) {
  if ( !treeview_handle )
    throw std::runtime_error( "invalid treeview handle" );

  SendMessage( treeview_handle, TVM_EXPAND, TVE_EXPAND,
               ( LPARAM )parentHandle );
}

void Collapse( const HWND treeview_handle, HTREEITEM parentHandle ) {
  if ( !treeview_handle )
    throw std::runtime_error( "invalid treeview handle" );

  SendMessage( treeview_handle, TVM_EXPAND, TVE_COLLAPSE,
               ( LPARAM )parentHandle );
}

// return -1 if not found
int FindString( const HWND treeview_handle, std::wstring findText ) {
  if ( !treeview_handle )
    throw std::runtime_error( "invalid treeview handle" );

  HTREEITEM topItem = TreeView_GetFirstVisible( treeview_handle );
  TVITEM tvi = { 0 };
  bool foundIt = false;

  int itemCount = GetItemCount( treeview_handle );

  for ( int i = 0; i < itemCount; ++i ) {
    tvi = GetItemAttr( treeview_handle, topItem );

    if ( tvi.cChildren == 1 ) {
      topItem = TreeView_GetChild( treeview_handle, topItem );
    } else {
      topItem = TreeView_GetNextVisible( treeview_handle, topItem );
    }

    if ( wcscmp( findText.c_str(), tvi.pszText ) == 0 ) {
      MakeVisible( treeview_handle, tvi.hItem );
      SetSelectedItem( treeview_handle, tvi.hItem );
      foundIt = true;
    }
  }

  delete[] tvi.pszText;

  if ( !foundIt )
    return -1;

  return 0;
}

std::wstring GetSelectedText( const HWND treeview_handle ) {
  HTREEITEM selectedItem = GetSelectedHandle( treeview_handle );
  TVITEM tvi = GetItemAttr( treeview_handle, selectedItem );

  std::wstring str = tvi.pszText;
  delete[] tvi.pszText;
  return str;
}

void MakeVisible( const HWND treeview_handle, HTREEITEM itemHandle ) {
  if ( !treeview_handle )
    throw std::runtime_error( "invalid treeview handle" );

  SendMessage( treeview_handle, TVM_ENSUREVISIBLE, 0, ( LPARAM )itemHandle );
}

bool HasChildren( const HWND treeview_handle ) {
  HTREEITEM selectedItem = GetSelectedHandle( treeview_handle );
  TVITEM tvi = GetItemAttr( treeview_handle, selectedItem );

  delete[] tvi.pszText;

  if ( tvi.cChildren == 1 )
    return true;

  return false;
}

int GetItemCount( const HWND treeview_handle ) {
  if ( !treeview_handle )
    throw std::runtime_error( "invalid treeview handle" );

  LRESULT count = SendMessage( treeview_handle, TVM_GETCOUNT, 0, 0 );
  return ( int )count;
}

void SetSelectedItem( const HWND treeview_handle, HTREEITEM itemHandle ) {
  if ( !treeview_handle )
    throw std::runtime_error( "invalid treeview handle" );

  SendMessage( treeview_handle, TVM_SELECTITEM, TVGN_CARET,
               ( LPARAM )itemHandle );
}

HTREEITEM GetSelectedHandle( const HWND treeview_handle ) {
  if ( !treeview_handle )
    throw std::runtime_error( "invalid treeview handle" );

  LRESULT handle = SendMessage( treeview_handle, TVM_GETNEXTITEM, TVGN_CARET,
                                ( LPARAM )treeview_handle );

  return ( HTREEITEM )handle;
}

// don't forget to delete[] tvi.pszText
TVITEM GetItemAttr( const HWND treeview_handle, HTREEITEM itemHandle ) {
  if ( !treeview_handle )
    throw std::runtime_error( "invalid treeview handle" );

  wchar_t* buffer = new wchar_t[ 1024 ];
  memset( buffer, 0, sizeof( buffer ) );

  TVITEM tvi = { 0 };
  tvi.hItem = itemHandle;
  tvi.mask = TVIF_TEXT | TVIF_CHILDREN;
  tvi.pszText = buffer;
  tvi.cchTextMax = 15;

  SendMessage( treeview_handle, TVM_GETITEM, NULL, ( LPARAM )&tvi );

  return tvi;
}

void SetItemAttr( const HWND treeview_handle,
                  TVITEM& tvi,
                  uint32_t mask,
                  int cChildren ) {
  if ( !treeview_handle )
    throw std::runtime_error( "invalid treeview handle" );

  tvi.mask = mask;
  tvi.cChildren = cChildren;

  SendMessage( treeview_handle, TVM_SETITEM, 0, ( LPARAM )&tvi );
}

HTREEITEM _AddItem( const HWND treeview_handle,
                    std::wstring text,
                    HTREEITEM parentHandle ) {
  if ( !treeview_handle )
    throw std::runtime_error( "invalid treeview handle" );

  TVINSERTSTRUCT tvmis = { 0 };
  TVITEM tvi = { 0 };

  tvi.mask = TVIF_TEXT | TVIF_CHILDREN;
  tvi.hItem = 0;
  tvi.pszText = ( LPWSTR )text.c_str();
  tvi.cchTextMax = sizeof( text );
  tvi.cChildren = 0;

  tvmis.hParent = parentHandle;
  tvmis.hInsertAfter = TVI_LAST;
  tvmis.item = tvi;

  LRESULT newItem =
      SendMessage( treeview_handle, TVM_INSERTITEM, 0, ( LPARAM )&tvmis );

  return ( HTREEITEM )newItem;
}

}  // namespace Treeview

}  // namespace gwingui