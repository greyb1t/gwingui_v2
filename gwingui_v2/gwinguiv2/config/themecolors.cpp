#include "pch.h"
#include "themecolors.h"

namespace gwingui {

namespace themecolors {

COLORREF kGeneralText = RGB( 214, 214, 214 );
COLORREF kGeneralTextHover = RGB( 224, 224, 224 );
COLORREF kGeneralTextDisabled = RGB( 144, 144, 144 );
COLORREF kGeneralBorderDisabled = RGB( 50, 50, 50 );
COLORREF kGeneralBackgroundDisabled = RGB( 37, 37, 37 );

COLORREF kTitlebarBorder = RGB( 96, 106, 119 );
// COLORREF kTitlebarBorder = RGB( 32, 32, 35 );
COLORREF kTitlebarBackground = RGB( 32, 32, 35 );
COLORREF kWindowBackground = RGB( 45, 45, 48 );

SafeBrush GeneralBackgroundBrush = CreateSolidBrush( kWindowBackground );

COLORREF kMenuBarItemBackground = kTitlebarBackground;
COLORREF kMenuBarItemBackgroundHover = RGB( 47, 47, 50 );
COLORREF kMenuBarItemText = kGeneralText;

namespace button {

COLORREF kBorder = RGB( 106, 106, 108 );
COLORREF kBorderDisabled = kGeneralBorderDisabled;
COLORREF kText = kGeneralText;
COLORREF kTextHover = kGeneralTextHover;
COLORREF kTextDisabled = kGeneralTextDisabled;
COLORREF kBackgroundDown = RGB( 32, 32, 35 );
COLORREF kBackgroundHover = RGB( 72, 72, 77 );
COLORREF kBackgroundNormal = RGB( 45, 45, 48 );
COLORREF kBackgroundDisabled = kGeneralBackgroundDisabled;

}  // namespace button

namespace checkbox {

COLORREF kText = kGeneralText;
COLORREF kTextDisabled = kGeneralTextDisabled;
}  // namespace checkbox

namespace radiobutton {

COLORREF kText = kGeneralText;
COLORREF kTextDisabled = kGeneralTextDisabled;
}  // namespace radiobutton

namespace groupbox {

COLORREF kText = kGeneralText;
COLORREF kLine = RGB( 210, 210, 210 );
}  // namespace groupbox

namespace editcontrol {

COLORREF kText = kGeneralText;
COLORREF kTextDisabled = RGB( 255, 0, 0 );  // temp
COLORREF kTextPlaceholder = RGB( 130, 130, 130 );
COLORREF kTextPlaceholderDisabled = RGB( 100, 100, 100 );

COLORREF kBackground = RGB( 69, 69, 70 );
COLORREF kBackgroundDisabled = kGeneralBackgroundDisabled;

COLORREF kBorderNormal = RGB( 89, 89, 90 );
COLORREF kBorderFocus = RGB( 98, 201, 251 );
COLORREF kBorderDisabled = kGeneralBackgroundDisabled;

SafeBrush BackgroundBrush = CreateSolidBrush( editcontrol::kBackground );
SafeBrush BackgroundBrushDisabled =
    CreateSolidBrush( editcontrol::kBackgroundDisabled );

}  // namespace editcontrol

namespace combobox {

COLORREF kBackground = RGB( 52, 52, 55 );
COLORREF kBackgroundHover = RGB( 72, 72, 77 );
COLORREF kBackgroundDisabled = kGeneralBackgroundDisabled;

COLORREF kItemBackground = RGB( 42, 42, 45 );
COLORREF kItemBackgroundFocused = RGB( 62, 62, 65 );

COLORREF kText = kGeneralText;
COLORREF kTextHover = kGeneralTextHover;
COLORREF kTextDisabled = kGeneralTextDisabled;

COLORREF kBorder = RGB( 61, 61, 63 );
COLORREF kBorderHover = RGB( 95, 95, 101 );
COLORREF kBorderDisabled = kGeneralBorderDisabled;

}  // namespace combobox

namespace listbox {

COLORREF kBackground = RGB( 45, 45, 48 );
COLORREF kBackgroundDisabled = kGeneralBackgroundDisabled;
COLORREF kBorder = RGB( 130, 135, 144 );
COLORREF kBorderDisabled = RGB( 83, 87, 94 );
// COLORREF kItemBorder = RGB( 89, 89, 90 );
// COLORREF kItemBorderActive = RGB( 155, 155, 155 );
// COLORREF kItemBorderHover = RGB( 145, 145, 145 );
// COLORREF kItemBorderDisabled = kGeneralBorderDisabled;
COLORREF kItemBackgroundEven = RGB( 52, 52, 57 );
COLORREF kItemBackgroundOdd = RGB( 45, 45, 48 );
COLORREF kItemBackgroundActive = RGB( 135, 135, 135 );
COLORREF kItemBackgroundHover = RGB( 82, 82, 87 );
COLORREF kItemBackgroundDisabled = kGeneralBackgroundDisabled;
COLORREF kText = kGeneralText;
COLORREF kTextActive = RGB( 255, 255, 255 );
COLORREF kTextDisabled = kGeneralTextDisabled;

SafeBrush BackgroundBrush = CreateSolidBrush( listbox::kBackground );
SafeBrush BackgroundBrushDisabled =
    CreateSolidBrush( listbox::kBackgroundDisabled );

}  // namespace listbox

namespace listview {

COLORREF kBackground = RGB( 45, 45, 48 );
COLORREF kBorder = RGB( 130, 135, 144 );
COLORREF kBorderDisabled = RGB( 83, 87, 94 );

COLORREF kItemOdd = RGB( 45, 45, 48 );
COLORREF kItemEven = RGB( 52, 52, 57 );
COLORREF kItemFocus = RGB( 64, 177, 208 );
COLORREF kItemDisabled = kGeneralBackgroundDisabled;

COLORREF kText = kGeneralText;
COLORREF kTextFocus = RGB( 255, 255, 255 );
COLORREF kTextDisabled = kGeneralTextDisabled;

}  // namespace listview

namespace header {

COLORREF kItemBackground RGB( 32, 32, 35 );
COLORREF kItemBorder = RGB( 32, 32, 35 );
COLORREF kText = RGB( 156, 164, 181 );

}  // namespace header

namespace trackbar {

COLORREF kBackground = RGB( 45, 45, 48 );

COLORREF kChannel = RGB( 52, 52, 55 );
COLORREF kChannelBorder = RGB( 110, 110, 110 );

COLORREF kThumb = RGB( 205, 205, 205 );
COLORREF kThumbBorder = RGB( 255, 255, 255 );
COLORREF kThumbHover = RGB( 255, 255, 255 );
COLORREF kThumbFocus = RGB( 255, 255, 255 );

SafeBrush BackgroundBrush = CreateSolidBrush( trackbar::kBackground );

}  // namespace trackbar

namespace richedit {

COLORREF kText = kGeneralText;
COLORREF kBackground = kWindowBackground;
COLORREF kBorder = RGB( 130, 135, 144 );

}  // namespace richedit

namespace tabcontrol {

COLORREF kText = RGB( 152, 158, 165 );
COLORREF kTextHover = RGB( 185, 185, 185 );
COLORREF kTextSelected = RGB( 185, 185, 185 );

COLORREF kBackground = kTitlebarBackground;
COLORREF kBackgroundTabHover = RGB( 57, 57, 60 );
COLORREF kBackgroundTabSelected = RGB( 45, 45, 48 );

COLORREF kTabUnderlineHover = RGB( 65, 177, 225 );
COLORREF kTabUnderlineSelected = RGB( 25, 137, 185 );
COLORREF kTabUnderlineUnSelected = RGB( 204, 204, 204 );

}  // namespace tabcontrol

namespace progressbar {

COLORREF kBorder = kTitlebarBackground;
COLORREF kBackgroundFilled = RGB( 25, 137, 185 );
COLORREF kBackgroundEmpty = RGB( 191, 82, 82 );

}  // namespace progressbar

namespace hotkey {

COLORREF kBackground = RGB( 45, 45, 48 );
COLORREF kText = kGeneralText;
COLORREF kBorder = RGB( 106, 106, 108 );

COLORREF kBackgroundFocus = RGB( 32, 32, 35 );
COLORREF kTextFocus = kGeneralTextHover;
COLORREF kBorderFocus = RGB( 126, 126, 128 );

COLORREF kBackgroundDisabled = kGeneralBackgroundDisabled;
COLORREF kTextDisabled = kGeneralTextDisabled;
COLORREF kBorderDisabled = kGeneralBorderDisabled;

}  // namespace hotkey

}  // namespace themecolors

}  // namespace gwingui