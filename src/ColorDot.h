//
// Created by guill on 08/10/2023.
//

#ifndef CMAKESFMLPROJECT_COLORDOT_H
#define CMAKESFMLPROJECT_COLORDOT_H


#include <SFML/Graphics.hpp>


class ColorDot {

    sf::CircleShape cir;
public:
    sf::Vector2f pos;
    sf::Color color;

    ColorDot(sf::Vector2f pos, float r, sf::Color col): pos(pos), color(col) {
        cir.setRadius(r);
        cir.setOrigin(r, r);
        cir.setFillColor(col);
    }

    void draw(sf::RenderWindow& win){
        cir.setPosition(pos);
        win.draw(cir);
    }
};


#endif //CMAKESFMLPROJECT_COLORDOT_H
