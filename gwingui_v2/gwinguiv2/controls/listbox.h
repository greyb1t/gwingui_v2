#pragma once

#include <Windows.h>
#include <string>

namespace gwingui {

namespace listbox {

// AddString adds a string to the listbox
// returns the newly added item index, if failed LB_ERR, if no space LB_ERRSPACE
LRESULT AddString( const HWND listbox_handle, const std::wstring& text );

// DeleteString deletes an item by an index
// returns list count, CB_ERR if index greater than item count
LRESULT DeleteItem( const HWND listbox_handle, const uint32_t index );

// SetSelectedItem sets the currently selected item
void SetSelectedItem( const HWND listbox_handle, const uint32_t index );

// SetItemText sets the text of an item by the index
// returns false if failed
bool SetItemText( const HWND listbox_handle,
                  const uint32_t index,
                  const std::wstring& text );

// GetText returns the text of an item by index
std::wstring GetText( const HWND listbox_handle, const uint32_t index );

// GetSelectedString returns the selected item string
std::wstring GetSelectedString( const HWND listbox_handle );

// GetSelectedIndex returns the selected item index
// returns -1 if no one was selected
LRESULT GetSelectedIndex( const HWND listbox_handle );

// GetCount returns the total item count
LRESULT GetCount( const HWND listbox_handle );

// FindString returns index if item is found, LB_ERR if not
LRESULT FindString( const HWND listbox_handle,
                    const uint32_t start_index,
                    const std::wstring& text );

void ClearItems( const HWND listbox_handle );

}  // namespace Listbox

}  // namespace gwingui