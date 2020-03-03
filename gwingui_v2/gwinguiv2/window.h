#pragma once

#include <windows.h>
#include <stdint.h>
#include <array>
#include <CommCtrl.h>
#include <Richedit.h>
#include <TextServ.h>

#include "gui.h"
#include "theme/titlebar_button_bitmap.h"
#include "theme/titlebar_button_text.h"

#include "window_manager.h"

namespace gwingui {

class TitlebarButton;

namespace control {
HWND GetWindowHandle( const uint32_t id );
HWND FindInWindow( const std::unique_ptr<Window>& window, const uint32_t id );
}  // namespace control

namespace window {
Window* GetWindow( const HWND window_handle );
}  // namespace window

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

void ScrollWindowUpdate( const HWND window_handle,
                         const int32_t bar,
                         const int32_t pos );

namespace titlebar {

void DrawTitlebar( const HWND window_handle, const WPARAM wparam );
INT_PTR HandleHitTest( const HWND window_handle, const LPARAM lparam );
void HandleMouseLeave( const HWND window_handle );
INT_PTR HandleLeftButtonDown( const int32_t hittest, const HWND window_handle );
void HandleLeftButtonUp( const HWND window_handle );

}  // namespace titlebar

// Forward declare these windows structs, otherwise the compiler cries and won't
// compile due to some issue with the windows header inclusion order

class Window {
 public:
  // Make the Window class a friend of Gui to we can access the private members
  // of this class, better than making unnesecarry getters and setters.
  friend class Gui;
  friend class WindowManager;

  Window();
  Window( const Window& ) = default;
  ~Window() = default;

  uint32_t GetResourceId() const;
  HWND GetHandle() const;

 protected:
  void AddTabWindow( const HWND tabcontrol_handle,
                     const uint32_t dialog_id,
                     const std::wstring& tab_name,
                     Window* window );

  virtual void Initialize( const HWND window_handle ){};

  virtual INT_PTR CALLBACK DialogProc( HWND window_handle,
                                       UINT message,
                                       WPARAM wparam,
                                       LPARAM lparam ) {
    return FALSE;
  }

 public:
  // Controls events
  virtual void OnControlHover( const HWND control_handle,
                               const uint32_t control_id ) {}
  virtual void OnControlLeave( const HWND control_handle,
                               const uint32_t control_id ) {}

  // Button events
  virtual void OnButtonClick( const HWND button_handle,
                              const uint32_t button_id ) {}

  // Listbox events
  virtual void OnListBoxDoubleClick() {}
  virtual void OnListBoxSelectionChanged( const uint32_t control_id,
                                          const HWND control_handle ) {}
  virtual void OnListBoxItemDeleted( const uint32_t control_id,
                                     const HWND control_handle,
                                     const uint32_t index ) {}

  // Listview events
  virtual void OnListViewClick( NMITEMACTIVATE* pItem ) {}
  virtual void OnListViewDoubleClick( NMITEMACTIVATE* pItem ) {}
  virtual void OnListViewItemSelectionChanged( NMLISTVIEW* lv ) {}
  virtual void OnListViewItemAdded( NMLISTVIEW* lv ) {}
  virtual void OnListViewItemDeleted( NMLISTVIEW* lv ) {}

  // Combobox events
  virtual void OnComboboxSelectionChanged( uint32_t control_id,
                                           HWND control_handle ) {}

  // Trackbar events
  virtual void OnTrackbarSliderChanging( const HWND trackbar_handle,
                                         int32_t position ) {}

  // Richedit events
  virtual void OnRichEditTextChanged( uint32_t ctrlId, CHANGENOTIFY* pCn ) {}

  virtual void OnHotkeyChanged( const HWND hotkey_handle,
                                const uint32_t control_id ) {}

 private:
  void SetupMenu( const HMENU menu_handle );

 private:
  uint32_t resource_id_;
  HWND handle_;

  WindowManager window_manager_;

  std::vector<TitlebarButtonText> menubar_;

  // Consider making these two a struct
  std::array<TitlebarButtonBitmap, 3> titlebar_buttons_;
  int32_t last_hover_index_;

  int32_t previous_scroll_x_;
  int32_t previous_scroll_y_;

  friend Window* window::GetWindow( const HWND window_handle );

  friend HWND control::GetWindowHandle( const uint32_t id );
  friend HWND control::FindInWindow( const std::unique_ptr<Window>& window,
                                     const uint32_t id );

  friend INT_PTR CALLBACK gui::DefaultDialogProc( HWND window_handle,
                                                  UINT message,
                                                  WPARAM wparam,
                                                  LPARAM lparam );
  friend INT_PTR CALLBACK gui::ChildDialogProc( HWND window_handle,
                                                UINT message,
                                                WPARAM wparam,
                                                LPARAM lparam );

  friend void titlebar::DrawTitlebar( const HWND window_handle,
                                      const WPARAM wparam );
  friend INT_PTR titlebar::HandleHitTest( const HWND window_handle,
                                          const LPARAM lparam );
  friend void titlebar::HandleMouseLeave( const HWND window_handle );
  friend INT_PTR titlebar::HandleLeftButtonDown( const int32_t hittest,
                                                 const HWND window_handle );
  friend void titlebar::HandleLeftButtonUp( const HWND window_handle );

  friend void ScrollWindowUpdate( const HWND window_handle,
                                  const int32_t bar,
                                  const int32_t pos );
};

}  // namespace gwingui