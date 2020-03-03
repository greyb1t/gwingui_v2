#pragma once

#include <memory>
#include <functional>
#include <Windows.h>

namespace gwingui {

template <typename T, typename freeType>
using SafeObjectPtr =
    std::shared_ptr<typename std::remove_pointer<T>::type>;  // Shared Pointer

template <typename HandleType>
using SafeObject = SafeObjectPtr<HandleType, std::function<void( HandleType )>>;

// Always call reset when you don't need the object anymore
struct SafeSelectObject {
  SafeSelectObject( HDC hdc, HGDIOBJ object ) : hdc_( hdc ) {
    old_object_ = SelectObject( hdc, object );
  }

  void Reset() {
    SelectObject( hdc_, old_object_ );
  }

 private:
  HDC hdc_;
  HGDIOBJ old_object_;
};

template <typename T>
struct __SafeDeleteObject {
  using HandleType = T;

  static void CleanUp( HandleType object ) {
    DeleteObject( object );
  }
};

template <typename T>
struct __SafeDeleteDC {
  using HandleType = T;

  static void CleanUp( HandleType object ) {
    DeleteDC( object );
  }
};

template <typename T>
struct __SafeReleaseDC {
  using HandleType = T;

  static void CleanUp( HandleType object ) {
    ReleaseDC( WindowFromDC( object ), object );
  }
};

template <typename T>
struct __SafeHandleImplementation {
  using HandleType = typename T::HandleType;

  __SafeHandleImplementation() {}

  __SafeHandleImplementation( HandleType object ) {
    std::function<void( HandleType )> m_deleter = []( HandleType object ) {
      T::CleanUp( object );
    };

    SafeObject<HandleType> pObject( object, m_deleter );
    object_ = std::move( pObject );
  }

  inline const HandleType GetValue() const {
    return object_.get();
  }

 private:
  SafeObject<HandleType> object_;
};

using SafeBitmap = __SafeHandleImplementation<__SafeDeleteObject<HBITMAP>>;
using SafeBrush = __SafeHandleImplementation<__SafeDeleteObject<HBRUSH>>;
using SafeFont = __SafeHandleImplementation<__SafeDeleteObject<HFONT>>;
using SafePalette = __SafeHandleImplementation<__SafeDeleteObject<HPALETTE>>;
using SafePen = __SafeHandleImplementation<__SafeDeleteObject<HPEN>>;
using SafeRegion = __SafeHandleImplementation<__SafeDeleteObject<HRGN>>;

using SafeDeleteDC = __SafeHandleImplementation<__SafeDeleteDC<HDC>>;
using SafeReleaseDC = __SafeHandleImplementation<__SafeReleaseDC<HDC>>;

}  // namespace gwingui