#pragma once

#include <windows.h>

#include "../config/themebitmaps.h"

namespace gwingui {

class TitlebarButton {
 public:
  TitlebarButton() : rect{ 0 }, is_down( false ), is_hovering( false ) {}

  RECT GetDrawRect( const uint32_t x_offset ) const {
    RECT rect_copy = rect;
    OffsetRect( &rect_copy, x_offset, 0 );
    return rect_copy;
  }

  RECT rect;
  bool is_down;
  bool is_hovering;
};

}  // namespace gwingui