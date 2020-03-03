#pragma once

#include "gwinguiv2/window.h"
#include "gwinguiv2/controls/control.h"
#include "gwinguiv2/controls/editcontrol.h"
#include "gwinguiv2/controls/combobox.h"
#include "gwinguiv2/controls/listbox.h"
#include "gwinguiv2/controls/listview.h"
#include "gwinguiv2/controls/tabcontrol.h"
#include "gwinguiv2/controls/progressbar.h"
#include "gwinguiv2/controls/trackbar.h"
#include "gwinguiv2/controls/richedit.h"
#include "gwinguiv2/controls/hotkey.h"

#include "tab_window1.h"
#include "tab_window2.h"
#include "tab_window3.h"
#include "tab_window4.h"

class BasicWindow1 : public gwingui::Window {
 public:
  BasicWindow1( const HINSTANCE instance_handle )
      : instance_handle_( instance_handle ) {}
  BasicWindow1( const BasicWindow1& ) = default;
  ~BasicWindow1() = default;

  void OnButtonClick( const HWND button_handle,
                      const uint32_t button_id ) override {
    using namespace gwingui;

    switch ( button_id ) {
      case IDC_RADIO1: {
        const auto lol = GWH( IDC_LIST1 );
        listbox::DeleteItem( lol, 0 );
      } break;

      case IDC_RADIO2: {
        const auto lol = GWH( IDC_COMBO1 );
        combobox::SetSelectedIndex( lol, -1 );
      } break;

      case IDC_RADIO3: {
        const auto limit = 50;
        const auto lol = GWH( IDC_RICHEDIT21 );
        const auto length = richedit::GetTextLength( lol );
        if ( length > limit ) {
          richedit::SetText( lol, TEXT( "" ) );
        }

        richedit::AppendText( lol, TEXT( "dicky\n" ) );
      } break;

      case IDC_CHECK1: {
        const auto combobox_handle = GWH( IDC_LIST1 );
        listbox::AddString( combobox_handle, TEXT( "dicksss" ) );

        const auto hotkey_handle = GWH( IDC_HOTKEY1 );

        const auto vk = hotkey::GetCurrentHotkey( hotkey_handle );
        messagebox::Warning( std::to_wstring( vk ) );

      } break;

      case BUTTON_OK: {
        SendMessage( HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2 );

        const auto button_cancel_handle = GWH( BUTTON_CANCEL );

        control::SetText( button_cancel_handle, TEXT( "dicks" ) );

        break;

        const auto menu_handle =
            LoadMenu( instance_handle_, MAKEINTRESOURCE( IDR_MENU1 ) );
        const auto sub_menu_handle = GetSubMenu( menu_handle, 0 );

        assert( sub_menu_handle != 0 );

        /*
        Use the TPM_RECURSE flag to display a menu when another menu is already
        displayed. This is intended to support context menus within a menu.
        */

        TPMPARAMS lol;
        lol.cbSize = sizeof( lol );
        lol.rcExclude = {};
        auto test =
            TrackPopupMenuEx( sub_menu_handle,
                              TPM_LEFTALIGN | TPM_TOPALIGN | TPM_NONOTIFY |
                                  TPM_LEFTBUTTON | TPM_NOANIMATION,
                              0, 0, GetHandle(), &lol );
      } break;

      case BUTTON_CANCEL: {
        {
          const auto handle = control::GetWindowHandle( BUTTON_OK );
          control::EnableOrDisable( handle, !control::IsEnabled( handle ) );
        }
        {
          const auto handle = control::GetWindowHandle( IDC_CHECK1 );
          control::EnableOrDisable( handle, !control::IsEnabled( handle ) );
        }
        {
          const auto handle = control::GetWindowHandle( IDC_EDIT1 );
          control::EnableOrDisable( handle, !control::IsEnabled( handle ) );
        }
        {
          const auto handle = control::GetWindowHandle( IDC_COMBO1 );
          control::EnableOrDisable( handle, !control::IsEnabled( handle ) );
        }
        {
          const auto handle = control::GetWindowHandle( IDC_LIST1 );
          control::EnableOrDisable( handle, !control::IsEnabled( handle ) );
        }
        {
          const auto handle = control::GetWindowHandle( IDC_RADIO1 );
          control::EnableOrDisable( handle, !control::IsEnabled( handle ) );
        }
        {
          const auto handle = control::GetWindowHandle( IDC_SLIDER1 );
          control::EnableOrDisable( handle, !control::IsEnabled( handle ) );
        }
        {
          const auto handle = control::GetWindowHandle( PROGRESS_ONE );
          control::EnableOrDisable( handle, !control::IsEnabled( handle ) );
        }
        {
          const auto handle = control::GetWindowHandle( IDC_RICHEDIT21 );
          control::EnableOrDisable( handle, !control::IsEnabled( handle ) );
        }
        {
          // const auto handle = control::GetWindowHandle( IDC_TAB1 );
          // control::EnableOrDisable( handle, !control::IsEnabled( handle ) );
        } {
          const auto handle = control::GetWindowHandle( IDC_LIST2 );
          control::EnableOrDisable( handle, !control::IsEnabled( handle ) );
        }
        {
          const auto handle = control::GetWindowHandle( LISTBOX_WHORE );
          control::EnableOrDisable( handle, !control::IsEnabled( handle ) );
        }
        {
          const auto handle = control::GetWindowHandle( IDC_WHORE_LISTVIEW );
          control::EnableOrDisable( handle, !control::IsEnabled( handle ) );
        }
        {
          const auto handle = control::GetWindowHandle( IDC_HOTKEY1 );
          control::EnableOrDisable( handle, !control::IsEnabled( handle ) );
        }
      } break;
      default:
        break;
    }
  }

  void Initialize( const HWND window_handle ) override {
    using namespace gwingui;

    ShowWindow( window_handle, SW_SHOW );

    gwingui::control::SetText( window_handle, TEXT( "The window title" ) );

    const auto tabcontrol_handle1 =
        gwingui::control::GetWindowHandle( IDC_TAB1 );

    AddTabWindow( tabcontrol_handle1, IDD_DIALOG2, TEXT( "Tab 1" ),
                  new TabWindow1 );

    AddTabWindow( tabcontrol_handle1, IDD_DIALOG3, TEXT( "Tab 2" ),
                  new TabWindow2 );

    const auto tabcontrol_handle2 =
        gwingui::control::GetWindowHandle( IDC_TAB2 );

    AddTabWindow( tabcontrol_handle2, IDD_DIALOG4, TEXT( "Tab 1" ),
                  new TabWindow3 );

    AddTabWindow( tabcontrol_handle2, IDD_DIALOG5, TEXT( "Tab 2" ),
                  new TabWindow4 );

    const HWND edit_handle = gwingui::control::GetWindowHandle( IDC_EDIT1 );
    gwingui::editcontrol::SetPlaceholder( edit_handle,
                                          TEXT( "A placeholder" ) );

    const HWND combo_handle = gwingui::control::GetWindowHandle( IDC_COMBO1 );
    for ( int i = 0; i < 30; ++i )
      gwingui::combobox::AddString( combo_handle,
                                    TEXT( "Item " ) + std::to_wstring( i ) );

    const HWND listbox_handle = gwingui::control::GetWindowHandle( IDC_LIST1 );
    for ( int i = 0; i < 2; ++i )
      gwingui::listbox::AddString( listbox_handle,
                                   TEXT( "Item " ) + std::to_wstring( i ) );

    const HWND listview_handle = gwingui::control::GetWindowHandle( IDC_LIST2 );

    for ( int i = 0; i < 3; ++i )
      gwingui::listview::AddColumn(
          listview_handle, TEXT( "Column " ) + std::to_wstring( i ), 0 );

    listview::SetColumnWidthAutoSize( listview_handle, 0 );
    listview::SetColumnWidthAutoSize( listview_handle, 1 );
    listview::SetColumnWidthAutoSize( listview_handle, 2 );

    for ( int i = 0; i < 30; ++i ) {
      gwingui::listview::AddItem( listview_handle,
                                  TEXT( "Item " ) + std::to_wstring( i ), 0 );
      if ( i % 30 )
        gwingui::listview::AddSubitem(
            listview_handle, TEXT( "Subitem " ) + std::to_wstring( i ), 0, 2 );
    }

    gwingui::trackbar::SetRange( GWH( IDC_SLIDER1 ), 0, 100 );
    gwingui::progressbar::SetRange( GWH( PROGRESS_ONE ), 0, 100 );
  }

  // Listbox events
  virtual void OnListBoxDoubleClick() {}
  virtual void OnListBoxSelectionChanged( const uint32_t control_id,
                                          const HWND control_handle ) {}
  virtual void OnListBoxItemDeleted( const uint32_t control_id,
                                     const HWND control_handle,
                                     const uint32_t index ) {}

  // Listview events
  virtual void OnListViewClick( NMITEMACTIVATE* pItem ) {}
  virtual void OnListViewDoubleClick( NMITEMACTIVATE* pItem ) {}
  virtual void OnListViewItemSelectionChanged( NMLISTVIEW* lv ) {}
  virtual void OnListViewItemAdded( NMLISTVIEW* lv ) {}
  virtual void OnListViewItemDeleted( NMLISTVIEW* lv ) {}

  // Combobox events
  virtual void OnComboboxSelectionChanged( uint32_t control_id,
                                           HWND control_handle ) {}

  // Trackbar events
  virtual void OnTrackbarSliderChanging( const HWND trackbar_handle,
                                         int32_t position ) {
    gwingui::progressbar::SetPos( GWH( PROGRESS_ONE ), position );
  }

  // Richedit events
  virtual void OnRichEditTextChanged( uint32_t ctrlId, CHANGENOTIFY* pCn ) {}

 private:
  HINSTANCE instance_handle_;
};