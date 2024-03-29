#ifndef FISH_H
#define FISH_H

#include <SFML/Graphics.hpp>
#include<bits/stdc++.h>


class Fish
{


public:
    bool male, searchingMate;
    int x, y, vision, maxEnergy, energy, liveExpectation, id, life, initialDensity,
    innerRadiousX, innerRadiousY, outterRadiousX, outterRadiousY;
    double minSize, currSize, maxSize, speed, rot, reproductive_percentage, startLookingMate, stopLookingMate,
        ratio;
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Color c1,c2;
    Fish *tagged;
    Fish(int x, int y, double rot, double speed, int vision, double minSize, double maxSize,
         int maxEnergy, int liveExpectation, double reproductive_percentage, double startLookingMate,
         double stopLookingMate, int innerRadiousX, int innerRadiousY, int outterRadiousX, int outterRadiousY,
         int initialDensity, double ratio, sf::Color c1, sf::Color c2, bool isShark);
    bool operator< (const Fish &f) const{
        return this->id < f.id;
    }
    void move(sf::RenderWindow &window, int maxX, int maxY);
    void fishFeed(std::vector<std::vector<short> > &food);
    void sharkFeed(std::set<Fish*> &fish);
    void fishSearchFood(std::vector<std::vector<short> > &food);
    void sharkSearchFood(std::set<Fish*> &fish);
    void searchMate(std::set<Fish*> &fish);
    void avoid_obstacles(std::set<Fish*> &sharks, std::vector<std::pair<int,int> > &rockspos);
    double customAtan2(double y, double x);
    void sexualReproducction(Fish *f, std::set<Fish*> &fish);
    sf::Uint8* pixels();
    void setFishPNG();
    void setSharkPNG();
    int distance_basic(int x1,int y1,int x2,int y2);
    int distance(int x1,int y1,int x2,int y2, float &rot);

};

void run_fish(sf::RenderWindow &window, int maxX, int maxY, std::set<Fish*> &fish, std::vector<std::vector<short> > &food,
              std::set<Fish*> &sharks, std::vector<std::pair<int,int> > rockspos);
#endif // FISH_H
