#include "pch.h"
#include "titlebar.h"

#include "../utils/safe_handles.h"
#include "../drawing.h"
#include "../config/themecolors.h"
#include "../config/theme_constants.h"
#include "../controls/control.h"
#include "../window.h"

#include "../config/themefont.h"

#include "theme.h"

namespace gwingui {

namespace titlebar {

std::vector<RECT> GetTitlebarButtonRects( const uint32_t window_width ) {
  constexpr int32_t right_button_padding = 1;
  constexpr int32_t top_button_padding = 1;

  RECT x_button_rect = kTitlebarButtonRect;
  OffsetRect( &x_button_rect,
              window_width - kTitlebarButtonWidth - right_button_padding,
              top_button_padding );

  RECT maximize_button_rect = kTitlebarButtonRect;
  OffsetRect( &maximize_button_rect,
              window_width - kTitlebarButtonWidth * 2 - right_button_padding,
              top_button_padding );

  RECT minimize_button_rect = kTitlebarButtonRect;
  OffsetRect( &minimize_button_rect,
              window_width - kTitlebarButtonWidth * 3 - right_button_padding,
              top_button_padding );

  constexpr auto icon_width_height = kIconRect.bottom - kIconRect.top;

  const auto icon_padding =
      themeconstants::kTitlebarHeight / 2 - icon_width_height / 2;

  RECT window_icon_rect = kIconRect;
  OffsetRect( &window_icon_rect, themeconstants::kTitlebarBorderThickness,
              icon_padding );

  return { x_button_rect, maximize_button_rect, minimize_button_rect
           /*window_icon_rect */ };
}

void DrawTitlebar( const HWND window_handle, const WPARAM wparam ) {
  // Use the region wParam to only repaint the parts that are requested
  SafeRegion region = reinterpret_cast<HRGN>( wparam );

  // If wParam is one the whole window has to be repainted
  if ( wparam == 1 ) {
    // Since regions are not relative to window, I might need to change this
    // and not use GetWindowRect. But what the fuck do I know, maybe not.
    const RECT rect = control::GetWindowRect( window_handle );

    region = CreateRectRgnIndirect( &rect );
  }

  SafeReleaseDC hdc = GetDCEx( window_handle, region.GetValue(),
                               DCX_WINDOW | DCX_INTERSECTRGN | DCX_CACHE );

  const RECT window_rect = control::GetWindowRect( window_handle );

  RECT window_rect_window = control::ScreenRectToWindowRect( window_rect );

  const uint32_t window_width =
      window_rect_window.right - window_rect_window.left;

  drawing::BorderRectangle( hdc.GetValue(), window_rect_window,
                            themecolors::kTitlebarBorder );

  const auto& window = window::GetWindow( window_handle );

  for ( const auto& button : window->titlebar_buttons_ ) {
    const RECT draw_rect = button.GetDrawRect( window_width );

    // TODO: Consider changing the button.is_down and .is_hovering to one enum
    // instead.
    if ( button.is_down ) {
      drawing::DrawBitmap( hdc.GetValue(), draw_rect,
                           button.bmp_down_.GetValue() );
    } else if ( button.is_hovering ) {
      drawing::DrawBitmap( hdc.GetValue(), draw_rect,
                           button.bmp_hover_.GetValue() );
    } else {
      drawing::DrawBitmap( hdc.GetValue(), draw_rect,
                           button.bmp_normal_.GetValue() );
    }

    // Exclude the components to not draw over them
    drawing::ExcludeRectFromDC( hdc.GetValue(), draw_rect );
  }

  // DRAW THE TITLE ICON

  constexpr auto icon_width_height = kIconRect.bottom - kIconRect.top;

  const auto icon_padding =
      themeconstants::kTitlebarHeight / 2 - icon_width_height / 2;

  RECT window_icon_rect = kIconRect;
  OffsetRect( &window_icon_rect, themeconstants::kTitlebarBorderThickness,
              icon_padding );

  drawing::DrawBitmap( hdc.GetValue(), window_icon_rect,
                       themebitmaps::g_titlebar_icon.GetValue() );

  drawing::ExcludeRectFromDC( hdc.GetValue(), window_icon_rect );

  // DRAW MENU

  // Select the font used to draw the menu bar text
  SafeSelectObject select_font_2( hdc.GetValue(),
                                  gwingui::themefont::GetDefaultThemeFont() );

  for ( const auto menuitem : window->menubar_ ) {
    COLORREF menuitem_background_color = themecolors::kMenuBarItemBackground;

    if ( menuitem.is_hovering ) {
      menuitem_background_color = themecolors::kMenuBarItemBackgroundHover;
    }

    const RECT text_rect = menuitem.text_rect_;

    // set the background color for the texts
    SetBkColor( hdc.GetValue(), menuitem_background_color );

    drawing::Text( hdc.GetValue(), text_rect.left, text_rect.top,
                   themecolors::kMenuBarItemText, menuitem.text_ );

    // Exclude the components to not draw over them
    drawing::ExcludeRectFromDC( hdc.GetValue(), text_rect );

    const RECT draw_rect = menuitem.GetDrawRect( 0 );

    drawing::FilledRectangle( hdc.GetValue(), draw_rect,
                              menuitem_background_color );

    drawing::ExcludeRectFromDC( hdc.GetValue(), draw_rect );
  }

  select_font_2.Reset();

  /*
  int menu_item_x = window_icon_rect.right;

  // Select the font used to draw the menu bar text
  SafeSelectObject select_font_2( hdc.GetValue(),
                                  gwingui::themefont::GetDefaultThemeFont() );

  for ( const auto& menuitem : window->menu_ ) {
    const auto& menuitem_text = menuitem.text;
    const auto font_size =
        drawing::GetSelectedFontTextDimensions( hdc.GetValue(), menuitem_text );

    RECT menu_bar_item_rect = { 0 };
    menu_bar_item_rect.right = font_size.cx + 15;
    menu_bar_item_rect.bottom = themeconstants::kTitlebarHeight;

    OffsetRect( &menu_bar_item_rect, menu_item_x, 0 );

    const auto center_text_pos =
        theme::GetCenterPosition( menu_bar_item_rect, font_size );

    RECT text_rect = { 0 };
    text_rect.left = center_text_pos.x;
    text_rect.top = center_text_pos.y;
    text_rect.right = text_rect.left + font_size.cx;
    text_rect.bottom = text_rect.top + font_size.cy;

    drawing::Text( hdc.GetValue(), text_rect.left, text_rect.top,
                   RGB( 255, 0, 0 ), menuitem_text );

    drawing::ExcludeRectFromDC( hdc.GetValue(), text_rect );

    drawing::FilledRectangle(hdc.GetValue(), menu_bar_item_rect, RGB(255, 0,
  0));

    drawing::ExcludeRectFromDC( hdc.GetValue(), menu_bar_item_rect );

    menu_item_x += ( menu_bar_item_rect.right - menu_bar_item_rect.left );
  }

  select_font_2.Reset();
  */

  // DRAW THE TITLE TEXT

  // Generate the title text rect
  wchar_t window_title_text[ MAX_PATH ] = { 0 };
  GetWindowText( window_handle, window_title_text, MAX_PATH );

  // A horrible fix to the issue that the window does not retrive the font
  // properly
  SafeSelectObject select_font( hdc.GetValue(),
                                gwingui::themefont::GetDefaultTitlebarFont() );

  const SIZE font_dimensions = drawing::GetSelectedFontTextDimensions(
      hdc.GetValue(), window_title_text );

  const auto text_padding_y =
      themeconstants::kTitlebarHeight / 2 - font_dimensions.cy / 2;

  // constexpr auto icon_width_height = kIconRect.bottom - kIconRect.top;

  // RECT title_text_rect = { themeconstants::kTitlebarBorderThickness +
  //                              icon_width_height + 5,
  //                          text_padding,
  //                          themeconstants::kTitlebarBorderThickness +
  //                              font_dimensions.cx + icon_width_height + 5,
  //                          font_dimensions.cy + text_padding };

  // drawing::Text( hdc.GetValue(), title_text_rect.left, title_text_rect.top,
  //                themecolors::kGeneralText, window_title_text );

  SetBkColor( hdc.GetValue(), themecolors::kTitlebarBackground );

  RECT window_client_rect;
  GetClientRect( window_handle, &window_client_rect );

  auto center_pos =
      gwingui::theme::GetCenterPosition( window_client_rect, font_dimensions );

  center_pos.y = text_padding_y;

  drawing::Text( hdc.GetValue(), center_pos.x, center_pos.y,
                 themecolors::kGeneralText, window_title_text );

  select_font.Reset();

  RECT title_text_rect;
  title_text_rect.left = center_pos.x;
  title_text_rect.top = center_pos.y;
  title_text_rect.bottom = title_text_rect.top + font_dimensions.cy;
  title_text_rect.right = title_text_rect.left + font_dimensions.cx;

  // Exclude the text to avoid drawing over
  drawing::ExcludeRectFromDC( hdc.GetValue(), title_text_rect );

  window_rect_window = drawing::GrowRect( window_rect_window, -1, -1 );

  // Draw the rest of the background
  drawing::FilledRectangle( hdc.GetValue(), window_rect_window,
                            themecolors::kTitlebarBackground );
}

INT_PTR HandleHitTest( const HWND window_handle, const LPARAM lparam ) {
  const POINT hit_point = { GET_X_LPARAM( lparam ), GET_Y_LPARAM( lparam ) };
  const RECT window_rect = control::GetWindowRect( window_handle );

  if ( !IsMaximized( window_handle ) ) {
    const bool is_left_side =
        hit_point.x >= window_rect.left &&
        hit_point.x <
            window_rect.left + themeconstants::kTitlebarBorderThickness;

    const bool is_top_side =
        hit_point.y >= window_rect.top &&
        hit_point.y <
            window_rect.top + themeconstants::kTitlebarBorderThickness;

    const bool is_bottom_side =
        hit_point.y >=
            window_rect.bottom - themeconstants::kTitlebarBorderThickness &&
        hit_point.y < window_rect.bottom;

    const bool is_right_side =
        hit_point.x >=
            window_rect.right - themeconstants::kTitlebarBorderThickness &&
        hit_point.x < window_rect.right;

    bool is_hitting_side = false;
    int32_t retval = 0;

    if ( is_left_side && is_top_side ) {
      retval = HTTOPLEFT;
      is_hitting_side = true;
    } else if ( is_left_side && is_bottom_side ) {
      retval = HTBOTTOMLEFT;
      is_hitting_side = true;
    } else if ( is_left_side && !is_top_side && !is_bottom_side ) {
      retval = HTLEFT;
      is_hitting_side = true;
    } else if ( is_right_side && is_top_side ) {
      retval = HTTOPRIGHT;
      is_hitting_side = true;
    } else if ( is_right_side && is_bottom_side ) {
      retval = HTBOTTOMRIGHT;
      is_hitting_side = true;
    } else if ( is_right_side && !is_top_side && !is_bottom_side ) {
      retval = HTRIGHT;
      is_hitting_side = true;
    } else if ( is_top_side && !is_right_side && !is_left_side ) {
      retval = HTTOP;
      is_hitting_side = true;
    } else if ( is_bottom_side && !is_right_side && !is_left_side ) {
      retval = HTBOTTOM;
      is_hitting_side = true;
    }

    // If we are hitting the side of the window, remove and down or hover status
    // from the buttons and update them
    if ( is_hitting_side ) {
      const auto& window = window::GetWindow( window_handle );

      SafeRegion whole_update_region = CreateRectRgn( 0, 0, 0, 0 );

      for ( auto& button : window->titlebar_buttons_ ) {
        if ( button.is_down || button.is_hovering ) {
          button.is_down = false;
          button.is_hovering = false;

          RECT button_rect =
              button.GetDrawRect( window_rect.right - window_rect.left );

          OffsetRect( &button_rect, window_rect.left, window_rect.top );

          SafeRegion button_update_region =
              CreateRectRgnIndirect( &button_rect );

          CombineRgn( whole_update_region.GetValue(),
                      whole_update_region.GetValue(),
                      button_update_region.GetValue(), RGN_OR );
        }
      }

      for ( auto& button : window->menubar_ ) {
        if ( button.is_down || button.is_hovering ) {
          button.is_down = false;
          button.is_hovering = false;

          RECT button_rect = button.GetDrawRect( 0 );

          OffsetRect( &button_rect, window_rect.left, window_rect.top );

          SafeRegion button_update_region =
              CreateRectRgnIndirect( &button_rect );

          CombineRgn( whole_update_region.GetValue(),
                      whole_update_region.GetValue(),
                      button_update_region.GetValue(), RGN_OR );
        }
      }

      if ( whole_update_region.GetValue() ) {
        // Tell the window to update the specified region on the window
        // non-client area. This avoids flickering.
        SendMessage( window_handle, WM_NCPAINT,
                     reinterpret_cast<WPARAM>( whole_update_region.GetValue() ),
                     NULL );
      }

      return retval;
    }
  }

  const auto& window = window::GetWindow( window_handle );

  auto& titlebar_buttons = window->titlebar_buttons_;

  bool is_hovering_over_any_button = false;

  const int32_t prev_hover_index = window->last_hover_index_;

  SafeRegion prev_update_region = nullptr;

  for ( size_t i = 0; i < titlebar_buttons.size(); ++i ) {
    const auto& button = titlebar_buttons[ i ];

    RECT button_rect =
        button.GetDrawRect( window_rect.right - window_rect.left );

    const POINT window_point =
        control::ScreenPointToWindowPoint( window_handle, hit_point );

    const bool in_rect = PtInRect( &button_rect, window_point );

    if ( in_rect )
      is_hovering_over_any_button = true;

    if ( in_rect && prev_hover_index != i ) {
      window->titlebar_buttons_[ i ].is_hovering = true;

      // If we have previously hovered over any button, then un-hover it and add
      // it to the update region
      if ( prev_hover_index >= 0 ) {
        window->titlebar_buttons_[ prev_hover_index ].is_hovering = false;

        RECT prev_hovered_button_rect =
            titlebar_buttons[ prev_hover_index ].GetDrawRect(
                window_rect.right - window_rect.left );

        // Offset the rect to screen position to create a region of the screen.
        OffsetRect( &prev_hovered_button_rect, window_rect.left,
                    window_rect.top );

        prev_update_region = CreateRectRgnIndirect( &prev_hovered_button_rect );
      }

      // Offset the rect to screen position to create a region of the screen
      // In other words, WindowRectToScreen
      OffsetRect( &button_rect, window_rect.left, window_rect.top );

      SafeRegion update_region = CreateRectRgnIndirect( &button_rect );

      if ( prev_update_region.GetValue() ) {
        CombineRgn( update_region.GetValue(), update_region.GetValue(),
                    prev_update_region.GetValue(), RGN_OR );
      }

      // Tell the window to update the specified region on the window non-client
      // area. This avoids flickering.
      SendMessage( window_handle, WM_NCPAINT,
                   reinterpret_cast<WPARAM>( update_region.GetValue() ), NULL );

      window->last_hover_index_ = i;

      break;
    }
  }

  ///

  const auto& menubar = window->menubar_;

  for ( size_t i = 0; i < menubar.size(); ++i ) {
    const auto& button = menubar[ i ];

    RECT button_rect = button.GetDrawRect( 0 );

    const POINT window_point =
        control::ScreenPointToWindowPoint( window_handle, hit_point );

    const bool in_rect = PtInRect( &button_rect, window_point );

    if ( in_rect )
      is_hovering_over_any_button = true;

    if ( in_rect && prev_hover_index != i ) {
      window->menubar_[ i ].is_hovering = true;

      // If we have previously hovered over any button, then un-hover it and add
      // it to the update region
      if ( prev_hover_index >= 0 ) {
        window->menubar_[ prev_hover_index ].is_hovering = false;

        RECT prev_hovered_button_rect =
            menubar[ prev_hover_index ].GetDrawRect( 0 );

        // Offset the rect to screen position to create a region of the screen.
        OffsetRect( &prev_hovered_button_rect, window_rect.left,
                    window_rect.top );

        prev_update_region = CreateRectRgnIndirect( &prev_hovered_button_rect );
      }

      // Offset the rect to screen position to create a region of the screen
      // In other words, WindowRectToScreen
      OffsetRect( &button_rect, window_rect.left, window_rect.top );

      SafeRegion update_region = CreateRectRgnIndirect( &button_rect );

      if ( prev_update_region.GetValue() ) {
        CombineRgn( update_region.GetValue(), update_region.GetValue(),
                    prev_update_region.GetValue(), RGN_OR );
      }

      // Tell the window to update the specified region on the window non-client
      // area. This avoids flickering.
      SendMessage( window_handle, WM_NCPAINT,
                   reinterpret_cast<WPARAM>( update_region.GetValue() ), NULL );

      window->last_hover_index_ = i;

      break;
    }
  }

  if ( is_hovering_over_any_button ) {
    for ( size_t i = 0; i < window->titlebar_buttons_.size(); ++i ) {
      if ( window->titlebar_buttons_[ i ].is_hovering ) {
        switch ( i ) {
          case TitlebarButtonIndexX:
          case TitlebarButtonIndexMaximize:
          case TitlebarButtonIndexMinimize:
            // We have to return HTBORDER because if the return anything else,
            // we will not be able to receive WM_NCLBUTTONUP for some reason.
            return HTBORDER;
          default:
            assert( false && "oops" );
            break;
        }
      }
    }

    for ( size_t i = 0; i < window->menubar_.size(); ++i ) {
      if ( window->menubar_[ i ].is_hovering ) {
        return HTBORDER;
      }
    }
  }

  const auto minimize_button_rect =
      titlebar_buttons[ TitlebarButtonIndexMinimize ].GetDrawRect(
          window_rect.right - window_rect.left );

  RECT titlebar_rect;
  titlebar_rect.left = window_rect.left;
  titlebar_rect.top = window_rect.top;
  titlebar_rect.bottom = titlebar_rect.top + themeconstants::kTitlebarHeight;
  titlebar_rect.right = titlebar_rect.left + minimize_button_rect.left;

  // Are we hovering on the caption part of the titlebar?
  if ( PtInRect( &titlebar_rect, hit_point ) ) {
    HandleMouseLeave( window_handle );
    return HTCAPTION;
  }

  // If the user clicks and hold down the left mouse button on any of the
  // titlebar buttons and moves the mouse away.
  for ( auto& button : window->titlebar_buttons_ ) {
    button.is_down = false;
  }

  // If we have not yet returned, then we are in the client area
  return HTCLIENT;
}

INT_PTR HandleLeftButtonDown( const int32_t hittest,
                              const HWND window_handle ) {
  if ( hittest == HTBORDER ) {
    const RECT window_rect = control::GetWindowRect( window_handle );
    const auto& window = window::GetWindow( window_handle );

    auto& titlebar_buttons = window->titlebar_buttons_;

    for ( size_t i = 0; i < titlebar_buttons.size(); ++i ) {
      auto& button = titlebar_buttons[ i ];
      if ( button.is_hovering ) {
        button.is_down = true;

        // NOTE: When I uncomment the line above, the WM_NCLBUTTONDOWN is never
        // sent to the window.
        // button.is_hovering = false;

        RECT button_rect =
            button.GetDrawRect( window_rect.right - window_rect.left );

        // Offset the rect to screen position to create a region of the screen
        OffsetRect( &button_rect, window_rect.left, window_rect.top );

        SafeRegion btn_region = CreateRectRgnIndirect( &button_rect );

        // Tell the window to update the specified region on the window
        // non-client area. This avoids flickering.
        SendMessage( window_handle, WM_NCPAINT,
                     reinterpret_cast<WPARAM>( btn_region.GetValue() ), NULL );
      }
    }

    auto& menubar_items = window->menubar_;

    for ( size_t i = 0; i < menubar_items.size(); ++i ) {
      auto& button = menubar_items[ i ];
      if ( button.is_hovering ) {
        button.is_down = true;

        // shows the context menu window that it created earlier

        // NOTE: When I uncomment the line above, the WM_NCLBUTTONDOWN is never
        // sent to the window.
        // button.is_hovering = false;

        /*
        RECT button_rect =
            button.GetDrawRect( window_rect.right - window_rect.left );

        // Offset the rect to screen position to create a region of the screen
        OffsetRect( &button_rect, window_rect.left, window_rect.top );

        SafeRegion btn_region = CreateRectRgnIndirect( &button_rect );

        // Tell the window to update the specified region on the window
        // non-client area. This avoids flickering.
        SendMessage( window_handle, WM_NCPAINT,
                     reinterpret_cast<WPARAM>( btn_region.GetValue() ), NULL );
        */
      }
    }
  }

  // We did not process the message, do not override default behaviour
  return FALSE;
}

void HandleLeftButtonUp( const HWND window_handle ) {
  const RECT window_rect = control::GetWindowRect( window_handle );

  const auto& window = window::GetWindow( window_handle );

  auto& titlebar_buttons = window->titlebar_buttons_;

  for ( size_t i = 0; i < titlebar_buttons.size(); ++i ) {
    auto& button = titlebar_buttons[ i ];

    if ( button.is_down ) {
      switch ( i ) {
        case TitlebarButtonIndexX:
          SendMessage( window_handle, WM_CLOSE, 0, 0 );
          break;
        case TitlebarButtonIndexMaximize:
          if ( IsMaximized( window_handle ) )
            ShowWindow( window_handle, SW_RESTORE );
          else
            ShowWindow( window_handle, SW_MAXIMIZE );
          break;
        case TitlebarButtonIndexMinimize:
          ShowWindow( window_handle, SW_MINIMIZE );
          break;
        default:
          break;
      }
    }

    button.is_down = false;

    RECT button_rect =
        button.GetDrawRect( window_rect.right - window_rect.left );

    // Offset the rect to screen position to create a region of the screen
    OffsetRect( &button_rect, window_rect.left, window_rect.top );

    SafeRegion btn_region = CreateRectRgnIndirect( &button_rect );

    // Tell the window to update the specified region on the window non-client
    // area. This avoids flickering.
    SendMessage( window_handle, WM_NCPAINT,
                 reinterpret_cast<WPARAM>( btn_region.GetValue() ), NULL );
  }
}

void HandleMouseLeave( const HWND window_handle ) {
  const RECT window_rect = control::GetWindowRect( window_handle );

  const auto& window = window::GetWindow( window_handle );
  const auto& titlebar_buttons = window->titlebar_buttons_;
  const auto& menubar_buttons = window->menubar_;

  window->last_hover_index_ = -1;

  for ( size_t i = 0; i < titlebar_buttons.size(); ++i ) {
    const auto& button = titlebar_buttons[ i ];

    if ( button.is_hovering || button.is_down ) {
      window->titlebar_buttons_[ i ].is_hovering = false;
      window->titlebar_buttons_[ i ].is_down = false;

      RECT button_rect =
          button.GetDrawRect( window_rect.right - window_rect.left );

      // Offset the rect to screen position to create a region of the screen
      OffsetRect( &button_rect, window_rect.left, window_rect.top );

      SafeRegion btn_region = CreateRectRgnIndirect( &button_rect );

      // Tell the window to update the specified region on the window non-client
      // area. This avoids flickering.
      SendMessage( window_handle, WM_NCPAINT,
                   reinterpret_cast<WPARAM>( btn_region.GetValue() ), NULL );
    }
  }

  for ( size_t i = 0; i < menubar_buttons.size(); ++i ) {
    const auto& button = menubar_buttons[ i ];

    if ( button.is_hovering || button.is_down ) {
      window->menubar_[ i ].is_hovering = false;
      window->menubar_[ i ].is_down = false;

      RECT button_rect = button.GetDrawRect( 0 );

      // Offset the rect to screen position to create a region of the screen
      OffsetRect( &button_rect, window_rect.left, window_rect.top );

      SafeRegion btn_region = CreateRectRgnIndirect( &button_rect );

      // Tell the window to update the specified region on the window non-client
      // area. This avoids flickering.
      SendMessage( window_handle, WM_NCPAINT,
                   reinterpret_cast<WPARAM>( btn_region.GetValue() ), NULL );
    }
  }
}

}  // namespace titlebar

}  // namespace gwingui