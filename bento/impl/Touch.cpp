#include "../Touch.hpp"
#include "nds/arm9/input.h"

static touchPosition touchtemp;

namespace ppx
{
  bool GetTouch(TouchPhase phase, touchPosition &touch)
  {
    touchRead(&touch);

    switch (phase){
      case TouchPhase_RELEASE:
      {
        // store prev pos because px,py return 0 when KeysUp()
        if (keysHeld() & KEY_TOUCH) touchtemp = touch;
        if (keysUp() & KEY_TOUCH)
        {
          touch = touchtemp;
          return true;
        }
        return false;
      }

      case TouchPhase_DOWN: return (keysDown() & KEY_TOUCH);
      case TouchPhase_HELD: return (keysDown() & KEY_TOUCH);
    }

    return false;
  }
}