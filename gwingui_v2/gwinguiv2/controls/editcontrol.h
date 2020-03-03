#pragma once

#include <Windows.h>
#include <string>
#include <Richedit.h>

namespace gwingui {

namespace editcontrol {

// TODO: Add more advanced function to edit control, not done

// GetText returns the text inside the edit control
std::wstring GetText( const HWND edit_control_handle );

// GetBigText gives back the text isnide an edit control
void GetBigText( const HWND edit_control_handle,
                 std::wstring* text,
                 const uint32_t bufferSize );

// SetText sets the text inside the edit control
// returns false if failed
bool SetText( const HWND edit_control_handle, const std::wstring& text );

// GetSelectedText returns the selected string in the edit control
std::wstring GetSelectedText( const HWND edit_control_handle );

// GetSelectedIndex returns the selected text start and ending indexes
CHARRANGE GetSelectedIndex( const HWND edit_control_handle );

// AppendText appends text to an edit control
void AppendText( const HWND edit_control_handle, const std::wstring& text );

// GetInt returns text of an edit control into an int
int32_t GetInt( const HWND edit_control_handle );

// SetInt sets the text of the edit control to the specified int value
bool SetInt( const HWND edit_control_handle, const int32_t value );

// GetFloat returns text of an edit control into an float
float GetFloat( const HWND edit_control_handle );

// SetFloat sets the text of the edit control to the specified float value
bool SetFloat( const HWND edit_control_handle, const float value );

// ShowBalloonTip shows a message inside a balloon thingy
bool ShowBalloonTip( const HWND edit_control_handle,
                     const std::wstring& title,
                     const std::wstring& description );

// SetPlaceholder sets a little placeholder text in an edit control
// multiline edit controls are not supported
void SetPlaceholder( const HWND edit_control_handle,
                     const std::wstring& placeholder );

}  // namespace editcontrol

}  // namespace gwingui