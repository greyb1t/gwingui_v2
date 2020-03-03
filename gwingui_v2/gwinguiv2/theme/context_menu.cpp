#include "pch.h"
#include "context_menu.h"

#include "../drawing.h"
#include "../config/themefont.h"

namespace gwingui {

namespace context_menu {

LRESULT CALLBACK MenuWindowProc( HWND window_handle,
                                 UINT message,
                                 WPARAM wparam,
                                 LPARAM lparam ) {
  // i AM GOING TO HAVE TO MAKE A NEW FILE FOR MENU ITEM
  // BECAUSE WE GONNA NEED HOVER AND STUFF

  switch ( message ) {
    case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint( window_handle, &ps );

      const auto menuitem = reinterpret_cast<MenuItem*>(
          GetWindowLongPtr( window_handle, GWLP_USERDATA ) );

      assert( menuitem );

      RECT item_rect = { 0 };

      item_rect.bottom = 30;
      item_rect.right = 100;

      for ( const auto child_menu_items : menuitem->menu_items ) {
        drawing::FilledRectangle( ps.hdc, item_rect, RGB( 0, 50, 150 ) );

        // move it down 40 pixels
        OffsetRect( &item_rect, 0, 40 );
      }

      EndPaint( window_handle, &ps );

      // we processed the message
      return 0;
    } break;

    case WM_NCHITTEST: {
      const auto mouse_pos = MAKEPOINTS( lparam );

    } break;

    case WM_CREATE: {
      const auto cs = reinterpret_cast<CREATESTRUCT*>( lparam );

      const auto temp_copy = reinterpret_cast<MenuItem*>( cs->lpCreateParams );

      // allocate a copy of the menuitem
      MenuItem* menuitem = new MenuItem( *temp_copy );

      SetWindowLongPtr( window_handle, GWLP_USERDATA,
                        reinterpret_cast<LONG_PTR>( menuitem ) );

      /*
      if ( menuitem->menu_items.size() > 0 ) {
        const auto sub_item_window_handle =
            CreateContextMenuWindow( *menuitem );
      }
      */

      // we processed the message, continue creation of window
      return 0;
    } break;

    case WM_DESTROY: {
      const auto menuitem = reinterpret_cast<MenuItem*>(
          GetWindowLongPtr( window_handle, GWLP_USERDATA ) );

      // free the resources that was allocated when we created the window
      delete menuitem;
    } break;

    default:
      break;
  }

  return DefWindowProc( window_handle, message, wparam, lparam );
}

HWND CreateContextMenuWindow( const MenuItem& menuitem ) {
  // TODO: find a better solution than getting hinstance like this lol
  const auto instance_handle =
      reinterpret_cast<HINSTANCE>( GetModuleHandle( 0 ) );

  WNDCLASSEX wcex = { 0 };

  // is the window class already created?
  if ( !GetClassInfoEx( instance_handle, MENU_WINDOW_CLASS_NAME, &wcex ) ) {
    wcex.cbSize = sizeof( wcex );
    wcex.lpfnWndProc = MenuWindowProc;
    wcex.hInstance = instance_handle;
    // TODO: replace loadcursor with loadimage
    wcex.hCursor = reinterpret_cast<HCURSOR>( LoadCursor(
        0, reinterpret_cast<LPCWSTR>( MAKEINTRESOURCE( IDC_ARROW ) ) ) );
    wcex.hbrBackground = CreateSolidBrush( RGB( 255, 0, 0 ) );
    wcex.lpszClassName = MENU_WINDOW_CLASS_NAME;

    ATOM atom = RegisterClassEx( &wcex );

    assert( atom );
  }

  size_t longest_text_length = 0;
  std::wstring longest_menu_text = TEXT( "" );

  // Get the longest string
  for ( const auto& item : menuitem.menu_items ) {
    if ( item.text.size() > longest_text_length ) {
      longest_text_length = item.text.size();
      longest_menu_text = item.text;
    }
  }

  const auto font_dims = drawing::GetFontTextDimensions(
      longest_menu_text, gwingui::themefont::GetDefaultThemeFont() );

  int menu_window_x = font_dims.cx * 2;

  // Adjust the window width size based on that

  const auto window_handle = CreateWindow(
      MENU_WINDOW_CLASS_NAME, TEXT( "" ), WS_POPUP, 0, 0, menu_window_x, 500,
      NULL, NULL, instance_handle, const_cast<MenuItem*>( &menuitem ) );

  assert( window_handle );

  ShowWindow( window_handle, SW_SHOW );

  return window_handle;
}

}  // namespace context_menu

}  // namespace gwingui