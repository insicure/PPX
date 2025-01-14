#pragma once

#include "nds/touch.h"

namespace ppx
{
  enum TouchPhase { TouchPhase_RELEASE, TouchPhase_DOWN, TouchPhase_HELD };
  
  bool GetTouch(TouchPhase phase, touchPosition &touch);
}