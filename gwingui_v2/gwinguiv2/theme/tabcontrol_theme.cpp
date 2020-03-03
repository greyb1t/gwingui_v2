#include "pch.h"
#include "tabcontrol_theme.h"
#include "../controls/control.h"
#include "../config/themefont.h"
#include "../config/themecolors.h"
#include "../controls/tabcontrol.h"
#include "../utils/safe_handles.h"
#include "../drawing.h"
#include "theme.h"

using namespace gwingui;

void gwingui::tabcontroltheme::Initialize( const HWND tabcontrol_handle ) {
  control::SetFont( tabcontrol_handle, themefont::GetDefaultThemeFont() );

  const auto tabcontrol_data = new TabControlData;
  tabcontrol_data->hovering_index = -1;
  tabcontrol_data->prev_window_handle = reinterpret_cast<HWND>( -1 );

  SetWindowLongPtr( tabcontrol_handle, GWLP_USERDATA,
                    reinterpret_cast<LONG_PTR>( tabcontrol_data ) );

  default_tabcontrol_proc_ = reinterpret_cast<WNDPROC>(
      SetWindowLongPtr( tabcontrol_handle, GWLP_WNDPROC,
                        reinterpret_cast<LONG_PTR>( TabControlThemeProc ) ) );
}

void GetTabItemHighlightUnderlineRect( const HWND tab_control_handle,
                                       const uint32_t index,
                                       RECT* rect_out ) {
  RECT tabitem_highlight_rect;

  tabcontrol::GetTabItemRect( tab_control_handle, index,
                              &tabitem_highlight_rect );

  const uint32_t tabitem_height =
      tabitem_highlight_rect.bottom - tabitem_highlight_rect.top;

  const uint32_t kHighlightRectThiccness = 5;

  tabitem_highlight_rect.top =
      tabitem_highlight_rect.top + tabitem_height - kHighlightRectThiccness;

  tabitem_highlight_rect.bottom += 1;

  *rect_out = tabitem_highlight_rect;
}

LRESULT CALLBACK
gwingui::tabcontroltheme::TabControlThemeProc( HWND tabcontrol_handle,
                                               uint32_t message,
                                               WPARAM wparam,
                                               LPARAM lparam ) {
  // Due to the owner drawing not having enough functionality, I am drawing the
  // control myself

  switch ( message ) {
    case WM_PAINT: {
      PAINTSTRUCT ps;
      const HDC hdc = BeginPaint( tabcontrol_handle, &ps );

      RECT client_rect;
      GetClientRect( tabcontrol_handle, &client_rect );

      SafeSelectObject selected_font( hdc,
                                      control::GetFont( tabcontrol_handle ) );

      // Get the tab highlight rect to use the top and bottom positions
      RECT tab_highlight_underline_rect;
      GetTabItemHighlightUnderlineRect( tabcontrol_handle, 0,
                                        &tab_highlight_underline_rect );

      // Make the recently gotten underline rect as wide as the whole tab
      // control
      tab_highlight_underline_rect.right = client_rect.right - 1;
      tab_highlight_underline_rect.left = client_rect.left + 1;

      // Draw the long tabitems underline
      drawing::FilledRectangle(
          hdc, tab_highlight_underline_rect,
          themecolors::tabcontrol::kTabUnderlineUnSelected );

      const uint32_t tab_count = tabcontrol::GetTabCount( tabcontrol_handle );

      for ( uint32_t i = 0; i < tab_count; ++i ) {
        RECT tab_item_rect;
        tabcontrol::GetTabItemRect( tabcontrol_handle, i, &tab_item_rect );

        const auto tabcontrol_data = reinterpret_cast<TabControlData*>(
            GetWindowLongPtr( tabcontrol_handle, GWLP_USERDATA ) );

        const bool is_hovering = tabcontrol_data->hovering_index == i;
        const bool is_selected_tab =
            tabcontrol::GetSelectedTab( tabcontrol_handle ) == i;

        COLORREF text_color = themecolors::tabcontrol::kText;

        const std::wstring tabitem_text =
            tabcontrol::GetTabItemText( tabcontrol_handle, i );

        const SIZE text_size =
            drawing::GetSelectedFontTextDimensions( hdc, tabitem_text );

        const POINT text_pos =
            theme::GetCenterPosition( tab_item_rect, text_size );

        RECT text_rect;
        text_rect.left = text_pos.x;
        text_rect.top = text_pos.y;
        text_rect.right = text_rect.left + text_size.cx;
        text_rect.bottom = text_rect.top + text_size.cy;

        if ( is_hovering ) {
          SetBkColor( hdc, themecolors::tabcontrol::kBackgroundTabHover );
          text_color = themecolors::tabcontrol::kTextHover;
        } else if ( is_selected_tab ) {
          SetBkColor( hdc, themecolors::tabcontrol::kBackgroundTabSelected );
          text_color = themecolors::tabcontrol::kTextSelected;
        } else {
          SetBkColor( hdc, themecolors::tabcontrol::kBackground );
        }

        drawing::Text( hdc, text_pos.x, text_pos.y, text_color, tabitem_text );

        drawing::ExcludeRectFromDC( hdc, text_rect );

        RECT tabitem_highlight_underline_rect;

        GetTabItemHighlightUnderlineRect( tabcontrol_handle, i,
                                          &tabitem_highlight_underline_rect );

        tab_item_rect.bottom = tabitem_highlight_underline_rect.top;

        // Because the tab control by default has some weird padding on the tab
        // items in order to have a 1 pixel border on the tab items, we move the
        // tabs to the left by one pixel
        tabitem_highlight_underline_rect =
            drawing::MoveRectX( tabitem_highlight_underline_rect, -1 );
        tab_item_rect = drawing::MoveRectX( tab_item_rect, -1 );

        tab_item_rect.top -= 1;

        if ( is_hovering ) {
          drawing::FilledRectangle(
              hdc, tabitem_highlight_underline_rect,
              themecolors::tabcontrol::kTabUnderlineHover );
          drawing::FilledRectangle(
              hdc, tab_item_rect,
              themecolors::tabcontrol::kBackgroundTabHover );
          drawing::ExcludeRectFromDC( hdc, tab_item_rect );
        } else if ( is_selected_tab ) {
          drawing::FilledRectangle(
              hdc, tabitem_highlight_underline_rect,
              themecolors::tabcontrol::kTabUnderlineSelected );
          drawing::FilledRectangle(
              hdc, tab_item_rect,
              themecolors::tabcontrol::kBackgroundTabSelected );
          drawing::ExcludeRectFromDC( hdc, tab_item_rect );
        }
      }

      drawing::ExcludeRectFromDC( hdc, tab_highlight_underline_rect );

      RECT tab_display_window_rect = client_rect;

      tab_display_window_rect.right = tab_display_window_rect.right - 1;

      tab_display_window_rect.left = tab_display_window_rect.left + 1;

      tab_display_window_rect.bottom = tab_display_window_rect.bottom - 1;

      tab_display_window_rect.top = tab_highlight_underline_rect.bottom + 1;

      // Exclude the display window area
      drawing::ExcludeRectFromDC( hdc, tab_display_window_rect );

      // Draw the background last after I have excluded all the other parts
      drawing::FilledRectangle( hdc, client_rect,
                                themecolors::tabcontrol::kBackground );

      selected_font.Reset();

      EndPaint( tabcontrol_handle, &ps );

      return 0;
    } break;

    case WM_NCHITTEST: {
      POINT pos;
      POINTSTOPOINT( pos, MAKEPOINTS( lparam ) );

      // Convert the screen position to tabcontrol client position
      MapWindowPoints( nullptr, tabcontrol_handle, &pos, 1 );

      const auto def_ret =
          CallWindowProc( default_tabcontrol_proc_, tabcontrol_handle, message,
                          wparam, lparam );

      // Mouse is over the nothing area
      if ( def_ret == HTTRANSPARENT ) {
        const auto tabcontrol_data = reinterpret_cast<TabControlData*>(
            GetWindowLongPtr( tabcontrol_handle, GWLP_USERDATA ) );
        tabcontrol_data->hovering_index = -1;
      } else if ( def_ret == HTCLIENT ) {
        const uint32_t tab_count = tabcontrol::GetTabCount( tabcontrol_handle );

        for ( uint32_t i = 0; i < tab_count; ++i ) {
          RECT tab_item_rect;
          tabcontrol::GetTabItemRect( tabcontrol_handle, i, &tab_item_rect );

          const bool is_hovering = PtInRect( &tab_item_rect, pos );

          if ( is_hovering ) {
            const auto tabcontrol_data = reinterpret_cast<TabControlData*>(
                GetWindowLongPtr( tabcontrol_handle, GWLP_USERDATA ) );
            tabcontrol_data->hovering_index = i;
          }
        }
      }

    } break;

    case WM_MOUSELEAVE: {
      // Unhover any tab item
      const auto tabcontrol_data = reinterpret_cast<TabControlData*>(
          GetWindowLongPtr( tabcontrol_handle, GWLP_USERDATA ) );
      tabcontrol_data->hovering_index = -1;
    } break;

    case WM_ERASEBKGND:
      return 1;

    default:
      break;
  }

  return CallWindowProc( default_tabcontrol_proc_, tabcontrol_handle, message,
                         wparam, lparam );
}