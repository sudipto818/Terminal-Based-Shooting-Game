#include "Entity.h"

bool collides(const Entity &a, const Entity &b) {
    if (a.y != b.y) return false;
    int aL = a.x, aR = a.x + a.sprite.size() - 1;
    int bL = b.x, bR = b.x + b.sprite.size() - 1;
    return !(aR < bL || bR < aL);
}

void drawSprite(std::vector<std::string> &screen, int x, int y, const std::string &sprite) {
    if (y < 0 || y >= (int)screen.size()) return;
    for (int i = 0; i < (int)sprite.size(); i++) {
        int px = x + i;
        if (px >= 0 && px < (int)screen[0].size()){
            screen[y][px] = sprite[i];
        }
    }
}
