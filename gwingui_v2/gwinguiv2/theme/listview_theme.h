#pragma once

namespace gwingui {

namespace listviewtheme {

static WNDPROC default_listview_proc;

void InitializeListview( const HWND listview_handle );

void OnOwnerDraw( const DRAWITEMSTRUCT* dis );

LRESULT ListViewThemeProc( HWND listview_handle,
                           UINT message,
                           WPARAM wparam,
                           LPARAM lparam );

}  // namespace listviewtheme

}  // namespace gwingui