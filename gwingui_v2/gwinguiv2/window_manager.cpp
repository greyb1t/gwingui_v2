#include "pch.h"
#include "window_manager.h"
#include "window.h"

#include "gui.h"
#include "message_box.h"

namespace gwingui {

HWND WindowManager::AddWindow( const uint32_t window_id,
                               const HWND parent_handle,
                               const DLGPROC dialog_proc,
                               Window* window ) {
  window->resource_id_ = window_id;
  windows_.push_back( std::move( std::unique_ptr<Window>( window ) ) );

  const HWND handle = CreateDialogParam(
      Gui::instance_, MAKEINTRESOURCE( window_id ), parent_handle, dialog_proc,
      reinterpret_cast<LPARAM>( window ) );

  if ( !handle )
    messagebox::Error( TEXT( "CreateDialogParam failed with " ) +
                       std::to_wstring( GetLastError() ) );

  // assert( handle );

  return handle;
}

}  // namespace gwingui
