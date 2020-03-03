#include "pch.h"
#include "window.h"
#include "theme/titlebar.h"
#include "theme/titlebar_button_text_menu_item.h"

#include "../controls/control.h"

#include "controls/tabcontrol.h"
#include "../theme/tabcontrol_theme.h"
#include "../theme/context_menu.h"

//

#include "drawing.h"
#include "theme/theme.h"
#include "config/themefont.h"

namespace gwingui {

Window::Window()
    : resource_id_( 0 ),
      handle_( NULL ),
      last_hover_index_( -1 ),
      previous_scroll_x_( 0 ),
      previous_scroll_y_( 0 ),
      titlebar_buttons_{
        TitlebarButtonBitmap( themebitmaps::g_titlebar_x_normal,
                              themebitmaps::g_titlebar_x_hover,
                              themebitmaps::g_titlebar_x_down ),
        TitlebarButtonBitmap( themebitmaps::g_titlebar_maximize_normal,
                              themebitmaps::g_titlebar_maximize_hover,
                              themebitmaps::g_titlebar_maximize_down ),
        TitlebarButtonBitmap( themebitmaps::g_titlebar_minimize_normal,
                              themebitmaps::g_titlebar_minimize_hover,
                              themebitmaps::g_titlebar_minimize_down )
        // TitlebarButton( themebitmaps::g_titlebar_icon,
        //                themebitmaps::g_titlebar_icon,
        //                themebitmaps::g_titlebar_icon),
      } {
  // Initialize the titlebar buttons
  const auto titlebar_button_rects = titlebar::GetTitlebarButtonRects( 0 );

  for ( size_t i = 0; i < titlebar_buttons_.size(); ++i ) {
    titlebar_buttons_[ i ].rect = titlebar_button_rects[ i ];
    titlebar_buttons_[ i ].is_down = false;
    titlebar_buttons_[ i ].is_hovering = false;
  }
}

uint32_t Window::GetResourceId() const {
  return resource_id_;
}

HWND Window::GetHandle() const {
  return handle_;
}

void Window::AddTabWindow( const HWND tabcontrol_handle,
                           const uint32_t window_id,
                           const std::wstring& tab_name,
                           Window* window ) {
  tabcontrol::SetTabItemSize( tabcontrol_handle, 100, 35 );

  const auto tab_index = tabcontrol::GetTabCount( tabcontrol_handle );

  tabcontrol::AddTab( tabcontrol_handle, tab_name,
                      tabcontrol::GetTabCount( tabcontrol_handle ) );

  const HWND tab_window = window_manager_.AddWindow(
      window_id, tabcontrol_handle, gui::ChildDialogProc, window );

  TCITEM tci;
  tci.mask = TCIF_PARAM;
  tci.lParam = reinterpret_cast<LPARAM>( tab_window );

  // Represents the associated window to be displayed for that specific tabitem
  SendMessage( tabcontrol_handle, TCM_SETITEM, tab_index,
               reinterpret_cast<LPARAM>( &tci ) );

  // POSITION THE TAB WINDOWS PROPERLY, MAKE THIS TO ITS OWN FUNCTION?

  RECT tabitem_rect;
  TabCtrl_GetItemRect( tabcontrol_handle, 0, &tabitem_rect );

  RECT rect;
  GetClientRect( tab_window, &rect );

  POINT pos;
  pos.x = rect.left + 1;
  pos.y = rect.top + tabitem_rect.bottom + 2;

  // Set the position correctly for the tabwindow
  control::SetControlPosition( tab_window, pos );

  // After changing the window position, we have to hide the window because it
  // shows it when changing the position, shitty.
  ShowWindow( tab_window, SW_HIDE );

  SIZE tabcontrol_size = control::GetControlSize( tabcontrol_handle );

  // TODO: Don't know how good these magic numbers are with other windows
  // versions, fuck it tho ;)
  tabcontrol_size.cx -= 2;
  tabcontrol_size.cy -= ( tabitem_rect.bottom - tabitem_rect.top );
  tabcontrol_size.cy -= 5;

  control::SetControlSize( tab_window, tabcontrol_size );

  // If the tab is the first tab the user has added into the tab control, we
  // show the window
  if ( tab_index == 0 ) {
    ShowWindow( tab_window, SW_SHOW );
    // Represents the last displayed window for the whole tab control
    auto tabcontrol_data = reinterpret_cast<tabcontroltheme::TabControlData*>(
        GetWindowLongPtr( tabcontrol_handle, GWLP_USERDATA ) );
    tabcontrol_data->prev_window_handle = tab_window;
  }
}

// GetMenulist creates a menu list struture based on a real win32 menu
context_menu::MenuList GetMenulist( const HMENU menu_handle ) {
  context_menu::MenuList menu_list;

  const auto menu_item_count = GetMenuItemCount( menu_handle );

  if ( menu_item_count > 0 ) {
    for ( int i = 0; i < menu_item_count; ++i ) {
      wchar_t buf[ MAX_PATH ]{ 0 };

      MENUITEMINFO mii = { 0 };
      mii.cbSize = sizeof( mii );
      mii.fMask = MIIM_STRING | MIIM_FTYPE | MIIM_SUBMENU;
      mii.fType = MFT_STRING;
      mii.dwTypeData = 0;

      // Get the size of the string
      if ( !GetMenuItemInfo( menu_handle, i, TRUE, &mii ) )
        continue;

      mii.cch += 1;
      mii.dwTypeData = buf;

      assert( mii.cch < sizeof( buf ) );

      // Get the string into the buffer
      if ( !GetMenuItemInfo( menu_handle, i, TRUE, &mii ) )
        continue;

      context_menu::MenuItem menu_item;
      menu_item.text = buf;

      if ( ( mii.fType & MFT_SEPARATOR ) == MFT_SEPARATOR )
        menu_item.type = context_menu::MenuItemType::kSeperator;
      else
        menu_item.type = context_menu::MenuItemType::kTextOnly;

      if ( mii.hSubMenu ) {
        const auto sub_menu_list = GetMenulist( mii.hSubMenu );
        menu_item.menu_items = sub_menu_list;
      }

      menu_list.push_back( menu_item );
    }
  }

  return menu_list;
}

void Window::SetupMenu( const HMENU menu_handle ) {
  const auto menulist = GetMenulist( menu_handle );

  SafeReleaseDC hdc = GetWindowDC( handle_ );

  // Select the font used to draw the menu bar text
  SafeSelectObject select_font( hdc.GetValue(),
                                gwingui::themefont::GetDefaultThemeFont() );

  constexpr int32_t kTopButtonPadding = 1;

  // Specifies the first x position of the first menu item
  int menu_item_x = 28;

  for ( const auto& menuitem : menulist ) {
    auto menu_titlebar_item = TitlebarButtonTextMenuItem( menuitem.text );

    const auto& menuitem_text = menuitem.text;
    const auto font_size =
        drawing::GetSelectedFontTextDimensions( hdc.GetValue(), menuitem_text );

    RECT menu_bar_item_rect = { 0 };
    menu_bar_item_rect.top = kTopButtonPadding;
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

    menu_titlebar_item.text_rect_ = text_rect;
    menu_titlebar_item.rect = menu_bar_item_rect;

    const auto menu_window_handle = CreateContextMenuWindow( menuitem );

    // ShowWindow( menu_window_handle, SW_SHOW );

    menubar_.push_back( menu_titlebar_item );

    menu_item_x += ( menu_bar_item_rect.right - menu_bar_item_rect.left );
  }

  select_font.Reset();
}

namespace window {

Window* GetWindow( const HWND window_handle ) {
  for ( const auto& window : Gui::window_manager_.windows_ ) {
    if ( window->handle_ == window_handle )
      return window.get();

    for ( const auto& child_window : window->window_manager_.windows_ ) {
      if ( child_window->handle_ == window_handle )
        return child_window.get();
    }
  }

  return nullptr;
}

}  // namespace window

}  // namespace gwingui