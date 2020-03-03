#include "pch.h"
#include "button_theme.h"
#include "../controls/control.h"
#include "../utils/safe_handles.h"
#include "../drawing.h"
#include "../config/themecolors.h"
#include "../config/themefont.h"
#include "theme.h"
#include "../utils/mouse_tracker.h"

using namespace gwingui;

static WNDPROC default_button_proc_ = 0;

void DrawButton( const HDC hdc,
                 const HWND button_handle,
                 const RECT& rect,
                 const COLORREF text_color,
                 const COLORREF background_color,
                 const COLORREF border_color ) {
  const std::wstring text = control::GetText( button_handle );

  const HFONT font_handle = control::GetFont( button_handle );

  SafeSelectObject selected_object( hdc, font_handle );

  SIZE size = drawing::GetSelectedFontTextDimensions( hdc, text );

  const auto text_pos = theme::GetCenterPosition( rect, size );

  drawing::Text( hdc, text_pos.x, text_pos.y, text_color, text );

  // Exclude the text rect to avoid drawing over it
  drawing::ExcludeRectFromDC( hdc, text_pos.x, text_pos.y, text_pos.x + size.cx,
                              text_pos.y + size.cy );

  drawing::BorderRectangle( hdc, rect, border_color );

  const RECT smaller_rect = drawing::GrowRect( rect, -1, -1 );

  drawing::FilledRectangle( hdc, smaller_rect, background_color );

  selected_object.Reset();
}

LRESULT CALLBACK ButtonThemeProc( HWND button_handle,
                                  uint32_t message,
                                  WPARAM wparam,
                                  LPARAM lparam ) {
  GetMouseTracker().ReportHoverLeaveEvents( button_handle, message );

  switch ( message ) {
    case WM_SETTEXT: {
      // By default WM_PAINT does not get sent when changing the button text,
      // therefore we do it manually ourselves
      const auto result = CallWindowProc(
        default_button_proc_, button_handle, message, wparam, lparam );

      InvalidateRect( button_handle, 0, FALSE );

      return result;
    } break;

    case WM_ENABLE: {
      // For some reason WM_PAINT does not get sent when disabling a control,
      // therefore we have to handle this specific case manually
      InvalidateRect( button_handle, 0, FALSE );

      // We processed the message
      return FALSE;
    } break;

    case WM_PAINT: {
      const LRESULT button_state =
          SendMessage( button_handle, BM_GETSTATE, 0, 0 );

      if ( control::GetText( button_handle ) == TEXT( "dicks" ) ) {
        int test = 0;
      }

      COLORREF background_color;
      COLORREF border_color = themecolors::button::kBorder;
      COLORREF text_color = themecolors::button::kText;

      const auto is_state_set = [=]( const uint32_t flag ) {
        return ( button_state & flag ) == flag;
      };

      if ( control::IsEnabled( button_handle ) ) {
        if ( is_state_set( BST_PUSHED ) ) {
          background_color = themecolors::button::kBackgroundDown;
          text_color = themecolors::button::kTextHover;
        } else if ( is_state_set( BST_HOT ) ) {
          background_color = themecolors::button::kBackgroundHover;
          text_color = themecolors::button::kTextHover;
        } else {
          background_color = themecolors::button::kBackgroundNormal;
        }
      } else {
        background_color = themecolors::button::kBackgroundDisabled;
        text_color = themecolors::button::kTextDisabled;
        border_color = themecolors::button::kBorderDisabled;
      }

      PAINTSTRUCT ps;
      const HDC hdc = BeginPaint( button_handle, &ps );

      RECT rect;
      GetClientRect( button_handle, &rect );

      SetBkColor( hdc, background_color );

      DrawButton( hdc, button_handle, rect, text_color, background_color,
                  border_color );

      EndPaint( button_handle, &ps );
      // We processed the message
      return FALSE;
    } break;

    default:
      break;
  }

  return CallWindowProc( default_button_proc_, button_handle, message,
                         wparam, lparam );
}

void gwingui::buttontheme::InitializeControl( const HWND window_handle ) {
  control::SetFont( window_handle, themefont::GetDefaultThemeFont() );

  default_button_proc_ = reinterpret_cast<WNDPROC>(
      SetWindowLongPtr( window_handle, GWLP_WNDPROC,
                        reinterpret_cast<LONG_PTR>( ButtonThemeProc ) ) );
}