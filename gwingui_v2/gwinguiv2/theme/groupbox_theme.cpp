#include "pch.h"
#include "groupbox_theme.h"
#include "../controls/control.h"
#include "../utils/safe_handles.h"
#include "../drawing.h"
#include "../config/themecolors.h"
#include "../config/themefont.h"

using namespace gwingui;

static WNDPROC default_groupbox_proc_ = 0;

LRESULT GroupBoxThemeProc( HWND groupbox_handle,
                           uint32_t message,
                           WPARAM wparam,
                           LPARAM lparam ) {
  switch ( message ) {
    case WM_PAINT: {
      PAINTSTRUCT ps;
      const HDC hdc = BeginPaint( groupbox_handle, &ps );

      RECT client_rect;
      GetClientRect( groupbox_handle, &client_rect );

      drawing::FilledRectangle( hdc, client_rect,
                                themecolors::kWindowBackground );

      SafeSelectObject selected_font( hdc,
                                      control::GetFont( groupbox_handle ) );

      const std::wstring text = control::GetText( groupbox_handle );

      SetBkColor( hdc, themecolors::kWindowBackground );

      const uint32_t x = 5;
      drawing::Text( hdc, x, 0, themecolors::groupbox::kText, text );

      selected_font.Reset();

      const uint32_t line_x = 35;

      POINT p1;
      p1.x = client_rect.left;
      p1.y = line_x;

      POINT p2;
      p2.x = client_rect.right;
      p2.y = line_x;

      drawing::Line( hdc, p1, p2, themecolors::groupbox::kLine, 1 );

      EndPaint( groupbox_handle, &ps );

      return 0;
    } break;

    default:
      break;
  }

  return CallWindowProc( default_groupbox_proc_, groupbox_handle, message,
                         wparam, lparam );
}

void gwingui::groupboxtheme::InitializeGroupbox( const HWND groupbox_handle ) {
  control::SetFont( groupbox_handle, themefont::GetDefaultGroupBoxFont() );

  default_groupbox_proc_ = reinterpret_cast<WNDPROC>(
      SetWindowLongPtr( groupbox_handle, GWLP_WNDPROC,
                        reinterpret_cast<LONG_PTR>( GroupBoxThemeProc ) ) );
}