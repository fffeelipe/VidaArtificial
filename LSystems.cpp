#include <SFML/Graphics.hpp>
#include<bits/stdc++.h>
#include<LSystems.h>
#define _USE_MATH_DEFINES



void addtrees(std::vector<std::pair<int,int> > &pos, sf::RenderWindow &window){
    int i = 0;
    for(auto p : pos){
        if(i % 3 == 0)
            drawTree1(window, p.first, p.second, 30, 5, -90, 10);
        if(i % 3 == 1)
            drawTree2(window, p.first, p.second, 40, 3, -90, 7);
        if(i % 3 == 2)
            drawTree3(window, p.first, p.second, 50, 2, -90, 10);
        ++i;
    }
}

void drawTree1(sf::RenderWindow &window, double x, double y, double len, double thick, double rot, int c){
    if(c <= 0){
        return;
    }
    thick = std::max(thick, 1.0);
    sf::RectangleShape line(sf::Vector2f(len, thick));
    line.setPosition(x,y);
    line.setFillColor(sf::Color::Green);
    line.rotate(rot);
    window.draw(line);
    double nx = len * cos(rot/180*(M_PI)) + x, ny = len * sin(rot/180*(M_PI)) + y;
    drawTree1(window, nx, ny,len*0.9,thick-1, rot + 30 + rand()%2-1, c-1);
    drawTree1(window, nx, ny,len*0.9,thick-1, rot - 30 + rand()%2-1, c-1);
}

void drawTree2(sf::RenderWindow &window, double x, double y, double len, double thick, double rot, int c){
    if(c <= 0){
        return;
    }
    thick = std::max(thick, 1.0);
    sf::RectangleShape line(sf::Vector2f(len, thick));
    line.setPosition(x,y);
    line.setFillColor(sf::Color::Green);
    line.rotate(rot);
    window.draw(line);
    double nx = len * cos(rot/180*(M_PI)) + x, ny = len * sin(rot/180*(M_PI)) + y;
    //recursive part
    drawTree2(window, nx, ny,len*0.9,thick-0.5, rot + rand()%4-2, c-1);
    drawTree2(window, nx, ny,len*0.9,thick-1, rot + 20 + rand()%4-2, c-1);
    drawTree2(window, nx, ny,len*0.9,thick-1, rot - 20 + rand()%4-2, c-1);
}

void drawTree3(sf::RenderWindow &window, double x, double y, double len, double thick, double rot, int c){
    if(c <= 0){
        return;
    }
    thick = std::max(thick, 1.0);
    sf::RectangleShape line(sf::Vector2f(len, thick));
    line.setPosition(x,y);
    line.setFillColor(sf::Color::Green);
    line.rotate(rot);
    window.draw(line);
    double nx = len * cos(rot/180*(M_PI)) + x, ny = len * sin(rot/180*(M_PI)) + y;

    drawTree3(window, nx, ny,len*0.7,thick-0.5, rot + rand()%2-1, c-1);
    drawTree3(window, nx, ny,len*0.5,thick-1, rot + 30 + rand()%4-2, c/2);
    drawTree3(window, nx, ny,len*0.5,thick-1, rot - 30 + rand()%4-2, c/2);
}
