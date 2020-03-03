#pragma once

#include <memory>
#include <vector>
#include <stdint.h>
#include <windows.h>

namespace gwingui {

class Window;
class WindowManager;

// Forward declare the function inside the namespace to use as a friend for this
// class
namespace control {
HWND GetWindowHandle( const uint32_t id );
}

namespace window {
Window* GetWindow( const HWND window_handle );
}

namespace theme {
void InitializeControls( const HWND window_handle );
}

namespace gui {

INT_PTR CALLBACK DefaultDialogProc( HWND window_handle,
                                    UINT message,
                                    WPARAM wparam,
                                    LPARAM lparam );

INT_PTR CALLBACK ChildDialogProc( HWND window_handle,
                                  UINT message,
                                  WPARAM wparam,
                                  LPARAM lparam );

}  // namespace gui

class Gui {
 public:
  friend class WindowManager;

  Gui( const HINSTANCE instance );

  Gui( const Gui& ) = default;
  ~Gui() = default;

  HWND AddWindow( const uint32_t window_id, const HWND parent_handle, Window* window );

  void Run();

 private:
  friend HWND control::GetWindowHandle( const uint32_t id );
  friend Window* window::GetWindow( const HWND window_handle );
  friend INT_PTR CALLBACK gui::DefaultDialogProc( HWND window_handle,
                                                  UINT message,
                                                  WPARAM wparam,
                                                  LPARAM lparam );
  friend void theme::InitializeControls( const HWND window_handle );

 private:
  static WindowManager window_manager_;
  static HINSTANCE instance_;
};

}  // namespace gwingui