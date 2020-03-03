#pragma once

#include <Windows.h>
#include <string>
#include <stdint.h>

namespace gwingui {

namespace tabcontrol {

// TODO: Do this correctly without any macros, only sendmessage

LRESULT AddTab( const HWND tabcontrol_handle,
                const std::wstring& tab_name,
                const uint32_t index );

void SetTabItemSize( const HWND tabcontrol_handle,
                     const uint32_t width,
                     const uint32_t height );

void GetTabItemRect( const HWND tabcontrol_handle,
                     const uint32_t index,
                     RECT* rect );

bool DeleteAllItems( const HWND tabcontrol_handle );

bool DeleteItem( const HWND tabcontrol_handle, const uint32_t index );

LRESULT GetSelectedTab( const HWND tabcontrol_handle );

LRESULT GetTabCount( const HWND tabcontrol_handle );

void SetSelected( const HWND tabcontrol_handle, int index );

std::wstring GetTabItemText( const HWND tabcontrol_handle,
                             const uint32_t index );

}  // namespace tabcontrol

}  // namespace gwingui