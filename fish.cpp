#include "fish.h"
const bool MOVE = true;
Fish::Fish(int x, int y, double rot, double speed, int vision, double minSize, double maxSize,
           int maxEnergy, int liveExpectation, double reproductive_percentage, double startLookingMate,
           double stopLookingMate):
    x(x), y(y), rot(rot),
    speed(speed), vision(vision),
    currSize(minSize),maxSize(maxSize),
    maxEnergy(maxEnergy),liveExpectation(liveExpectation),
    reproductive_percentage(reproductive_percentage),
    id(rand()), male(rand()%2),
    searchingMate(false), startLookingMate(startLookingMate),
    stopLookingMate(stopLookingMate)
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
    sprite.setOrigin(spriteSize.width/2,spriteSize.height/2);
    auto s = texture.getSize();


    }

double Fish::customAtan2(double y, double x){
    y = -y;
    double temp = atan2(y,x);
    if(temp<=0){
        temp *=-1;
    }else{
        temp = M_PI * 2 - temp;
    }
    //std::cerr<<x<<' '<<y<<'\n'<<temp * 180/M_PI<<'\n';
    return temp * 180/M_PI;
}

void Fish::move(sf::RenderWindow &window, int maxX, int maxY){
    if(MOVE){
        x = speed * cos(rot/180*(M_PI)) + x + maxX;
        x%=maxX;
        y = speed * sin(rot/180*(M_PI)) + y + maxY;
        y%=maxY;
    }
    rot += (rand()%10)-5;
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
            energy += food[i][j]*food[i][j]*100;
            food[i][j] = 0;
            //std::cout<<"feeded\n";
        }
    }
    if(energy > maxEnergy * startLookingMate and (1 - reproductive_percentage)*liveExpectation > life){
        if(!searchingMate)std::cout<<"now reproductive\n";
        searchingMate = true;
    }
    if(energy < maxEnergy * stopLookingMate)
        searchingMate = false;
}
void Fish::searchFood(std::vector<std::vector<short> > &food){
    int mindis = 10000000;
    int maxfood = 2;
    for(int i = std::max(0, y-(vision)/2); i<std::min(int(food.size()),y+(vision)/2);i++){
        for(int j = std::max(0, x-(vision)/2); j<std::min(int(food[0].size()) ,x+(vision)/2);j++){
            if(food[i][j] > maxfood or (food[i][j] == maxfood and
                                        abs(x-j)*abs(x-j) + abs(y-i)*abs(y-i) < mindis)){
                maxfood = food[i][j];
                mindis = abs(x-j)*abs(x-j) + abs(y-i)*abs(y-i);
                auto temrot = customAtan2(i-y,j-x);
                rot = (rot*5 + temrot)/6;
                //std::cerr<<x-i<<' '<<j-y<<' '<<" <- distancias x,y \n";
            }
        }
    }
}

void Fish::searchMate(std::set<Fish*> &fish){
    int mindis = 1000000000;
    for(auto f : fish){
        int dis = abs(x-f->x)*abs(x-f->x)+abs(y-f->y)*abs(y-f->y);
        if(f->searchingMate and male != f->male and dis <= vision*vision and dis < mindis ){
            mindis = dis;
            rot = customAtan2(-y+f->y, -x+f->x);
            if(mindis < 250){
                Fish * newfish = new Fish(x,y,0,speed,vision,currSize,maxSize,maxEnergy,liveExpectation,reproductive_percentage,startLookingMate,stopLookingMate);
                fish.insert(newfish);
                energy = maxEnergy*stopLookingMate;
                searchingMate = false;
                f->energy = f->maxEnergy*f->stopLookingMate;
                f->searchingMate = false;
                std::cout<<"added new fish\n";

            }
        }
    }
}

void Fish::avoid_obstacles(std::set<Shark*> &sharks, std::vector<std::pair<int,int> > &rockspos){
    for(auto &r : rockspos){
        int dis = abs(x-r.first)*abs(x-r.first)+abs(y-r.second)*abs(y-r.second);
        if(dis < 1000){
            rot = customAtan2(r.second-y, r.first - x) + 180;
            return;
        }
    }
}

void run_fish(sf::RenderWindow &window, int maxX, int maxY, std::set<Fish*> &fish, std::vector<std::vector<short> > &food,
              std::set<Shark*> shaks, std::vector<std::pair<int,int> > rockspos){
    std::vector<Fish*> dead;
    for(auto &f : fish){
        f->searchFood(food);
        f->avoid_obstacles(shaks, rockspos);
        if(f->searchingMate)
            f->searchMate(fish);
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
