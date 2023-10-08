#include <SFML/Graphics.hpp>
#include <cmath>
#include <bits/stdc++.h>
#include "Dot.h"
#include "ColorDot.h"


sf::Vector2u size = {1920, 1080};
int cellSize = 10;
float magnitude = 100;
float scale = 0.007;
float multiplier = 1;
int nbP =3000; // Number of particles
bool isAngleFix = false;
int trailLength = 400;
bool isFlowFieldsVisible = false;
bool isParticleVisible = true;
bool isParticleHeadVisible = false;
bool isColorDotVisible = false;


class VelocityPoint{
private:
    sf::Vertex line[2];
public:
    sf::Vector2f pos;
    sf::Vector2f vel;
    float magnitude;
    float angle;

    VelocityPoint(sf::Vector2f pos, float angle, float magnitude): pos(pos), magnitude(magnitude), angle(angle){
        vel.x += magnitude * cos(angle);
        vel.y += magnitude * sin(angle);

        line[0].color = sf::Color::Blue;
        line[1].color = sf::Color::Blue;
        line[0].position = pos;
    }

    void update(float &t){
        if(isAngleFix){
            vel.x = magnitude * cos(angle  );
            vel.y = magnitude * sin(angle  );
        }else{
            vel.x = magnitude * cos(angle + (t * scale) );
            vel.y = magnitude * sin(angle + (t * scale) );
        }

    }

    void draw(sf::RenderWindow& win){
        line[1].position = pos + vel;
        win.draw(line, 2, sf::Lines);
    }
};

class Particle{
private:
    sf::CircleShape c;
    std::vector<sf::Vertex> trail;
public:
    float r;
    sf::Vector2f pos;
    sf::Vector2f vel;

    float t0;
    float tlast;
    sf::Color color;

    Particle(sf::Vector2f pos, float r, sf::Color col, float t0): pos(pos), r(r), t0(t0), tlast(t0), color(col){
        c.setRadius(r);
        c.setOrigin(r, r);
        c.setFillColor(col);
    }

    void checkEdges(){
        if(pos.x < 0 ){
            if(rand() % 10 > 5){
                pos.x = rand() % size.x;
                pos.y = rand() % size.y;
            }else{
                pos.x = size.x ;
            }
            trail.clear();
        }
        else if(pos.y < 0){
            if(rand() % 10 > 5){
                pos.x = rand() % size.x;
                pos.y = rand() % size.y;
            }else{
                pos.y = size.y  ;
            }
            trail.clear();
        }
        else if(pos.x > size.x ){
            if(rand() % 10 > 5){
                pos.x = rand() % size.x;
                pos.y = rand() % size.y;
            }else{
                pos.x = 0;
            };
            trail.clear();
        }
        else if(pos.y > size.y ){
            if(rand() % 10 > 5){
                pos.x = rand() % size.x;
                pos.y = rand() % size.y;
            }else{
                pos.y = 0;
            }
            trail.clear();
        }
    }

    void update(float &t, sf::Vector2f newVel, sf::Color newColor){

        if(trail.size() > trailLength){
            trail.erase(trail.begin());
            trail.erase(trail.begin());

        }

        vel = newVel;
        color = newColor;
        pos += vel * (t - tlast);
        sf::Vertex v1;
        v1.position = {pos.x +(int) r, pos.y};
        v1.color = newColor;
        sf::Vertex v2;
        v2.position = {pos.x -(int) r, pos.y};
        v2.color = newColor;
        trail.emplace_back(v1);
        trail.emplace_back(v2);


        tlast = t;

    }
    void draw(sf::RenderWindow& win){
        for(int i = 0; i < trail.size(); i++){
            trail[i].color.a =  (i/(float) trail.size()) * 255.f ;
        }
        c.setPosition(pos);
        win.draw(trail.data(), trail.size() ,sf::TriangleStrip);
        if(isFlowFieldsVisible){
            win.draw(c);
        }
    }
};


sf::Vector2f randomGradient(int ix, int iy) {
    // No precomputed gradients mean this works for any number of grid coordinates
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2;
    unsigned a = ix, b = iy;
    a *= 3287897443;

    b ^= a << s | a >> w - s;
    b *= 1917890717;

    a ^= b << s | b >> w - s;
    a *= 2047899325;
    float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]

    // Create the vector from the angle
    sf::Vector2f v;
    v.x = sin(random);
    v.y = cos(random);

    return v;
}

float dotGradient(int ix, int iy, float x, float y){
    //Get random gradient on corner
    sf::Vector2f gradient = randomGradient(ix, iy);

    //Get the values of the distance vector
    float dx = x - (float) ix;
    float dy = y - (float) iy;

    //Return dot product between them
    return (dx * gradient.x + dy * gradient.y);

}

float interpolate(float d1,float d2, float w ){
    return (d2 - d1) * (3.0 - w * 2.0) * w * w + d1; // Some kind of formula for interpolation idk
}

float perlin(float x, float y){
    //Get the values of the closest pixel and it's opposite
    int x0 = (int) x;
    int y0 = (int) y;
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    //Get the distance between (aka interpolation weights)
    float dx = x - (float) x0;
    float dy = y - (float) y0;

    //Get the values of the dot product for each corner and interpolate them
    float d1 = dotGradient(x0 ,y0, x, y); // TOP LEFT CORNER
    float d2 = dotGradient(x1, y0, x, y); // TOP RIGHT CORNER
    float ix0 = interpolate(d1, d2, dx); //Interpolate top corners on the x axis because they are on the same y;

    d1 = dotGradient(x0, y1, x, y); // BOTTOM LEFT CORNER
    d2 = dotGradient(x1, y1, x, y); // BOTTOM RIGHT CORNER
    float ix1 = interpolate(d1, d2, dx); //Interpolate the bottoms corners on the x axis because same y

    //FInally interpolate the two previous interpolations on the y axis
    float val = interpolate(ix0, ix1, dy);

    return val;

}

int main()
{
    auto window = sf::RenderWindow{ { size.x, size.y }, "CMake SFML Project" };
    window.setFramerateLimit(144);


    sf::Clock clock;
    float t = clock.getElapsedTime().asSeconds();

    //Create vector field
    std::vector<std::vector<VelocityPoint>> velPoints;
    for(float y = 0; y <= size.y; y+= cellSize){
        std::vector<VelocityPoint> tmp;
        for(float x = 0; x <= size.x; x+= cellSize){

            float perlinNoise = perlin(x * scale, y * scale );
            float angle = cos(perlinNoise * (M_PI * 2 * multiplier));
            tmp.emplace_back(sf::Vector2f{x + cellSize, y +cellSize}, angle, magnitude);

        }
        velPoints.emplace_back(tmp);
    }
    //Create color field

    std::vector<std::vector<ColorDot>> colors;
    for(float y = 0; y <= size.y; y+= cellSize){
        std::vector<ColorDot> tmp;
        for(float x = 0; x <= size.x; x+= cellSize){

            float r = (int) (abs(perlin(x * scale, y * scale ) * 255) + 110);
            float g = abs(perlin(x * scale, y * scale ) * 0);
            float b = (int) (abs(perlin((x +200) * scale, (y +200 ) * scale ) * 255) + 110);
            tmp.emplace_back(sf::Vector2f {x + cellSize, y + cellSize}, 5, sf::Color(r, g, b)); // (r > 255) ? 255: r,(g > 255)? 255: g,(b > 255)? 255: b)

        }
        colors.emplace_back(tmp);
    }

    //Create particles
    std::vector<Particle> particles;
    for(int i = 0; i< nbP; i++){
        particles.emplace_back(sf::Vector2f { (float) (rand() % size.x),(float) (rand() % size.y)}, 3, sf::Color(178, 41, 255), t);
    }





    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        window.clear(sf::Color(30, 12, 36));

        t = clock.getElapsedTime().asSeconds();
        if(isFlowFieldsVisible){
            for(std::vector<VelocityPoint> &vPointRow: velPoints){
                for(VelocityPoint &vpoint: vPointRow){
                    vpoint.update(t);
                    vpoint.draw(window);
                }

            }
        }else if(isParticleVisible){
            for(Particle& p: particles){
//                float perlinNoise = perlin(p.pos.x * scale, p.pos.y * scale );
//                float angle = cos(perlinNoise * (M_PI * 2 * multiplier));
                p.checkEdges();
                int xg = (int) p.pos.x / cellSize ;
                int yg = (int) p.pos.y / cellSize ;
                VelocityPoint vp = velPoints[yg][xg];
                sf::Color color = colors[yg][xg].color;
                p.update(t, vp.vel, color);
                p.draw(window);
            }

        }else if(isColorDotVisible){
            for(std::vector<ColorDot> rows: colors){
                for(ColorDot& cd: rows){
                    cd.draw(window);
                }
            }
        }


        window.display();
    }
}