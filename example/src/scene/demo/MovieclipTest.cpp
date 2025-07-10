#include "ppx/Drawing.hpp"
#include "MovieclipTest.hpp"
#include "ppx/TextureAtlas.hpp"
#include "ppx/Tracelog.hpp"
#include "ppx/Vec2.hpp"
#include "nds/arm9/console.h"
#include "nds/arm9/input.h"
#include "nds/arm9/video.h"
#include "nds/input.h"

MovieclipTest::MovieclipTest()
{
  videoSetMode(MODE_0_3D);
  videoSetModeSub(MODE_0_2D);

  vramSetPrimaryBanks(
    VRAM_A_TEXTURE,
    VRAM_B_TEXTURE,
    VRAM_C_SUB_BG_0x06200000,
    VRAM_D_MAIN_BG_0x06000000);

  vramSetBankF(VRAM_F_TEX_PALETTE);

  lcdMainOnBottom();
  InitDrawing();

  consoleInit(&console, 1, BgType_Text4bpp, BgSize_T_256x256, 22, 3, false, true);
  consoleSelect(&console);

  setBrightness(3, -16);
}

MovieclipTest::~MovieclipTest()
{
  Unload_TextureAtlas(atlas);
}

void MovieclipTest::Preload()
{
  atlas = Load_TextureAtlas("nitro:/atlas/player.txt");

  setBrightness(3, 0);
}

void MovieclipTest::Update()
{
  static int x = 0;
  static int y = 0;
  static int rotatiton = 0;

  if (keysHeld() & KEY_LEFT) x--;
  if (keysHeld() & KEY_RIGHT) x++;
  if (keysHeld() & KEY_DOWN) y++;
  if (keysHeld() & KEY_UP) y--;

  if (keysHeld() & KEY_X) rotatiton--;
  if (keysHeld() & KEY_Y) rotatiton++;

  BeginDrawing();
  {
    ClearBackground(Color::Gray());
    
    if (atlas)
    {
      TextureMap *tex = atlas->find("Idle01");
      // tex->rotated = false;
      // TraceLog("rot %u", tex->rotated);
  
      if (tex)
      {
        SetColor(Color::Red());
        DrawRect({0, 0, tex->width, tex->height});

        int offx = 4;
        int offy = 5;
        int sizex = 20;
        int sizey = 30;
  
        tex->Draw({x, y}, {1, 1}, {0, 0}, rotatiton);
        if (keysHeld() & KEY_B) tex->Draw({x, y}, {1, 1}, {0, 0}, rotatiton, false, false, {offx, offy, sizex, sizey}, Color::Blue());
      }
    }
  }
  EndDrawing();
}
