#include "pch.h"
#include "progressbar_theme.h"

#include "../controls/control.h"

#include "../config/themecolors.h"

#include "../drawing.h"

static WNDPROC default_progressbar_proc_ = 0;

LRESULT CALLBACK ProgressBarThemeProc( HWND progressbar_handle,
                                       uint32_t message,
                                       WPARAM wparam,
                                       LPARAM lparam ) {
  using namespace gwingui;

  switch ( message ) {
    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint( progressbar_handle, &ps );

      RECT client_rect;
      GetClientRect( progressbar_handle, &client_rect );

      // Draw the border
      drawing::BorderRectangle( hdc, client_rect,
                                themecolors::progressbar::kBorder );

      // Decrease the rect size to avoid drawing over the border
      RECT inner_rect = drawing::GrowRect( client_rect, -1, -1 );

      PBRANGE range;
      SendMessage( progressbar_handle, PBM_GETRANGE, 0,
                   reinterpret_cast<LPARAM>( &range ) );

      uint32_t pos = SendMessage( progressbar_handle, PBM_GETPOS, 0, 0 );

      // Calculate how many pixels one step in the progressbar is based on the
      // range
      const auto progressbar_width = inner_rect.right - inner_rect.left;

      const float step_pixel_width = static_cast<float>( progressbar_width ) /
                                     float( range.iHigh - range.iLow );

      auto draw_width = pos * step_pixel_width;

      // Prevent it from drawing over the left border
      if ( draw_width == 0 )
        draw_width = 1;
      else if (draw_width == progressbar_width)
        draw_width = client_rect.right - 1;

      inner_rect.right = draw_width;

      // Draw the filled part
      drawing::FilledRectangle( hdc, inner_rect,
                                themecolors::progressbar::kBackgroundFilled );

      inner_rect.left = inner_rect.right;
      inner_rect.right =
          client_rect.right - 1;  // - 1 to exclude the right border

      // Draw the empty part
      drawing::FilledRectangle( hdc, inner_rect,
                                themecolors::progressbar::kBackgroundEmpty );

      EndPaint( progressbar_handle, &ps );

      // We processed the message
      return FALSE;
    } break;

    default:
      break;
  }

  return CallWindowProc( default_progressbar_proc_, progressbar_handle, message,
                         wparam, lparam );
}

void gwingui::progressbartheme::InitializeControl(
    const HWND progressbar_handle ) {
  default_progressbar_proc_ = reinterpret_cast<WNDPROC>(
      SetWindowLongPtr( progressbar_handle, GWLP_WNDPROC,
                        reinterpret_cast<LONG_PTR>( ProgressBarThemeProc ) ) );
}
