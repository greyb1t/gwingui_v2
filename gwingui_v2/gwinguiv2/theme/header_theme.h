#pragma once

namespace gwingui {

namespace headertheme {

// TODO: When compiling in x64 mode, the header control theme is not being
// drawn.

LRESULT OnCustomDraw( HWND parent_window_handle, NMCUSTOMDRAW* custom_draw );
}  // namespace headertheme

}  // namespace gwingui