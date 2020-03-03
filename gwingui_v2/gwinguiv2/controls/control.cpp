#include "pch.h"
#include "control.h"

#include "../gui.h"
#include "../window.h"

namespace gwingui {

namespace control {

HWND FindInWindow( const std::unique_ptr<Window>& window, const uint32_t id ) {
  const auto window_handle = window->GetHandle();

  // Is the window itself the specified id?
  if ( window->GetResourceId() == id ) {
    return window_handle;
  }

  // Test if the control is inside current window, if so, return it
  const HWND control_handle = GetDlgItem( window_handle, id );

  if ( control_handle )
    return control_handle;

  HWND found_in_child_handle = 0;

  // Go through child windows
  for ( const auto& child : window->window_manager_.windows_ ) {
    found_in_child_handle = FindInWindow( child, id );

    if ( found_in_child_handle )
      return found_in_child_handle;
  }

  return 0;
};

HWND GetWindowHandle( const uint32_t id ) {
  // Due to the fact that a window can contains multiple other windows, which
  // then contains another window we need to recusivly find the handle of the
  // specified id.

  const auto& windows = Gui::window_manager_.windows_;

  for ( const auto& window : windows ) {
    const auto handle = FindInWindow( window, id );

    if ( handle )
      return handle;
  }

  return NULL;
}

uint32_t GetWindowId( const HWND window_handle ) {
  return GetDlgCtrlID( window_handle );
}

void SetStyles( const HWND control_handle, const LONG_PTR styles ) {
  SetWindowLongPtr( control_handle, GWL_STYLE, styles );
}

void AddStyles( const HWND control_handle, const LONG_PTR styles ) {
  SetWindowLongPtr( control_handle, GWL_STYLE,
                    GetWindowLongPtr( control_handle, GWL_STYLE ) | styles );
}

void RemoveStyles( const HWND control_handle, const LONG_PTR styles ) {
  SetWindowLongPtr( control_handle, GWL_STYLE,
                    GetWindowLongPtr( control_handle, GWL_STYLE ) & ~styles );
}

bool HasStyles( const HWND control_handle, const LONG_PTR styles ) {
  return ( GetWindowLongPtr( control_handle, GWL_STYLE ) & styles ) == styles;
}

void SetExtendedStyles( const HWND control_handle, const LONG_PTR styles ) {
  SetWindowLongPtr( control_handle, GWL_EXSTYLE, styles );
}

void AddExtendedStyles( const HWND control_handle, const LONG_PTR styles ) {
  SetWindowLongPtr( control_handle, GWL_EXSTYLE,
                    GetWindowLongPtr( control_handle, GWL_EXSTYLE ) | styles );
}

void RemoveExtendedStyles( const HWND control_handle, const LONG_PTR styles ) {
  SetWindowLongPtr( control_handle, GWL_EXSTYLE,
                    GetWindowLongPtr( control_handle, GWL_EXSTYLE ) & ~styles );
}

bool HasExtendedStyles( const HWND control_handle, const LONG_PTR styles ) {
  return ( GetWindowLongPtr( control_handle, GWL_EXSTYLE ) & styles ) == styles;
}

void UpdateControl( const HWND control_handle ) {
  SetWindowPos( control_handle, 0, 0, 0, 0, 0,
                SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED |
                    SWP_NOACTIVATE );
}

void EnableOrDisable( const HWND control_handle, bool enable ) {
  EnableWindow( control_handle, static_cast<BOOL>( enable ) );
}

bool IsEnabled( const HWND control_handle ) {
  if ( !control_handle )
    throw std::runtime_error( "invalid control handle" );

  return !!IsWindowEnabled( control_handle );
}

bool SetText( const HWND control_handle, const std::wstring& text ) {
  if ( !control_handle )
    throw std::runtime_error( "invalid control handle" );

  return !!SetWindowText( control_handle, text.c_str() );
}

std::wstring GetText( const HWND control_handle ) {
  if ( !control_handle )
    throw std::runtime_error( "invalid control handle" );

  const int text_length = GetWindowTextLength( control_handle );

  std::wstring str_buf;
  str_buf.resize( text_length );

  GetWindowText( control_handle, &str_buf[ 0 ], text_length + 1 );

  return str_buf;
}

void SetFont( const HWND window_handle, const HFONT font_handle ) {
  SendMessage( window_handle, WM_SETFONT,
               reinterpret_cast<WPARAM>( font_handle ), MAKELPARAM( TRUE, 0 ) );
}

HFONT GetFont( const HWND control_handle ) {
  if ( !control_handle )
    throw std::runtime_error( "invalid control handle" );

  return reinterpret_cast<HFONT>(
      SendMessage( control_handle, WM_GETFONT, NULL, NULL ) );
}

HWND CreateTooltip( const HWND control_handle,
                    const HINSTANCE instance,
                    const std::wstring& text,
                    const std::wstring title ) {
  const HWND tooltip_handle =
      CreateWindowEx( NULL, TOOLTIPS_CLASS, TEXT( "ttwn" ),
                      WS_POPUP | TTS_ALWAYSTIP | TTS_NOFADE, CW_USEDEFAULT,
                      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                      GetParent( control_handle ), NULL, instance, NULL );

  if ( !title.empty() )
    SendMessage( tooltip_handle, TTM_SETTITLE, NULL,
                 reinterpret_cast<LPARAM>( title.c_str() ) );

  if ( !tooltip_handle )
    throw std::runtime_error( "failed to create tooltip control" );

  LPARAM lparam = MAKELPARAM( 30000, 0 );

  SendMessage( tooltip_handle, TTM_SETDELAYTIME, TTDT_AUTOPOP, lparam );

  lparam = MAKELPARAM( 1, 0 );

  SendMessage( tooltip_handle, TTM_SETDELAYTIME, TTDT_INITIAL, lparam );
  SendMessage( tooltip_handle, TTM_SETDELAYTIME, TTDT_RESHOW, lparam );

  TOOLINFO tool_info = { 0 };
  tool_info.cbSize = sizeof( TOOLINFO );
  tool_info.hwnd = GetParent( control_handle );
  tool_info.uFlags = TTF_IDISHWND | TTF_SUBCLASS | TTF_PARSELINKS;
  tool_info.uId = reinterpret_cast<UINT_PTR>( control_handle );
  tool_info.lpszText = ( LPWSTR )text.c_str();

  LRESULT err =
      SendMessage( tooltip_handle, TTM_ADDTOOL, NULL, ( LPARAM )&tool_info );

  if ( !err )
    throw std::runtime_error( "failed to create tooltip control" );

  return tooltip_handle;
}

POINT ScreenPointToWindowPoint( const HWND window_handle, const POINT& p ) {
  RECT rect;
  GetWindowRect( window_handle, &rect );

  POINT new_point = p;
  new_point.x -= rect.left;
  new_point.y -= rect.top;

  return new_point;
};

RECT ScreenRectToWindowRect( const RECT& rect ) {
  RECT new_rect;
  new_rect.right = rect.right - rect.left;
  new_rect.bottom = rect.bottom - rect.top;

  new_rect.left = 0;
  new_rect.top = 0;

  return new_rect;
}

SIZE GetControlSize( const HWND window_handle ) {
  RECT rect = GetWindowRect( window_handle );

  SIZE size;
  size.cx = rect.right - rect.left;
  size.cy = rect.bottom - rect.top;

  return size;
}

bool SetControlSize( const HWND window_handle, const SIZE& size ) {
  return !!SetWindowPos( window_handle, NULL, 0, 0, size.cx, size.cy,
                         SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER );
}

bool SetControlPosition( const HWND window_handle, const POINT& p ) {
  const RECT rect = control::GetWindowRect( window_handle );
  const bool result =
      !!SetWindowPos( window_handle, NULL, p.x, p.y, 0, 0,
                      SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOSIZE );

  return result;
}

RECT GetWindowRect( const HWND window_handle ) {
  RECT rect;
  GetWindowRect( window_handle, &rect );
  return rect;
}

WindowType GetWindowType( const HWND window_handle ) {
  using DetWindType_t = WindowType ( * )( const HWND window_handle );

  static std::map<ATOM, DetWindType_t> control_atom_map;

  // Add the default win32 control class names to a map
  static const std::map<std::wstring, DetWindType_t> control_name_map = {
    std::make_pair(
        TEXT( "Button" ),
        []( const HWND window_handle ) {
          const LONG_PTR styles = GetWindowLongPtr( window_handle, GWL_STYLE );
          const auto typemask = styles & BS_TYPEMASK;

          if ( typemask == BS_RADIOBUTTON || typemask == BS_AUTORADIOBUTTON ) {
            return WindowType::kRadioButton;
          } else if ( typemask == BS_GROUPBOX ) {
            return WindowType::kGroupBox;
          } else if ( typemask == BS_CHECKBOX || typemask == BS_AUTOCHECKBOX ) {
            return WindowType::kCheckBox;
          } else {
            return WindowType::kButton;
          }
        } ),
    std::make_pair(
        TEXT( "Edit" ),
        []( const HWND window_handle ) { return WindowType::kEdit; } ),
    std::make_pair(
        TEXT( "ComboBox" ),
        []( const HWND window_handle ) { return WindowType::kCombobox; } ),
    std::make_pair(
        TEXT( "ListBox" ),
        []( const HWND window_handle ) { return WindowType::kListbox; } ),
    std::make_pair(
        TEXT( "SysListView32" ),
        []( const HWND window_handle ) { return WindowType::kListView; } ),
    std::make_pair(
        TEXT( "SysHeader32" ),
        []( const HWND window_handle ) { return WindowType::kHeaderControl; } ),
    std::make_pair(
        TEXT( "msctls_trackbar32" ),
        []( const HWND window_handle ) { return WindowType::kTrackbar; } ),
    std::make_pair(
        TEXT( "RichEdit20W" ),
        []( const HWND window_handle ) { return WindowType::kRichEdit20; } ),
    std::make_pair(
        TEXT( "RichEdit20A" ),
        []( const HWND window_handle ) { return WindowType::kRichEdit20; } ),
    std::make_pair(
        TEXT( "RichEdit50W" ),
        []( const HWND window_handle ) { return WindowType::kRichEdit50; } ),
    std::make_pair(
        TEXT( "RichEdit50A" ),
        []( const HWND window_handle ) { return WindowType::kRichEdit50; } ),
    std::make_pair(
        TEXT( "Static" ),
        []( const HWND window_handle ) { return WindowType::kStaticText; } ),
    std::make_pair(
        TEXT( "SysTabControl32" ),
        []( const HWND window_handle ) { return WindowType::kTabControl; } ),
    std::make_pair(
        TEXT( "Static" ),
        []( const HWND window_handle ) { return WindowType::kStaticText; } ),
    std::make_pair(
        TEXT( "msctls_progress32" ),
        []( const HWND window_handle ) { return WindowType::kProgressBar; } ),
    std::make_pair(
        TEXT( "#32770" ),
        []( const HWND window_handle ) { return WindowType::kDialog; } ),
    std::make_pair(
        TEXT( "msctls_hotkey32" ),
        []( const HWND window_handle ) { return WindowType::kHotKey; } ),
  };

  // Get the class atom of the control handle
  const auto atom =
      static_cast<ATOM>( GetClassLongPtr( window_handle, GCW_ATOM ) );

  const auto found_atom = control_atom_map.find( atom );

  // Have we saved the atom?
  if ( found_atom != control_atom_map.end() ) {
    // Call the initialization for that control
    return found_atom->second( window_handle );
  } else {
    // If the control has not been previously saved, use the control classname
    const uint32_t kClassNameBufferSize = 100;
    wchar_t classname[ kClassNameBufferSize ];

    GetClassName( window_handle, classname, kClassNameBufferSize - 1 );

    const auto found_class = control_name_map.find( classname );

    // If we found it, call the init
    if ( found_class != control_name_map.end() ) {
      // Add to the atom table
      control_atom_map[ atom ] = found_class->second;

      return found_class->second( window_handle );
    }
  }

  return WindowType::kUnknown;
}  // namespace control

}  // namespace control

}  // namespace gwingui