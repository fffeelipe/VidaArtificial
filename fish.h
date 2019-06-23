#ifndef FISH_H
#define FISH_H

#include <SFML/Graphics.hpp>
#include<bits/stdc++.h>

class Fish
{


public:
    bool male;
    int x, y, vision, maxEnergy, energy, liveExpectation, id, life;
    double currSize, maxSize, speed, rot, reproductive_percentage;
    sf::Sprite sprite;
    sf::Texture texture;
    Fish(int x, int y, double rot, double speed, int vision, double currSize, double maxSize,
         int maxEnergy, int liveExpectation, double reproductive_percentage);
    bool operator< (const Fish &f) const{
        return this->id < f.id;
    }
    void move(sf::RenderWindow &window, int maxX, int maxY);
    void feed(std::vector<std::vector<short> > &food);

};

void run_fish(sf::RenderWindow &window, int maxX, int maxY, std::set<Fish*> &fish, std::vector<std::vector<short> > &food);

#endif // FISH_H
