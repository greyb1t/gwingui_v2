#include "pch.h"
#include "progressbar.h"
#include "control.h"

namespace gwingui {

namespace progressbar {

bool SetRange( const HWND progressbar_handle,
               const uint32_t min,
               const uint32_t max ) {
  if ( !progressbar_handle )
    throw std::runtime_error( "invalid progress bar handle" );

  const LRESULT result = SendMessage( progressbar_handle, PBM_SETRANGE, 0,
                                      MAKELPARAM( min, max ) );

  return !!result;
}

LRESULT GetPos( const HWND progressbar_handle ) {
  if ( !progressbar_handle )
    throw std::runtime_error( "invalid progress bar handle" );

  return SendMessage( progressbar_handle, PBM_GETPOS, 0, 0 );
}

void SetPos( const HWND progressbar_handle, const uint32_t value ) {
  if ( !progressbar_handle )
    throw std::runtime_error( "invalid progress bar handle" );

  SendMessage( progressbar_handle, PBM_SETPOS, value, 0 );
}

void IncementPos( const HWND progressbar_handle, const uint32_t value ) {
  if ( !progressbar_handle )
    throw std::runtime_error( "invalid progress bar handle" );

  SendMessage( progressbar_handle, PBM_DELTAPOS, value, 0 );
}

void EnableMarquee( const HWND progressbar_handle,
                    const uint32_t update_interval ) {
  if ( !progressbar_handle )
    throw std::runtime_error( "invalid progress bar handle" );

  SendMessage( progressbar_handle, PBM_SETMARQUEE, TRUE, update_interval );
}

void DisableMarquee( const HWND progressbar_handle ) {
  if ( !progressbar_handle )
    throw std::runtime_error( "invalid progress bar handle" );

  SendMessage( progressbar_handle, PBM_SETMARQUEE, FALSE, 0 );
}

}  // namespace progressbar

}  // namespace gwingui