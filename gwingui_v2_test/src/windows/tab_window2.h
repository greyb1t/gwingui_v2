#pragma once

#include "gwinguiv2/window.h"

#include "gwinguiv2/controls/control.h"

class TabWindow2 : public gwingui::Window {
 public:
  TabWindow2() = default;
  TabWindow2( const TabWindow2& ) = default;
  ~TabWindow2() = default;

  virtual void Initialize( const HWND window_handle ) override {
    ShowWindow( GetHandle(), SW_SHOW );
  }

  static INT_PTR CALLBACK DialogProc( HWND hwndDlg,
                                      UINT uMsg,
                                      WPARAM wParam,
                                      LPARAM lParam ) {
    return FALSE;
  }
};