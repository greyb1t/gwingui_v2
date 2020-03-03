#pragma once

#include "utils/safe_handles.h"

#pragma comment( lib, "Msimg32.lib" )

namespace gwingui {

namespace drawing {

bool FilledRectangle( const HDC hdc, const RECT& rect, const SafeBrush& sb );
bool FilledRectangle( const HDC hdc, const RECT& rect, const COLORREF color );

bool BorderRectangle( const HDC hdc, const RECT& rect, const SafeBrush& sb );
bool BorderRectangle( const HDC hdc, const RECT& rect, const COLORREF color );

void Line( const HDC hDc,
           const POINT& origin,
           const POINT& dest,
           const COLORREF color,
           const uint32_t thickness );

SIZE GetFontTextDimensions( const std::wstring& text, const HFONT font_handle );

SIZE GetSelectedFontTextDimensions( const HDC hdc, const std::wstring& text );

bool Text( const HDC hdc,
           const int32_t x,
           const int32_t y,
           const COLORREF color,
           const std::wstring& text );

HBITMAP CreateBitmapFromBuffer( const uint8_t* buffer,
                                const uint32_t width,
                                const uint32_t height );

void DrawBitmap( const HDC hdc,
                 const int32_t x,
                 const int32_t y,
                 const uint32_t width,
                 const uint32_t height,
                 const HBITMAP bitmap_handle );

void DrawBitmap( const HDC hdc, const RECT& rect, const HBITMAP bitmap_handle );

void DrawBitmapTransparent( const HDC hdc,
                            const int x,
                            const int y,
                            const int width,
                            const int height,
                            const HBITMAP bitmap_handle );

void DrawBitmapTransparent( const HDC hdc,
                            const RECT& rect,
                            const HBITMAP bitmap_handle );

BITMAP GetBitmapDimensions( const HBITMAP bitmap_handle );

void ExcludeRectFromDC( const HDC hdc, const RECT& rect );

void ExcludeRectFromDC( const HDC hdc,
                        const int32_t left,
                        const int32_t top,
                        const int32_t right,
                        const int32_t bottom );

void IncludeRectInDC( const HDC hdc, const RECT& rect );

RECT GrowRect( const RECT& rect, const int32_t x, const int32_t y );
RECT MoveRectX(const RECT& rect, const int32_t x);

}  // namespace drawing

}  // namespace gwingui