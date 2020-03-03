#include "pch.h"
#include "editcontrol.h"
#include "control.h"

namespace gwingui {

namespace editcontrol {

std::wstring GetText( const HWND edit_control_handle ) {
  if (!edit_control_handle)
    throw std::runtime_error( "invalid edit control handle" );

  return control::GetText( edit_control_handle );
}

void GetBigText( const HWND edit_control_handle,
                 std::wstring* text,
                 const uint32_t buf_size ) {
  if ( !edit_control_handle )
    throw std::runtime_error( "invalid edit control handle" );

  text->resize( buf_size );

  GetWindowText( edit_control_handle, &( *text )[ 0 ], buf_size + 1 );
}

bool SetText( const HWND edit_control_handle, const std::wstring& text ) {
  if ( !edit_control_handle )
    throw std::runtime_error( "invalid edit control handle" );

  return !!control::SetText( edit_control_handle, text );
}

std::wstring GetSelectedText( const HWND edit_control_handle ) {
  if ( !edit_control_handle )
    throw std::runtime_error( "invalid edit control handle" );

  const auto selection = GetSelectedIndex( edit_control_handle );

  std::wstring text = GetText( edit_control_handle );

  text = text.substr( selection.cpMin, selection.cpMax - selection.cpMin );

  return text;
}

CHARRANGE GetSelectedIndex( const HWND edit_control_handle ) {
  if ( !edit_control_handle )
    throw std::runtime_error( "invalid edit control handle" );

  DWORD sel_start_index;
  DWORD sel_end_index;

  SendMessage( edit_control_handle, EM_GETSEL,
               reinterpret_cast<WPARAM>( &sel_start_index ),
               reinterpret_cast<LPARAM>( &sel_end_index ) );

  CHARRANGE sel;
  sel.cpMin = sel_start_index;
  sel.cpMax = sel_end_index;

  return sel;
}

void AppendText( const HWND edit_control_handle, const std::wstring& text ) {
  if ( !edit_control_handle )
    throw std::runtime_error( "invalid edit control handle" );

  // Save current selection or caret position
  const auto selection = GetSelectedIndex( edit_control_handle );

  const int text_length = GetWindowTextLength( edit_control_handle );

  // Set the caret at end of text
  SendMessage( edit_control_handle, EM_SETSEL, text_length, text_length );

  SendMessage( edit_control_handle, EM_REPLACESEL, TRUE,
               reinterpret_cast<LPARAM>( text.c_str() ) );

  // Restore saved caret pos or selection
  SendMessage( edit_control_handle, EM_SETSEL, selection.cpMin,
               selection.cpMax );
}

int32_t GetInt( const HWND edit_control_handle ) {
  if ( !edit_control_handle )
    throw std::runtime_error( "invalid edit control handle" );

  const auto text = GetText( edit_control_handle );

  return std::stoi( text );
}

bool SetInt( const HWND edit_control_handle, const int32_t value ) {
  if ( !edit_control_handle )
    throw std::runtime_error( "invalid edit control handle" );

  return SetText( edit_control_handle, std::to_wstring( value ) );
}

float GetFloat( const HWND edit_control_handle ) {
  if ( !edit_control_handle )
    throw std::runtime_error( "invalid edit control handle" );

  const auto text = GetText( edit_control_handle );

  return std::stof( text );
}

bool SetFloat( const HWND edit_control_handle, const float value ) {
  if ( !edit_control_handle )
    throw std::runtime_error( "invalid edit control handle" );

  return SetText( edit_control_handle, std::to_wstring( value ) );
}

bool ShowBalloonTip( const HWND edit_control_handle,
                     const std::wstring& title,
                     const std::wstring& description ) {
  if ( !edit_control_handle )
    throw std::runtime_error( "invalid edit control handle" );

  // Icons:
  // https://msdn.microsoft.com/en-us/library/windows/desktop/bb775466%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396
  EDITBALLOONTIP balloon;
  balloon.cbStruct = sizeof( EDITBALLOONTIP );
  balloon.pszText = description.c_str();
  balloon.pszTitle = title.c_str();
  balloon.ttiIcon = TTI_NONE;

  const LRESULT result = SendMessage( edit_control_handle, EM_SHOWBALLOONTIP, 0,
                                      ( LPARAM )&balloon );

  return !!result;
}

void SetPlaceholder( const HWND edit_control_handle,
                     const std::wstring& placeholder ) {
  if ( !edit_control_handle )
    throw std::runtime_error( "invalid edit control handle" );

  SendMessage( edit_control_handle, EM_SETCUEBANNER, TRUE,
               reinterpret_cast<LPARAM>( placeholder.c_str() ) );
}

}  // namespace editcontrol

}  // namespace gwingui