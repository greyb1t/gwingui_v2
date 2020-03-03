#pragma once

#include <Windows.h>
#include <memory>

#define GWH( id ) ( gwingui::control::GetWindowHandle( id ) )

namespace gwingui {

enum class WindowType {
  kButton,
  kRichEdit20,
  kEdit,
  kTrackbar,
  kCombobox,
  kListbox,
  kListView,
  kTreeView,
  kStaticText,
  kPictureBox,
  kTabControl,
  kProgressBar,
  kHeaderControl,
  kGroupBox,
  kCheckBox,
  kRadioButton,
  kDialog,
  kStatusbar,
  kBitmapButton,
  kRichEdit50,
  kHotKey,
  kUnknown
};

class Window;

namespace control {

// GetControlHandle: returns NULL if no handle was found
HWND GetWindowHandle( const uint32_t id );

uint32_t GetWindowId( const HWND window_handle );

// SetStyles sets the specified styles for a control
void SetStyles( const HWND control_handle, const LONG_PTR styles );

// AddStyles adds the specified styles to the control
void AddStyles( const HWND control_handle, const LONG_PTR styles );

// RemoveStyles removes the specified styles from the control
void RemoveStyles( const HWND control_handle, const LONG_PTR styles );

// HasStyles returns true of false based on whether the control has the
// specified styles
bool HasStyles( const HWND control_handle, const LONG_PTR styles );

// SetExtendedStyles sets the extended styles for a control
void SetExtendedStyles( const HWND control_handle, const LONG_PTR styles );

// AddExtendedStyles adds the exended styles to the control
void AddExtendedStyles( const HWND control_handle, const LONG_PTR styles );

// RemoveExtendedStyles removes the specified extended styles from a control
void RemoveExtendedStyles( const HWND control_handle, const LONG_PTR styles );

// HasExtendedStyles returns true of false based on whether the control has the
// specified styles
bool HasExtendedStyles( const HWND control_handle, const LONG_PTR styles );

// UpdateControl updates a control
void UpdateControl( const HWND control_handle );

inline const LONG_PTR SetReturnValue( const HWND window_handle, const LONG_PTR value ) {
  return SetWindowLongPtr( window_handle, DWLP_MSGRESULT, value );
}

void EnableOrDisable( const HWND control_handle, bool enable );
bool IsEnabled( const HWND control_handle );

bool SetText( const HWND control_handle, const std::wstring& text );
std::wstring GetText( const HWND control_handle );

void SetFont( const HWND control_handle, const HFONT font_handle );
HFONT GetFont( const HWND control_handle );

// CreateTooltip: creates a tooltip for the specified control and returns a
// handle to it returns NULL if failed
HWND CreateTooltip( const HWND control_handle,
                    const HINSTANCE instance,
                    const std::wstring& text,
                    const std::wstring title = TEXT( "" ) );

// ScreenToWindow: Convert screen coordinates to window coordinates
POINT ScreenPointToWindowPoint( const HWND window_handle, const POINT& p );

// ScreenRectToWindowRect: Convert a screen-coord-rect to a window-coord-rect
RECT ScreenRectToWindowRect( const RECT& rect );

SIZE GetControlSize( const HWND window_handle );
bool SetControlSize( const HWND window_handle, const SIZE& size );

bool SetControlPosition( const HWND window_handle, const POINT& p );

RECT GetWindowRect( const HWND window_handle );

WindowType GetWindowType( const HWND window_handle );

}  // namespace control

}  // namespace gwingui