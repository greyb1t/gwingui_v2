#include "pch.h"
#include "mouse_tracker.h"
#include "../controls/control.h"
#include "../window.h"

bool gwingui::MouseTracker::is_tracking_mouse_ = false;

void gwingui::MouseTracker::HandleEvents( const HWND window_handle,
                                          const UINT message,
                                          EventCallback_t on_hover,
                                          EventCallback_t on_leave ) {
  switch ( message ) {
    case WM_MOUSEMOVE: {
      if ( !is_tracking_mouse_ ) {
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof( tme );
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = window_handle;
        // tme.dwHoverTime = 1;

        TrackMouseEvent( &tme );

        is_tracking_mouse_ = true;

        on_hover( window_handle );
      }
    } break;

    case WM_MOUSELEAVE: {
      is_tracking_mouse_ = false;

      on_leave( window_handle );
    } break;

    default:
      break;
  }
}

void gwingui::MouseTracker::ReportHoverLeaveEvents( const HWND window_handle,
                                                    const UINT message ) {
  GetMouseTracker().HandleEvents(
      window_handle, message,
      []( const HWND window_handle ) {
        const auto window = window::GetWindow( GetParent( window_handle ) );
        window->OnControlHover( window_handle,
                                control::GetWindowId( window_handle ) );
      },
      []( const HWND window_handle ) {
        const auto window = window::GetWindow( GetParent( window_handle ) );
        window->OnControlLeave( window_handle,
                                control::GetWindowId( window_handle ) );
      } );
}
