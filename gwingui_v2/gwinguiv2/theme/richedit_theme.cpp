#include "pch.h"
#include "richedit_theme.h"
#include "../config/themecolors.h"
#include "../controls/control.h"
#include "../drawing.h"
#include "../controls/richedit.h"
#include "../utils/mouse_tracker.h"
#include "../config/themefont.h"

void gwingui::richedittheme::InitializeRichedit( const HWND richedit_handle ) {
  SendMessage( richedit_handle, EM_SETEVENTMASK, NULL, ENM_CHANGE );

  // TODO: Properly set the font in richedit controls with CHARFORMAT2

  // richedit::SetFont(richedit_handle, TEXT(""), );

  control::SetFont( richedit_handle, themefont::GetDefaultThemeFont() );

  richedit::SetTextColor( richedit_handle, themecolors::richedit::kText );

  SendMessage( richedit_handle, EM_SETBKGNDCOLOR, NULL,
               themecolors::richedit::kBackground );

  default_richedit_proc_ = reinterpret_cast<WNDPROC>(
      SetWindowLongPtr( richedit_handle, GWLP_WNDPROC,
                        reinterpret_cast<LONG_PTR>( RichEditThemeProc ) ) );
}

LRESULT gwingui::richedittheme::RichEditThemeProc( HWND richedit_handle,
                                                   uint32_t message,
                                                   WPARAM wparam,
                                                   LPARAM lparam ) {
  GetMouseTracker().ReportHoverLeaveEvents( richedit_handle, message );

  switch ( message ) {
    case WM_ERASEBKGND: {
      // Says we erased the background, the reason is to avoid some possible
      // flickering
      return TRUE;
    } break;

    case WM_NCPAINT: {
      // Use the region wParam to only repaint the parts that are requested
      SafeRegion region = reinterpret_cast<HRGN>( wparam );

      // GetDCEx with DCX_INTERSECTRGN takes ownership of HRGN, make a copy of
      // it and pass that to GetDCEx to leave the one we get from wparam intact

      //

      // If wParam is one the whole window has to be repainted
      if ( wparam == 1 ) {
        // Since regions are not relative to window, I might need to change this
        // and not use GetWindowRect. But what the fuck do I know, maybe not.
        RECT rect;
        GetWindowRect( richedit_handle, &rect );

        region = CreateRectRgnIndirect( &rect );
      }

      SafeReleaseDC hdc = GetDCEx(
          richedit_handle, region.GetValue(),
          DCX_WINDOW | DCX_INTERSECTRGN | DCX_CACHE | DCX_LOCKWINDOWUPDATE );

      RECT control_region_rect;

      SIZE control_size = control::GetControlSize( richedit_handle );

      control_region_rect.right = control_size.cx;
      control_region_rect.bottom = control_size.cy;
      control_region_rect.top = 0;
      control_region_rect.left = 0;

      drawing::BorderRectangle( hdc.GetValue(), control_region_rect,
                                themecolors::richedit::kBorder );
      return 0;
    } break;

    case WM_MOUSEWHEEL: {
      // Remove the smooth scrolling
      if ( GET_WHEEL_DELTA_WPARAM( wparam ) >
           0 )  // A positive value indicates that the wheel was rotated
                // forward, away from the user;
        SendMessageA( richedit_handle, WM_VSCROLL, SB_LINEUP, NULL );
      else if ( GET_WHEEL_DELTA_WPARAM( wparam ) <
                0 )  // A negative value indicates that the wheel was rotated
                     // backward, toward the user.
        SendMessageA( richedit_handle, WM_VSCROLL, SB_LINEDOWN, NULL );

      return TRUE;
    } break;

    default:
      break;
  }

  return CallWindowProc( default_richedit_proc_, richedit_handle, message,
                         wparam, lparam );
}