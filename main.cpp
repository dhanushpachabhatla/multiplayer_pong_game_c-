#include <iostream>
#include <raylib.h>
#include <chrono>
#include <thread>

using namespace std;

int screenWidth = 900;
int screenHeight = 1000;
int player1Score = 0;
int player2Score = 0;

class ball {
public:
    float x, y, radius;
    double speedX, speedY;
    bool isIdle;  // Flag to indicate if the ball should be idle
    chrono::steady_clock::time_point idleStartTime;  // Time when idle started

    ball() : isIdle(false) {}  // Initialize the idle flag to false

    void update() {
        if (isIdle) {
            auto now = chrono::steady_clock::now();
            auto elapsed = chrono::duration_cast<chrono::seconds>(now - idleStartTime).count();
            if (elapsed >= 1) {  // 1 second idle time
                isIdle = false;  // End the idle state
                speedX = GetRandomValue(9, -9);
                speedY = speedY > 0 ? -10 : 10;
            }
            return;  // Exit the update function to keep the ball idle
        }

        x += speedX;
        y += speedY;
        if (x + radius > screenWidth || x - radius < 0) {
            speedX *= -1;
        }
        if (y + radius > screenHeight) {
            player1Score++;
            resetBall();
            isIdle = true;  // Start the idle state
            idleStartTime = chrono::steady_clock::now();  // Record the start time
        }
        if (y - radius < 0) {
            player2Score++;
            resetBall();
            isIdle = true;  // Start the idle state
            idleStartTime = chrono::steady_clock::now();  // Record the start time
        }
    }

    void draw() {
        DrawCircle(x, y, radius, GREEN);
    }

private:
    void resetBall() {
        x = screenWidth / 2;
        y = screenHeight / 2;
        speedX = GetRandomValue(9, -9);
        speedY = 10;
    }
};

class rod {
public:
    float width, height, x, y;
    double speedX;

    void update() {
        if (IsKeyDown(KEY_E)) {
            x += speedX;
        }
        if (IsKeyDown(KEY_Q)) {
            x -= speedX;
        }
        if (x + width > screenWidth) {
            x = screenWidth - width;
        }
        if (x < 0) {
            x = 0;
        }
    }

    void draw() {
        DrawRectangle(x, y, width, height, WHITE);
    }
};

class downrod : public rod {
public:
    void update() {
        if (IsKeyDown(KEY_RIGHT)) {
            x += speedX;
        }
        if (IsKeyDown(KEY_LEFT)) {
            x -= speedX;
        }
        if (x + width > screenWidth) {
            x = screenWidth - width;
        }
        if (x < 0) {
            x = 0;
        }
    }

    void draw() {
        DrawRectangle(x, y, width, height, WHITE);
    }
};

int main() {
    ball b1;
    b1.speedX = 10;
    b1.speedY = 10;
    b1.x = screenWidth / 2;
    b1.y = screenHeight / 2;
    b1.radius = 20;

    rod p1;
    p1.width = 200;
    p1.x = screenWidth / 2 - p1.width / 2;
    p1.y = 10;
    p1.height = 30;
    p1.speedX = 8;

    downrod p2;
    p2.width = 200;
    p2.x = screenWidth / 2 - p2.width / 2;
    p2.y = screenHeight - 45;
    p2.height = 30;
    p2.speedX = 8;

    InitWindow(screenWidth, screenHeight, "Game started");
    SetTargetFPS(90);

    while (!WindowShouldClose()) {
        ClearBackground(BLACK);
        DrawLine(0, screenHeight / 2, screenWidth, screenHeight / 2, WHITE); // (starting pts, ending pts, color)

        // Update
        b1.update();
        p1.update();
        p2.update();

        // Collision detection
        if (CheckCollisionCircleRec(Vector2{ b1.x, b1.y }, b1.radius, Rectangle{ p1.x, p1.y, p1.width, p1.height })) {
            b1.speedY *= -1;
        }
        if (CheckCollisionCircleRec(Vector2{ b1.x, b1.y }, b1.radius, Rectangle{ p2.x, p2.y, p2.width, p2.height })) {
            b1.speedY *= -1;
        }

        // Drawing
        b1.draw();
        p1.draw();
        p2.draw();

        // Score
        DrawText(TextFormat("%i", player1Score), screenWidth / 2, 60, 40, YELLOW);
        DrawText(TextFormat("%i", player2Score), screenWidth / 2, screenHeight - 60, 40, YELLOW);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
