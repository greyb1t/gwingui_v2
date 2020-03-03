#include "pch.h"
#include "drawing.h"

namespace gwingui {

namespace drawing {

bool FilledRectangle( const HDC hdc, const RECT& rect, const SafeBrush& sb ) {
  return !!FillRect( hdc, &rect, sb.GetValue() );
}

bool FilledRectangle( const HDC hdc, const RECT& rect, const COLORREF color ) {
  return FilledRectangle( hdc, rect, CreateSolidBrush( color ) );
}

bool BorderRectangle( const HDC hdc, const RECT& rect, const SafeBrush& sb ) {
  return !!FrameRect( hdc, &rect, sb.GetValue() );
}

bool BorderRectangle( const HDC hdc, const RECT& rect, const COLORREF color ) {
  return BorderRectangle( hdc, rect, CreateSolidBrush( color ) );
}

void Line( const HDC hDc,
           const POINT& origin,
           const POINT& dest,
           const COLORREF color,
           const uint32_t thickness ) {
  SafePen pen = CreatePen( PS_SOLID, thickness, color );
  SafeSelectObject selected_object( hDc, pen.GetValue() );

  MoveToEx( hDc, origin.x, origin.y, NULL );
  LineTo( hDc, dest.x, dest.y );

  selected_object.Reset();
}

SIZE GetFontTextDimensions( const std::wstring& text,
                            const HFONT font_handle ) {
  SafeReleaseDC hdc = GetDC( 0 );
  SafeSelectObject selected_object( hdc.GetValue(), font_handle );

  const auto dims = GetSelectedFontTextDimensions( hdc.GetValue(), text );

  selected_object.Reset();

  return dims;
}

SIZE GetSelectedFontTextDimensions( const HDC hdc, const std::wstring& text ) {
  SIZE size;
  GetTextExtentPoint32( hdc, text.c_str(), static_cast<int>( text.size() ),
                        &size );
  return size;
}

bool Text( const HDC hdc,
           const int32_t x,
           const int32_t y,
           const COLORREF color,
           const std::wstring& text ) {
  SetTextColor( hdc, color );
  return !!TextOut( hdc, x, y, text.c_str(), static_cast<int>( text.size() ) );
}

HBITMAP CreateBitmapFromBuffer( const uint8_t* buffer,
                                const uint32_t width,
                                const uint32_t height ) {
  BITMAPINFO bmi = { 0 };
  bmi.bmiHeader.biSize = sizeof( bmi.bmiHeader );
  bmi.bmiHeader.biWidth = width;
  bmi.bmiHeader.biHeight = height;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;

  SafeReleaseDC hdc = GetDC( NULL );

  const HBITMAP bitmap_handle = CreateCompatibleBitmap(
      hdc.GetValue(), bmi.bmiHeader.biWidth, bmi.bmiHeader.biHeight );

  SetDIBits( hdc.GetValue(), bitmap_handle, 0, bmi.bmiHeader.biHeight, buffer,
             &bmi, DIB_RGB_COLORS );

  return bitmap_handle;
}

void DrawBitmap( const HDC hdc,
                 const int32_t x,
                 const int32_t y,
                 const uint32_t width,
                 const uint32_t height,
                 const HBITMAP bitmap_handle ) {
  SafeDeleteDC new_hdc( CreateCompatibleDC( NULL ) );
  SafeSelectObject selected_object( new_hdc.GetValue(), bitmap_handle );

  BitBlt( hdc, x, y, width, height, new_hdc.GetValue(), 0, 0, SRCCOPY );

  selected_object.Reset();
}

void DrawBitmap( const HDC hdc,
                 const RECT& rect,
                 const HBITMAP bitmap_handle ) {
  DrawBitmap( hdc, rect.left, rect.top, rect.right, rect.bottom,
              bitmap_handle );
}

void DrawBitmapTransparent( const HDC hdc,
                            const RECT& rect,
                            const HBITMAP bitmap_handle ) {
  DrawBitmapTransparent( hdc, rect.left, rect.top, rect.right, rect.bottom,
                         bitmap_handle );
}

void DrawBitmapTransparent( const HDC hdc,
                            const int x,
                            const int y,
                            const int width,
                            const int height,
                            const HBITMAP bitmap_handle ) {
  SafeDeleteDC new_hdc( CreateCompatibleDC( NULL ) );
  SafeSelectObject selected_object( new_hdc.GetValue(), bitmap_handle );

  TransparentBlt( hdc, x, y, width, height, new_hdc.GetValue(), 0, 0, width,
                  height, RGB( 255, 0, 200 ) );
  selected_object.Reset();
}

BITMAP GetBitmapDimensions( const HBITMAP bitmap_handle ) {
  BITMAP bitmap;
  assert( bitmap_handle );
  auto ret = GetObject( bitmap_handle, sizeof( bitmap ), &bitmap );
  assert( ret );
  return bitmap;
}

void ExcludeRectFromDC( const HDC hdc, const RECT& rect ) {
  ExcludeRectFromDC( hdc, rect.left, rect.top, rect.right, rect.bottom );
}

void ExcludeRectFromDC( const HDC hdc,
                        const int32_t left,
                        const int32_t top,
                        const int32_t right,
                        const int32_t bottom ) {
  ExcludeClipRect( hdc, left, top, right, bottom );
}

void IncludeRectInDC( const HDC hdc, const RECT& rect ) {
  IntersectClipRect( hdc, rect.left, rect.top, rect.right, rect.bottom );
}

RECT GrowRect( const RECT& rect, const int32_t x, const int32_t y ) {
  RECT rect_copy = rect;
  InflateRect( &rect_copy, x, y );
  return rect_copy;
}

RECT MoveRectX( const RECT& rect, const int32_t x ) {
  RECT rect_copy = rect;
  rect_copy.left += x;
  rect_copy.right += x;
  return rect_copy;
}

}  // namespace drawing

}  // namespace gwingui