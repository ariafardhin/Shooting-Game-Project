#ifndef SPECIALBUBBLE_H
#define SPECIALBUBBLE_H

#include "shooter.h"

class SpecialBubble {
public:
    enum Type {
        SLOW_DOWN,
        FREEZE,
        ADD_EXTRA,
        CUT_TIME
    };

    int x, y;
    Type type;
    bool isActive;
    static const int BUBBLE_RADIUS = 15;

    inline SpecialBubble(int startX, int startY, Type bubbleType);
    inline void drawBubble();
    inline void triggerEffect(Shooter &opponent);
    inline bool checkCollision(const Bubble& projectile);
};

#endif