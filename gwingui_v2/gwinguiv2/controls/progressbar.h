#pragma once

#include <Windows.h>
#include <stdint.h>

namespace gwingui {

namespace progressbar {

// SetRange sets a new range for a progress bar
// returns false if failed
bool SetRange( const HWND progressbar_handle,
               const uint32_t min,
               const uint32_t max );

// GetPos returns the current position of a progressbar
LRESULT GetPos( const HWND progressbar_handle );

// SetPos sets the position of a progressbar
void SetPos( const HWND progressbar_handle, const uint32_t value );

// IncementPos increments the progressbar position by a value
void IncementPos( const HWND progressbar_handle, const uint32_t value );

void EnableMarquee( const HWND progressbar_handle,
                    const uint32_t update_interval );

void DisableMarquee( const HWND progressbar_handle );

}  // namespace Progressbar

}  // namespace gwingui