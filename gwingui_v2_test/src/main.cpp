#include <Windows.h>
#include <assert.h>

#include "gwinguiv2/gui.h"

#include "resources/resource.h"
#include "windows/basic_window1.h"

#pragma comment( lib, "gwingui_v2.lib" )

int CALLBACK WinMain( HINSTANCE instance,
                      HINSTANCE prev_instance,
                      LPSTR cmd_line,
                      int cmd_show ) {
  gwingui::Gui gui( instance );

  gui.AddWindow( IDD_DIALOG1, nullptr, new BasicWindow1( instance ) );

  gui.Run();

  /*

  IDD_DIALOG1
  is_themed?
  tabs? : Add the tabs whenever you want to, at the same time, create the new
  windows and attach them to a tab.

  --

  Gui gui;

  gui.AddDialog(dialog_id, make_unique<any object that inherits from Window e.g.
  OnInitialize>(a pointer to value that represents that window in memory))

  gui.Run()

  */

  /*
  MSG msg;
  BOOL message_ret = GetMessage(&msg, nullptr, 0, 0);

  while ((message_ret = GetMessage(&msg, nullptr, 0, 0)) != 0) {
    if (message_ret != -1) {
      









    } else {
      assert(false && "GetMessage returns -1");
    }
  }
  */

  return TRUE;
}