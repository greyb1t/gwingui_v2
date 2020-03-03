#pragma once

#include <Windows.h>

#include "../utils/safe_handles.h"

namespace gwingui {

namespace themecolors {

extern COLORREF kGeneralText;
extern COLORREF kGeneralTextHover;
extern COLORREF kGeneralTextDisabled;
extern COLORREF kGeneralBorderDisabled;
extern COLORREF kGeneralBackgroundDisabled;

extern COLORREF kTitlebarBorder;
extern COLORREF kTitlebarBackground;
extern COLORREF kWindowBackground;

extern SafeBrush GeneralBackgroundBrush;

extern COLORREF kMenuBarItemBackground;
extern COLORREF kMenuBarItemBackgroundHover;
extern COLORREF kMenuBarItemText;

namespace button {

extern COLORREF kBorder;
extern COLORREF kBorderDisabled;
extern COLORREF kText;
extern COLORREF kTextHover;
extern COLORREF kTextDisabled;
extern COLORREF kBackgroundDown;
extern COLORREF kBackgroundHover;
extern COLORREF kBackgroundNormal;
extern COLORREF kBackgroundDisabled;

}  // namespace button

namespace checkbox {

extern COLORREF kText;
extern COLORREF kTextDisabled;

}  // namespace checkbox

namespace radiobutton {

extern COLORREF kText;
extern COLORREF kTextDisabled;

}  // namespace radiobutton

namespace groupbox {

extern COLORREF kText;
extern COLORREF kLine;

}  // namespace groupbox

namespace editcontrol {

extern COLORREF kText;
extern COLORREF kTextDisabled;
extern COLORREF kTextPlaceholder;
extern COLORREF kTextPlaceholderDisabled;

extern COLORREF kBackground;
extern COLORREF kBackgroundDisabled;

extern COLORREF kBorderNormal;
extern COLORREF kBorderFocus;
extern COLORREF kBorderDisabled;

extern SafeBrush BackgroundBrush;
extern SafeBrush BackgroundBrushDisabled;

}  // namespace editcontrol

namespace combobox {

extern COLORREF kBackground;
extern COLORREF kBackgroundHover;
extern COLORREF kBackgroundDisabled;

extern COLORREF kItemBackground;
extern COLORREF kItemBackgroundFocused;

extern COLORREF kText;
extern COLORREF kTextHover;
extern COLORREF kTextDisabled;

extern COLORREF kBorder;
extern COLORREF kBorderHover;
extern COLORREF kBorderDisabled;

}  // namespace combobox

namespace listbox {

extern COLORREF kBackground;
extern COLORREF kBackgroundDisabled;
extern COLORREF kBorder;
extern COLORREF kBorderDisabled;
// extern COLORREF kItemBorder;
// extern COLORREF kItemBorderActive;
// extern COLORREF kItemBorderHover;
// extern COLORREF kItemBorderDisabled;
extern COLORREF kItemBackgroundEven;
extern COLORREF kItemBackgroundOdd;
extern COLORREF kItemBackgroundActive;
extern COLORREF kItemBackgroundHover;
extern COLORREF kItemBackgroundDisabled;
extern COLORREF kText;
extern COLORREF kTextActive;
extern COLORREF kTextDisabled;

extern SafeBrush BackgroundBrush;
extern SafeBrush BackgroundBrushDisabled;

}  // namespace listbox

namespace listview {

extern COLORREF kBackground;
extern COLORREF kBorder;
extern COLORREF kBorderDisabled;

extern COLORREF kItemOdd;
extern COLORREF kItemEven;
extern COLORREF kItemFocus;
extern COLORREF kItemDisabled;

extern COLORREF kText;
extern COLORREF kTextFocus;
extern COLORREF kTextDisabled;

}  // namespace listview

namespace header {

extern COLORREF kItemBackground;
extern COLORREF kItemBorder;
extern COLORREF kText;

}  // namespace header

namespace trackbar {

extern COLORREF kBackground;

extern COLORREF kChannel;
extern COLORREF kChannelBorder;

extern COLORREF kThumb;
extern COLORREF kThumbBorder;
extern COLORREF kThumbHover;
extern COLORREF kThumbFocus;

extern SafeBrush BackgroundBrush;

}  // namespace trackbar

namespace richedit {

extern COLORREF kText;
extern COLORREF kBackground;
extern COLORREF kBorder;

}  // namespace richedit

namespace tabcontrol {

extern COLORREF kText;
extern COLORREF kTextHover;
extern COLORREF kTextSelected;

extern COLORREF kBackground;
extern COLORREF kBackgroundTabHover;
extern COLORREF kBackgroundTabSelected;

extern COLORREF kTabUnderlineHover;
extern COLORREF kTabUnderlineSelected;
extern COLORREF kTabUnderlineUnSelected;

}  // namespace tabcontrol

namespace progressbar {

extern COLORREF kBorder;
extern COLORREF kBackgroundFilled;
extern COLORREF kBackgroundEmpty;

}  // namespace progressbar

namespace hotkey {

extern COLORREF kBackground;
extern COLORREF kText;
extern COLORREF kBorder;

extern COLORREF kBackgroundFocus;
extern COLORREF kTextFocus;
extern COLORREF kBorderFocus;

extern COLORREF kBackgroundDisabled;
extern COLORREF kTextDisabled;
extern COLORREF kBorderDisabled;

}  // namespace hotkey

}  // namespace themecolors

}  // namespace gwingui