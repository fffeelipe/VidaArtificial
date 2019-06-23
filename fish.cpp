#include "fish.h"

Fish::Fish(int x, int y, double rot, double speed, int vision, double minSize, double maxSize,
           int maxEnergy, int liveExpectation, double reproductive_percentage):x(x), y(y), rot(rot),
                speed(speed), vision(vision), currSize(minSize),maxSize(maxSize), maxEnergy(maxEnergy),
                liveExpectation(liveExpectation), reproductive_percentage(reproductive_percentage),
                id(rand()), male(rand()%2)
    {
    if (!texture.loadFromFile("fishfixed.png")){
            std::cerr << "Error while loading texture" << std::endl;
            return;
    }
    std::random_device rd;
    std::mt19937 e2(rd());
    std::normal_distribution<> dist(liveExpectation, liveExpectation*0.1);
    energy = maxEnergy/2;
    life = dist(e2);
    sprite.setTexture(texture);
    sf::FloatRect spriteSize=sprite.getGlobalBounds();
    sprite.setOrigin(spriteSize.width/2.,spriteSize.height/2.);


}
void Fish::move(sf::RenderWindow &window, int maxX, int maxY){
    x = speed * cos(rot/180*(M_PI)) + x + maxX;
    x%=maxX;
    y = speed * sin(rot/180*(M_PI)) + y + maxY;
    y%=maxY;
    rot += (rand()%6)-3;
    energy -= 1;
    life -= 1;
    //draw related stuff
    sprite.setPosition(x,y);
    sprite.setRotation(rot+180);
    window.draw(sprite);

}

void Fish::feed(std::vector<std::vector<short> > &food){
    for(int i = std::max(0.0, y-(50*currSize)/2); i<std::min(food.size() +0.0,y+(50*currSize)/2);i++){
        for(int j = std::max(0.0, x-(50*currSize)/2); j<std::min(food[0].size() +0.0,x+(50*currSize)/2);j++){
            if(energy > maxEnergy)
                return;
            energy += food[i][j];
            food[i][j] = 0;
            //std::cout<<"feeded\n";
        }
    }
}

void run_fish(sf::RenderWindow &window, int maxX, int maxY, std::set<Fish*> &fish, std::vector<std::vector<short> > &food){
    std::vector<Fish*> dead;
    for(auto &f : fish){
        f->move(window, maxX, maxY);
        f->feed(food);
        //std::cout<<f->energy <<"\n";
        if(f->life<0 or f->energy<0)
            dead.push_back(f);
    }
    for(auto &f : dead){
        std::cout<<"the fuck? energy: "<<f->energy<<", life: "<<f->life<<"\n";
        fish.erase(f);
        delete f;
    }
}
