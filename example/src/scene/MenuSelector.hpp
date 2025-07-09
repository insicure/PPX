#include "nds/arm9/console.h"
#include "ppx/Scene.hpp"
#include "ppx/CircularCounter.hpp"
#include <string>

using namespace ppx;

struct SceneDesc
{
  std::string name;
  std::string desc;
};

class MenuSelector : public Scene {
public:
  CircularCounter counter_selected;
  CircularCounter counter_page;
  
  PrintConsole console_main;
  PrintConsole console_sub;

  MenuSelector();
  ~MenuSelector() override;
  void Preload() override;
  void Update() override;
  void selectMenu();
};