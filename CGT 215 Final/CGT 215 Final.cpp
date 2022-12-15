// duckBuster.cpp : This file contains the 'main' function. Program execution begins and ends there.
//



#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include <vector>\


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
    PhysicsSprite& stickmanSliding = *new PhysicsSprite();

    Texture stickmanTex1, stickmanTex2, stickmanTex3, stickmanTex4;
    LoadTex(stickmanTex1, "images/stickman1-2.png");    //running frame 1
    LoadTex(stickmanTex2, "images/stickman2-2.png");    //running frame 2
    LoadTex(stickmanTex3, "images/stickman3-2.png");    //sliding (vertical)
    LoadTex(stickmanTex4, "images/stickman4-2.png");    //sliding (horizontal)

    stickman.setTexture(stickmanTex1);
    //stickman.setCenter(Vector2f(400, 500));
    //world.AddPhysicsBody(stickman);
    
    
    stickmanSliding.setTexture(stickmanTex4);
    
    
    PhysicsRectangle hitboxCenter, hitbox1, hitbox2, hitbox3;
    hitboxCenter.setSize(Vector2f(60, 60));
    hitbox1.setSize(Vector2f(60, 60));
    hitbox2.setSize(Vector2f(60, 40));
    hitbox3.setSize(Vector2f(60, 60));
    hitboxCenter.setCenter(Vector2f(400, 520));

    world2.AddPhysicsBody(hitboxCenter);
    world.AddPhysicsBody(hitbox1); 
    //world.AddPhysicsBody(hitbox2);  
    //world.AddPhysicsBody(hitbox3);  
    hitbox1.setStatic(true);
    hitbox2.setStatic(true);
    hitbox3.setStatic(true);

    bool running = true;
    bool jumping = false;
    bool sliding = false;
    bool hasDoubleJump = true;



    PhysicsCircle center;
    center.setSize(Vector2f(10, 10));
    center.setCenter(Vector2f(400, 500));
    center.setStatic(true);
    center.setFillColor(sf::Color{ 255, 0, 0, 255 });
    
    PhysicsCircle corner;
    corner.setSize(Vector2f(10, 10));
    corner.setCenter(Vector2f(400, 500));
    corner.setStatic(true);
    corner.setFillColor(sf::Color{ 0, 255, 0, 255 });

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
    //long elapsed = 0;
    bool anim = true;
    int max = 100;
    int randInt;
    int consecutiveBlocks = 0; //counts number of consecutive blocks spawned; should not exceed 2 consecutive blocks
    bool lastState = false;
    world.AddPhysicsBody(hitbox2);
    Vector2f centerStandingPoint, centerSlidingPoint;
    
    
    while (running) {
                      
        currentTime = clock.getElapsedTime();
        Time deltaTime = currentTime - lastTime;
        int deltaMS = deltaTime.asMilliseconds();
        interval += deltaMS;
        
        if (deltaMS > 9) {
            window.clear();
            cout << "1" << endl;
            hitbox1.setCenter(hitboxCenter.getCenter());
            hitbox2.setCenter(Vector2f(hitboxCenter.getCenter().x, hitboxCenter.getCenter().y - 70));
            hitbox3.setCenter(Vector2f(hitboxCenter.getCenter().x - 60, hitboxCenter.getCenter().y));

            centerStandingPoint = Vector2f(hitbox1.getCenter().x, hitbox1.getCenter().y - 40);
            centerSlidingPoint = Vector2f(hitbox1.getCenter().x - 30, hitbox1.getCenter().y);

            stickmanSliding.setCenter(centerSlidingPoint);
            

            lastTime = currentTime;
            world.UpdatePhysics(deltaMS);
            world2.UpdatePhysics(deltaMS);
            //MoveCrossbow(crossBow, deltaMS);

            center.setCenter(stickman.getCenter());
            corner.setCenter(stickman.getCenter()-stickman.getOrigin());
            cout << "2" << endl;
                                                                                                                
            if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) {
                if (jumping == false) {                                 
                    jumping = true;
                    hitboxCenter.applyImpulse(Vector2f(0, -0.50));
                }
                else if (jumping == true && hasDoubleJump == true && hitboxCenter.getVelocity().y >= -0.2) {   
                    hasDoubleJump = false;
                    hitboxCenter.setVelocity(Vector2f(0, 0));
                    hitboxCenter.applyImpulse(Vector2f(0, -0.50));
                }
            }
            cout << "3" << endl;
            
            if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) {
                sliding = true;
                //stickman.setRotation(-90);
                //stickman.setTexture(stickmanTex3);
                stickmanSliding.setCenter(centerSlidingPoint);
                //hitbox2.setCenter(Vector2f(hitbox1.getCenter().x - 60, hitbox1.getCenter().y));
                if (lastState == false) {
                    world.RemovePhysicsBody(hitbox2);
                    world.AddPhysicsBody(hitbox3);
                }
                window.draw(stickmanSliding);
                lastState = true;
            }
            else if (!Keyboard::isKeyPressed(Keyboard::S) && !Keyboard::isKeyPressed(Keyboard::Down)) {
                sliding = false;
                stickman.setRotation(0);
                stickman.setCenter(centerStandingPoint);
                if (lastState == true) {
                    world.RemovePhysicsBody(hitbox3);
                    world.AddPhysicsBody(hitbox2);
                }
                window.draw(stickman);
                lastState = false;
            }
            cout << "4" << endl;
            
            for (PhysicsShape& block : blocks) {
                window.draw((PhysicsSprite&)block);
            }
            window.draw(center);
            cout << "5" << endl;
            if (interval >= 32000000) {
                cout << "count" << endl;
                //cout << jumping << endl;
                //cout << hasDoubleJump << endl;

                if (sliding == false) {
                    if (anim == true) {
                        stickman.setTexture(stickmanTex2);
                        anim = false;
                    }
                    else if (anim == false) {
                        stickman.setTexture(stickmanTex1);
                        anim = true;
                    }
                }
                
                
                PhysicsSprite& block = blocks.Create();
                block.setTexture(blockTex);
                Vector2f sz = block.getSize();
                randInt = rand() % max;
                if (randInt < 33) {
                    if (consecutiveBlocks < 2) {
                        block.setCenter(Vector2f(0, 500 + (sz.y / 2)));  //upper
                        block.setVelocity(Vector2f(0.25, 0));
                        world.AddPhysicsBody(block);
                        consecutiveBlocks++;
                    }
                    else {
                        blocks.QueueRemove(block);
                        consecutiveBlocks = 0;
                    }
                    
                }
                else if (randInt >= 33 && randInt < 66) {
                    if (consecutiveBlocks < 2) {
                    block.setCenter(Vector2f(0, 550 + (sz.y / 2)));  //lower
                    block.setVelocity(Vector2f(0.25, 0));
                    world.AddPhysicsBody(block);
                    consecutiveBlocks++;
                    }
                    else {
                        blocks.QueueRemove(block);
                        consecutiveBlocks = 0;
                    }
                }
                else {
                    
                    blocks.QueueRemove(block);
                    consecutiveBlocks = 0;
                }
                

                block.onCollision =
                    [&running, &world, &block, &blocks, &score, &stickman, &stickmanSliding, &hitbox1, &hitbox2, &hitbox3]
                (PhysicsBodyCollisionResult result) {
                    if (result.object2 == stickman || result.object2 == stickmanSliding) {
                        running = false;
                    }

                };
                hitboxCenter.onCollision =
                    [&jumping, &stickman, &floor, &hasDoubleJump, &hitboxCenter]
                (PhysicsBodyCollisionResult result) {
                    if (result.object2 == floor) {
                        hitboxCenter.setCenter(Vector2f(400, 560));
                        hitboxCenter.setVelocity(Vector2f(0, 0));
                        jumping = false;
                        hasDoubleJump = true;
                    }
                };
                //elapsed += interval;
                interval = 0;
                
            }

            
            window.draw(floor);
            //window.draw(stickman);
            //window.draw(corner);
            //stickman.visualizeBounds(window);
            /*hitbox1.visualizeBounds(window);
            hitbox2.visualizeBounds(window);
            hitbox3.visualizeBounds(window);*/
            world.VisualizeAllBounds(window);
            world2.VisualizeAllBounds(window);
            scoreText.setString(to_string(score));
            FloatRect textBounds = scoreText.getGlobalBounds();
            scoreText.setPosition(
                Vector2f(790 - textBounds.width, 10 + textBounds.height));
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
