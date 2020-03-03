#include "pch.h"
#include "checkbox_theme.h"
#include "../config/themecolors.h"
#include "../config/themebitmaps.h"
#include "../config/themefont.h"
#include "../controls/control.h"
#include "../config/res/theme_bitmap_buffers.h"
#include "../drawing.h"
#include "theme.h"
#include "../utils/mouse_tracker.h"

using namespace gwingui;

static WNDPROC default_checkbox_proc_ = 0;

void DrawCheckbox( const HDC hdc,
                   const HWND checkbox_handle,
                   const RECT& rect,
                   const HBITMAP checkbox_bitmap,
                   const COLORREF text_color ) {
  drawing::DrawBitmap( hdc, rect, checkbox_bitmap );

  SafeSelectObject selected_object( hdc, control::GetFont( checkbox_handle ) );

  const std::wstring text = control::GetText( checkbox_handle );

  SIZE size = drawing::GetSelectedFontTextDimensions( hdc, text );

  const auto center_pos = theme::GetCenterPosition( rect, size );

  const int32_t x = themebitmapbuffers::CheckBoxNormalChecked::width + 5;

  SetBkColor( hdc, themecolors::kWindowBackground );

  drawing::Text( hdc, x, center_pos.y, text_color, text );

  selected_object.Reset();
}

LRESULT CheckboxThemeProc( HWND checkbox_handle,
                           uint32_t message,
                           WPARAM wparam,
                           LPARAM lparam ) {
  GetMouseTracker().ReportHoverLeaveEvents( checkbox_handle, message );

  switch ( message ) {
    case WM_ENABLE: {
      // For some reason WM_PAINT does not get sent when disabling a control,
      // therefore we have to handle this specific case manually

      InvalidateRect( checkbox_handle, 0, FALSE );

      // We processed the message
      return FALSE;
    } break;

    case WM_PAINT: {
      const LRESULT button_state =
          SendMessage( checkbox_handle, BM_GETSTATE, 0, 0 );

      const auto is_state_set = [=]( const uint32_t flag ) {
        return ( button_state & flag ) == flag;
      };

      const bool is_checked = is_state_set( BST_CHECKED );

      COLORREF text_color = gwingui::themecolors::checkbox::kText;

      HBITMAP checkbox_bitmap = 0;

      if ( control::IsEnabled( checkbox_handle ) ) {
        const bool is_pushed = is_state_set( BST_PUSHED );
        const bool is_hovering = is_state_set( BST_HOT );

        if ( !is_checked ) {
          if ( is_pushed ) {
            checkbox_bitmap =
                themebitmaps::g_bitmap_unchecked_pushed_handle.GetValue();
          } else if ( is_hovering ) {
            checkbox_bitmap =
                themebitmaps::g_bitmap_unchecked_hover_handle.GetValue();
          } else {
            checkbox_bitmap =
                themebitmaps::g_bitmap_unchecked_handle.GetValue();
          }
        } else if ( is_checked ) {
          if ( is_pushed ) {
            checkbox_bitmap =
                themebitmaps::g_bitmap_checked_pushed_handle.GetValue();
          } else if ( is_hovering ) {
            checkbox_bitmap =
                themebitmaps::g_bitmap_checked_hover_handle.GetValue();
          } else {
            checkbox_bitmap = themebitmaps::g_bitmap_checked_handle.GetValue();
          }
        } else if ( is_state_set( BST_INDETERMINATE ) ) {
          assert( false &&
                  "BST_INDETERMINATE in theme is current unsupported" );
        }
      } else {
        text_color = themecolors::checkbox::kTextDisabled;

        if ( is_checked ) {
          checkbox_bitmap =
              themebitmaps::g_bitmap_checked_disabled_handle.GetValue();
        } else {
          checkbox_bitmap =
              themebitmaps::g_bitmap_unchecked_disabled_handle.GetValue();
        }
      }

      PAINTSTRUCT ps;
      const HDC hdc = BeginPaint( checkbox_handle, &ps );

      RECT rect;
      GetClientRect( checkbox_handle, &rect );

      drawing::FilledRectangle( hdc, rect, themecolors::kWindowBackground );

      DrawCheckbox( hdc, checkbox_handle, rect, checkbox_bitmap, text_color );

      EndPaint( checkbox_handle, &ps );

      // We processed the message
      return FALSE;
    } break;

    default:
      break;
  }

  return CallWindowProc( default_checkbox_proc_, checkbox_handle, message,
                         wparam, lparam );
}

void gwingui::checkboxtheme::InitializeCheckbox( const HWND checkbox_handle ) {
  control::SetFont( checkbox_handle, themefont::GetDefaultThemeFont() );

  SIZE size = control::GetControlSize( checkbox_handle );

  // If the bitmaps are different sizes, consider going back to dynamically
  // getting the bitmap size
  size.cy = themebitmapbuffers::CheckBoxNormalUnchecked::height;

  // Make the width greater to avoid the text from being clipped out when
  // drawing
  size.cx += themebitmapbuffers::CheckBoxNormalUnchecked::width;

  // Make the checkbox bigger to fit the themed checkbox
  control::SetControlSize( checkbox_handle, size );

  default_checkbox_proc_ = reinterpret_cast<WNDPROC>(
      SetWindowLongPtr( checkbox_handle, GWLP_WNDPROC,
                        reinterpret_cast<LONG_PTR>( CheckboxThemeProc ) ) );
}