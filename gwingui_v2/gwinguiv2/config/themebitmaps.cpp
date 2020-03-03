#include "pch.h"
#include "themebitmaps.h"
#include "../drawing.h"

namespace gwingui {

namespace themebitmaps {

SafeBitmap g_titlebar_x_normal = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::TitlebarXNormal::buf,
    themebitmapbuffers::TitlebarXNormal::width,
    themebitmapbuffers::TitlebarXNormal::height );

SafeBitmap g_titlebar_x_hover = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::TitlebarXHover::buf,
    themebitmapbuffers::TitlebarXHover::width,
    themebitmapbuffers::TitlebarXHover::height );

SafeBitmap g_titlebar_x_down = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::TitlebarXDown::buf,
    themebitmapbuffers::TitlebarXDown::width,
    themebitmapbuffers::TitlebarXDown::height );

SafeBitmap g_titlebar_maximize_normal = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::TitlebarMaxNormal::buf,
    themebitmapbuffers::TitlebarMaxNormal::width,
    themebitmapbuffers::TitlebarMaxNormal::height );

SafeBitmap g_titlebar_maximize_hover = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::TitlebarMaxHover::buf,
    themebitmapbuffers::TitlebarMaxHover::width,
    themebitmapbuffers::TitlebarMaxHover::height );

SafeBitmap g_titlebar_maximize_down = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::TitlebarMaxDown::buf,
    themebitmapbuffers::TitlebarMaxDown::width,
    themebitmapbuffers::TitlebarMaxDown::height );

SafeBitmap g_titlebar_minimize_normal = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::TitlebarMinNormal::buf,
    themebitmapbuffers::TitlebarMinNormal::width,
    themebitmapbuffers::TitlebarMinNormal::height );

SafeBitmap g_titlebar_minimize_hover = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::TitlebarMinHover::buf,
    themebitmapbuffers::TitlebarMinHover::width,
    themebitmapbuffers::TitlebarMinHover::height );

SafeBitmap g_titlebar_minimize_down = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::TitlebarMinDown::buf,
    themebitmapbuffers::TitlebarMinDown::width,
    themebitmapbuffers::TitlebarMinDown::height );

SafeBitmap g_titlebar_icon =
    drawing::CreateBitmapFromBuffer( themebitmapbuffers::TitlebarIcon::buf,
                                     themebitmapbuffers::TitlebarIcon::width,
                                     themebitmapbuffers::TitlebarIcon::height );

SafeBitmap g_bitmap_checked_handle = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::CheckBoxNormalChecked::buf,
    themebitmapbuffers::CheckBoxNormalChecked::width,
    themebitmapbuffers::CheckBoxNormalChecked::height );

SafeBitmap g_bitmap_unchecked_handle = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::CheckBoxNormalUnchecked::buf,
    themebitmapbuffers::CheckBoxNormalUnchecked::width,
    themebitmapbuffers::CheckBoxNormalUnchecked::height );

SafeBitmap g_bitmap_checked_hover_handle = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::CheckBoxHoverChecked::buf,
    themebitmapbuffers::CheckBoxHoverChecked::width,
    themebitmapbuffers::CheckBoxHoverChecked::height );

SafeBitmap g_bitmap_unchecked_hover_handle = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::CheckBoxHoverUnchecked::buf,
    themebitmapbuffers::CheckBoxHoverUnchecked::width,
    themebitmapbuffers::CheckBoxHoverUnchecked::height );

SafeBitmap g_bitmap_checked_pushed_handle = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::CheckBoxCheckedPushed::buf,
    themebitmapbuffers::CheckBoxCheckedPushed::width,
    themebitmapbuffers::CheckBoxCheckedPushed::height );

SafeBitmap g_bitmap_unchecked_pushed_handle = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::CheckBoxUncheckedPushed::buf,
    themebitmapbuffers::CheckBoxUncheckedPushed::width,
    themebitmapbuffers::CheckBoxUncheckedPushed::height );

SafeBitmap g_bitmap_checked_disabled_handle = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::CheckBoxCheckedDisabled::buf,
    themebitmapbuffers::CheckBoxCheckedDisabled::width,
    themebitmapbuffers::CheckBoxCheckedDisabled::height );

SafeBitmap g_bitmap_unchecked_disabled_handle = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::CheckBoxUncheckedDisabled::buf,
    themebitmapbuffers::CheckBoxUncheckedDisabled::width,
    themebitmapbuffers::CheckBoxUncheckedDisabled::height );

SafeBitmap g_bitmap_combobox_arrow_normal = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::ComboDropdownNormal::buf,
    themebitmapbuffers::ComboDropdownNormal::width,
    themebitmapbuffers::ComboDropdownNormal::height );

SafeBitmap g_bitmap_combobox_arrow_disabled = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::ComboDropdownDisabled::buf,
    themebitmapbuffers::ComboDropdownDisabled::width,
    themebitmapbuffers::ComboDropdownDisabled::height );

SafeBitmap g_bitmap_combobox_arrow_hover = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::ComboDropdownHover::buf,
    themebitmapbuffers::ComboDropdownHover::width,
    themebitmapbuffers::ComboDropdownHover::height );

SafeBitmap g_bitmap_combobox_arrow_down = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::ComboDropdownDown::buf,
    themebitmapbuffers::ComboDropdownDown::width,
    themebitmapbuffers::ComboDropdownDown::height );

SafeBitmap g_bitmap_rb_checked = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::RadioButtonNormalChecked::buf,
    themebitmapbuffers::RadioButtonNormalChecked::width,
    themebitmapbuffers::RadioButtonNormalChecked::height );

SafeBitmap g_bitmap_rb_unchecked = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::RadioButtonNormalUnchecked::buf,
    themebitmapbuffers::RadioButtonNormalUnchecked::width,
    themebitmapbuffers::RadioButtonNormalUnchecked::height );

SafeBitmap g_bitmap_rb_hover_checked = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::RadioButtonNormalHoverChecked::buf,
    themebitmapbuffers::RadioButtonNormalHoverChecked::width,
    themebitmapbuffers::RadioButtonNormalHoverChecked::height );

SafeBitmap g_bitmap_rb_hover_unchecked = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::RadioButtonNormalHoverUnchecked::buf,
    themebitmapbuffers::RadioButtonNormalHoverUnchecked::width,
    themebitmapbuffers::RadioButtonNormalHoverUnchecked::height );

SafeBitmap g_bitmap_rb_pushed = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::RadioButtonPushed::buf,
    themebitmapbuffers::RadioButtonPushed::width,
    themebitmapbuffers::RadioButtonPushed::height );

SafeBitmap g_bitmap_rb_disabled_checked = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::RadioButtonDisabledChecked::buf,
    themebitmapbuffers::RadioButtonDisabledChecked::width,
    themebitmapbuffers::RadioButtonDisabledChecked::height );

SafeBitmap g_bitmap_rb_disabled_unchecked = drawing::CreateBitmapFromBuffer(
    themebitmapbuffers::RadioButtonDisabledUnchecked::buf,
    themebitmapbuffers::RadioButtonDisabledUnchecked::width,
    themebitmapbuffers::RadioButtonDisabledUnchecked::height );

}  // namespace themebitmaps

}  // namespace gwingui