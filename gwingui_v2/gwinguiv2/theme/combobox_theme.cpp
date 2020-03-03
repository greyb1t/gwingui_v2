#include "pch.h"
#include "combobox_theme.h"
#include "../controls/control.h"
#include "../controls/combobox.h"
#include "../config/themefont.h"
#include "../config/themecolors.h"
#include "../config/themebitmaps.h"
#include "../config/res/theme_bitmap_buffers.h"
#include "../drawing.h"
#include "theme.h"
#include "../utils/mouse_tracker.h"

using namespace gwingui;

static WNDPROC default_combobox_proc_ = 0;

LRESULT ComboBoxThemeProc( HWND combobox_handle,
                           uint32_t message,
                           WPARAM wparam,
                           LPARAM lparam ) {
  GetMouseTracker().ReportHoverLeaveEvents( combobox_handle, message );

  switch ( message ) {
    case CB_SETCURSEL: {
      control::UpdateControl( combobox_handle );
    } break;

    case WM_MOUSEWHEEL: {
      // Prevent the scrollwheel from changing selected value
      return 0;
    } break;

    case WM_KILLFOCUS: {
      // Prevent the combobox from repainting the window once it looses focus
      // The reason is that we use a global (static) hovering variable, when the
      // user clicks on another combobox, and the previous combobox redraws
      // itself, it uses that same hover variable causing the previous combobox
      // to also drawing like its being hovered
      return 0;
    } break;

    case WM_MOUSEMOVE: {
      // We are now hovering
      SetWindowLongPtr( combobox_handle, GWLP_USERDATA, TRUE );
    } break;

    case WM_MOUSELEAVE: {
      // We are now not hovering anymore
      SetWindowLongPtr( combobox_handle, GWLP_USERDATA, FALSE );
    } break;

    case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint( combobox_handle, &ps );

      // We use our own HDC because the HDC that BeginPaint gives us has edit
      // control borders excluded so the drawing would be weird. Maybe not the
      // proper way of doing it, but fuck it.
      SafeReleaseDC hdc( GetDC( combobox_handle ) );

      COLORREF background_color;
      COLORREF border_color;
      COLORREF text_color;

      HBITMAP bitmap;

      // Is the user hovering over the combobox?
      if ( GetWindowLongPtr( combobox_handle, GWLP_USERDATA ) ) {
        background_color = themecolors::combobox::kBackgroundHover;
        border_color = themecolors::combobox::kBorderHover;
        text_color = themecolors::combobox::kTextHover;

        bitmap = themebitmaps::g_bitmap_combobox_arrow_hover.GetValue();
      } else {
        background_color = themecolors::combobox::kBackground;
        border_color = themecolors::combobox::kBorder;
        text_color = themecolors::combobox::kText;

        bitmap = themebitmaps::g_bitmap_combobox_arrow_normal.GetValue();
      }

      const auto is_dropdown_visible = static_cast<BOOL>(
          SendMessage( combobox_handle, CB_GETDROPPEDSTATE, 0, 0 ) );

      if ( is_dropdown_visible ) {
        background_color = themecolors::combobox::kItemBackground;
        bitmap = themebitmaps::g_bitmap_combobox_arrow_down.GetValue();
      }

      // Is is not enabled?
      if ( !control::IsEnabled( combobox_handle ) ) {
        background_color = themecolors::combobox::kBackgroundDisabled;
        border_color = themecolors::combobox::kBorderDisabled;
        text_color = themecolors::combobox::kTextDisabled;

        bitmap = themebitmaps::g_bitmap_combobox_arrow_disabled.GetValue();
      }

      RECT rect;
      GetClientRect( combobox_handle, &rect );

      const uint32_t combo_arrow_bitmap_width =
          themebitmapbuffers::ComboDropdownNormal::width;
      const uint32_t combo_arrow_bitmap_height =
          themebitmapbuffers::ComboDropdownNormal::height;

      const uint32_t arrow_right_padding = 5;

      const uint32_t arrow_x =
          rect.right - combo_arrow_bitmap_width - arrow_right_padding +
          1 /* + 1 fixes the white line on the right side of arrow */;

      const uint32_t arrow_y = ( ( rect.bottom - rect.top ) / 2 ) -
                               ( combo_arrow_bitmap_height / 2 ) + rect.top;

      drawing::DrawBitmap( hdc.GetValue(), arrow_x, arrow_y,
                           combo_arrow_bitmap_width, combo_arrow_bitmap_height,
                           bitmap );

      drawing::ExcludeRectFromDC( hdc.GetValue(), arrow_x, arrow_y,
                                  arrow_x + combo_arrow_bitmap_width,
                                  arrow_y + combo_arrow_bitmap_height );

      SafeSelectObject selected_object( hdc.GetValue(),
                                        control::GetFont( combobox_handle ) );

      SetBkColor( hdc.GetValue(), background_color );

      std::wstring text = combobox::GetSelectedString( combobox_handle );

      SIZE text_size =
          drawing::GetSelectedFontTextDimensions( hdc.GetValue(), text );

      // If the text width is bigger than the combobox
      // NOTE: Consider changing the text that is shown based on the combobox
      // width
      if ( text_size.cx > ( rect.right - rect.left ) )
        text = text.substr( 0, 8 ) + TEXT( ".." );

      const auto text_center_pos = theme::GetCenterPosition( rect, text_size );

      const uint32_t text_x = rect.left + 5;
      // const uint32_t text_y =
      //     ( ( rect.bottom - rect.top ) / 2 ) - ( text_size.cy / 2 ) +
      //     rect.top;

      drawing::Text( hdc.GetValue(), text_x, text_center_pos.y, text_color,
                     text );

      // Exclude the text rect to avoid drawing over it
      drawing::ExcludeRectFromDC( hdc.GetValue(), text_x, text_center_pos.y,
                                  text_x + text_size.cx,
                                  text_center_pos.y + text_size.cy );

      selected_object.Reset();

      drawing::BorderRectangle( hdc.GetValue(), rect, border_color );

      rect = drawing::GrowRect( rect, -1, -1 );

      drawing::FilledRectangle( hdc.GetValue(), rect, background_color );

      EndPaint( combobox_handle, &ps );

      // We processed the message
      return FALSE;
    } break;

    default:
      break;
  }

  return CallWindowProc( default_combobox_proc_, combobox_handle, message,
                         wparam, lparam );
}

void gwingui::comboboxtheme::InitializeCombobox( const HWND combobox_handle ) {
  control::SetFont( combobox_handle, themefont::GetDefaultThemeFont() );

  // The userdata represents if the combobox is being hovered on or not
  SetWindowLongPtr( combobox_handle, GWLP_USERDATA, FALSE );

  default_combobox_proc_ = reinterpret_cast<WNDPROC>(
      SetWindowLongPtr( combobox_handle, GWLP_WNDPROC,
                        reinterpret_cast<LONG_PTR>( ComboBoxThemeProc ) ) );
}

void gwingui::comboboxtheme::OnOwnerDrawDropdown( DRAWITEMSTRUCT* dis ) {
  const HDC hdc = dis->hDC;

  const auto is_state_set = [=]( const uint32_t flag ) {
    return ( dis->itemState & flag ) == flag;
  };

  switch ( dis->itemAction ) {
    case ODA_SELECT:
    case ODA_DRAWENTIRE: {
      // Prevent the edit control from being drawn,
      // in future, fix this to support more types of comboboxes
      if ( is_state_set( ODS_COMBOBOXEDIT ) )
        return;

      // this color is the problem
      COLORREF listbox_item_background_color =
          themecolors::combobox::kItemBackground;

      if ( is_state_set( ODS_SELECTED ) )
        listbox_item_background_color =
            themecolors::combobox::kItemBackgroundFocused;

      drawing::FilledRectangle( hdc, dis->rcItem,
                                listbox_item_background_color );

      SafeSelectObject selected_object( hdc,
                                        control::GetFont( dis->hwndItem ) );

      const std::wstring text =
          combobox::GetString( dis->hwndItem, dis->itemID );

      const SIZE text_size =
          drawing::GetSelectedFontTextDimensions( hdc, text );

      const RECT& combo_listbox_item_rect = dis->rcItem;

      const auto center_pos =
          theme::GetCenterPosition( combo_listbox_item_rect, text_size );

      const uint32_t left_padding = 5;
      const uint32_t x = combo_listbox_item_rect.left + left_padding;

      SetBkColor( hdc, listbox_item_background_color );
      drawing::Text( hdc, x, center_pos.y, themecolors::combobox::kText, text );

      selected_object.Reset();
    } break;

    // In the future in case I care about control focus, probably won't though.
    // Fuck that shit.
    case ODA_FOCUS: {
    } break;

    default:
      break;
  }
}