#pragma once

#include <Windows.h>
#include <vector>
#include <string>

namespace gwingui {

namespace listview {

// EnableFullRowSelect enables full row selection
void EnableFullRowSelect( const HWND listview_handle );

// ClearItems clears the listview of all items
// returns false if failed
bool ClearItems( const HWND listview_handle );

// GetSelectedColumn returns an index of the selected column
LRESULT GetSelectedColumn( const HWND listview_handle );

// GetSelectedItem returns an index of the selected item
// if no selection, -1
LRESULT GetSelectedItem( const HWND listview_handle );

// GetItemCount returns the total count of items in the listbox
LRESULT GetItemCount( const HWND listview_handle );

// GetSelectedItemCount returns the amount of selected items
LRESULT GetSelectedItemCount( const HWND listview_handle );

// GetColumnCount gets the total count of columns
LRESULT GetColumnCount( const HWND listview_handle );

// GetItemRowText returns a vector containing the strings of each item in a row
std::vector<std::wstring> GetItemRowText( const HWND listview_handle,
                                          const uint32_t index );

// AddColumn adds a new column to the listview
// returns index of new column, -1 if failed
int AddColumn( const HWND listview_handle,
               const std::wstring& column_name,
               const uint32_t column_index );

// AddItem adds an item to the listview
// returns the index of new item, -1 if failed
int AddItem( const HWND listview_handle,
             const std::wstring& name,
             const uint32_t index );

// AddSubitem adds a subitem to an already existing item
// returns false if failed
bool AddSubitem( const HWND listview_handle,
                 const std::wstring& name,
                 const uint32_t item_index,
                 const uint32_t subitem_index );

// SetItemText sets the text of an item or subitem
// returns false if failed
bool SetItemText( const HWND listview_handle,
                  const uint32_t index,
                  const uint32_t subitem_index,
                  const std::wstring& text );

// SetSelectedItemText sets the text of an item that is currently selected
// returns false if failed
bool SetSelectedItemText( const HWND listview_handle,
                          const std::wstring& text );

// SetSelectedItem sets the currently selected item
// Set index to -1 to select all items
void SetSelectedItem( const HWND listview_handle, const uint32_t index );

// RemoveItem removes an item for listview
// returns false if failed
bool RemoveItem( const HWND listview_handle, const uint32_t index );

// RemoveSelectedItem removes the selected item
// return false if failed
bool RemoveSelectedItem( const HWND listview_handle );

// GetItemText gets the text of an item or subitem
std::wstring GetItemText( const HWND listview_handle,
                          int index,
                          int subItemIndex );

// GetSelectedItemRowText returns a vector containing the strings of each item
// in a row
std::vector<std::wstring> GetSelectedItemRowText( const HWND listview_handle );

// TODO: Make some better finding functions when needed, can't bother at the
// moment FindItem searches for the item name from the beginning if found it
// ensures its visibility and selects it returns -1 if not found
int FindItem( const HWND listview_handle, const std::wstring& name );

// SetColumnWidthAutoSize enables auto size for the specific column
// returns false if failed
bool SetColumnWidthAutoSize( const HWND listview_handle,
                             const uint32_t column_index );

// SetColumnWidth sets the width of the specified column
// returns false if failed
bool SetColumnWidth( const HWND listview_handle,
                     const uint32_t index,
                     const uint32_t width );

// GetColumnWidth
uint32_t GetColumnWidth( const HWND listview_handle,
                     const uint32_t index );

}  // namespace listview

}  // namespace gwingui