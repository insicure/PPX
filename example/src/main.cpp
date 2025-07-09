#include "filesystem.h"
#include "nds/arm9/input.h"
#include "nds/interrupts.h"
#include "scene/MenuSelector.hpp"
#include "nds/arm9/console.h"
#include "ppx/App.hpp"

using namespace ppx;

int main()
{
  consoleDemoInit();
  consoleDebugInit(DebugDevice_NOCASH);

  assert(nitroFSInit(nullptr) && "failed to initialize nitrofs");
  
  App &app = App::Get();
  app.SetScene(new MenuSelector());

  while (true) {
    scanKeys();
    
    // restart
    if (keysHeld() == (KEY_START | KEY_SELECT))
      app.SetScene(new MenuSelector());
    
    // exit
    if (keysHeld() == (KEY_START | KEY_SELECT | KEY_DOWN)) break;

    app.AppUpdate();
    swiWaitForVBlank();
  }
  
  return 0;
}