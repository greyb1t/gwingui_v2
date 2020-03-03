#include "pch.h"
#include "radiobutton_theme.h"
#include "../controls/control.h"
#include "../config/themefont.h"
#include "../drawing.h"
#include "../config/themebitmaps.h"
#include "../config/themecolors.h"
#include "theme.h"
#include "../utils/mouse_tracker.h"

using namespace gwingui;

static WNDPROC default_radiobutton_proc_ = 0;

void DrawRadiobutton( const HDC hdc,
                      const HWND radiobutton_handle,
                      const RECT& rect,
                      const HBITMAP radiobutton_bitmap,
                      const COLORREF text_color ) {
  drawing::DrawBitmap( hdc, rect, radiobutton_bitmap );

  SafeSelectObject selected_object( hdc,
                                    control::GetFont( radiobutton_handle ) );

  const std::wstring text = control::GetText( radiobutton_handle );

  const SIZE size = drawing::GetSelectedFontTextDimensions( hdc, text );

  const auto text_center_pos = theme::GetCenterPosition( rect, size );

  const int32_t x = themebitmapbuffers::RadioButtonNormalChecked::width + 5;

  SetBkColor( hdc, themecolors::kWindowBackground );

  drawing::Text( hdc, x, text_center_pos.y, text_color, text );

  selected_object.Reset();
}

LRESULT RadioButtonThemeProc( HWND radiobutton_handle,
                              uint32_t message,
                              WPARAM wparam,
                              LPARAM lparam ) {
  GetMouseTracker().ReportHoverLeaveEvents( radiobutton_handle, message );

  switch ( message ) {
    case WM_ERASEBKGND:
      return 1;

    case WM_ENABLE: {
      // For some reason WM_PAINT does not get sent when disabling a control,
      // therefore we have to handle this specific case manually

      InvalidateRect( radiobutton_handle, 0, FALSE );

      // Prevent the default proc from drawing the disabled version of the
      // control
      return 0;
    } break;

    case WM_PAINT: {
      PAINTSTRUCT ps;
      const HDC hdc = BeginPaint( radiobutton_handle, &ps );

      const LRESULT radiobutton_state =
          SendMessage( radiobutton_handle, BM_GETSTATE, 0, 0 );

      RECT rect;
      GetClientRect( radiobutton_handle, &rect );

      const auto is_state_set = [=]( const uint32_t flag ) {
        return ( radiobutton_state & flag ) == flag;
      };

      const bool is_checked = is_state_set( BST_CHECKED );

      COLORREF text_color = themecolors::radiobutton::kText;

      HBITMAP radiobutton_bitmap = 0;

      if ( control::IsEnabled( radiobutton_handle ) ) {
        const bool is_pushed = is_state_set( BST_PUSHED );
        const bool is_hovering = is_state_set( BST_HOT );

        if ( !is_checked ) {
          if ( is_pushed ) {
            radiobutton_bitmap = themebitmaps::g_bitmap_rb_pushed.GetValue();
          } else if ( is_hovering ) {
            radiobutton_bitmap =
                themebitmaps::g_bitmap_rb_hover_unchecked.GetValue();
          } else {
            radiobutton_bitmap = themebitmaps::g_bitmap_rb_unchecked.GetValue();
          }
        } else if ( is_checked ) {
          if ( is_pushed ) {
            radiobutton_bitmap = themebitmaps::g_bitmap_rb_pushed.GetValue();
          } else if ( is_hovering ) {
            radiobutton_bitmap =
                themebitmaps::g_bitmap_rb_hover_checked.GetValue();
          } else {
            radiobutton_bitmap = themebitmaps::g_bitmap_rb_checked.GetValue();
          }
        } else if ( is_state_set( BST_INDETERMINATE ) ) {
          assert( false &&
                  "BST_INDETERMINATE in theme is current unsupported" );
        }
      } else {
        text_color = themecolors::radiobutton::kTextDisabled;

        if ( is_checked ) {
          radiobutton_bitmap =
              themebitmaps::g_bitmap_rb_disabled_checked.GetValue();
        } else {
          radiobutton_bitmap =
              themebitmaps::g_bitmap_rb_disabled_unchecked.GetValue();
        }
      }

      DrawRadiobutton( hdc, radiobutton_handle, rect, radiobutton_bitmap,
                       text_color );

      EndPaint( radiobutton_handle, &ps );

      return FALSE;
    } break;

    default:
      break;
  }

  return CallWindowProc( default_radiobutton_proc_, radiobutton_handle, message,
                         wparam, lparam );
}

void gwingui::radiobuttontheme::InitializeRadiobutton(
    const HWND radiobutton_handle ) {
  control::SetFont( radiobutton_handle, themefont::GetDefaultThemeFont() );

  SIZE size = control::GetControlSize( radiobutton_handle );
  size.cy = themebitmapbuffers::RadioButtonNormalUnchecked::height;
  size.cx += themebitmapbuffers::RadioButtonNormalUnchecked::width;
  control::SetControlSize( radiobutton_handle, size );

  default_radiobutton_proc_ = reinterpret_cast<WNDPROC>(
      SetWindowLongPtr( radiobutton_handle, GWLP_WNDPROC,
                        reinterpret_cast<LONG_PTR>( RadioButtonThemeProc ) ) );
}