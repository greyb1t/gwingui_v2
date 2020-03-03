#pragma once

#include <string>
#include <Windows.h>
#include <stdint.h>

namespace gwingui {

namespace combobox {

// AddString returns zero-based index to the new string or CB_ERR if failed
LRESULT AddString( const HWND combobox_handle,
                   const std::wstring &text );

// DeleteItem returns the count left in the combobox, if failed CB_ERR
LRESULT DeleteItem( const HWND combobox_handle,
                    const uint32_t index );

// SetSelectedIndex returns the index of the item selected, if failed CB_ERR
LRESULT SetSelectedIndex( const HWND combobox_handle,
                          const uint32_t index );

// GetSelectedIndex returns index to selected item, if no selection CB_ERR
LRESULT GetSelectedIndex( const HWND combobox_handle );

// SetItemText returns CB_ERR if failed
LRESULT SetItemText( const HWND combobox_handle,
                     const uint32_t index,
                     const std::wstring &text );

// GetString returns the of the current index
std::wstring GetString(const HWND combobox_handle,
                        const uint32_t index );

// GetSelectedString returns the string of the selected item
std::wstring GetSelectedString(const HWND combobox_handle );

// GetCount returns the item count, CB_ERR if failed
LRESULT GetCount( const HWND combobox_handle );

// FindString searches the combobox for an item beginning with the text
// Returns index of matched item, if not found CB_ERR
LRESULT FindString( const HWND combobox_handle,
                    const uint32_t startIndex,
                    const std::wstring &text );

// SetItemHeight sets the height of list items in combobox
// If height is 0, all items receives the specified height
LRESULT SetItemHeight( const HWND combobox_handle,
                       const uint32_t height,
                       const uint32_t index );

// GetItemHeight determines the items in combobox
LRESULT GetItemHeight( const HWND combobox_handle,
                       const uint32_t index);

}

}