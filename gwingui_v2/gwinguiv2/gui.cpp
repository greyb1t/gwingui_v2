#include "pch.h"
#include "gui.h"

#include "window.h"

#include "config/theme_constants.h"
#include "config/themecolors.h"
#include "config/themefont.h"
#include "drawing.h"
#include "theme/titlebar.h"
#include "window_manager.h"
#include "message_box.h"

#include "controls/control.h"
#include "controls/tabcontrol.h"
#include "controls/trackbar.h"
#include "controls/listbox.h"

#include "theme/button_theme.h"
#include "theme/editcontrol_theme.h"
#include "theme/combobox_theme.h"
#include "theme/listbox_theme.h"
#include "theme/listview_theme.h"
#include "theme/header_theme.h"
#include "theme/trackbar_theme.h"
#include "theme/tabcontrol_theme.h"
#include "theme/theme.h"

#pragma comment( lib, "Comctl32.lib" )

namespace gwingui {

WindowManager Gui::window_manager_;
HINSTANCE Gui::instance_;

// Centers a window position relative to another window
void CenterWindowPositionTo( const HWND window_handle,
                             const HWND relative_window_handle ) {
  const RECT parent_rect = control::GetWindowRect( relative_window_handle );
  const auto p = theme::GetCenterPosition(
      parent_rect, control::GetControlSize( window_handle ) );
  control::SetControlPosition( window_handle, p );
}

Gui::Gui( const HINSTANCE instance ) {
  instance_ = instance;

  // Ensure that the proper DLL that are required are loaded
  INITCOMMONCONTROLSEX iccex;
  iccex.dwSize = sizeof( iccex );
  iccex.dwICC = ICC_WIN95_CLASSES | ICC_STANDARD_CLASSES;

  InitCommonControlsEx( &iccex );

  // Initialize the richedit usage before creating any window
  // If we create any window containing a richedit before this, it will fail
  // without any error message
  HMODULE msftedit = LoadLibrary( TEXT( "Msftedit.dll" ) );
  assert( msftedit );
  HMODULE riched20 = LoadLibrary( TEXT( "Riched20.dll" ) );
  assert( riched20 );

  // Gives the default controls hover functionality and such, can't bother to do
  // that myself
  auto vis_ret = theme::EnableVisualStyles();
  assert( vis_ret );

  // Solves the blurry controls when using the gui on laptops
  SetProcessDpiAwarenessContext( DPI_AWARENESS_CONTEXT_SYSTEM_AWARE );
}

HWND Gui::AddWindow( const uint32_t window_id,
                     const HWND parent_handle,
                     Window* window ) {
  return window_manager_.AddWindow( window_id, parent_handle,
                                    gui::DefaultDialogProc, window );
}

HWND HandleMouseWheel( const HWND window_handle, const WPARAM wparam ) {
  SCROLLINFO scroll_info;
  scroll_info.cbSize = sizeof( scroll_info );
  scroll_info.fMask = SIF_ALL;
  GetScrollInfo( window_handle, SB_VERT, &scroll_info );

  const int scroll_delta = GET_WHEEL_DELTA_WPARAM( wparam );

  SCROLLBARINFO scrollbar_info;
  scrollbar_info.cbSize = sizeof( scrollbar_info );
  GetScrollBarInfo( window_handle, OBJID_VSCROLL, &scrollbar_info );

  const auto scrollbar_state = scrollbar_info.rgstate[ 0 ];

  const bool is_scrollbar_invisible = scrollbar_state & STATE_SYSTEM_INVISIBLE;

  if ( scroll_delta > 0 ) {
    // if we scroll up

    if ( scroll_info.nPos == 0 || is_scrollbar_invisible ) {
      // if true, scrol the parent instead
      // return GetParent( window_handle );
      // here, check if the parent can scroll, if not, check if that
      // parent can scroll
      // if true, scrol the parent instead
      auto parent = GetParent( window_handle );

      if ( parent ) {
        // here, check if the parent can scroll, if not, check if that
        // parent can scroll
        return HandleMouseWheel( parent, wparam );
      }
    } else {
      // If we can scroll more, return the scrollable window handle
      return window_handle;
    }
  } else {
    // if we scroll down

    // have we reached the max scrolling downwards?
    if ( ( scroll_info.nPos + scroll_info.nPage ) == ( scroll_info.nMax + 1 ) ||
         is_scrollbar_invisible ) {
      // if true, scrol the parent instead
      auto parent = GetParent( window_handle );

      if ( parent ) {
        // here, check if the parent can scroll, if not, check if that
        // parent can scroll
        return HandleMouseWheel( parent, wparam );
      }
    } else {
      // If we can scroll more, return the scrollable window handle
      return window_handle;
    }
  }

  return 0;
}

void Gui::Run() {
  MSG msg;
  BOOL message_ret = FALSE;

  while ( ( message_ret = GetMessage( &msg, nullptr, 0, 0 ) ) != 0 ) {
    if ( message_ret != -1 ) {
      // If the user is scrolling, the code below is a quality of life change
      if ( msg.message == WM_MOUSEWHEEL ) {
        msg.hwnd = HandleMouseWheel( msg.hwnd, msg.wParam );
      }

      TranslateMessage( &msg );
      DispatchMessage( &msg );
    } else {
      assert( false && "GetMessage returns -1" );
    }
  }
}

void OnListViewNotify( const NMHDR& notify_header,
                       Window* window,
                       const WPARAM wparam,
                       const LPARAM lparam ) {
  switch ( notify_header.code ) {
    case LVN_DELETEITEM:
      window->OnListViewItemDeleted( reinterpret_cast<NMLISTVIEW*>( lparam ) );
      break;
    case LVN_INSERTITEM:
      window->OnListViewItemAdded( reinterpret_cast<NMLISTVIEW*>( lparam ) );
      break;
    case NM_CLICK:
      window->OnListViewClick( reinterpret_cast<NMITEMACTIVATE*>( lparam ) );
      break;
    case NM_DBLCLK:
      window->OnListViewDoubleClick(
          reinterpret_cast<NMITEMACTIVATE*>( lparam ) );
    case LVN_ITEMCHANGED:
      window->OnListViewItemSelectionChanged(
          reinterpret_cast<NMLISTVIEW*>( lparam ) );
      break;
      break;
    default:
      break;
  }
}

void OnListboxNotify( const LPARAM lparam,
                      const WPARAM wparam,
                      Window* window ) {
  switch ( HIWORD( wparam ) ) {
    case LBN_DBLCLK:
      window->OnListBoxDoubleClick();
      break;
    case LBN_SELCHANGE:
      window->OnListBoxSelectionChanged( LOWORD( wparam ), ( HWND )lparam );
      break;
    default:
      break;
  }
}

void OnRicheditNotify( const LPARAM lparam,
                       const WPARAM wparam,
                       Window* window ) {
  switch ( HIWORD( wparam ) ) {
    case EN_CHANGE:
      window->OnRichEditTextChanged( LOWORD( wparam ),
                                     ( CHANGENOTIFY* )lparam );
      break;
    default:
      break;
  }
}

void OnComboboxNotify( const LPARAM lparam,
                       const WPARAM wparam,
                       Window* window ) {
  switch ( HIWORD( wparam ) ) {
    case CBN_SELCHANGE:
      window->OnComboboxSelectionChanged( LOWORD( wparam ), ( HWND )lparam );
      break;
    default:
      break;
  }
}

INT_PTR CALLBACK gui::DefaultDialogProc( HWND window_handle,
                                         UINT message,
                                         WPARAM wparam,
                                         LPARAM lparam ) {
  /*
    Typically, the dialog box procedure should return TRUE if it processed the
    message, and FALSE if it did not. If the dialog box procedure returns FALSE,
    the dialog manager performs the default dialog operation in response to the
    message.

    If the dialog box procedure processes a message that requires a specific
    return value, the dialog box procedure should set the desired return value
    by calling SetWindowLong(hwndDlg, DWL_MSGRESULT, lResult) immediately before
    returning TRUE.

    Exceptions to above regarding return value:
      WM_CHARTOITEM
      WM_COMPAREITEM
      WM_CTLCOLORBTN
      WM_CTLCOLORDLG
      WM_CTLCOLOREDIT
      WM_CTLCOLORLISTBOX
      WM_CTLCOLORSCROLLBAR
      WM_CTLCOLORSTATIC
      WM_INITDIALOG
      WM_QUERYDRAGICON
      WM_VKEYTOITEM
  */

  const auto window = window::GetWindow( window_handle );

  if ( window ) {
    INT_PTR ret = window->DialogProc( window_handle, message, wparam, lparam );
    // If the message was processed we do not continue
    if ( ret == TRUE )
      return ret;
  }

  switch ( message ) {
    case WM_DELETEITEM: {
      const auto window = window::GetWindow( window_handle );

      if ( window ) {
        const auto dis = ( DELETEITEMSTRUCT* )lparam;

        if ( dis->CtlType == ODT_LISTBOX ) {
          // Remove the selected item to avoid issues with drawing the listbox
          // selected and hovering bullshit
          const auto listbox_data = reinterpret_cast<ListboxThemeData*>(
              GetWindowLongPtr( dis->hwndItem, GWLP_USERDATA ) );
          listbox_data->prev_selected_item_index = -1;

          window->OnListBoxItemDeleted( static_cast<uint32_t>( wparam ),
                                        dis->hwndItem, dis->itemID );
        }
      }
    } break;

    case WM_NOTIFY: {
      const auto notify_header = reinterpret_cast<NMHDR*>( lparam );

      if ( window ) {
        OnListViewNotify( *notify_header, window, wparam, lparam );
      }

      switch ( notify_header->code ) {
        case NM_CUSTOMDRAW: {
          const auto custom_draw_data =
              reinterpret_cast<NMCUSTOMDRAW*>( lparam );
          const auto window_type =
              control::GetWindowType( notify_header->hwndFrom );

          switch ( window_type ) {
            case WindowType::kHeaderControl: {
              return headertheme::OnCustomDraw( window_handle,
                                                custom_draw_data );
            } break;
            case WindowType::kTrackbar: {
              return trackbartheme::OnCustomDraw( window_handle,
                                                  custom_draw_data );
            } break;
            default:
              break;
          }
        } break;

        case TCN_SELCHANGE: {
          const HWND tabcontrol_handle = notify_header->hwndFrom;
          const auto tabcontrol_count =
              tabcontrol::GetTabCount( tabcontrol_handle );

          const auto selected_tab_index =
              tabcontrol::GetSelectedTab( tabcontrol_handle );

          for ( uint32_t i = 0; i < tabcontrol_count; ++i ) {
            if ( selected_tab_index == i ) {
              TCITEM tci;
              tci.mask = TCIF_PARAM;
              SendMessage( tabcontrol_handle, TCM_GETITEM, i,
                           reinterpret_cast<LPARAM>( &tci ) );
              HWND associated_window_handle =
                  reinterpret_cast<HWND>( tci.lParam );

              const auto tabcontrol_data =
                  reinterpret_cast<tabcontroltheme::TabControlData*>(
                      GetWindowLongPtr( tabcontrol_handle, GWLP_USERDATA ) );

              if ( tabcontrol_data->prev_window_handle !=
                   reinterpret_cast<HWND>( -1 ) )
                ShowWindow( tabcontrol_data->prev_window_handle, SW_HIDE );

              ShowWindow( associated_window_handle, SW_SHOW );

              tabcontrol_data->prev_window_handle = associated_window_handle;

              // Set the previous shown window
              // SetWindowLongPtr(
              //    tabcontrol_handle, GWLP_USERDATA,
              //    reinterpret_cast<LONG_PTR>( associated_window_handle ) );
            }
          }
        } break;

        default:
          break;
      }
    } break;

    case WM_MEASUREITEM: {
      const auto msi = reinterpret_cast<MEASUREITEMSTRUCT*>( lparam );
      switch ( msi->CtlType ) {
        case ODT_LISTBOX:
          msi->itemHeight = themeconstants::kListBoxItemHeight;
          break;
        case ODT_COMBOBOX:
          msi->itemHeight = themeconstants::kComboboxItemHeight;
          break;
        case ODT_LISTVIEW:
          msi->itemHeight = themeconstants::kListViewItemHeight;
          break;
        default:
          break;
      }
    } break;

    case WM_DRAWITEM: {
      const auto dis = reinterpret_cast<DRAWITEMSTRUCT*>( lparam );

      switch ( dis->CtlType ) {
        case ODT_COMBOBOX:
          comboboxtheme::OnOwnerDrawDropdown( dis );
          break;
        case ODT_LISTBOX:
          listboxtheme::OnOwnerDraw( dis );
          break;
        case ODT_LISTVIEW:
          listviewtheme::OnOwnerDraw( dis );
          break;
        default:
          break;
      }
    } break;

    case WM_CTLCOLORLISTBOX: {
      const HWND control_handle = reinterpret_cast<HWND>( lparam );
      const auto type = control::GetWindowType( control_handle );
      const auto control_hdc = reinterpret_cast<HDC>( wparam );

      if ( type == WindowType::kListbox ) {
        ListboxThemeData* listbox_theme_data =
            reinterpret_cast<ListboxThemeData*>(
                GetWindowLongPtr( control_handle, GWLP_USERDATA ) );

        if ( listbox_theme_data ) {
          if ( control::IsEnabled( control_handle ) ) {
            if ( listbox_theme_data->has_control_status_changed ) {
              listbox_theme_data->has_control_status_changed = false;

              RECT rect;
              GetClientRect( control_handle, &rect );

              drawing::FilledRectangle( control_hdc, rect,
                                        themecolors::listbox::kBackground );

              // Update the non-client area (border)
              control::UpdateControl( control_handle );
            }

            return reinterpret_cast<INT_PTR>(
                themecolors::listbox::BackgroundBrush.GetValue() );
          } else {
            if ( listbox_theme_data->has_control_status_changed ) {
              listbox_theme_data->has_control_status_changed = false;

              RECT rect;
              GetClientRect( control_handle, &rect );

              drawing::FilledRectangle(
                  control_hdc, rect,
                  themecolors::listbox::kBackgroundDisabled );

              // Update the non-client area (border)
              control::UpdateControl( control_handle );
            }

            return reinterpret_cast<INT_PTR>(
                themecolors::listbox::BackgroundBrushDisabled.GetValue() );
          }
        }
      }
    } break;

    case WM_CTLCOLOREDIT: {
      const HWND control_handle = reinterpret_cast<HWND>( lparam );
      const auto type = control::GetWindowType( control_handle );

      switch ( type ) {
        case WindowType::kEdit: {
          editcontroltheme::DrawTextPart( reinterpret_cast<HDC>( wparam ),
                                          reinterpret_cast<HWND>( lparam ) );
          return reinterpret_cast<INT_PTR>(
              themecolors::editcontrol::BackgroundBrush.GetValue() );
        } break;
        default:
          // If the control is not supposed to be handled we return FALSE
          return FALSE;
          break;
      }
    } break;

    case WM_CTLCOLORSTATIC: {
      const HWND control_handle = reinterpret_cast<HWND>( lparam );
      const auto type = control::GetWindowType( control_handle );
      switch ( type ) {
        case WindowType::kEdit: {
          editcontroltheme::DrawTextPart( reinterpret_cast<HDC>( wparam ),
                                          control_handle );
          return reinterpret_cast<INT_PTR>(
              themecolors::editcontrol::BackgroundBrushDisabled.GetValue() );
        } break;

        case WindowType::kTrackbar: {
          return reinterpret_cast<INT_PTR>(
              themecolors::trackbar::BackgroundBrush.GetValue() );
        } break;

        case WindowType::kStaticText: {
          const auto hdc = reinterpret_cast<HDC>( wparam );
          SetBkColor( hdc, themecolors::kWindowBackground );
          SetTextColor( hdc, themecolors::kGeneralText );
          return ( INT_PTR )themecolors::GeneralBackgroundBrush.GetValue();
        } break;

        default:
          // If the control is not supposed to be handled we return FALSE
          return FALSE;
          break;
      }
    } break;

    case WM_NCCALCSIZE: {
      // On return:
      // rgrc[0] is new client area rect
      // rgrc[1] and rgrc[2] does not seem to do anything...

      // If wparam is TRUE, return 0 without doing anything removes the window
      // frame

      if ( wparam ) {
        auto calcsize_params = reinterpret_cast<NCCALCSIZE_PARAMS*>( lparam );

        auto& new_client_area = calcsize_params->rgrc[ 0 ];

        new_client_area.left += themeconstants::kTitlebarBorderThickness;

        new_client_area.top += themeconstants::kTitlebarHeight;

        new_client_area.right -= themeconstants::kTitlebarBorderThickness;

        new_client_area.bottom -= themeconstants::kTitlebarBorderThickness;

        return 0;
      }

      assert( false && "handle this?" );
      // return DefWindowProc( dialog_handle, message, wparam, lparam );
    } break;

    case WM_ERASEBKGND: {
      // Says we erased the background, the reason is to avoid some possible
      // flickering
      return TRUE;
    } break;

    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint( window_handle, &ps );

      drawing::FilledRectangle( hdc, ps.rcPaint,
                                themecolors::kWindowBackground );

      EndPaint( window_handle, &ps );
    } break;

    case WM_NCPAINT: {
      titlebar::DrawTitlebar( window_handle, wparam );
    } break;

    case WM_NCHITTEST: {
      INT_PTR retval = titlebar::HandleHitTest( window_handle, lparam );
      control::SetReturnValue( window_handle, retval );
    } break;

    case WM_NCLBUTTONDOWN: {
      return titlebar::HandleLeftButtonDown( static_cast<int32_t>( wparam ),
                                             window_handle );
    } break;

    case WM_NCLBUTTONUP: {
      titlebar::HandleLeftButtonUp( window_handle );
      control::SetReturnValue( window_handle, FALSE );
      return FALSE;
    } break;

    case WM_NCMOUSELEAVE: {
      titlebar::HandleMouseLeave( window_handle );

      // We did not process the message
      control::SetReturnValue( window_handle, FALSE );
      return FALSE;
    } break;

    case WM_INITDIALOG: {
      Window* associated_window = reinterpret_cast<Window*>( lparam );

      /*
      const auto menu_handle = GetMenu( window_handle );

      if ( menu_handle ) {
        // Save the current win32 window menu
        // associated_window->menu_ = GetMenulist( menu_handle );

        associated_window->SetupMenu( menu_handle );

        // If there was a default menu attached to the window, remove it
        SetMenu( window_handle, 0 );

        // Now destroy the menu to free up resources
        DestroyMenu( menu_handle );
      }
      */

      // Assign the handle_ in the window object
      associated_window->handle_ = window_handle;

      CenterWindowPositionTo( window_handle, GetDesktopWindow() );

      theme::InitializeControls( window_handle );

      // Initialize the window to display the titlebar correctly
      control::RemoveStyles( window_handle,
                             WS_CAPTION | WS_THICKFRAME | WS_SYSMENU );

      // We add WS_CLIPCHILDREN to avoid the parent dialog WM_PAINT to paint
      // over the child windows/controls Not having WS_CLIPCHILDREN caused the
      // non-client area borders to be removed when turn off/on monitor
      control::AddStyles( window_handle,
                          WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_CLIPCHILDREN );

      control::SetFont( window_handle, themefont::GetDefaultThemeFont() );

      // Call the users initialize function last
      associated_window->Initialize( window_handle );

      // Due to the window being a dialogbox, WM_NCCALCSIZE is not being sent
      // by default. Therefore I completely redraw the whole window to force
      // that message to be sent.
      control::UpdateControl( window_handle );
    } break;

    case WM_COMMAND: {
      const auto control_id = LOWORD( wparam );
      const auto control_handle = reinterpret_cast<HWND>( lparam );

      const auto window = window::GetWindow( window_handle );

      if ( window ) {
        window->OnButtonClick( control_handle, control_id );
        OnListboxNotify( lparam, wparam, window );
        OnRicheditNotify( lparam, wparam, window );
        OnComboboxNotify( lparam, wparam, window );
      }
    } break;

    case WM_HSCROLL: {
      const auto window = window::GetWindow( window_handle );

      if ( window && lparam ) {
        const auto trackbar_handle = reinterpret_cast<HWND>( lparam );

        if ( LOWORD( wparam ) == TB_THUMBPOSITION ||
             LOWORD( wparam ) == TB_THUMBTRACK ) {
          window->OnTrackbarSliderChanging( trackbar_handle, HIWORD( wparam ) );
        } else {
          window->OnTrackbarSliderChanging(
              trackbar_handle, trackbar::GetPos( trackbar_handle ) );
        }
      }
    } break;

    case WM_CLOSE: {
      DestroyWindow( window_handle );
    } break;

    case WM_DESTROY: {
      PostQuitMessage( 0 );
    } break;

    default:
      control::SetReturnValue( window_handle, FALSE );
      // We did not process the message, perform default operations
      return FALSE;
      break;
  }

  // We processed the message
  return TRUE;
}  // namespace gwingui

// Credits to someone else for the scollbar code, I did not write that.
void InitializeWindowScrollbar( const HWND window_handle ) {
  RECT client_rect;
  GetClientRect( window_handle, &client_rect );

  SIZE scrollbar_size;
  scrollbar_size.cx = client_rect.right - client_rect.left;
  scrollbar_size.cy = client_rect.bottom - client_rect.top;

  SCROLLINFO si = { 0 };
  si.cbSize = sizeof( si );
  si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
  si.nPos = 0;
  si.nMin = 0;
  si.nMax = scrollbar_size.cx;
  si.nPage = scrollbar_size.cx;

  SetScrollInfo( window_handle, SB_HORZ, &si, FALSE );

  si.nMax = scrollbar_size.cy;
  si.nPage = scrollbar_size.cy;

  SetScrollInfo( window_handle, SB_VERT, &si, FALSE );
}

void ScrollWindowUpdate( const HWND window_handle,
                         const int32_t bar,
                         const int32_t pos ) {
  const auto& window = window::GetWindow( window_handle );

  assert( window );

  auto previous_x = &window->previous_scroll_x_;
  auto previous_y = &window->previous_scroll_y_;

  int32_t cx = 0;
  int32_t cy = 0;

  if ( bar == SB_HORZ ) {
    cx = *previous_x - pos;
    *previous_x = pos;
  } else if ( bar == SB_VERT ) {
    cy = *previous_y - pos;
    *previous_y = pos;
  }

  if ( cx || cy ) {
    ScrollWindow( window_handle, cx, cy, nullptr, nullptr );
    UpdateWindow( window_handle );
  }
}

void ScrollbarOnSize( const HWND window_handle,
                      const uint32_t state,
                      const int cx,
                      const int cy ) {
  if ( state != SIZE_RESTORED && state != SIZE_MAXIMIZED )
    return;

  SCROLLINFO si = { 0 };
  si.cbSize = sizeof( si );

  const int bar[] = { SB_HORZ, SB_VERT };
  const int page[] = { cx, cy };

  for ( size_t i = 0; i < ARRAYSIZE( bar ); ++i ) {
    si.fMask = SIF_PAGE;
    si.nPage = page[ i ];
    SetScrollInfo( window_handle, bar[ i ], &si, TRUE );

    si.fMask = SIF_RANGE | SIF_POS;
    GetScrollInfo( window_handle, bar[ i ], &si );

    const int max_scroll_pos = si.nMax - ( page[ i ] - 1 );

    // Scroll client only if scroll bar is visible and window's
    // content is fully scrolled toward right and/or bottom side.
    // Also, update window's content on maximize.
    const bool should_scroll =
        ( si.nPos != si.nMin && si.nPos == max_scroll_pos ) ||
        ( state == SIZE_MAXIMIZED );

    if ( should_scroll )
      ScrollWindowUpdate( window_handle, bar[ i ], si.nPos );
  }
}

int32_t GetScrollingPosition( const HWND window_handle,
                              const int32_t bar,
                              const uint32_t code ) {
  SCROLLINFO si = { 0 };
  si.cbSize = sizeof( SCROLLINFO );
  si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE | SIF_TRACKPOS;
  GetScrollInfo( window_handle, bar, &si );

  const int minPos = si.nMin;
  const int maxPos = si.nMax - ( si.nPage - 1 );

  int result = -1;

  switch ( code ) {
    case SB_LINEUP:
      result = max( si.nPos - 5, minPos );
      break;

    case SB_LINEDOWN:
      result = min( si.nPos + 5, maxPos );
      break;

    case SB_PAGEUP:
      result = max( si.nPos - ( int )( si.nPage / 8 ), minPos );
      break;

    case SB_PAGEDOWN:
      result = min( si.nPos + ( int )( si.nPage / 8 ), maxPos );
      break;

    case SB_THUMBTRACK:
      result = si.nTrackPos;
      break;

    case SB_TOP:
      result = minPos;
      break;

    case SB_BOTTOM:
      result = maxPos;
      break;
  }

  return result;
}

void ScrollbarOnScroll( const HWND window_handle,
                        const int32_t bar,
                        const uint32_t code ) {
  const int32_t new_scroll_position =
      GetScrollingPosition( window_handle, bar, code );

  if ( new_scroll_position == -1 )
    return;

  SCROLLINFO scroll_info;
  scroll_info.cbSize = sizeof( scroll_info );
  scroll_info.fMask = SIF_POS;
  scroll_info.nPos = new_scroll_position;

  SetScrollInfo( window_handle, bar, &scroll_info, TRUE );
  ScrollWindowUpdate( window_handle, bar, new_scroll_position );
}

void ScrollbarOnWheel( const HWND window_handle, const WPARAM wparam ) {
  const auto scroll_delta = GET_WHEEL_DELTA_WPARAM( wparam );

  int32_t bar_to_scroll = SB_VERT;

  // If the CTRL key is down, then we scroll on the horizontal bar
  if ( GET_KEYSTATE_WPARAM( wparam ) & MK_CONTROL ) {
    bar_to_scroll = SB_HORZ;
  }

  if ( scroll_delta > 0 ) {
    ScrollbarOnScroll( window_handle, bar_to_scroll, SB_PAGEUP );
  } else if ( scroll_delta < 0 ) {
    ScrollbarOnScroll( window_handle, bar_to_scroll, SB_PAGEDOWN );
  }
}

INT_PTR CALLBACK gui::ChildDialogProc( HWND window_handle,
                                       UINT message,
                                       WPARAM wparam,
                                       LPARAM lparam ) {
  const auto window = window::GetWindow( window_handle );

  if ( window ) {
    INT_PTR ret = window->DialogProc( window_handle, message, wparam, lparam );
    // If the message was processed we do not continue
    if ( ret == TRUE )
      return ret;
  }

  switch ( message ) {
    case WM_INITDIALOG:
      InitializeWindowScrollbar( window_handle );
    case WM_ERASEBKGND:
    case WM_COMMAND:
    case WM_PAINT:
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORLISTBOX:
    case WM_DRAWITEM:
    case WM_MEASUREITEM:
    case WM_DELETEITEM:
    case WM_NOTIFY: {
      return gui::DefaultDialogProc( window_handle, message, wparam, lparam );
    } break;

    case WM_SIZE: {
      ScrollbarOnSize( window_handle, static_cast<uint32_t>( wparam ),
                       LOWORD( lparam ), HIWORD( lparam ) );
    } break;

    case WM_VSCROLL: {
      ScrollbarOnScroll( window_handle, SB_VERT, LOWORD( wparam ) );
    } break;

    case WM_HSCROLL: {
      ScrollbarOnScroll( window_handle, SB_HORZ, LOWORD( wparam ) );
      return gui::DefaultDialogProc( window_handle, message, wparam, lparam );
    } break;

    case WM_MOUSEWHEEL: {
      ScrollbarOnWheel( window_handle, wparam );
    } break;

    case WM_CTLCOLORDLG: {
      // Handle this message to avoid some quick white flickering when scrolling
      // a window inside a tab control
      return reinterpret_cast<INT_PTR>(
          themecolors::GeneralBackgroundBrush.GetValue() );
    } break;

    default:
      // We did not process the message
      return FALSE;
      break;
  }

  return TRUE;
}

}  // namespace gwingui