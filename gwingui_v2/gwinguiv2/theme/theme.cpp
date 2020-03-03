#include "pch.h"
#include "theme.h"

#include "../gui.h"

#include "button_theme.h"
#include "checkbox_theme.h"
#include "radiobutton_theme.h"
#include "groupbox_theme.h"
#include "editcontrol_theme.h"
#include "combobox_theme.h"
#include "listbox_theme.h"
#include "listview_theme.h"
#include "trackbar_theme.h"
#include "richedit_theme.h"
#include "tabcontrol_theme.h"
#include "progressbar_theme.h"
#include "hotkey_theme.h"

#include "../config/themefont.h"

#include "../controls/control.h"

#include "../utils/safe_handles.h"
#include "../drawing.h"

void gwingui::theme::InitializeControls( const HWND window_handle ) {
  HWND handle = GetTopWindow( window_handle );

  // For each control in this window
  while ( handle ) {
    const auto window_type = control::GetWindowType( handle );

    switch ( window_type ) {
      case WindowType::kButton:
        gwingui::buttontheme::InitializeControl( handle );
        break;
      case WindowType::kEdit:
        gwingui::editcontroltheme::InitializeEditControl( handle );
        break;
      case WindowType::kCheckBox:
        checkboxtheme::InitializeCheckbox( handle );
        break;
      case WindowType::kGroupBox:
        groupboxtheme::InitializeGroupbox( handle );
        break;
      case WindowType::kRadioButton:
        radiobuttontheme::InitializeRadiobutton( handle );
        break;
      case WindowType::kCombobox:
        comboboxtheme::InitializeCombobox( handle );
        break;
      case WindowType::kListbox:
        listboxtheme::InitializeListbox( handle );
        break;
      case WindowType::kListView:
        listviewtheme::InitializeListview( handle );
        break;
      case WindowType::kTrackbar:
        trackbartheme::InitializeTrackbar( handle );
        break;
      case WindowType::kRichEdit20:
        richedittheme::InitializeRichedit( handle );
        break;
      case WindowType::kTabControl:
        tabcontroltheme::Initialize( handle );
        break;
      case WindowType::kStaticText: {
        const auto font_handle = themefont::GetDefaultThemeFont();
        control::SetFont( handle, font_handle );
      } break;
      case WindowType::kProgressBar:
        progressbartheme::InitializeControl( handle );
        break;
      case WindowType::kHotKey:
        hotkeytheme::InitializeControl( handle );
        break;
      default:
        break;
    }

    handle = GetNextWindow( handle, GW_HWNDNEXT );
  }
}

bool gwingui::theme::EnableVisualStyles() {
  TCHAR shell32_path[ MAX_PATH ];

  GetSystemDirectory( shell32_path, MAX_PATH );

  ACTCTX actCtx = { 0 };

  actCtx.cbSize = sizeof( actCtx );
  actCtx.dwFlags =
      ACTCTX_FLAG_RESOURCE_NAME_VALID | ACTCTX_FLAG_ASSEMBLY_DIRECTORY_VALID;
  actCtx.lpSource = TEXT( "shell32.dll" );
  actCtx.lpAssemblyDirectory = shell32_path;
  actCtx.lpResourceName = ( LPCTSTR )124;

  auto ctx_handle = CreateActCtx( &actCtx );

  if ( ctx_handle == INVALID_HANDLE_VALUE )
    return false;

  ULONG_PTR cookie;
  return ActivateActCtx( ctx_handle, &cookie );
}

POINT gwingui::theme::GetCenterPosition( const RECT& rect,
                                         const SIZE text_size ) {
  return { ( rect.right - rect.left ) / 2 - text_size.cx / 2 + rect.left,
           ( rect.bottom - rect.top ) / 2 - text_size.cy / 2 + rect.top };
}
LRESULT gwingui::theme::HandleListNcPaint(
    const HWND list_handle,
    const uint32_t message,
    const WPARAM wparam,
    const LPARAM lparam,
    const COLORREF border_color_in,
    const COLORREF border_color_disabled,
    const COLORREF inner_border_color_in,
    const COLORREF inner_border_color_disabled ) {
  // Ensures that the default scrollbar is drawn before drawing my own shit
  DefWindowProc( list_handle, message, wparam, lparam );

  SafeReleaseDC hdc = GetWindowDC( list_handle );

  SIZE control_size = control::GetControlSize( list_handle );

  RECT control_rect;
  control_rect.right = control_size.cx;
  control_rect.bottom = control_size.cy;
  control_rect.top = 0;
  control_rect.left = 0;

  COLORREF border_color = border_color_in;
  COLORREF inner_border_color = inner_border_color_in;

  if ( !control::IsEnabled( list_handle ) ) {
    border_color = border_color_disabled;
    inner_border_color = inner_border_color_disabled;
  }

  // TODO: Support horizontal scroll in future
  // Currently only supports vertical scrolls on the right side
  const bool has_vertical_scroll =
      control::HasStyles( list_handle, WS_VSCROLL );

  const bool has_horizontal_scroll =
      control::HasStyles( list_handle, WS_HSCROLL );

  if ( has_vertical_scroll || has_horizontal_scroll ) {
    SCROLLBARINFO sb_info;
    sb_info.cbSize = sizeof( sb_info );

    const BOOL sb_info_result = GetScrollBarInfo(
        list_handle, has_vertical_scroll ? OBJID_VSCROLL : OBJID_VSCROLL,
        &sb_info );

    if ( sb_info_result ) {
      constexpr auto kListBoxBorderWidth = 2;

      // Shrink the control rect to remove the borders from the rect
      const RECT control_rect_no_borders = drawing::GrowRect(
          control_rect, -kListBoxBorderWidth, -kListBoxBorderWidth );

      // Exclude whole listbox except the borders
      drawing::ExcludeRectFromDC( hdc.GetValue(), control_rect_no_borders );
    }
  }

  // Draw the outer border
  drawing::BorderRectangle( hdc.GetValue(), control_rect, border_color );

  control_rect = drawing::GrowRect( control_rect, -1, -1 );

  // Draw the inner border
  drawing::BorderRectangle( hdc.GetValue(), control_rect, inner_border_color );

  // When returning FALSE from this NCPAINT, we prevent the scroll bar
  // from being drawn. We can fix that by creating our own scroll bar using
  // the "bad way" written in
  // https://www.gamedev.net/forums/topic/452471-trying-to-create-a-custom-scrollbar-with-c-in-a-win32-enviromentno-mfc/

  return FALSE;
};
