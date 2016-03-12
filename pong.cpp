#include <SFML/Graphics.hpp>
#include <cmath>

class Paddle {
    sf::RenderWindow *window;
    sf::RectangleShape rectangle;
    int y;
    int player;
public:
    Paddle(sf::RenderWindow *window, int player) {
        this->y = 300;
        this->player = player;

        this->window = window;

        this->rectangle.setSize(sf::Vector2f(10, 100));
        this->rectangle.setOrigin(5, 50);

        if(this->player == 1) {
            this->rectangle.setPosition(25, this->y);
        }
        else {
            this->rectangle.setPosition(775, this->y);
        }
    }

    void draw() {
        this->window->draw(this->rectangle);
    }

    void update(float time) {
        if(this->player != 1) {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                if(this->y >= 50) {
                    this->y -= time * 0.5;
                    this->setY(this->y);
                }
            }
            else {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    if(this->y <= 550) {
                        this->y += time * 0.5;
                        this->setY(this->y);
                    }
                }
            }
        }
        else {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                if(this->y >= 50) {
                    this->y -= time * 0.5;
                    this->setY(this->y);
                }
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                if(this->y <= 550) {
                    this->y += time * 0.5;
                    this->setY(this->y);
                }
            }
        }
    }

    void setY(float y) {
        if(this->player == 1) {
            this->rectangle.setPosition(25, this->y);
        }
        else {
            this->rectangle.setPosition(775, this->y);
        }

    }

    int getY() {
        return this->y;
    }
};

class Ball {
    sf::RenderWindow *window;
    sf::RectangleShape shape;
    int x;
    int y;
    float dx;
    float dy;
    void initialize() {
        this->x = 400;
        this->y = 297;
        this->dx = 0.15;
        this->dy = 0;

        this->shape.setSize(sf::Vector2f(10, 10));
        this->shape.setOrigin(5, 5);

        this->shape.setPosition(this->x, this->y);
    }

    void collide(Paddle *player1, Paddle *player2) {
        if(this->x >= 770) {
            int delta = player2->getY() - this->y;
            if(fabs(delta) < 50) {
                this->x = 769;
                this->dx = (float) -(fabs(this->dx) + 0.03);
                changeDy(delta);
            }
        }
        else if (this->x <= 30) {
            int delta = player1->getY() -this->y;
            if(fabs(delta) < 50) {
                this->x = 31;
                this->dx = (float) (fabs(this->dx) + 0.03);
                changeDy(delta);
            }
        }

        if(this->y >= 600) {
            this->y = 599;
            this->dy = -this->dy;
        }
        if(this->y <= 0) {
            this->y = 1;
            this->dy = -this->dy;
        }
    }

    void changeDy(int delta) {

        if(delta == 0) {
            dy = 0;
        } else if(delta > 40) {
            dy = (float) -0.6;
        } else if(delta > 30) {
            dy = (float) -0.5;
        } else if(delta > 20) {
            dy = (float) -0.3;
        } else if(delta > 10) {
            dy = (float) -0.2;
        } else if(delta > 0) {
            dy = (float) -0.1;
        } else if(delta < -40) {
            dy = (float) 0.6;
        } else if(delta < -30) {
            dy = (float) 0.5;
        } else if(delta < -20) {
            dy = 0.3;
        } else if(delta < -10) {
            dy = (float) 0.2;
        } else if(delta < 0) {
            dy = (float) 0.1;
        }

        if(fabs(dy) > fabs(dx)) {
            
            if(delta > 0) {
                dy = (float) -fabs(dx);
            } else {
                dy = (float) fabs(dx);
            }
            
        }
    }
public:
    Ball(sf::RenderWindow *window) {

        this->window = window;
        this->initialize();
    }

    void draw() {
        this->window->draw(this->shape);
    }

    void setPosition(int x, int y) {
        this->shape.setPosition(x, y);
    }

    void update(float time, Paddle *player1, Paddle *player2) {
        this->x += time * this->dx;
        this->y += time * this->dy;
        collide(player1, player2);
        this->setPosition(this->x, this->y);
        if(this->x < 0 || this->x > 800) {
            this->initialize();
        }
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong");//, sf::Style::Fullscreen);
    window.setMouseCursorVisible(false);

    sf::Clock clock;
    float time;

    window.setVerticalSyncEnabled(true);
    Paddle player1(&window, 1);
    Paddle player2(&window, 0);
    Ball ball(&window);


    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed ||
               event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
        }

        window.clear();
        time = clock.getElapsedTime().asMilliseconds();
        player1.update(time);
        player2.update(time);
        ball.update(time, &player1, &player2);
        player1.draw();
        player2.draw();
        ball.draw();
        clock.restart();
        window.display();
    }

    return 0;
}
