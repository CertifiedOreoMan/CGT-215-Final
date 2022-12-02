// duckBuster.cpp : This file contains the 'main' function. Program execution begins and ends there.
//



#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include <vector>

using namespace std;
using namespace sf;
using namespace sfp;

const float KB_SPEED = 0.2;

void LoadTex(Texture& tex, string filename) {
    if (!tex.loadFromFile(filename)) {
        cout << "Could not load " << filename << endl;
    }
}


int main()
{
    RenderWindow window(VideoMode(800, 600), "Stickman Sprint");
    World world(Vector2f(0, 0));
    World world2(Vector2f(0, 1));
    int score(0);

    PhysicsSprite& stickman = *new PhysicsSprite();
    Texture stickmanTex1, stickmanTex2;
    LoadTex(stickmanTex1, "images/stickman1.png");
    LoadTex(stickmanTex2, "images/stickman2.png");
    stickman.setTexture(stickmanTex1);
    Vector2f sz = stickman.getSize();
    stickman.setCenter(Vector2f(400, 400));
    bool running = true;
    bool jumping = false;
    world2.AddPhysicsBody(stickman);


    /*PhysicsRectangle top;
    top.setSize(Vector2f(800, 10));
    top.setCenter(Vector2f(400, 5));
    top.setStatic(true);
    world.AddPhysicsBody(top);*/

    PhysicsRectangle floor;
    floor.setSize(Vector2f(800, 10));
    floor.setCenter(Vector2f(400, 595));
    floor.setStatic(true);
    floor.setFillColor(sf::Color{100, 100, 100, 255});
    world2.AddPhysicsBody(floor);

    Texture blockTex;
    LoadTex(blockTex, "images/block.png");
    PhysicsShapeList<PhysicsSprite> blocks;


    /*top.onCollision = [&drawingArrow, &world, &arrow]
    (PhysicsBodyCollisionResult result) {
        drawingArrow = false;
        world.RemovePhysicsBody(arrow);
    };*/

    Text scoreText;
    Font font;
    if (!font.loadFromFile("c:/windows/fonts/Arial.ttf")) {
        cout << "Couldn't load font arial.ttf" << endl;
        exit(1);
    }
    scoreText.setFont(font);
    Text arrowCountText;
    arrowCountText.setFont(font);

    Clock clock;
    Time lastTime(clock.getElapsedTime());
    Time currentTime(lastTime);
    long interval = 0;
    bool anim = true;
    while (running) {
        currentTime = clock.getElapsedTime();
        Time deltaTime = currentTime - lastTime;
        int deltaMS = deltaTime.asMilliseconds();
        interval += deltaMS;

        if (deltaMS > 9) {
            lastTime = currentTime;
            world.UpdatePhysics(deltaMS);
            world2.UpdatePhysics(deltaMS);
            //MoveCrossbow(crossBow, deltaMS);
            if (Keyboard::isKeyPressed(Keyboard::Space) &&
                !jumping) {
                jumping = true;
            }

            window.clear();
            for (PhysicsShape& block : blocks) {
                window.draw((PhysicsSprite&)block);
            }
            
            if (interval >= 42000000) {
                cout << "count" << endl;
                

                if (anim == true) {
                    stickman.setTexture(stickmanTex2);
                    anim = false;
                }
                else{
                    stickman.setTexture(stickmanTex1);
                    anim = true;
                }

                PhysicsSprite& block = blocks.Create();
                block.setTexture(blockTex);
                Vector2f sz = block.getSize();
                block.setCenter(Vector2f(0, 20 + (sz.y / 2)));
                block.setVelocity(Vector2f(0.25, 0));
                world.AddPhysicsBody(block);

                block.onCollision =
                    [&running, &world, &block, &blocks, &score, &stickman]
                (PhysicsBodyCollisionResult result) {
                    if (result.object2 == stickman) {
                        running = false;
                    }

                };
                interval = 0;
            }

            window.draw(stickman);
            window.draw(floor);
            scoreText.setString(to_string(score));
            FloatRect textBounds = scoreText.getGlobalBounds();
            scoreText.setPosition(
                Vector2f(790 - textBounds.width, 590 - textBounds.height));
            window.draw(scoreText);
            //arrowCountText.setString(to_string(arrows));
            textBounds = arrowCountText.getGlobalBounds();
            arrowCountText.setPosition(
                Vector2f(10, 590 - textBounds.height));
            window.draw(arrowCountText);
            //world.VisualizeAllBounds(window);

            window.display();
            //ducks.DoRemovals();
        }
    }
    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    FloatRect textBounds = gameOverText.getGlobalBounds();
    gameOverText.setPosition(Vector2f(
        400 - (textBounds.width / 2),
        300 - (textBounds.height / 2)
    ));
    window.draw(gameOverText);
    window.display();
    while (true);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
