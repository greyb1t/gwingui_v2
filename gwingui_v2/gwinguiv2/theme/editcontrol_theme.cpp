#include "pch.h"
#include "editcontrol_theme.h"

#include "../controls/control.h"
#include "../config/themefont.h"
#include "../config/themecolors.h"
#include "../utils/safe_handles.h"
#include "../drawing.h"
#include "../utils/mouse_tracker.h"

using namespace gwingui;

static WNDPROC default_edit_proc_ = 0;

LRESULT EditThemeProc( HWND edit_handle,
                       uint32_t message,
                       WPARAM wparam,
                       LPARAM lparam ) {
  const uint32_t kEditControlTextPadding = 5;
  const uint32_t kEditControlTextPaddingNoBorder = 1;

  GetMouseTracker().ReportHoverLeaveEvents( edit_handle, message );

  switch ( message ) {
    case EM_SETCUEBANNER: {
      // TODO: Fix memory leak reported in DrMemory in this function

      // Handle EM_SETCUEBANNER since we're using our own way of drawing a
      // placeholder
      const wchar_t* cuebanner = ( wchar_t* )lparam;
      wchar_t* placeholder_buffer = new wchar_t[ wcslen( cuebanner ) + 1 ]{ 0 };

      wcscpy_s( placeholder_buffer, wcslen( cuebanner ) + 1, cuebanner );

      SetWindowLongPtr( edit_handle, GWLP_USERDATA,
                        ( LONG_PTR )placeholder_buffer );
      return 0;
    } break;

    case WM_PRINTCLIENT:
    case WM_PAINT: {
      // Draw our own placeholder
      if ( GetWindowTextLength( edit_handle ) == 0 &&
           GetFocus() != edit_handle ) {
        const auto cue_banner_text =
            ( wchar_t* )GetWindowLongPtr( edit_handle, GWLP_USERDATA );

        if ( !cue_banner_text )
          break;

        RECT update_rect;
        GetUpdateRect( edit_handle, &update_rect, TRUE );

        SafeReleaseDC hdc = GetDC( edit_handle );

        // Only include the region that has to be updated, otherwise when
        // resizing and redrawing part of the edit control, the whole edit
        // control would be redrawn causing FAT text to be drawn.
        drawing::IncludeRectInDC( hdc.GetValue(), update_rect );

        const auto def_wndproc_result =
            default_edit_proc_( edit_handle, message, wparam, lparam );

        // Get the font and margin so the cue banner text has a
        // consistent appearance and placement with existing text.
        RECT edit_rect;
        SendMessage( edit_handle, EM_GETRECT, 0,
                     reinterpret_cast<LPARAM>( &edit_rect ) );

        SafeSelectObject selected_font( hdc.GetValue(),
                                        control::GetFont( edit_handle ) );

        if ( control::IsEnabled( edit_handle ) )
          SetTextColor( hdc.GetValue(),
                        themecolors::editcontrol::kTextPlaceholder );
        else
          SetTextColor( hdc.GetValue(),
                        themecolors::editcontrol::kTextPlaceholderDisabled );

        SetBkMode( hdc.GetValue(), TRANSPARENT );

        DrawText( hdc.GetValue(), cue_banner_text,
                  static_cast<int>( wcslen( cue_banner_text ) ), &edit_rect,
                  DT_TOP | DT_LEFT | DT_NOPREFIX | DT_NOCLIP );

        selected_font.Reset();

        return def_wndproc_result;
      }
    } break;

    case WM_NCCALCSIZE: {
      // On return:
      // rgrc[0] is new client area rect
      // rgrc[1] and rgrc[2] does not seem to do anything...

      // If wparam is TRUE, return 0 without doing anything removes the window
      // frame

      if ( wparam == TRUE ) {
        auto calcsize_params = reinterpret_cast<NCCALCSIZE_PARAMS*>( lparam );

        uint32_t padding = 0;

        // Does the edit control have a border?
        if ( control::HasExtendedStyles( edit_handle, WS_EX_CLIENTEDGE ) ) {
          // Make enough non-client area for a border
          padding = kEditControlTextPadding;
        } else {
          // Make enough non-client area for a small focus border
          padding = kEditControlTextPaddingNoBorder;
        }

        calcsize_params->rgrc[ 0 ].top += padding;
        calcsize_params->rgrc[ 0 ].bottom -= padding;

        calcsize_params->rgrc[ 0 ].left += padding;
        calcsize_params->rgrc[ 0 ].right -= padding;

        return 0;
      }
    } break;

    case WM_NCPAINT: {
      SafeReleaseDC hdc = GetWindowDC( edit_handle );

      const bool has_border =
          control::HasExtendedStyles( edit_handle, WS_EX_CLIENTEDGE );

      // Exclude the client area of the edit control (we do not want to draw
      // over it)
      {
        RECT client_rect;
        GetClientRect( edit_handle, &client_rect );

        const uint32_t padding = has_border ? kEditControlTextPadding
                                            : kEditControlTextPaddingNoBorder;

        client_rect.left += padding;
        client_rect.right += padding;

        client_rect.top += padding;
        client_rect.bottom += padding;

        drawing::ExcludeRectFromDC( hdc.GetValue(), client_rect );
      }

      SIZE control_size = control::GetControlSize( edit_handle );
      RECT rect;
      rect.left = 0;
      rect.right = control_size.cx;
      rect.top = 0;
      rect.bottom = control_size.cy;

      COLORREF border_color = themecolors::editcontrol::kBackground;

      COLORREF background_color = themecolors::editcontrol::kBackground;

      if ( control::IsEnabled( edit_handle ) ) {
        if ( GetFocus() == edit_handle ) {
          border_color = themecolors::editcontrol::kBorderFocus;
        } else if ( has_border ) {
          border_color = themecolors::editcontrol::kBorderNormal;
        } else {
          border_color = themecolors::editcontrol::kBackground;
        }
      } else {
        border_color = themecolors::editcontrol::kBorderDisabled;
        background_color = themecolors::editcontrol::kBackgroundDisabled;
      }

      drawing::FilledRectangle( hdc.GetValue(), rect, border_color );

      // RECT_SHRINK( rect, 1 );
      rect = drawing::GrowRect( rect, -1, -1 );

      drawing::FilledRectangle( hdc.GetValue(), rect, background_color );

      // Return 0 to prevent the edit control default handler to draw a border
      // (if it has one)
      return 0;
    } break;

    case WM_CONTEXTMENU: {
      // TODO: Add support to display a context menu someday..
    } break;

    default:
      break;
  }

  return CallWindowProc( default_edit_proc_, edit_handle, message, wparam,
                         lparam );
}

void gwingui::editcontroltheme::InitializeEditControl(
    const HWND edit_control_handle ) {
  control::SetFont( edit_control_handle, themefont::GetDefaultThemeFont() );

  // TODO: Subclass and turn a edit control into a richedit control with its
  // features example: Ctrl+Backspace

  default_edit_proc_ = reinterpret_cast<WNDPROC>(
      SetWindowLongPtr( edit_control_handle, GWLP_WNDPROC,
                        reinterpret_cast<LONG_PTR>( EditThemeProc ) ) );

  // Update the control before setting the edit control rect
  // the reason is that we change the control in with the WM_NCCALCSIZE
  control::UpdateControl( edit_control_handle );

  const HFONT font = reinterpret_cast<HFONT>(
      SendMessage( edit_control_handle, WM_GETFONT, 0, 0 ) );

  SIZE fontsize = { 0 };

  // We are just interested in the height, so this works fine
  // but if the user changes the font manually later on, it might stop working
  // we can fix it by handling WM_SETFONT probably, but in the future not now.
  if ( font != NULL )
    fontsize = drawing::GetFontTextDimensions( TEXT( "Y" ), font );

  RECT rect;
  GetClientRect( edit_control_handle, &rect );

  const uint32_t height = rect.bottom - rect.top;

  rect.top = ( height / 2 ) - ( fontsize.cy / 2 );

  rect.bottom = rect.bottom + rect.top;
  rect.left = rect.left + 5;
  rect.right = rect.right - 5;

  // Center the edit rect inside the edit control
  // Sadly only works on multiline controls
  SendMessage( edit_control_handle, EM_SETRECTNP, 0,
               reinterpret_cast<LPARAM>( &rect ) );
}

void gwingui::editcontroltheme::DrawTextPart( const HDC hdc,
                                              const HWND edit_control_handle ) {
  COLORREF background_color = themecolors::editcontrol::kBackground;
  COLORREF text_color = themecolors::editcontrol::kText;

  if ( !control::IsEnabled( edit_control_handle ) ) {
    background_color = themecolors::editcontrol::kBackgroundDisabled;
    text_color = themecolors::editcontrol::kTextDisabled;
  }

  SetBkColor( hdc, background_color );
  SetTextColor( hdc, text_color );
}