#pragma once

namespace gwingui {

namespace trackbartheme {

LRESULT OnCustomDraw( const HWND parent_handle, NMCUSTOMDRAW* custom_draw );

void InitializeTrackbar( HWND trackbar_handle );

}

}