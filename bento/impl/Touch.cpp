#include "../Touch.hpp"
#include "nds/arm9/input.h"

static touchPosition s_lastPosition;

namespace ppx
{
  bool GetTouch(TouchPhase phase, touchPosition &touch)
  {
    touchRead(&touch);
    const bool touched = (keysHeld() & KEY_TOUCH);

    switch (phase){
      case TouchPhase_RELEASE:
        if (!touched && (keysUp() & KEY_TOUCH))
        {
          touch = s_lastPosition;
          return true;
        }
        break;
      
      case TouchPhase_DOWN: return (keysDown() & KEY_TOUCH);
      case TouchPhase_HELD: return touched;
    }
    
    // store prev pos because px,py return 0 when KeysUp()
    s_lastPosition = touch;
    return false;
  }
}