#include "bento/drawing.hpp"

namespace nb
{
  // void DrawSprite(const Texture &texture, const SpriteMap &sprite, const Vec2 &position, const int rotation, const f32 &scale)
  // {
  //   Vec2 offset = (sprite.isRotated)
  //   ? Vec2(
  //     sprite.offsetY+sprite.frameW - sprite.height*sprite.pivotY,
  //     -sprite.offsetX + sprite.width*sprite.pivotX)
  //   : Vec2(
  //     -sprite.offsetX + sprite.width*sprite.pivotX,
  //     -sprite.offsetY + sprite.height*sprite.pivotY);

  //   offset.x = offset.x * scale;
  //   offset.y = offset.y * scale;

  //   DrawTexture(
  //     texture,
  //     {sprite.frameX, sprite.frameY, sprite.frameW, sprite.frameH},
  //     {position.x, position.y, sprite.frameW*scale, sprite.frameH*scale},
  //     offset,
  //     ((sprite.isRotated) ? -90 : 0) + rotation
  //   );
  // }
}