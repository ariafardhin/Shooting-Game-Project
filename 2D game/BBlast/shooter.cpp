#include "shooter.h"
#include <graphics.h>
#include <cmath>
#include <conio.h>
#include "bubble.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

Shooter::Shooter(int startX, int startY) {
    x = startX;
    y = startY;
    speed = 5;
    frozen = false;
    freezeTime = 0;
    isShooting = false;
    bubbleCount = MAX_BUBBLES;
    actionTime = 60;  // 60 second default action time
    remainingTime = actionTime;
    angle = 90;  // Start pointing upward
    cooldownTimer = 0;
    loadedBubble = new Bubble(x, y);
}

Shooter::~Shooter() {
    delete loadedBubble;
}

void Shooter::setSpeed(int newSpeed) {
    speed = newSpeed;
    if (speed < 1) speed = 1;  // Ensure minimum speed
}

int Shooter::getSpeed() const {
    return speed;
}

void Shooter::freeze(int duration) {
    frozen = true;
    freezeTime = duration;
}

bool Shooter::isFrozen() const {
    return frozen;
}

void Shooter::moveLeft() {
    if (!frozen) {
        x -= speed;
        if (x < SHOOTER_WIDTH/2) {
            x = SHOOTER_WIDTH/2;  // Prevent moving off screen
        }
        // Update loaded bubble position if not shooting
        if (!isShooting && loadedBubble) {
            loadedBubble->x = x;
        }
    }
}

void Shooter::moveRight() {
    if (!frozen) {
        x += speed;
        if (x > SCREEN_WIDTH - SHOOTER_WIDTH/2) {
            x = SCREEN_WIDTH - SHOOTER_WIDTH/2;  // Prevent moving off screen
        }
        // Update loaded bubble position if not shooting
        if (!isShooting && loadedBubble) {
            loadedBubble->x = x;
        }
    }
}

void Shooter::rotateLeft() {
    if (!frozen) {
        angle += 2;
        if (angle > 170) angle = 170;  // Limit rotation
    }
}

void Shooter::rotateRight() {
    if (!frozen) {
        angle -= 2;
        if (angle < 10) angle = 10;  // Limit rotation
    }
}

void Shooter::addExtraBubbles(int count) {
    bubbleCount += count;
    if (bubbleCount > MAX_BUBBLES) {
        bubbleCount = MAX_BUBBLES;  // Cap at maximum
    }
}

void Shooter::setRemainingTime(int newTime) {
    remainingTime = newTime;
    actionTime = newTime;  // Update the action time as well
}

int Shooter::getRemainingTime() const {
    return remainingTime;
}

void Shooter::reduceTime(int amount) {
    remainingTime -= amount;
    if (remainingTime < 0) {
        remainingTime = 0;  // Don't allow negative time
    }
}

void Shooter::update() {
    if (frozen) {
        freezeTime--;
        if (freezeTime <= 0) {
            frozen = false;
        }
        return;
    }

    if (cooldownTimer > 0) {
        cooldownTimer--;
    }

    if (isShooting && loadedBubble) {
        loadedBubble->move();
        // Check if bubble has stopped or hit something
        if (!loadedBubble->isMoving) {
            delete loadedBubble;
            loadedBubble = new Bubble(x, y);
            isShooting = false;
            cooldownTimer = 20;  // 20 frame cooldown
        }
    }
}

void Shooter::draw() {
    if (frozen) {
        setcolor(LIGHTBLUE);
    } else {
        setcolor(RED);
    }

    // Draw shooter body
    rectangle(x - SHOOTER_WIDTH/2, y, x + SHOOTER_WIDTH/2, y + SHOOTER_HEIGHT);
    
    // Draw shooter barrel
    double barrelLength = 20;
    int endX = x + cos(angle * M_PI / 180) * barrelLength;
    int endY = y - sin(angle * M_PI / 180) * barrelLength;
    line(x, y, endX, endY);

    // Draw loaded bubble
    if (loadedBubble && !isShooting) {
        loadedBubble->x = x;
        loadedBubble->y = y;
        loadedBubble->draw();
    }

    // Draw cooldown indicator
    if (cooldownTimer > 0) {
        setcolor(YELLOW);
        arc(x, y, 0, (360 * (20 - cooldownTimer)) / 20, 25);
    }
}

void Shooter::shoot() {
    if (canShoot() && loadedBubble && bubbleCount > 0) {
        loadedBubble->setDirection(angle);
        loadedBubble->isMoving = true;
        isShooting = true;
        bubbleCount--;
    }
}


