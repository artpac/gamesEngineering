#include <SFML/Graphics.hpp>


using namespace sf;
using namespace std;

const Keyboard::Key controls[4] = {
    Keyboard::A,   // Player1 UP
    Keyboard::Z,   // Player1 Down
    Keyboard::Up,  // Player2 UP
    Keyboard::Down // Player2 Down
};

Vector2f ballVelocity;
bool isPlayer1Serving = false;
const float initialVelocityX = 100.f;
const float initialVelocityY = 60.f;

const float velocityMultiplier = 1.1f;


const Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.f;
const float paddleOffsetWall = 10.f;

CircleShape ball;
RectangleShape paddles[2];
void Reset() {
    paddles[0].setPosition(Vector2f(paddleOffsetWall + paddleSize.x / 2.f, gameHeight / 2.f));
    paddles[1].setPosition(Vector2f(gameWidth - paddleOffsetWall - paddleSize.x / 2.f, gameHeight / 2.f));
    // reset Ball Position
    ball.setPosition(gameWidth / 2.f, gameHeight / 2.f);
}
void Load() {
    // Set size and origin of paddles
    for (auto& p : paddles) {
        p.setSize(paddleSize);
        p.setOrigin(paddleSize / 2.f);
    }
    // Set size and origin of ball
    ball.setRadius(ballRadius);
    ball.setOrigin(ballRadius, ballRadius); //Should be half the ball width and height
    // reset paddle position

    ballVelocity = { (isPlayer1Serving ? initialVelocityX : -initialVelocityX), initialVelocityY };
}
void Update(RenderWindow& window) {
    // Reset clock, recalculate deltatime
    static Clock clock;
    float dt = clock.restart().asSeconds();
    // check and consume events
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
            return;
        }
    }

    // Quit Via ESC Key
    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        window.close();
    }

    // handle paddle movement
    float direction = 0.0f;
    if (Keyboard::isKeyPressed(controls[0])) {
        direction--;
    }
    if (Keyboard::isKeyPressed(controls[1])) {
        direction++;
    }
    paddles[0].move(Vector2f(0.f, direction * paddleSpeed * dt));

    float aiPaddleSpeed = 300.f;
    float ballY = ball.getPosition().y;
    float aiPaddleY = paddles[1].getPosition().y;

    if (aiPaddleY < ballY) {
        //If the ball is below the AI paddle move dowb
        paddles[1].move(0.f, aiPaddleSpeed * dt);
    }
    else if (aiPaddleY > ballY) {
        //If the ball is above the AI paddle move up
        paddles[1].move(0.f, -aiPaddleSpeed * dt);
    }

    // Prevent AI paddle from going off the screen
    if (paddles[1].getPosition().y - paddleSize.y / 2.f < 0.f)
        paddles[1].setPosition(paddles[1].getPosition().x, paddleSize.y / 2.f);
    if (paddles[1].getPosition().y + paddleSize.y / 2.f > gameHeight)
        paddles[1].setPosition(paddles[1].getPosition().x, gameHeight - paddleSize.y / 2.f);


    ball.move(ballVelocity * dt);
    // check ball collision
    const float bx = ball.getPosition().x;
    const float by = ball.getPosition().y;
    if (by > gameHeight) { //bottom wall
        // bottom wall
        ballVelocity.x *= velocityMultiplier;
        ballVelocity.y *= -velocityMultiplier;
        ball.move(Vector2f(0.f, -10.f));
    }
    else if (by < 0) { //top wall
        // top wall
        ballVelocity.x *= velocityMultiplier;
        ballVelocity.y *= -velocityMultiplier;
        ball.move(Vector2f(0.f, 10.f));
    }
    else if (bx > gameWidth) {
        // right wall
        Reset();
    }
    else if (bx < 0) {
        // left wall
        Reset();
    }
    else if (
        //ball is inline or behind paddle AND
        bx < paddleSize.x + paddleOffsetWall &&
        //ball is below top edge of paddle AND
        by > paddles[0].getPosition().y - (paddleSize.y * 0.5) &&
        //ball is above bottom edge of paddle
        by < paddles[0].getPosition().y + (paddleSize.y * 0.5))
    {
        // bounce off left paddle
        ballVelocity.x *= -1.0f;
    }
    else if (
        // Ball is inline or behind right paddle
        bx > gameWidth - paddleSize.x - paddleOffsetWall &&
        // Ball is within the vertical bounds of the right paddle
        by > paddles[1].getPosition().y - (paddleSize.y * 0.5) &&
        by < paddles[1].getPosition().y + (paddleSize.y * 0.5)
        ) {
        // Bounce off right paddle
        ballVelocity.x *= -1.0f;  // Reverse the x-direction of ball
    }

    // Clamp paddles within screen bounds
    if (paddles[0].getPosition().y - paddleSize.y / 2.f < 0.f)
        paddles[0].setPosition(paddles[0].getPosition().x, paddleSize.y / 2.f);
    if (paddles[0].getPosition().y + paddleSize.y / 2.f > gameHeight)
        paddles[0].setPosition(paddles[0].getPosition().x, gameHeight - paddleSize.y / 2.f);

    if (paddles[1].getPosition().y - paddleSize.y / 2.f < 0.f)
        paddles[1].setPosition(paddles[1].getPosition().x, paddleSize.y / 2.f);
    if (paddles[1].getPosition().y + paddleSize.y / 2.f > gameHeight)
        paddles[1].setPosition(paddles[1].getPosition().x, gameHeight - paddleSize.y / 2.f);

   }
void Render(RenderWindow& window) {
    // Draw Everything
    window.draw(paddles[0]);
    window.draw(paddles[1]);
    window.draw(ball);
}

int main() {
    RenderWindow window(VideoMode(gameWidth, gameHeight), "PONG");
    Load();
    while (window.isOpen()) {
        window.clear();
        Update(window);
        Render(window);
        window.display();
    }
    return 0;
}
