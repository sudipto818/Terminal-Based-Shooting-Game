#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>

struct Entity {
    int x, y;
    std::string sprite;
    int hp;
    char type;
};

bool collides(const Entity &a, const Entity &b);
void drawSprite(std::vector<std::string> &screen, int x, int y, const std::string &sprite);

#endif