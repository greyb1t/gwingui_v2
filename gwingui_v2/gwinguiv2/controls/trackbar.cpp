#include "pch.h"
#include "trackbar.h"
#include "control.h"

namespace gwingui {

namespace trackbar {

void SetRange( const HWND trackbar_handle,
               const uint32_t min,
               const uint32_t max ) {
  if ( !trackbar_handle )
    throw std::runtime_error( "invalid trackbar handle" );

  SendMessage( trackbar_handle, TBM_SETRANGEMIN, TRUE, min );
  SendMessage( trackbar_handle, TBM_SETRANGEMAX, TRUE, max );
}

void SetPos( const HWND trackbar_handle, const uint32_t position ) {
  if ( !trackbar_handle )
    throw std::runtime_error( "invalid trackbar handle" );

  SendMessage( trackbar_handle, TBM_SETPOS, TRUE, position );
}

LRESULT GetPos( const HWND trackbar_handle ) {
  if ( !trackbar_handle )
    throw std::runtime_error( "invalid trackbar handle" );

  return SendMessage( trackbar_handle, TBM_GETPOS, 0, 0 );
}

}  // namespace trackbar

}  // namespace gwingui