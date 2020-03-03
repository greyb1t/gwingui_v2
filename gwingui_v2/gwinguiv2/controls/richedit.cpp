#include "pch.h"
#include "richedit.h"

#include "editcontrol.h"

namespace gwingui {

namespace richedit {

void SetTextLimit( const HWND richedit_handle, int max_characters ) {
  if ( !richedit_handle )
    throw std::runtime_error( "invalid richedit handle" );

  SendMessage( richedit_handle, EM_EXLIMITTEXT, 0, max_characters );
}

int FindStringIndex( const HWND richedit_handle, const std::wstring& text ) {
  if ( !richedit_handle )
    throw std::runtime_error( "invalid richedit handle" );

  FINDTEXTEX ftx;
  ftx.chrg.cpMin = 0;
  ftx.chrg.cpMax = -1;
  ftx.lpstrText = text.c_str();

  const LRESULT position = SendMessage( richedit_handle, EM_FINDTEXTEX, FR_DOWN,
                                        reinterpret_cast<LPARAM>( &ftx ) );

  SetSelection( richedit_handle, ftx.chrgText.cpMin, ftx.chrgText.cpMax );

  return static_cast<int>( position );
}

int FindStringIndexCaseSensitive( const HWND richedit_handle,
                                  const std::wstring& text ) {
  if ( !richedit_handle )
    throw std::runtime_error( "invalid richedit handle" );

  FINDTEXTEX ftx;

  ftx.chrg.cpMin = 0;
  ftx.chrg.cpMax = -1;
  ftx.lpstrText = text.c_str();

  const LRESULT position =
      SendMessage( richedit_handle, EM_FINDTEXTEX,
                   FR_DOWN | FR_MATCHCASE | FR_WHOLEWORD, ( LPARAM )&ftx );

  SetSelection( richedit_handle, ftx.chrgText.cpMin, ftx.chrgText.cpMax );

  return static_cast<int>( position );
}

std::wstring GetSelectedText( const HWND richedit_handle ) {
  if ( !richedit_handle )
    throw std::runtime_error( "invalid richedit handle" );

  const auto selection = GetSelection( richedit_handle );

  // Allocate a buffer same size as the selection,
  // don't know if compatible with unicode and other shit, works fine for simple
  // shit tho
  std::wstring str_buf;
  str_buf.resize( selection.cpMax - selection.cpMin );

  SendMessage( richedit_handle, EM_GETSELTEXT, 0,
               reinterpret_cast<LPARAM>( &str_buf[ 0 ] ) );

  return str_buf;
}

bool SetText( const HWND richedit_handle, const std::wstring& text ) {
  if ( !richedit_handle )
    throw std::runtime_error( "invalid richedit handle" );

  SETTEXTEX stx = { 0 };
  stx.flags = ST_DEFAULT | ST_UNICODE;
  stx.codepage = 1200;

  const LRESULT result =
      SendMessage( richedit_handle, EM_SETTEXTEX,
                   reinterpret_cast<WPARAM>( &stx ), ( LPARAM )text.c_str() );

  return !!result;
}

std::wstring GetText( const HWND richedit_handle ) {
  if ( !richedit_handle )
    throw std::runtime_error( "invalid richedit handle" );

  const int32_t buf_size = GetTextBufferSize( richedit_handle );

  const int32_t text_length = GetTextLength( richedit_handle );

  std::wstring str_buf;
  str_buf.resize( text_length );

  GETTEXTEX gtx = { 0 };
  gtx.cb = buf_size + sizeof( wchar_t );
  gtx.flags = GT_DEFAULT;
  gtx.codepage = 1200;

  SendMessage( richedit_handle, EM_GETTEXTEX, ( WPARAM )&gtx,
               reinterpret_cast<LPARAM>( &str_buf[ 0 ] ) );

  return str_buf;
}

void SetSelection( const HWND richedit_handle,
                   const uint32_t min,
                   const uint32_t max ) {
  if ( !richedit_handle )
    throw std::runtime_error( "invalid richedit handle" );

  CHARRANGE cr;
  cr.cpMin = min;
  cr.cpMax = max;

  SendMessage( richedit_handle, EM_EXSETSEL, 0,
               reinterpret_cast<LPARAM>( &cr ) );
}

CHARRANGE GetSelection( const HWND richedit_handle ) {
  if ( !richedit_handle )
    throw std::runtime_error( "invalid richedit handle" );

  CHARRANGE cr = { 0 };

  SendMessage( richedit_handle, EM_EXGETSEL, 0,
               reinterpret_cast<LPARAM>( &cr ) );

  return cr;
}

void AppendText( const HWND richedit_handle, const std::wstring& text ) {
  editcontrol::AppendText( richedit_handle, text );
}

LRESULT GetTextLength( const HWND richedit_handle ) {
  if ( !richedit_handle )
    throw std::runtime_error( "invalid richedit handle" );

  GETTEXTLENGTHEX gtlex = { 0 };
  gtlex.flags = GTL_DEFAULT;
  gtlex.codepage = 1200;

  return SendMessage( richedit_handle, EM_GETTEXTLENGTHEX, ( WPARAM )&gtlex,
                      0 );
}

LRESULT GetTextBufferSize( const HWND richedit_handle ) {
  if ( !richedit_handle )
    throw std::runtime_error( "invalid richedit handle" );

  GETTEXTLENGTHEX gtlex = { 0 };
  gtlex.flags = GTL_NUMBYTES | GTL_PRECISE;
  gtlex.codepage = 1200;

  return SendMessage( richedit_handle, EM_GETTEXTLENGTHEX, ( WPARAM )&gtlex,
                      0 );
}

void ScrollToBottom( const HWND richedit_handle ) {
  if ( !richedit_handle )
    throw std::runtime_error( "invalid richedit handle" );

  SendMessage( richedit_handle, WM_VSCROLL, SB_BOTTOM, 0L );
}

void EnableTextChangedNotification( const HWND richedit_handle ) {
  if ( !richedit_handle )
    throw std::runtime_error( "invalid richedit handle" );

  SendMessage( richedit_handle, EM_SETEVENTMASK, NULL, ENM_CHANGE );
}

bool SetTextColor( const HWND richedit_handle, const COLORREF color ) {
  if ( !richedit_handle )
    throw std::runtime_error( "invalid richedit handle" );

  CHARFORMAT cf = { 0 };

  cf.cbSize = sizeof( CHARFORMAT );
  cf.dwMask = CFM_COLOR;
  cf.crTextColor = color;

  const LRESULT result =
      SendMessage( richedit_handle, EM_SETCHARFORMAT, 0, ( LPARAM )&cf );

  return !!result;
}

bool SetTextItalic( const HWND richedit_handle, const bool italic ) {
  if ( !richedit_handle )
    throw std::runtime_error( "invalid richedit handle" );

  CHARFORMAT cf = { 0 };
  cf.cbSize = sizeof( CHARFORMAT );
  cf.dwMask = CFM_ITALIC;

  if ( italic )
    cf.dwEffects = CFE_ITALIC;
  else
    cf.dwEffects = 0;

  const LRESULT result =
      SendMessage( richedit_handle, EM_SETCHARFORMAT, 0, ( LPARAM )&cf );

  return !!result;
}

bool SetTextBold( const HWND richedit_handle, const bool bold ) {
  if ( !richedit_handle )
    throw std::runtime_error( "invalid richedit handle" );

  CHARFORMAT cf = { 0 };

  cf.cbSize = sizeof( CHARFORMAT );
  cf.dwMask = CFM_BOLD;

  if ( bold )
    cf.dwEffects = CFE_BOLD;
  else
    cf.dwEffects = 0;

  const LRESULT result =
      SendMessage( richedit_handle, EM_SETCHARFORMAT, 0, ( LPARAM )&cf );

  return !!result;
}

bool SetTextSize( const HWND richedit_handle, const uint32_t size ) {
  if ( !richedit_handle )
    throw std::runtime_error( "invalid richedit handle" );

  SetSelection( richedit_handle, 0, -1 );
  const LRESULT result =
      SendMessage( richedit_handle, EM_SETFONTSIZE, size, 0 );

  return !!result;
}

void SetFont( const HWND richedit_handle,
              const std::wstring& font_name,
              const uint32_t size ) {
  if ( !richedit_handle )
    throw std::runtime_error( "invalid richedit handle" );

  // Setup char format
  CHARFORMAT cfFormat;
  memset( &cfFormat, 0, sizeof( cfFormat ) );
  cfFormat.cbSize = sizeof( cfFormat );
  cfFormat.dwMask = CFM_CHARSET | CFM_FACE | CFM_SIZE;
  cfFormat.bCharSet = ANSI_CHARSET;
  cfFormat.bPitchAndFamily = FIXED_PITCH | FF_DONTCARE;
  cfFormat.yHeight = ( size * 1440 ) / 72;
  wcscpy_s( cfFormat.szFaceName, font_name.c_str() );

  // Set char format and goto end of text
  CHARRANGE cr;
  cr.cpMin = INT_MAX;
  cr.cpMax = INT_MAX;

  SendMessage( richedit_handle, EM_SETCHARFORMAT, SCF_ALL,
               ( LPARAM )&cfFormat );

  SendMessage( richedit_handle, EM_EXSETSEL, 0, ( LPARAM )&cr );
}

}  // namespace richedit

}  // namespace gwingui