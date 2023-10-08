//
// Created by guill on 07/10/2023.
//

#ifndef CMAKESFMLPROJECT_DOT_H
#define CMAKESFMLPROJECT_DOT_H


#include <valarray>
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

class Dot {

    sf::CircleShape c;
    sf::Vertex vline[2];
public:
    float r;
    sf::Vector2f vel;
    sf::Vector2f cpos;
    sf::Vector2f lpos0;
    sf::Vector2f lpos1;
    float unit = 1.0 / sqrt(2) * 50;
    float u;
    float v;
    float tmp{};
    float x1;
    float x2;
    float t0;
    float tlast;
    float dt;
    float pval;
    int x0{};
    int y0{};
    float angle;
    int cellSize;



    Dot(sf::Vector2f pos, sf::Color color, float r, float &t0, float angle, int cellSize) : cpos(pos), r(r), t0(t0), tlast(t0), angle(angle), cellSize(cellSize){

        c.setRadius(r);
        c.setOrigin(r, r);


//
//        vline[0].color = sf::Color::Blue;
//        vline[1].color = sf::Color::Blue;
    }

    void update(float &t){
        dt = t - tlast;
        tlast = t;
        vel.x = (cellSize / 2) * cos(angle + (t - t0));
        vel.y = (cellSize / 2) * sin(angle + (t - t0));
        c.setFillColor(sf::Color((angle + (t - t0)) * 255, 0, 0));

        lpos0 = cpos;
        lpos1 = lpos0 + (vel );
    }

    void draw(sf::RenderWindow& win){
        c.setPosition(cpos);
        vline[0].position = lpos0;
        vline[1].position = lpos1;
        win.draw(c);
    }
};


#endif //CMAKESFMLPROJECT_DOT_H
