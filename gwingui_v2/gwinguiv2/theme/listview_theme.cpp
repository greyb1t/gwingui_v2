#include "pch.h"
#include "listview_theme.h"
#include "../controls/control.h"
#include "../config/themefont.h"
#include "../config/themecolors.h"
#include "../controls/listview.h"
#include "../drawing.h"
#include "theme.h"
#include "../utils/mouse_tracker.h"

void gwingui::listviewtheme::InitializeListview( const HWND listview_handle ) {
  // TODO: In the future, add support to click and resize the header
  const HWND header_handle = reinterpret_cast<HWND>(
      SendMessage( listview_handle, LVM_GETHEADER, 0, 0 ) );

  control::SetStyles( header_handle, WS_CHILDWINDOW | WS_VISIBLE | HDS_HORZ |
                                         HDS_DRAGDROP |
                                         HDS_FULLDRAG /*| HDS_NOSIZING */ );

  control::SetFont( listview_handle, themefont::GetDefaultThemeFont() );

  SendMessage( listview_handle, LVM_SETEXTENDEDLISTVIEWSTYLE, 0,
               LVS_EX_DOUBLEBUFFER );

  // Required to make sure that the background is not white when the listview is
  // disabled
  listview::EnableFullRowSelect( listview_handle );

  SendMessage( listview_handle, LVM_SETBKCOLOR, 0,
               themecolors::listview::kBackground );

  default_listview_proc = reinterpret_cast<WNDPROC>(
      SetWindowLongPtr( listview_handle, GWLP_WNDPROC,
                        reinterpret_cast<LONG_PTR>( ListViewThemeProc ) ) );

  SetClassLongPtr( listview_handle, GCLP_HBRBACKGROUND,
                   ( LONG_PTR )CreateSolidBrush( RGB( 255, 0, 0 ) ) );
}

void gwingui::listviewtheme::OnOwnerDraw( const DRAWITEMSTRUCT* dis ) {
  switch ( dis->itemAction ) {
    case ODA_DRAWENTIRE: {
      const HWND listview_handle = dis->hwndItem;
      const HDC hdc = dis->hDC;
      const int32_t item_index = dis->itemID;
      const RECT& item_rect = dis->rcItem;

      const int32_t selected_item_index =
          listview::GetSelectedItem( listview_handle );

      COLORREF item_background_color;

      // Issue: When removing an item from a listview, the ownerdraw in not
      // written to support two different colors on the item background,
      // therefore a glitch occurs because they exclude some crucial parts of an
      // item from the hdc received in ownerdraw. The solution is to simply just
      // have on color for both the odd and even item.

      // Ensure that the color of the item is based on the index
      if ( item_index % 2 )
        item_background_color = themecolors::listview::kItemEven;
      else
        item_background_color = themecolors::listview::kItemEven;

      COLORREF text_color = themecolors::listview::kText;

      if ( item_index == selected_item_index ) {
        item_background_color = themecolors::listview::kItemFocus;
        text_color = themecolors::listview::kTextFocus;
      }

      if ( !control::IsEnabled( listview_handle ) ) {
        item_background_color = themecolors::listview::kItemDisabled;
        text_color = themecolors::listview::kTextDisabled;

        if ( item_index == ( listview::GetItemCount( listview_handle ) - 1 ) ) {
          RECT client_rect;
          GetClientRect( listview_handle, &client_rect );

          RECT empty_bottom_rect;
          empty_bottom_rect.top = item_rect.bottom;
          empty_bottom_rect.bottom = client_rect.bottom;
          empty_bottom_rect.left = client_rect.left;
          empty_bottom_rect.right = client_rect.right;

          drawing::FilledRectangle( hdc, empty_bottom_rect,
                                    themecolors::listview::kItemDisabled );
        }
      }

      drawing::FilledRectangle( hdc, item_rect, item_background_color );

      SetBkColor( hdc, item_background_color );

      SafeSelectObject selected_font( hdc,
                                      control::GetFont( listview_handle ) );

      const std::wstring text =
          listview::GetItemText( listview_handle, item_index, 0 );

      SIZE text_size = drawing::GetSelectedFontTextDimensions( hdc, text );

      // const uint32_t item_height = item_rect.bottom - item_rect.top;

      RECT item_rect_copy = item_rect;

      /*
      RECT r2 = { 0 };
      r2.left = LVIR_BOUNDS;
      SendMessage( listview_handle, LVM_GETITEMRECT, item_index,
                   ( LPARAM )&r2 );
      */

      /*
      RECT sub_item_rect;
      ListView_GetSubItemRect( listview_handle, item_index, 1, LVIR_BOUNDS,
                               &sub_item_rect );

      item_rect_copy.right = sub_item_rect.left;
      */

      const int32_t column_count = listview::GetColumnCount( listview_handle );

      if ( column_count > 1 ) {
        RECT sub_item_rect;
        ListView_GetSubItemRect( listview_handle, item_index, 1, LVIR_BOUNDS,
                                 &sub_item_rect );

        item_rect_copy.right = sub_item_rect.left;
      }

      const auto text_pos =
          theme::GetCenterPosition( item_rect_copy, text_size );

      drawing::Text( hdc, text_pos.x, text_pos.y, text_color, text );

      int subitem_index = 1;
      std::wstring subitem_text =
          listview::GetItemText( listview_handle, item_index, subitem_index );

      // TODO: Change this loop to a for loop by using a function to get the
      // total subitems if possible
      while ( !subitem_text.empty() ) {
        RECT subitem_rect;
        ListView_GetSubItemRect( listview_handle, item_index, subitem_index,
                                 LVIR_BOUNDS, &subitem_rect );

        subitem_rect.right -= 1;

        // subitem_rect.left = item_rect_copy.right;

        drawing::FilledRectangle( hdc, subitem_rect, item_background_color );

        SIZE subitem_text_size =
            drawing::GetSelectedFontTextDimensions( hdc, subitem_text );

        subitem_rect.right += 1;

        const auto subitem_text_pos =
            theme::GetCenterPosition( subitem_rect, subitem_text_size );

        drawing::Text( hdc, subitem_text_pos.x, subitem_text_pos.y, text_color,
                       subitem_text );

        ++subitem_index;

        subitem_text =
            listview::GetItemText( listview_handle, item_index, subitem_index );
      }

      selected_font.Reset();
    } break;
  }
}

LRESULT gwingui::listviewtheme::ListViewThemeProc( HWND listview_handle,
                                                   UINT message,
                                                   WPARAM wparam,
                                                   LPARAM lparam ) {
  GetMouseTracker().ReportHoverLeaveEvents( listview_handle, message );

  switch ( message ) {
    case WM_ENABLE: {
      control::UpdateControl( listview_handle );
    } break;

    //case WM_SIZE: {
    //  // listview::SetColumnWidthAutoSize(
    //  //     listview_handle, listview::GetColumnCount( listview_handle ) - 1 );
    //  const auto new_width = LOWORD( lparam );
    //  const auto new_height = HIWORD( lparam );

    //  const auto column_count = listview::GetColumnCount( listview_handle );

    //  RECT rect;
    //  GetWindowRect( listview_handle, &rect );

    //  int total = 0;

    //  for ( int i = 0; i < column_count - 1; ++i ) {
    //    const auto column_width =
    //        listview::GetColumnWidth( listview_handle, i );

    //    total += column_width;
    //  }

    //  listview::SetColumnWidth(
    //      listview_handle, column_count - 1,
    //      rect.right - rect.left - total - column_count - 1 );
    //} break;

    case WM_ERASEBKGND: {
      // If the listview gets disabled, then we draw the background area to the
      // background color, otherwise it would be white by default
      if ( !control::IsEnabled( listview_handle ) ) {
        if ( listview::GetItemCount( listview_handle ) == 0 ) {
          RECT rect;
          GetClientRect( listview_handle, &rect );

          SafeReleaseDC hdc = GetDC( listview_handle );

          HWND header_handle = ListView_GetHeader( listview_handle );
          RECT header_rect;
          Header_GetItemRect( header_handle, 0, &header_rect );

          rect.top = header_rect.bottom;

          drawing::FilledRectangle( hdc.GetValue(), rect,
                                    themecolors::listview::kItemDisabled );

          return 0;
        }
      }

      // return 0;
    } break;

    case WM_NCPAINT: {
      return theme::HandleListNcPaint( listview_handle, message, wparam, lparam,
                                       themecolors::listview::kBorder,
                                       themecolors::listview::kBorderDisabled,
                                       themecolors::listview::kBackground,
                                       themecolors::listview::kBackground );
    } break;

    default:
      break;
  }

  return CallWindowProc( default_listview_proc, listview_handle, message,
                         wparam, lparam );
}