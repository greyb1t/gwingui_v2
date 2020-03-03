#include "pch.h"
#include "themefont.h"
#include "../utils/safe_handles.h"

HFONT gwingui::themefont::GetDefaultThemeFont() {
  static LOGFONT font_desc = { 0 };
  font_desc.lfHeight = 14;
  font_desc.lfWidth = 0;
  font_desc.lfWeight = FW_MEDIUM;
  font_desc.lfItalic = FALSE;
  font_desc.lfUnderline = FALSE;
  font_desc.lfStrikeOut = FALSE;
  font_desc.lfCharSet = ANSI_CHARSET;  // Consider changing this?
  font_desc.lfOutPrecision = OUT_DEFAULT_PRECIS;
  font_desc.lfClipPrecision = CLIP_DEFAULT_PRECIS;
  font_desc.lfQuality = CLEARTYPE_QUALITY;
  font_desc.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

  wcscpy_s( font_desc.lfFaceName, TEXT( "Consolas" ) );

  static SafeFont font_handle = CreateFontIndirect( &font_desc );

  return font_handle.GetValue();
}

HFONT gwingui::themefont::GetDefaultGroupBoxFont() {
  static LOGFONT font_desc = { 0 };
  font_desc.lfHeight = 20;
  font_desc.lfWidth = 0;
  font_desc.lfWeight = FW_DONTCARE;
  font_desc.lfItalic = FALSE;
  font_desc.lfUnderline = FALSE;
  font_desc.lfStrikeOut = FALSE;
  font_desc.lfCharSet = ANSI_CHARSET;  // Consider changing this?
  font_desc.lfOutPrecision = OUT_DEFAULT_PRECIS;
  font_desc.lfClipPrecision = CLIP_DEFAULT_PRECIS;
  font_desc.lfQuality = CLEARTYPE_QUALITY;
  font_desc.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

  wcscpy_s( font_desc.lfFaceName, TEXT( "Consolas" ) );

  static SafeFont font_handle = CreateFontIndirect( &font_desc );

  return font_handle.GetValue();
}

HFONT gwingui::themefont::GetDefaultTitlebarFont() {
  static LOGFONT font_desc = { 0 };
  font_desc.lfHeight = 18;
  font_desc.lfWidth = 0;
  font_desc.lfWeight = FW_MEDIUM;
  font_desc.lfItalic = FALSE;
  font_desc.lfUnderline = FALSE;
  font_desc.lfStrikeOut = FALSE;
  font_desc.lfCharSet = ANSI_CHARSET;  // Consider changing this?
  font_desc.lfOutPrecision = OUT_DEFAULT_PRECIS;
  font_desc.lfClipPrecision = CLIP_DEFAULT_PRECIS;
  font_desc.lfQuality = CLEARTYPE_QUALITY;
  font_desc.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

  wcscpy_s( font_desc.lfFaceName, TEXT( "Consolas" ) );

  static SafeFont font_handle = CreateFontIndirect( &font_desc );

  return font_handle.GetValue();
}