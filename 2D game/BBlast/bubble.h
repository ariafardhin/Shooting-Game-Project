#ifndef BUBBLE_H
#define BUBBLE_H

class Bubble {
public:
    static const int BUBBLE_RADIUS = 10;
    int x, y;
    int color;
    bool isPopped;
    int speed;
    bool isMoving;
    
    inline Bubble(int startX, int startY);
    inline void move();
    inline void draw();
    inline void pop();
    inline bool checkCollision(Bubble& other);
    inline void setDirection(double angle);
    //const int BUBBLE_RADIUS;

private:
    double dx, dy; // Direction components
};

#endif