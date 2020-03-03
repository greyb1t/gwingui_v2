#include "pch.h"
#include "header_theme.h"
#include "../utils/safe_handles.h"
#include "../config/themefont.h"
#include "../config/themecolors.h"
#include "../drawing.h"
#include "../controls/control.h"
#include "../config/themecolors.h"
#include "theme.h"

LRESULT gwingui::headertheme::OnCustomDraw( HWND parent_window_handle,
                                            NMCUSTOMDRAW* custom_draw ) {
  switch ( custom_draw->dwDrawStage ) {
    case CDDS_PREPAINT: {
      control::SetReturnValue( parent_window_handle, CDRF_NOTIFYITEMDRAW );
      return CDRF_NOTIFYITEMDRAW;
    } break;

    case CDDS_ITEMPREPAINT: {
      // TODO: Continue fixing the WM_SETFONT for the header of the list view

      RECT header_item_rect = custom_draw->rc;
      const HDC hdc = custom_draw->hdc;

      wchar_t text_buf[ MAX_PATH ];

      HDITEM header_item;
      header_item.mask = HDI_TEXT;
      header_item.pszText = text_buf;
      header_item.cchTextMax = MAX_PATH;

      SendMessage( custom_draw->hdr.hwndFrom, HDM_GETITEM,
                   custom_draw->dwItemSpec,
                   reinterpret_cast<LPARAM>( &header_item ) );

      SafeSelectObject selected_object( hdc, themefont::GetDefaultThemeFont() );

      const SIZE text_size =
          drawing::GetSelectedFontTextDimensions( hdc, text_buf );

      const auto text_pos =
          theme::GetCenterPosition( header_item_rect, text_size );

      RECT text_rect;
      text_rect.left = text_pos.x;
      text_rect.top = text_pos.y;
      text_rect.right = text_rect.left + text_size.cx;
      text_rect.bottom = text_rect.top + text_size.cy;

      SetBkColor( hdc, themecolors::header::kItemBackground );

      drawing::Text( hdc, text_pos.x, text_pos.y, themecolors::header::kText,
                     text_buf );

      drawing::ExcludeRectFromDC( hdc, text_rect );

      drawing::BorderRectangle( hdc, header_item_rect,
                                themecolors::header::kItemBorder );

      header_item_rect = drawing::GrowRect( header_item_rect, -1, -1 );

      const LRESULT total_columns =
          SendMessage( custom_draw->hdr.hwndFrom, HDM_GETITEMCOUNT, 0, 0 );

      if ( ( total_columns - 1 ) != custom_draw->dwItemSpec )
        header_item_rect.right += 1;

      drawing::FilledRectangle( hdc, header_item_rect,
                                themecolors::header::kItemBackground );

      selected_object.Reset();

      control::SetReturnValue( parent_window_handle, CDRF_SKIPDEFAULT );
      return CDRF_SKIPDEFAULT;
    } break;

    default:
      break;
  }

  control::SetReturnValue( parent_window_handle, CDRF_SKIPDEFAULT );
  return CDRF_SKIPDEFAULT;
}