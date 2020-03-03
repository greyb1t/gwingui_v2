#pragma once

#include <string>
#include <Windows.h>

namespace gwingui {

namespace Treeview {

/*
  Example Usage:

  HTREEITEM root1 = addItemRoot(hDlg, itemID, TEXT("Sweden"));

  addItemChild(hDlg, itemID, root1, TEXT("Stockholm"));
  addItemChild(hDlg, itemID, root1, TEXT("Malmö"));

  HTREEITEM root2 = addItemRoot(hDlg, itemID, TEXT("Poland"));
  HTREEITEM child1 = addItemChild(hDlg, itemID, root2, TEXT("Warschaw"));
  addItemChild(hDlg, itemID, child1, TEXT("Next under warshav"));
*/

HTREEITEM AddItemRoot( const HWND treeview_handle, std::wstring rootName );
HTREEITEM AddItemChild( const HWND treeview_handle,
                        HTREEITEM parentHandle,
                        std::wstring childName );
HTREEITEM AddItemSelectedChild( const HWND treeview_handle,
                                std::wstring childName );

void DeleteItem( const HWND treeview_handle, HTREEITEM itemHandle );
void DeleteAll( const HWND treeview_handle );

void Expand( const HWND treeview_handle, HTREEITEM parentHandle );
void Collapse( const HWND treeview_handle, HTREEITEM parentHandle );

int FindString( const HWND treeview_handle, std::wstring findText );

std::wstring GetSelectedText( const HWND treeview_handle );

void MakeVisible( const HWND treeview_handle, HTREEITEM itemHandle );
bool HasChildren( const HWND treeview_handle );
int GetItemCount( const HWND treeview_handle );

void SetSelectedItem( const HWND treeview_handle, HTREEITEM itemHandle );
HTREEITEM GetSelectedHandle( const HWND treeview_handle );

TVITEM GetItemAttr( const HWND treeview_handle, HTREEITEM itemHandle );
void SetItemAttr( const HWND treeview_handle,
                  TVITEM& tvi,
                  uint32_t mask,
                  int cChildren );

HTREEITEM _AddItem( const HWND treeview_handle,
                    std::wstring text,
                    HTREEITEM parentHandle );

}  // namespace Treeview

}  // namespace gwingui