#pragma once

namespace gwingui {

namespace richedittheme {

static WNDPROC default_richedit_proc_;

void InitializeRichedit( const HWND richedit_handle );

LRESULT RichEditThemeProc( HWND richedit_handle,
                           uint32_t message,
                           WPARAM wparam,
                           LPARAM lparam );

}  // namespace richedittheme

}  // namespace gwingui