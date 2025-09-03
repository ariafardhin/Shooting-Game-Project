#include <graphics.h>
#include <cmath>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <queue>   // For std::queue

// Constants
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 500;
const int GRID_ROWS = 8;
const int GRID_COLS = 10;
const int BUBBLE_RADIUS = 15;
const int BUBBLE_SPACING = 40;
const int SHOOTER_RADIUS = 20;
const int SHOOT_BUBBLE_SPEED = 10;
const int MAX_SHOOTER_BALLS = 30;
// Function to display the welcome screen

void displayWelcomeScreen() {
    cleardevice();

    // Display "BBLAST" at the top center with fancy font, white color, and bold
    setcolor(WHITE);
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 5);
    setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
    outtextxy(SCREEN_WIDTH / 2 - 100, 50, (char*)"BBLAST");

    setcolor(LIGHTBLUE);
    setfillstyle(SOLID_FILL, LIGHTBLUE);
    bar(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2 + 50);
    setcolor(WHITE);
    settextstyle(SIMPLEX_FONT, HORIZ_DIR, 2);
    outtextxy(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 + 10, (char*)"Instructions");

    setcolor(GREEN);
    setfillstyle(SOLID_FILL, GREEN);
    bar(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 70, SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2 + 120);
    setcolor(WHITE);
    settextstyle(SIMPLEX_FONT, HORIZ_DIR, 2);
    outtextxy(SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 + 80, (char*)"Start");
}

void displayInstructionPage() {
    cleardevice();
    outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 100, (char*)"Instructions:");
    outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 70, (char*)"Player 1 Controls:");
    outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, (char*)"Move Left: Left Arrow");
    outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 30, (char*)"Move Right: Right Arrow");
    outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 10, (char*)"Shoot: Spacebar");

    outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 20, (char*)"Player 2 Controls:");
    outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 40, (char*)"Move Left: A");
    outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 60, (char*)"Move Right: D");
    outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 80, (char*)"Shoot: W");

    outtextxy(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 110, (char*)"Press ESC to return to Welcome Screen");
}

// Player control keys
const char PLAYER1_LEFT = 'A';
const char PLAYER1_RIGHT = 'D';
const char PLAYER1_SHOOT = 'W';
const char PLAYER2_LEFT = VK_LEFT;
const char PLAYER2_RIGHT = VK_RIGHT;
const char PLAYER2_SHOOT = VK_UP;
const char EXIT_KEY = 27; // ASCII for Esc

// Player scores
int player1Score = 0;
int player2Score = 0;
int shooterBalls1 = MAX_SHOOTER_BALLS; // Number of balls left in the shooter for Player 1
int shooterBalls2 = MAX_SHOOTER_BALLS; // Number of balls left in the shooter for Player 2
int level = 1; // Current level

// Bubble class
class Bubble {
private:
    int x, y;
    int color;
    bool isActive;

public:
    Bubble(int x, int y, int color) : x(x), y(y), color(color), isActive(true) {}

    void draw() {
        if (isActive) {
            setcolor(color);
            setfillstyle(SOLID_FILL, color);
            fillellipse(x, y, BUBBLE_RADIUS, BUBBLE_RADIUS);
        }
    }

    bool isHit(int shotX, int shotY, int shotColor) const {
        int distance = sqrt(pow(shotX - x, 2) + pow(shotY - y, 2));
        return isActive && distance <= BUBBLE_RADIUS && color == shotColor;
    }

    void pop() {
        isActive = false;
    }

    bool getActive() const {
        return isActive;
    }

    int getColor() const {
        return color;
    }

    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }
};

// Shooter class
class Shooter {
private:
    int x, y;
    int color;

public:
    Shooter(int startX, int startY, int color) : x(startX), y(startY), color(color) {}

    void draw() {
        setcolor(color);
        setfillstyle(SOLID_FILL, color);
        fillellipse(x, y, SHOOTER_RADIUS, SHOOTER_RADIUS);
    }

    void moveLeft(int boundary) {
        if (x - SHOOTER_RADIUS > boundary) {
            x -= 10;
        }
    }

    void moveRight(int boundary) {
        if (x + SHOOTER_RADIUS < boundary) {
            x += 10;
        }
    }

    void setColor(int newColor) {
        color = newColor;
    }

    int getColor() const {
        return color;
    }

    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }
};

// Function prototypes
void popAdjacentBubbles(Bubble* grid[GRID_ROWS][GRID_COLS], int row, int col, int color, int& playerScore);

// Global Variables
Bubble* grid1[GRID_ROWS][GRID_COLS];
Bubble* grid2[GRID_ROWS][GRID_COLS];
Shooter player1(SCREEN_WIDTH / 4, SCREEN_HEIGHT - 50, RED);
Shooter player2(3 * SCREEN_WIDTH / 4, SCREEN_HEIGHT - 50, BLUE);
Bubble* shootBubble1 = NULL;
Bubble* shootBubble2 = NULL;
bool isBubbleInAir1 = false;
bool isBubbleInAir2 = false;

// Directions for adjacent bubbles
const int dx[] = {0, 0, -1, 1, -1, 1};
const int dy[] = {-1, 1, 0, 0, -1, 1};

// Render Bubble Grid with Random Spacing
void renderBubbleGrid(Bubble* grid[GRID_ROWS][GRID_COLS], int offsetX) {
    for (int row = 0; row < GRID_ROWS; ++row) {
        for (int col = 0; col < GRID_COLS; ++col) {
            int x = col * BUBBLE_SPACING + BUBBLE_RADIUS + offsetX + 10;
            int y = row * BUBBLE_SPACING + BUBBLE_RADIUS + 10;
            int color = rand() % 6 + 1; // Random color between 1 and 6
            grid[row][col] = new Bubble(x, y, color);
        }
    }
}

void drawBubbleGrid(Bubble* grid[GRID_ROWS][GRID_COLS]) {
    for (int row = 0; row < GRID_ROWS; ++row) {
        for (int col = 0; col < GRID_COLS; ++col) {
            grid[row][col]->draw();
        }
    }
}

// Handle Bubble Shooting
void shootBubbleUpwards(Bubble*& shootBubble, bool& isBubbleInAir) {
    if (isBubbleInAir && shootBubble) {
        shootBubble->draw();
        shootBubble->pop();
    }
}

// Check Collision
bool checkCollision(Bubble* grid[GRID_ROWS][GRID_COLS], int shotX, int shotY, int shotColor, int& playerScore) {
    for (int row = 0; row < GRID_ROWS; ++row) {
        for (int col = 0; col < GRID_COLS; ++col) {
            if (grid[row][col]->isHit(shotX, shotY, shotColor)) {
                popAdjacentBubbles(grid, row, col, grid[row][col]->getColor(), playerScore);
                return true;
            }
        }
    }
    return false;
}

// Pop Adjacent Bubbles
void popAdjacentBubbles(Bubble* grid[GRID_ROWS][GRID_COLS], int row, int col, int color, int& playerScore) {
    std::queue<std::pair<int, int>> q;
    q.push({row, col});
    grid[row][col]->pop();
    playerScore += 10;

    while (!q.empty()) {
        int r = q.front().first;
        int c = q.front().second;
        q.pop();

        for (int i = 0; i < 6; ++i) {
            int nr = r + dy[i];
            int nc = c + dx[i];

            if (nr >= 0 && nr < GRID_ROWS && nc >= 0 && nc < GRID_COLS && grid[nr][nc]->getActive() && grid[nr][nc]->getColor() == color) {
                grid[nr][nc]->pop();
                playerScore += 10;
                q.push({nr, nc});
            }
        }
    }
}


void addBubbleToGrid(Bubble* grid[GRID_ROWS][GRID_COLS], int x, int y, int color, int offsetX) {
    for (int row = GRID_ROWS - 1; row >= 0; --row) {
        for (int col = 0; col < GRID_COLS; ++col) {
            int bubbleX = BUBBLE_SPACING * col + BUBBLE_RADIUS + offsetX + 10;
            int bubbleY = BUBBLE_SPACING * row + BUBBLE_RADIUS + 10;
            if (sqrt(pow(x - bubbleX, 2) + pow(y - bubbleY, 2)) <= BUBBLE_RADIUS * 2) {
                grid[row][col] = new Bubble(bubbleX, bubbleY, color);
                return;
            }
        }
    }
}
// Display Scores and Remaining Bubbles
void displayScoresAndBubbles() {
    setcolor(WHITE);
    settextstyle(SIMPLEX_FONT, HORIZ_DIR, 1);
    char scoreText1[20];
    sprintf(scoreText1, "P1 Score: %d", player1Score);
    outtextxy(20, 20, scoreText1);

    char bubblesText1[30];
    sprintf(bubblesText1, "P1 Bubbles Left: %d", shooterBalls1);
    outtextxy(20, 40, bubblesText1);

    char scoreText2[20];
    sprintf(scoreText2, "P2 Score: %d", player2Score);
    outtextxy(SCREEN_WIDTH / 2 + 20, 20, scoreText2);

    char bubblesText2[30];
    sprintf(bubblesText2, "P2 Bubbles Left: %d", shooterBalls2);
    outtextxy(SCREEN_WIDTH / 2 + 20, 40, bubblesText2);

    char levelText[20];
    sprintf(levelText, "Level: %d", level);
    outtextxy(SCREEN_WIDTH / 2 - 50, 60, levelText);
}

void initGraphics() {
    // Initialize in a way that works on modern systems
    DWORD screenWidth = GetSystemMetrics(SM_CXSCREEN);
    DWORD screenHeight = GetSystemMetrics(SM_CYSCREEN);
    
    initwindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bubble Shooter");  // Use initwindow instead of initgraph
    
    // Set up the graphics environment
    setbkcolor(BLACK);
    cleardevice();
}
void displayWinnerMessage(const char* winner) {
    cleardevice();
    setcolor(WHITE);
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 5);
    outtextxy(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 50, (char*)winner);
    delay(5000); // Display the winner message for 5 seconds
}

// Main Function
int main() {
    // Initialize Graphics
    initGraphics();

    if (graphresult() != grOk) {
        std::cout << "Graphics initialization failed" << std::endl;
        return 1;
    }

    bool showWelcomeScreen = true;
    bool showInstructionPage = false;

    while (true) {
        if (showWelcomeScreen) {
            displayWelcomeScreen();
            showWelcomeScreen = false;
            showInstructionPage = false;

            // Wait for user input
            while (true) {
                if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
                    break; // Start the game
                }
                if (GetAsyncKeyState(VK_UP) & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8000) {
                    showInstructionPage = true;
                    break;
                }
            }
        }

        if (showInstructionPage) {
            displayInstructionPage();
            showInstructionPage = false;

            // Wait for user input to return to welcome screen
            while (true) {
                if (GetAsyncKeyState(VK_DELETE) & 0x8000) {
                    showWelcomeScreen = true;
                    break;
                }
            }
        }

        // Initialize Bubble Grids
        renderBubbleGrid(grid1, 0);
        renderBubbleGrid(grid2, SCREEN_WIDTH / 2);

        // Seed random number generator
        srand(time(0));

        // Game Loop
        int activePage = 0;
        bool gameRunning = true;

        while (gameRunning) {
            setactivepage(activePage);
            setvisualpage(1 - activePage);
            cleardevice();

            // Draw background
            setfillstyle(SOLID_FILL, BLACK);
            bar(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

            // Draw divider line
            setcolor(WHITE);
            line(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT);

            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                displayWelcomeScreen();
            }

            // Render elements
            player1.draw();
            player2.draw();
            drawBubbleGrid(grid1);
            drawBubbleGrid(grid2);
            displayScoresAndBubbles();

            // Handle shooting bubbles for Player 1
            if (isBubbleInAir1 && shootBubble1) {
                int bubbleX = shootBubble1->getX();
                int bubbleY = shootBubble1->getY() - SHOOT_BUBBLE_SPEED;
                if (bubbleY <= 0 || checkCollision(grid1, bubbleX, bubbleY, shootBubble1->getColor(), player1Score)) {
                    isBubbleInAir1 = false;
                    delete shootBubble1;
                    shootBubble1 = NULL;
                    // Change the color of the shooter bubble after it reaches the grid
                    int newColor = rand() % 6 + 1; // Random color between 1 and 6
                    player1.setColor(newColor);
                } else {
                    // Check if the bubble collides with any existing bubble
                    bool collision = false;
                    for (int row = 0; row < GRID_ROWS; ++row) {
                        for (int col = 0; col < GRID_COLS; ++col) {
                            if (grid1[row][col]->getActive() && sqrt(pow(bubbleX - grid1[row][col]->getX(), 2) + pow(bubbleY - grid1[row][col]->getY(), 2)) <= BUBBLE_RADIUS * 2) {
                                collision = true;
                                if (grid1[row][col]->getColor() == shootBubble1->getColor()) {
                                    popAdjacentBubbles(grid1, row, col, grid1[row][col]->getColor(), player1Score);
                                } else {
                                    addBubbleToGrid(grid1, bubbleX, bubbleY, shootBubble1->getColor(), 0);
                                }
                                isBubbleInAir1 = false;
                                delete shootBubble1;
                                shootBubble1 = NULL;
                                // Change the color of the shooter bubble after it reaches the grid
                                int newColor = rand() % 6 + 1; // Random color between 1 and 6
                                player1.setColor(newColor);
                                break;
                            }
                        }
                        if (collision) break;
                    }
                    if (!collision) {
                        shootBubble1 = new Bubble(bubbleX, bubbleY, player1.getColor());
                        shootBubble1->draw();
                    }
                }
            }
            // Handle shooting bubbles for Player 2 (same as Player 1)
            if (isBubbleInAir2 && shootBubble2) {
                int bubbleX = shootBubble2->getX();
                int bubbleY = shootBubble2->getY() - SHOOT_BUBBLE_SPEED;
                if (bubbleY <= 0 || checkCollision(grid2, bubbleX, bubbleY, shootBubble2->getColor(), player2Score)) {
                    isBubbleInAir2 = false;
                    delete shootBubble2;
                    shootBubble2 = NULL;
                    // Change the color of the shooter bubble after it reaches the grid
                    int newColor = rand() % 6 + 1; // Random color between 1 and 6
                    player2.setColor(newColor);
                } else {
                    // Check if the bubble collides with any existing bubble
                    bool collision = false;
                    for (int row = 0; row < GRID_ROWS; ++row) {
                        for (int col = 0; col < GRID_COLS; ++col) {
                            if (grid2[row][col] && grid2[row][col]->getActive() && sqrt(pow(bubbleX - grid2[row][col]->getX(), 2) + pow(bubbleY - grid2[row][col]->getY(), 2)) <= BUBBLE_RADIUS * 2) {
                                collision = true;
                                if (grid2[row][col]->getColor() == shootBubble2->getColor()) {
                                    popAdjacentBubbles(grid2, row, col, grid2[row][col]->getColor(), player2Score);
                                } else {
                                    addBubbleToGrid(grid2, bubbleX, bubbleY, shootBubble2->getColor(), SCREEN_WIDTH / 2);
                                }
                                isBubbleInAir2 = false;
                                delete shootBubble2;
                                shootBubble2 = NULL;
                                // Change the color of the shooter bubble after it reaches the grid
                                int newColor = rand() % 6 + 1; // Random color between 1 and 6
                                player2.setColor(newColor);
                                break;
                            }
                        }
                        if (collision) break;
                    }
                    if (!collision) {
                        shootBubble2 = new Bubble(bubbleX, bubbleY, player2.getColor());
                        shootBubble2->draw();
                    }
                }
            }
            // Player 1 Controls
            if (GetAsyncKeyState(PLAYER1_LEFT)) {
                player1.moveLeft(0);  // Player 1 moves in the left half
            }
            if (GetAsyncKeyState(PLAYER1_RIGHT)) {
                player1.moveRight(SCREEN_WIDTH / 2);  // Player 1 moves in the left half
            }
            if (GetAsyncKeyState(PLAYER1_SHOOT) && !isBubbleInAir1 && shooterBalls1 > 0) {
                isBubbleInAir1 = true;
                shootBubble1 = new Bubble(player1.getX(), player1.getY() - SHOOTER_RADIUS - 10, player1.getColor());
                shooterBalls1--; // Decrease the number of balls in the shooter
            }

            // Player 2 Controls
            if (GetAsyncKeyState(PLAYER2_LEFT)) {
                player2.moveLeft(SCREEN_WIDTH / 2);  // Player 2 moves in the right half
            }
            if (GetAsyncKeyState(PLAYER2_RIGHT)) {
                player2.moveRight(SCREEN_WIDTH);  // Player 2 moves in the right half
            }
            if (GetAsyncKeyState(PLAYER2_SHOOT) && !isBubbleInAir2 && shooterBalls2 > 0) {
                isBubbleInAir2 = true;
                shootBubble2 = new Bubble(player2.getX(), player2.getY() - SHOOTER_RADIUS - 10, player2.getColor());
                shooterBalls2--; // Decrease the number of balls in the shooter
            }

            // Exit Game
            if (GetAsyncKeyState(VK_ESCAPE)) {
                gameRunning = false;
            }

            // Memory cleanup for old bubbles
            if (!isBubbleInAir1 && shootBubble1) {
                delete shootBubble1;
                shootBubble1 = NULL;
            }
            if (!isBubbleInAir2 && shootBubble2) {
                delete shootBubble2;
                shootBubble2 = NULL;
            }

            delay(30);                   // Control frame rate
            activePage = 1 - activePage; // Toggle active page for double buffering

            // Check if all bubbles are cleared to advance to the next level
            bool allCleared1 = true;
            bool allCleared2 = true;
            for (int row = 0; row < GRID_ROWS; ++row) {
                for (int col = 0; col < GRID_COLS; ++col) {
                    if (grid1[row][col]->getActive()) {
                        allCleared1 = false;
                    }
                    if (grid2[row][col]->getActive()) {
                        allCleared2 = false;
                    }
                }
            }

            if (allCleared1 && allCleared2) {
                level++;
                shooterBalls1 = MAX_SHOOTER_BALLS; // Reset shooter balls for Player 1
                shooterBalls2 = MAX_SHOOTER_BALLS; // Reset shooter balls for Player 2
                renderBubbleGrid(grid1, 0); // Re-render the bubble grid for Player 1
                renderBubbleGrid(grid2, SCREEN_WIDTH / 2); // Re-render the bubble grid for Player 2
            }

            // Check if all balls are used
            if (shooterBalls1 == 0 && shooterBalls2 == 0 && !isBubbleInAir1 && !isBubbleInAir2) {
                gameRunning = false;
            }
        }

        // Display winner message after the game loop ends
        if (player1Score > player2Score) {
            displayWinnerMessage("Player 1 Wins!");
        } else if (player2Score > player1Score) {
            displayWinnerMessage("Player 2 Wins!");
        } else {
            displayWinnerMessage("It's a Tie!");
        }

        // Clean up grid memory
        for (int row = 0; row < GRID_ROWS; ++row) {
            for (int col = 0; col < GRID_COLS; ++col) {
                delete grid1[row][col];
                delete grid2[row][col];
            }
        }

        closegraph();
        exit(0); // Terminate the program
    }
}