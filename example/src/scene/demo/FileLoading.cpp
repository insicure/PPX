#include "FileLoading.hpp"
#include "ppx/FileData.hpp"
#include "ppx/Memory.hpp"
#include "nds/arm9/sassert.h"
#include <cstdlib>
#include <nds/arm9/console.h>
#include <nds/arm9/video.h>
#include <nds/system.h>
#include <cinttypes>

using namespace ppx;

FileLoading::FileLoading()
{
  videoSetMode(MODE_0_2D);
  videoSetModeSub(MODE_0_2D);
  lcdMainOnTop();

  vramSetPrimaryBanks(
    VRAM_A_LCD,
    VRAM_B_LCD,
    VRAM_C_SUB_BG_0x06200000,
    VRAM_D_MAIN_BG_0x06000000);

  consoleInit(&console, 0, BgType_Text4bpp, BgSize_T_256x256, 17, 4, false, true);
  consoleSelect(&console);
}

FileLoading::~FileLoading()
{
  Unload_FileData(file);
}

void FileLoading::Preload()
{
  file = Load_FileData("nitro:/file.txt");
  sassert(file, "failed to load data");

  printf("\n file: ");
  consoleSetColor(nullptr, ConsoleColor::CONSOLE_LIGHT_YELLOW);
  printf("nitro:/file.txt\n");
  consoleSetColor(nullptr, ConsoleColor::CONSOLE_WHITE);
  printf(" size: ");
  consoleSetColor(nullptr, ConsoleColor::CONSOLE_LIGHT_YELLOW);
  printf("%" PRIu32 " bytes\n", file->length);

  consoleSetColor(nullptr, ConsoleColor::CONSOLE_WHITE);
  printf("\n\n content:\n\n");
  consoleSetColor(nullptr, ConsoleColor::CONSOLE_LIGHT_GRAY);
  for (size_t i=0; i<file->length; i++) consolePrintChar(file->data[i]);
}

void FileLoading::Update()
{

}
