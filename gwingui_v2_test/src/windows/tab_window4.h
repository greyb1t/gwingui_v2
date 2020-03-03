#pragma once

#include "gwinguiv2/window.h"

#include "gwinguiv2/controls/control.h"

class TabWindow4 : public gwingui::Window {
 public:
  TabWindow4() = default;
  TabWindow4( const TabWindow4& ) = default;
  ~TabWindow4() = default;

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