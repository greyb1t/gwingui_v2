#pragma once

#include <Windows.h>
#include <stdint.h>

namespace gwingui {

namespace trackbar {

void SetRange( const HWND trackbar_handle,
               const uint32_t min,
               const uint32_t max );

void SetPos( const HWND trackbar_handle, const uint32_t position );

LRESULT GetPos( const HWND trackbar_handle );

}  // namespace trackbar

}  // namespace gwingui