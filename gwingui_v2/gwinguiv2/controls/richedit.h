#pragma once

#include <Windows.h>
#include <string>
#include <richedit.h>

namespace gwingui {

namespace richedit {

// SetTextLimit limits the amount of characters
void SetTextLimit( const HWND richedit_handle, int max_characters );

// FindStringIndex searches for the text
// returns index if found and selects it
// returns -1 if not found
int FindStringIndex( const HWND richedit_handle, const std::wstring& text );

int FindStringIndexCaseSensitive( const HWND richedit_handle,
                                  const std::wstring& text );

std::wstring GetSelectedText( const HWND richedit_handle );

bool SetText( const HWND richedit_handle, const std::wstring& text );

std::wstring GetText( const HWND richedit_handle );

void SetSelection( const HWND richedit_handle,
                   const uint32_t min,
                   const uint32_t max );

CHARRANGE GetSelection( const HWND richedit_handle );

void AppendText( const HWND richedit_handle, const std::wstring& text );

LRESULT GetTextLength( const HWND richedit_handle );

LRESULT GetTextBufferSize( const HWND richedit_handle );

void ScrollToBottom( const HWND richedit_handle );

void EnableTextChangedNotification( const HWND richedit_handle );

bool SetTextColor( const HWND richedit_handle, const COLORREF color );

bool SetTextItalic( const HWND richedit_handle, const bool italic );

bool SetTextBold( const HWND richedit_handle, const bool bold );

bool SetTextSize( const HWND richedit_handle, const uint32_t size );

void SetFont( const HWND richedit_handle,
              const std::wstring& font_name,
              const uint32_t size );

}  // namespace richedit

}  // namespace gwingui