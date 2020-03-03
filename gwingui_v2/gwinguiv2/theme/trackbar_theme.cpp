#include "pch.h"
#include "trackbar_theme.h"
#include "../controls/control.h"
#include "../drawing.h"
#include "../config/themecolors.h"

LRESULT gwingui::trackbartheme::OnCustomDraw( const HWND parent_handle,
                                              NMCUSTOMDRAW* custom_draw ) {
  /*
    Drawing order:
    0x00000001 CDDS_PREPAINT
    0x00010001 CDDS_ITEMPREPAINT
    0x00010001 CDDS_ITEMPREPAINT
    0x00010001 CDDS_ITEMPREPAINT
    0x00000002 CDDS_POSTPAINT
  */

  switch ( custom_draw->dwDrawStage ) {
    case CDDS_PREPAINT:
      control::SetReturnValue( parent_handle, CDRF_NOTIFYITEMDRAW );
      return CDRF_NOTIFYITEMDRAW;

    case CDDS_ITEMPREPAINT: {
      switch ( custom_draw->dwItemSpec ) {
        case TBCD_CHANNEL: {
          drawing::FilledRectangle( custom_draw->hdc, custom_draw->rc,
                                    themecolors::trackbar::kChannelBorder );

          custom_draw->rc = drawing::GrowRect( custom_draw->rc, -1, -1 );

          drawing::FilledRectangle( custom_draw->hdc, custom_draw->rc,
                                    themecolors::trackbar::kChannel );

          control::SetReturnValue( parent_handle, CDRF_SKIPDEFAULT );
          return CDRF_SKIPDEFAULT;
        } break;

        case TBCD_THUMB: {
          POINT p;
          GetCursorPos( &p );
          ScreenToClient( custom_draw->hdr.hwndFrom, &p );

          COLORREF thumb_color = themecolors::trackbar::kThumb;

          if ( custom_draw->uItemState != 0 ) {
            // if pressed down
            thumb_color = themecolors::trackbar::kThumbFocus;
          } else if ( PtInRect( &custom_draw->rc, p ) ) {
            // if hover over thumb
            thumb_color = themecolors::trackbar::kThumbHover;
          }

          drawing::FilledRectangle( custom_draw->hdc, custom_draw->rc,
                                    themecolors::trackbar::kThumbBorder );

          custom_draw->rc = drawing::GrowRect( custom_draw->rc, -1, -1 );

          drawing::FilledRectangle( custom_draw->hdc, custom_draw->rc,
                                    thumb_color );

          control::SetReturnValue( parent_handle, CDRF_SKIPDEFAULT );
          return CDRF_SKIPDEFAULT;
        } break;
      }

      break;
    } break;

    default:
      break;
  }

  // DLGRETVAL(parent_handle, CDRF_SKIPDEFAULT);
  control::SetReturnValue( parent_handle, CDRF_SKIPDEFAULT );
  return CDRF_SKIPDEFAULT;
}

void gwingui::trackbartheme::InitializeTrackbar( HWND trackbar_handle ) {
  // Removes the focus dotted rectangle around the control
  SendMessage( trackbar_handle, WM_UPDATEUISTATE,
               MAKEWPARAM( UIS_SET, UISF_HIDEFOCUS ), 0 );
}