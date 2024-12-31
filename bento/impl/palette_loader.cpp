#include "bento/palette_loader.hpp"
#include "bento/file.hpp"
#include "bento/struct.hpp"
#include "bento/utils.hpp"
#include "nds/arm9/sassert.h"
#include "nds/arm9/videoGL.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>

#define IGNORED 0
using namespace nb;

Palette_loader::Palette_loader(const int InitialSize)
{
  list.reserve(InitialSize);
}

void Palette_loader::Unload()
{
  for (auto it = list.begin(); it != list.end(); ++it)
  {
    if (glDeleteTextures(1, &it->texid) == 1)
      TraceLog("pal io: unload pid %i, texid %i", it->pid, it->texid);
    else
      TraceLog("pal io: failed unload pid %i, texid %i", it->pid, it->texid);
  }

  // TODO: should i keep the pid if unload failed or just clear the list?
  list.clear();
}

Palette *Palette_loader::Add(const uint8_t pid)
{
  sillyimg_metadata metadata;
  Palette pal;
  char path[30];
  size_t length;

  // TODO: dont hardcode the directory
  sprintf(path, "%s%u_pal.bin", "nitro:/palette/", pid);

  unsigned char *buffer = LoadFileData(path, length);
  sassert(buffer != nullptr, "buffer == nullptr");

  metadata = *(sillyimg_metadata*)buffer;
  sassert(metadata.header == SILLYIMG_HEADER, "invalid metadata");
  sassert(metadata.version == 0, "invalid version");

  int res = glGenTextures(1, &pal.texid);
  sassert(res == 1, "failed gen texture");

  res = glBindTexture(IGNORED, pal.texid);
  sassert(res == 1, "failed bind texture");

  // res = glTexImage2D(IGNORED, IGNORED, GL_NOTEXTURE, 0, 0, IGNORED, 0, nullptr);
  // sassert(res == 1, "failed glTexImage2D");

  res = glColorTableEXT(IGNORED, IGNORED, metadata.width, IGNORED, IGNORED, buffer+20);
  sassert(res == 1, "failed glColorTableEXT");

  pal.pid = metadata.paletteId;
  free(buffer);

  TraceLog("load palette %u,%u", pal.pid, pal.texid);
  list.push_back(pal);
  return &list.back();
}

Palette *Palette_loader::Get(const uint8_t pid)
{
  for (auto &i : list)
  {
    if (i.pid == pid) return &i;
  }

  return Add(pid);
}

void Palette_loader::Remove(const uint8_t pid)
{
  for (auto it = list.begin(); it != list.end(); ++it)
  {
    if (it->pid == pid)
    {
      list.erase(it);
      return;
    }
  }
}
