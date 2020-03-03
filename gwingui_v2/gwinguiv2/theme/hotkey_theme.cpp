#include "pch.h"
#include "hotkey_theme.h"

#include "../controls/control.h"
#include "../controls/hotkey.h"
#include "../utils/safe_handles.h"
#include "../drawing.h"
#include "../config/themecolors.h"
#include "../config/themefont.h"
#include "theme.h"
#include "../utils/mouse_tracker.h"

#include "../window.h"

using namespace gwingui;

static WNDPROC default_hotkey_proc_ = 0;

LRESULT CALLBACK HotkeyThemeProc( HWND hotkey_handle,
                                  uint32_t message,
                                  WPARAM wparam,
                                  LPARAM lparam ) {
  GetMouseTracker().ReportHoverLeaveEvents( hotkey_handle, message );

  switch ( message ) {
    case WM_KEYDOWN: {
      // Since the only callbacks microsoft has added are only for non-child
      // windows, I have to make my own...thas sum buuullshit
      const auto parent_handle = GetParent( hotkey_handle );
      const auto window = window::GetWindow( parent_handle );
      if ( window ) {
        window->OnHotkeyChanged( hotkey_handle,
                                 control::GetWindowId( hotkey_handle ) );
      }

      SetFocus( 0 );
    } break;

    case HKM_SETHOTKEY: {
      // Since the only callbacks microsoft has added are only for non-child
      // windows, I have to make my own...thas sum buuullshit
      const auto parent_handle = GetParent( hotkey_handle );
      const auto window = window::GetWindow( parent_handle );
      if ( window ) {
        window->OnHotkeyChanged( hotkey_handle,
                                 control::GetWindowId( hotkey_handle ) );
      }
    } break;

    case WM_SETFOCUS: {
      // This removes the annoying caret
      RECT rect;
      GetClientRect( hotkey_handle, &rect );
      InvalidateRect( hotkey_handle, &rect, FALSE );
      return 0;
    } break;
    case WM_KILLFOCUS: {
      RECT rect;
      GetClientRect( hotkey_handle, &rect );
      InvalidateRect( hotkey_handle, &rect, FALSE );
    } break;

    case WM_NCPAINT: {
      return 0;
    } break;

    case WM_ERASEBKGND: {
      return 1;
    } break;

    case WM_PAINT: {
      PAINTSTRUCT ps;
      const HDC hdc = BeginPaint( hotkey_handle, &ps );

      RECT rect;
      GetClientRect( hotkey_handle, &rect );

      const auto virtual_keycode = hotkey::GetCurrentHotkey( hotkey_handle );

      std::wstring text;

      if ( virtual_keycode ) {
        text = hotkey::VirtualKeycodeToString( virtual_keycode );
      } else {
        text = TEXT( "None" );
      }

      auto border_color = themecolors::hotkey::kBorder;
      auto background_color = themecolors::hotkey::kBackground;
      auto text_color = themecolors::hotkey::kText;

      if ( GetFocus() == hotkey_handle ) {
        border_color = themecolors::hotkey::kBorderFocus;
        background_color = themecolors::hotkey::kBackgroundFocus;
        text_color = themecolors::hotkey::kTextFocus;

        text = TEXT( "Press a key" );
      }

      if ( !control::IsEnabled( hotkey_handle ) ) {
        border_color = themecolors::hotkey::kBorderDisabled;
        background_color = themecolors::hotkey::kBackgroundDisabled;
        text_color = themecolors::hotkey::kTextDisabled;
      }

      SetBkColor( hdc, background_color );

      const HFONT font_handle = control::GetFont( hotkey_handle );

      SafeSelectObject selected_object( hdc, font_handle );

      SIZE size = drawing::GetSelectedFontTextDimensions( hdc, text );

      auto text_pos = theme::GetCenterPosition( rect, size );

      text_pos.x = 5;

      drawing::Text( hdc, text_pos.x, text_pos.y, text_color, text );

      // Exclude the text rect to avoid drawing over it
      drawing::ExcludeRectFromDC( hdc, text_pos.x, text_pos.y,
                                  text_pos.x + size.cx, text_pos.y + size.cy );

      drawing::BorderRectangle( hdc, rect, border_color );

      const RECT smaller_rect = drawing::GrowRect( rect, -1, -1 );

      drawing::FilledRectangle( hdc, smaller_rect, background_color );

      selected_object.Reset();

      EndPaint( hotkey_handle, &ps );

      return FALSE;
    } break;

    default:
      break;
  }

  return CallWindowProc( default_hotkey_proc_, hotkey_handle, message, wparam,
                         lparam );
}

void gwingui::hotkeytheme::InitializeControl( const HWND window_handle ) {
  control::SetFont( window_handle, themefont::GetDefaultThemeFont() );

  // Remove fucking border since NCPAINT is a mess
  control::RemoveExtendedStyles( window_handle, WS_EX_CLIENTEDGE );

  control::UpdateControl( window_handle );

  default_hotkey_proc_ = reinterpret_cast<WNDPROC>(
      SetWindowLongPtr( window_handle, GWLP_WNDPROC,
                        reinterpret_cast<LONG_PTR>( HotkeyThemeProc ) ) );
}
