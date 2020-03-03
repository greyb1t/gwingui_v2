#pragma once

#include <string>
#include <Windows.h>

// TODO: In the future, create my own message box by creating a modal dialog box
// template in memory

namespace gwingui {

namespace messagebox {

void Error( const std::wstring& message,
            const std::wstring& title = TEXT( "Error" ),
            const HWND parent_handle = nullptr );
void Warning( const std::wstring& message,
              const std::wstring& title = TEXT( "Warning" ),
              const HWND parent_handle = nullptr );

}  // namespace messagebox

}  // namespace gwingui