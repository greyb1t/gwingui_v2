#pragma once

#include "gwinguiv2/window.h"

#include "gwinguiv2/message_box.h"
#include "gwinguiv2/controls/control.h"

class TabWindow1 : public gwingui::Window {
 public:
  TabWindow1() = default;
  TabWindow1( const TabWindow1& ) = default;
  ~TabWindow1() = default;

  virtual void Initialize( const HWND window_handle ) override {
    ShowWindow( GetHandle(), SW_SHOW );
    const auto lol = GWH( BUTTON_TAB2 );

    gwingui::listview ::AddColumn( GWH( IDC_WHORE_LISTVIEW ), TEXT( "col 1" ),
                                   0 );
    gwingui::listview::EnableFullRowSelect( GWH( IDC_WHORE_LISTVIEW ) );
    gwingui::listview::SetColumnWidthAutoSize( GWH( IDC_WHORE_LISTVIEW ), 0 );

    const auto listbox_whore =
        gwingui::control::GetWindowHandle( LISTBOX_WHORE );
    for ( int i = 0; i < 4; ++i ) {
      gwingui::listbox::AddString( listbox_whore,
                                   TEXT( "test" ) + std::to_wstring( i ) );

      gwingui::listview::AddItem( GWH( IDC_WHORE_LISTVIEW ),
                                  TEXT( "dicky" ) + std::to_wstring( i ), 0 );
    }
  }

  virtual void OnButtonClick( const HWND button_handle,
                              const uint32_t button_id ) {
    switch ( button_id ) {
      case BUTTON_TAB2: {
        const auto button_handle =
            gwingui::control::GetWindowHandle( BUTTON_TAB2 );

        gwingui::messagebox::Error( TEXT( "fuck off" ) );
      } break;
      default:
        break;
    }
  }

  virtual void OnHotkeyChanged( const HWND hotkey_handle,
                                const uint32_t control_id ) {
  }

  static INT_PTR CALLBACK DialogProc( HWND hwndDlg,
                                      UINT uMsg,
                                      WPARAM wParam,
                                      LPARAM lParam ) {
    return FALSE;
  }
};