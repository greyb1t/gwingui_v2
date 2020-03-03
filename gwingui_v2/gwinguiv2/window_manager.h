#pragma once

#include <memory>
#include <vector>
#include <stdint.h>

namespace gwingui {

class Window;
class Gui;

namespace control {
HWND GetWindowHandle( const uint32_t id );
HWND FindInWindow(const std::unique_ptr<Window>& window, const uint32_t id);
}

namespace window {
Window* GetWindow( const HWND window_handle );
}

namespace gui {
INT_PTR CALLBACK DefaultDialogProc( HWND window_handle,
                                    UINT message,
                                    WPARAM wparam,
                                    LPARAM lparam );
}  // namespace gui

class WindowManager {
 public:
  friend class Gui;

  HWND AddWindow( const uint32_t window_id,
                  const HWND parent_handle,
                  const DLGPROC dialog_proc,
                  Window* window );

  friend HWND control::GetWindowHandle( const uint32_t id );
  friend HWND control::FindInWindow(const std::unique_ptr<Window>& window, const uint32_t id);

  friend Window* window::GetWindow( const HWND window_handle );
  friend INT_PTR CALLBACK gui::DefaultDialogProc( HWND window_handle,
                                                  UINT message,
                                                  WPARAM wparam,
                                                  LPARAM lparam );

 private:
  std::vector<std::unique_ptr<Window>> windows_;
};

}  // namespace gwingui