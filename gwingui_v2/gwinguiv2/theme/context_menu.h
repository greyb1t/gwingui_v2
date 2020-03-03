#pragma once

#define MENU_WINDOW_CLASS_NAME ( TEXT( "menushitdick" ) )

namespace gwingui {

namespace context_menu {

enum class MenuItemType {
  kTextOnly,
  kCheckAndText,
  kSeperator,
};

struct MenuItem;

using MenuList = std::vector<MenuItem>;

struct MenuItem {
  std::wstring text;
  MenuItemType type;
  MenuList menu_items;
};

LRESULT CALLBACK MenuWindowProc( HWND window_handle,
                                 UINT message,
                                 WPARAM wparam,
                                 LPARAM lparam );

HWND CreateContextMenuWindow( const MenuItem& menuitem );

}  // namespace context_menu

}  // namespace gwingui