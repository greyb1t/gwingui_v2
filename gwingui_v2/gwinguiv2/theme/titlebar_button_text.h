#pragma once

#include <string>

#include "titlebar_button.h"

namespace gwingui {

class TitlebarButtonText : public TitlebarButton {
 public:
  TitlebarButtonText( const std::wstring& text )
      : text_( text ), text_rect_{ 0 } {}

  RECT text_rect_;
  const std::wstring text_;
};

}  // namespace gwingui