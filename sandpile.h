#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

void addgrain(std::vector<std::vector<short> > &food, int x, int y){
    if(food[x][y] < 3){
        ++food[x][y];
        return;
    }
    if(x and y and x < food.size() -1 and y < food[0].size() -1){
        food[x][y] = 0;
        addgrain(food,x-1,y);
        addgrain(food,x,y-1);
        addgrain(food,x+1,y);
        addgrain(food,x,y+1);
    }
    //std::cout<<"added a grain";
}

void addRandGrains(std::vector<std::vector<short> > &food, sf::RenderWindow &window, int c){
    while (c--){
        addgrain(food, rand()%food.size(), rand()%food[0].size());
    }
    addgrain(food, 200, 200); //sandpile demostration
    for(int i = 0; i<food.size(); i++){
        for(int j = 0; j<food[0].size(); j++){
            if(food[i][j]){
                //std::cout<<"added a circle\n";
                sf::CircleShape shape(1);
                if(food[i][j] == 1)
                    shape.setFillColor(sf::Color::Blue);
                if(food[i][j] == 2)
                    shape.setFillColor(sf::Color::Green);
                if(food[i][j] == 3)
                    shape.setFillColor(sf::Color::Red);
                shape.setPosition(j,i);
                window.draw(shape);

            }
        }
    }
}
