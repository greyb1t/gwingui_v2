#pragma once

#include "titlebar_button_text.h"

namespace gwingui {

class TitlebarButtonTextMenuItem : public TitlebarButtonText {
 public:
  TitlebarButtonTextMenuItem( const std::wstring& text )
      : TitlebarButtonText( text ) {}

  HWND context_menu_handle;
};

}  // namespace gwingui