#ifndef SHOOTER_H
#define SHOOTER_H

#include "bubble.h"

class Shooter {
private:
    int x, y;
    bool isShooting;
    int speed;
    bool frozen;
    int freezeTime;
    int bubbleCount;
    int actionTime;
    int remainingTime;
    double angle;  // Angle of the shooter
    Bubble* loadedBubble;  // Currently loaded bubble
    int cooldownTimer;
    static const int MAX_BUBBLES = 50;
    static const int SHOOTER_WIDTH = 20;
    static const int SHOOTER_HEIGHT = 30;

public:
    inline Shooter(int startX, int startY);
    inline ~Shooter();

    // Movement and control methods
    inline void setSpeed(int newSpeed);
    inline int getSpeed() const;
    inline void freeze(int duration);
    inline bool isFrozen() const;
    inline void moveLeft();
    inline void moveRight();
    inline void rotateLeft();
    inline void rotateRight();
    inline void shoot();
    inline void update();
    inline void draw();

    // Game mechanics methods
    inline void addExtraBubbles(int count);
    inline int getRemainingTime() const;
    inline void setRemainingTime(int newTime);
    inline void reduceTime(int amount);
    inline Bubble* getLoadedBubble() const { return loadedBubble; }
    bool canShoot() const { return !isShooting && cooldownTimer <= 0 && !frozen; }
};

#endif