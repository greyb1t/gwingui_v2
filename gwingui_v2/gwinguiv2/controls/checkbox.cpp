#include "pch.h"
#include "checkbox.h"

namespace gwingui {

namespace checkbox {

bool IsChecked( const HWND checkbox_handle ) {
  if ( !checkbox_handle )
    throw std::runtime_error( "Invalid button handle" );

  const LRESULT is_checked = SendMessage( checkbox_handle, BM_GETCHECK, 0, 0 );

  if ( is_checked == BST_INDETERMINATE )
    throw std::runtime_error( "checkbox BST_INDETERMINATE unhandled" );

  return !!is_checked;
}

void SetCheck( const HWND checkbox_handle, const bool value ) {
  if ( !checkbox_handle )
    throw std::runtime_error( "Invalid button handle" );

  SendMessage( checkbox_handle, BM_SETCHECK,
               value ? BST_CHECKED : BST_UNCHECKED, 0 );
}

}  // namespace checkbox

}  // namespace gwingui