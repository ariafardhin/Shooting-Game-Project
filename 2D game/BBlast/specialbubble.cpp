#include "specialbubble.h"
#include <graphics.h>
#include "shooter.h"
#include <iostream>
#include <cmath>


SpecialBubble::SpecialBubble(int startX, int startY, Type bubbleType) {
    x = startX;
    y = startY;
    type = bubbleType;
    isActive = true;
}

void SpecialBubble::drawBubble() {
    if (!isActive) return;

    // Draw the special bubble based on its type
    switch (type) {
        case SLOW_DOWN:
            setcolor(RED);
            setfillstyle(SOLID_FILL, RED);
            break;
        case FREEZE:
            setcolor(BLUE);
            setfillstyle(SOLID_FILL, BLUE);
            break;
        case ADD_EXTRA:
            setcolor(GREEN);
            setfillstyle(SOLID_FILL, GREEN);
            break;
        case CUT_TIME:
            setcolor(YELLOW);
            setfillstyle(SOLID_FILL, YELLOW);
            break;
    }
    
    circle(x, y, BUBBLE_RADIUS);
    floodfill(x, y, getcolor());
    
    // Draw special effect indicator
    setcolor(WHITE);
    switch(type) {
        case SLOW_DOWN:
            line(x-5, y, x+5, y); // Simple "-" symbol
            break;
        case FREEZE:
            circle(x, y, 5); // Small snowflake-like circle
            break;
        case ADD_EXTRA:
            line(x-5, y, x+5, y); // "+" symbol
            line(x, y-5, x, y+5);
            break;
        case CUT_TIME:
            arc(x, y, 0, 180, 5); // Clock-like symbol
            line(x, y, x+3, y-3);
            break;
    }
}

void SpecialBubble::triggerEffect(Shooter &opponent) {
    if (!isActive) return;

    switch (type) {
        case SLOW_DOWN:
            opponent.setSpeed(opponent.getSpeed() / 2);  // Reduce speed by half
            break;
        case FREEZE:
            opponent.freeze(5000);  // Freeze for 5 seconds
            break;
        case ADD_EXTRA:
            opponent.addExtraBubbles(3);
            break;
        case CUT_TIME:
            opponent.reduceTime(10);  // Reduce 10 seconds
            break;
    }
    
    isActive = false;  // Deactivate after use
}

bool SpecialBubble::checkCollision(const Bubble& projectile) {
    if (!isActive) return false;
    
    int dx = x - projectile.x;
    int dy = y - projectile.y;
    int distance = sqrt(dx*dx + dy*dy);
    return distance < (BUBBLE_RADIUS + projectile.BUBBLE_RADIUS);
}
