#pragma once

namespace gwingui {

class MouseTracker {
 private:
  MouseTracker() = default;

  using EventCallback_t = void ( * )( const HWND window_handle );

 public:
  void ReportHoverLeaveEvents( const HWND window_handle, const UINT message );

  // bool IsHovering() const {
  //   return is_tracking_mouse_;
  // }

  friend inline MouseTracker& GetMouseTracker();

 private:
  void HandleEvents( const HWND window_handle,
                     const UINT message,
                     EventCallback_t on_hover,
                     EventCallback_t on_leave );

  static bool is_tracking_mouse_;
};

inline MouseTracker& GetMouseTracker() {
  static MouseTracker mouse_tracker;
  return mouse_tracker;
}

}  // namespace gwingui