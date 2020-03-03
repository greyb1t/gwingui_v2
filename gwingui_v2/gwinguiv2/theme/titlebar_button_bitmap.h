#pragma once

#include "titlebar_button.h"
#include "../config/themebitmaps.h"

namespace gwingui {

class TitlebarButtonBitmap : public TitlebarButton {
 public:
  TitlebarButtonBitmap( const SafeBitmap& bmp_normal,
                        const SafeBitmap& bmp_hover,
                        const SafeBitmap& bmp_down )
      : bmp_normal_( bmp_normal ),
        bmp_hover_( bmp_hover ),
        bmp_down_( bmp_down ) {}

  const SafeBitmap& bmp_normal_;
  const SafeBitmap& bmp_hover_;
  const SafeBitmap& bmp_down_;
};

}  // namespace gwingui