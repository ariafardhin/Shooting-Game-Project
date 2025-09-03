#include "bubble.h"
#include <graphics.h>
#include <cmath>


#define SCREEN_WIDTH 800
#define BUBBLE_RADIUS 10

// Define the static constant
//const int BUBBLE_RADIUS = 10;

Bubble::Bubble(int startX, int startY){
    x = startX;
    y = startY;
    color = rand() % 5 + 1;  // 5 different colors
    isPopped = false;
    isMoving = false;
    speed = 5;
    dx = 0;
    dy = -1; // Default upward movement
}

void Bubble::setDirection(double angle) {
    dx = sin(angle * M_PI / 180.0);
    dy = -cos(angle * M_PI / 180.0);
}

void Bubble::move() {
    if (isMoving && !isPopped) {
        x += dx * speed;
        y += dy * speed;
        
        // Boundary checking
        if (x < BUBBLE_RADIUS || x > SCREEN_WIDTH - BUBBLE_RADIUS) {
            dx = -dx; // Bounce off walls
            x = (x < BUBBLE_RADIUS) ? BUBBLE_RADIUS : SCREEN_WIDTH - BUBBLE_RADIUS;
        }
        
        // Stop if hitting top of screen
        if (y < BUBBLE_RADIUS) {
            isMoving = false;
            y = BUBBLE_RADIUS;
        }
    }
}

void Bubble::draw() {
    if (!isPopped) {
        // Set color based on bubble color value
        switch(color) {
            case 1: setcolor(RED); setfillstyle(SOLID_FILL, RED); break;
            case 2: setcolor(BLUE); setfillstyle(SOLID_FILL, BLUE); break;
            case 3: setcolor(GREEN); setfillstyle(SOLID_FILL, GREEN); break;
            case 4: setcolor(YELLOW); setfillstyle(SOLID_FILL, YELLOW); break;
            case 5: setcolor(MAGENTA); setfillstyle(SOLID_FILL, MAGENTA); break;
        }
        
        circle(x, y, BUBBLE_RADIUS);
        floodfill(x, y, getcolor());
    }
}

void Bubble::pop() {
    if (!isPopped) {
        isPopped = true;
        isMoving = false;
        
        // Pop animation
        for (int i = 1; i <= 5; i++) {
            setcolor(WHITE);
            circle(x, y, i * 2);
            delay(20);
        }
    }
}

bool Bubble::checkCollision(Bubble& other) {
    if (isPopped || other.isPopped) return false;
    
    int dx = x - other.x;
    int dy = y - other.y;
    int distance = sqrt(dx*dx + dy*dy);
    return distance < BUBBLE_RADIUS * 2;
}

