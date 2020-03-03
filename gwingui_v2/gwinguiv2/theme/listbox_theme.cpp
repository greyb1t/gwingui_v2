#include "pch.h"
#include "listbox_theme.h"
#include "../controls/control.h"
#include "../config/themefont.h"
#include "../config/themecolors.h"
#include "../utils/safe_handles.h"
#include "../controls/listbox.h"
#include "../drawing.h"
#include "theme.h"

using namespace gwingui;

static WNDPROC default_listbox_proc_ = 0;

LRESULT ListBoxThemeProc( HWND listbox_handle,
                          uint32_t message,
                          WPARAM wparam,
                          LPARAM lparam );

void DrawItem( const DRAWITEMSTRUCT* dis,
               const COLORREF item_background_color,
               const COLORREF text_color );

void gwingui::listboxtheme::InitializeListbox( HWND listbox_handle ) {
  control::AddStyles( listbox_handle, LBS_OWNERDRAWFIXED );

  control::SetFont( listbox_handle, themefont::GetDefaultThemeFont() );

  // TODO: Fix reported memory leak from DrMemory here
  SetWindowLongPtr( listbox_handle, GWLP_USERDATA,
                    reinterpret_cast<LONG_PTR>( new ListboxThemeData ) );

  default_listbox_proc_ = reinterpret_cast<WNDPROC>(
      SetWindowLongPtr( listbox_handle, GWLP_WNDPROC,
                        reinterpret_cast<LONG_PTR>( ListBoxThemeProc ) ) );
}

void gwingui::listboxtheme::OnOwnerDraw( const DRAWITEMSTRUCT* dis ) {
  COLORREF item_background_color_even =
      themecolors::listbox::kItemBackgroundEven;
  COLORREF item_background_color_odd = themecolors::listbox::kItemBackgroundOdd;
  COLORREF text_color = themecolors::listbox::kText;

  const int32_t item_index = dis->itemID;

  switch ( dis->itemAction ) {
    case ODA_DRAWENTIRE: {
      // Is the index selected?
      if ( item_index == listbox::GetSelectedIndex( dis->hwndItem ) ) {
        item_background_color_even =
            themecolors::listbox::kItemBackgroundActive;
        item_background_color_odd = themecolors::listbox::kItemBackgroundActive;
        text_color = themecolors::listbox::kTextActive;
      }

      if ( dis->itemState & ODS_DISABLED ) {
        item_background_color_even =
            themecolors::listbox::kItemBackgroundDisabled;
        item_background_color_odd =
            themecolors::listbox::kItemBackgroundDisabled;
        text_color = themecolors::listbox::kTextDisabled;
      }

      if ( item_index % 2 ) {
        DrawItem( dis, item_background_color_odd, text_color );
      } else {
        DrawItem( dis, item_background_color_even, text_color );
      }
    } break;

    case ODA_SELECT: {
      auto listbox_theme_data = reinterpret_cast<ListboxThemeData*>(
          GetWindowLongPtr( dis->hwndItem, GWLP_USERDATA ) );

      if ( item_index != listbox_theme_data->prev_selected_item_index ) {
        item_background_color_even =
            themecolors::listbox::kItemBackgroundActive;
        item_background_color_odd = themecolors::listbox::kItemBackgroundActive;
        text_color = themecolors::listbox::kTextActive;
      }

      if ( item_index % 2 ) {
        DrawItem( dis, item_background_color_odd, text_color );
      } else {
        DrawItem( dis, item_background_color_even, text_color );
      }

      // The last selected item unique to that specific listbox
      listbox_theme_data->prev_selected_item_index = item_index;
    } break;

    case ODA_HOVER: {
      DrawItem( dis, themecolors::listbox::kItemBackgroundHover,
                themecolors::listbox::kTextActive );
    } break;

    default:
      break;
  }
}

void DrawItem( const DRAWITEMSTRUCT* dis,
               const COLORREF item_background_color,
               const COLORREF text_color ) {
  const HWND listbox_handle = dis->hwndItem;
  const HDC hdc = dis->hDC;
  const uint32_t item_index = dis->itemID;
  const RECT& item_rect = dis->rcItem;

  // Draw the item background
  drawing::FilledRectangle( hdc, item_rect, item_background_color );

  SetBkColor( hdc, item_background_color );

  SafeSelectObject selected_font( hdc, control::GetFont( listbox_handle ) );

  const std::wstring text = listbox::GetText( listbox_handle, item_index );

  const SIZE text_size = drawing::GetSelectedFontTextDimensions( hdc, text );

  const auto center_pos = theme::GetCenterPosition( item_rect, text_size );

  drawing::Text( hdc, center_pos.x, center_pos.y, text_color, text );

  selected_font.Reset();
}

LRESULT ListBoxThemeProc( HWND listbox_handle,
                          uint32_t message,
                          WPARAM wparam,
                          LPARAM lparam ) {
  switch ( message ) {
    case WM_ENABLE: {
      ListboxThemeData* listbox_data = reinterpret_cast<ListboxThemeData*>(
          GetWindowLongPtr( listbox_handle, GWLP_USERDATA ) );

      if ( listbox_data )
        listbox_data->has_control_status_changed = true;

      // TODO: Fix, when enabling or disabling the listbox control, ALL of the
      // controls in the window gets redrawn. Causing a major flick.

      // Issue: When handling WM_CTLCOLORLISTBOX, I call
      /*
                    RECT rect;
              GetClientRect(window_handle, &rect );
              InvalidateRect(window_handle, &rect, FALSE );
      */
      // for the whole window, that is wrong.
    } break;

    case WM_NCPAINT: {
      return theme::HandleListNcPaint(
          listbox_handle, message, wparam, lparam,
          themecolors::listbox::kBorder, themecolors::listbox::kBorderDisabled,
          themecolors::listbox::kBackground,
          themecolors::listbox::kBackgroundDisabled );
    } break;

    case WM_MOUSEMOVE: {
      SafeReleaseDC hdc = GetDC( listbox_handle );

      const LONG_PTR item_from_point =
          SendMessage( listbox_handle, LB_ITEMFROMPOINT, 0, lparam );

      const int32_t hover_item_index = LOWORD( item_from_point );

      const bool is_in_client_area = !HIWORD( item_from_point );

      ListboxThemeData* listbox_data = reinterpret_cast<ListboxThemeData*>(
          GetWindowLongPtr( listbox_handle, GWLP_USERDATA ) );

      if ( listbox_data->prev_hover_item_index != hover_item_index ||
           !is_in_client_area ) {
        RECT listbox_rect;  // = control::GetControlRect( listbox_handle );
        GetClientRect( listbox_handle, &listbox_rect );
        drawing::IncludeRectInDC( hdc.GetValue(), listbox_rect );

        // unselect the last one
        if ( listbox_data->prev_hover_item_index != -1 &&
             listbox_data->prev_selected_item_index !=
                 listbox_data->prev_hover_item_index ) {
          RECT prev_hovered_item_rect;
          SendMessage( listbox_handle, LB_GETITEMRECT,
                       listbox_data->prev_hover_item_index,
                       reinterpret_cast<LPARAM>( &prev_hovered_item_rect ) );

          DRAWITEMSTRUCT dis = { 0 };
          dis.itemAction = ODA_DRAWENTIRE;
          dis.hwndItem = listbox_handle;
          dis.hDC = hdc.GetValue();
          dis.itemID = listbox_data->prev_hover_item_index;
          dis.rcItem = prev_hovered_item_rect;

          listbox_data->prev_hover_item_index = -1;

          listboxtheme::OnOwnerDraw( &dis );
        }

        // select the current one
        if ( hover_item_index != listbox_data->prev_selected_item_index &&
             is_in_client_area ) {
          RECT hovered_item_rect;
          SendMessage( listbox_handle, LB_GETITEMRECT, hover_item_index,
                       reinterpret_cast<LPARAM>( &hovered_item_rect ) );

          DRAWITEMSTRUCT dis = { 0 };
          dis.itemAction = ODA_HOVER;
          dis.hwndItem = listbox_handle;
          dis.hDC = hdc.GetValue();
          dis.itemID = hover_item_index;
          dis.rcItem = hovered_item_rect;

          listboxtheme::OnOwnerDraw( &dis );

          listbox_data->prev_hover_item_index = hover_item_index;
        }
      }
    } break;

    case WM_MOUSELEAVE: {
      // I don't know why, but the listbox does not send the WM_MOUSELEAVE
      // message after having clicked the listbox twice or something like that.
      // In order to force it, we remove the focus of the listbox manually.
      // Crappy solution, but cannot bother trying to find the reason.
      if ( GetFocus() == listbox_handle )
        SetFocus( 0 );

      ListboxThemeData* listbox_data = reinterpret_cast<ListboxThemeData*>(
          GetWindowLongPtr( listbox_handle, GWLP_USERDATA ) );

      SafeReleaseDC hdc = GetDC( listbox_handle );

      if ( listbox_data->prev_hover_item_index != -1 ) {
        RECT prev_hover_item_rect;
        SendMessage( listbox_handle, LB_GETITEMRECT,
                     listbox_data->prev_hover_item_index,
                     reinterpret_cast<LPARAM>( &prev_hover_item_rect ) );

        DRAWITEMSTRUCT dis = { 0 };
        dis.itemAction = ODA_DRAWENTIRE;
        dis.hwndItem = listbox_handle;
        dis.hDC = hdc.GetValue();
        dis.itemID = listbox_data->prev_hover_item_index;
        dis.rcItem = prev_hover_item_rect;

        RECT listbox_rect;
        GetClientRect( listbox_handle, &listbox_rect );
        drawing::IncludeRectInDC( hdc.GetValue(), listbox_rect );

        listboxtheme::OnOwnerDraw( &dis );

        listbox_data->prev_hover_item_index = -1;
      }
    } break;

    default:
      break;
  }

  return CallWindowProc( default_listbox_proc_, listbox_handle, message, wparam,
                         lparam );
}