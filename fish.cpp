#include "fish.h"
const bool MOVE = true;
Fish::Fish(int x, int y, double rot, double speed, int vision, double minSize, double maxSize,
           int maxEnergy, int liveExpectation, double reproductive_percentage, double startLookingMate,
           double stopLookingMate, int innerRadiousX, int innerRadiousY, int outterRadiousX, int outterRadiousY,
           int initialDensity, double ratio, sf::Color c1, sf::Color c2, bool isShark):
    x(x), y(y), rot(rot),
    speed(speed), vision(vision),
    minSize(minSize),maxSize(maxSize),
    maxEnergy(maxEnergy),liveExpectation(liveExpectation),
    reproductive_percentage(reproductive_percentage),
    id(rand()), male(rand()%2),
    searchingMate(false), startLookingMate(startLookingMate),
    stopLookingMate(std::max(stopLookingMate,0.0)),
    innerRadiousX(innerRadiousX),
    innerRadiousY(innerRadiousY),
    outterRadiousX(std::max(outterRadiousX, innerRadiousX + 1)),
    outterRadiousY(std::max(outterRadiousY, innerRadiousY + 1)),
    initialDensity(initialDensity),
    ratio(ratio), currSize(minSize),
    c1(c1), c2(c2), tagged(nullptr)
    {
    if(!isShark)setFishPNG();
    else setSharkPNG();

//    if(rand()%100 < 33){
//        this->c2 = sf::Color(c2.r + (rand()%17)-8, c2.b + (rand()%17)-8, c2.g + (rand()%17)-8, c2.a);
//    }
//    if(rand()%100 < 33){
//        this->c1 = sf::Color(c1.r + (rand()%17)-8, c1.b + (rand()%17)-8, c1.g + (rand()%17)-8, c1.a);
//    }
    std::random_device rd;
    std::mt19937 e2(rd());
    std::normal_distribution<> dist(liveExpectation, liveExpectation*0.1);
    energy = maxEnergy/2;
    life = dist(e2);
    sf::FloatRect spriteSize=sprite.getGlobalBounds();
    sprite.setOrigin(spriteSize.width/2,spriteSize.height/2);
    texture.update(pixels());
    sprite.setTexture(texture);
    }
void Fish::setFishPNG(){
    if (!texture.loadFromFile("fishfixed.png")){
            std::cerr << "Error while loading texture" << std::endl;
            return;
    }
}

void Fish::setSharkPNG(){
    if (!texture.loadFromFile("sharkfixed.png")){
            std::cerr << "Error while loading texture" << std::endl;
            return;
    }
}



sf::Uint8* Fish::pixels(){
    auto s = texture.getSize();
    int len = s.x * s.y * 4;
    sf::Uint8* pixels = new sf::Uint8[s.x * s.y * 4]; // * 4 because pixels have 4 components (RGBA)
    bool turinmorph[s.y][s.x];
    for(int yy = 0; yy< s.y; yy++){
        for(int xx = 0; xx < s.x; xx++){
            turinmorph[yy][xx] = (rand()%100<initialDensity?true:false);
        }
    }

    bool temp[s.y][s.x];
    int repetitions = 20;
    while(repetitions--){
       for(int yy = 0; yy< s.y; yy++){
            for(int xx = 0; xx < s.x; xx++){
                double p = 0;
                //std::cerr<<"...."<<yy<<' '<<xx<<".....\n";
                for(int i = -outterRadiousY; i<= outterRadiousY; i++){
                    for(int j = -outterRadiousX; j <= outterRadiousX; j++){
                        //std::cerr<<"."<<(yy+i+s.y)%s.y<<' '<<(xx+j+s.x)%s.x<<".\n";
                        //std::cerr<<"."<<(yy+i+s.y)%s.y<<' '<<(xx+j+s.x)%s.x<<".\n";
                        if(turinmorph[(yy+i+s.y)%s.y][(xx+j+s.x)%s.x]){
                            if(abs(j) <= innerRadiousX and abs(i)<= innerRadiousY){
                                p += 1.0;
                                //std::cerr<<"good\n";
                            }else{
                                p -= ratio;
                                //std::cerr<<"bad\n";
                            }
                        }
                    }
                }
                //std::cerr<<"p = "<<p<<'\n';
                if(p>0.0)
                    temp[yy][xx]=true;
                else if(p<0.0)
                    temp[yy][xx]=false;
            }
        }
        //update main matrix
        for(int yy = 0; yy< s.y; yy++){
            for(int xx = 0; xx < s.x; xx++){
                turinmorph[yy][xx] = temp[yy][xx];
            }
        }

    }


    //aplly the turing pattern within the fish
    for(int i = 0; i< len; i+=4){
        //std::cerr<<"y = "<<(i/4)/s.x<<", x = "<<(i/4)%s.x<<'\n';
        if(pixels[i+3] != 0){
            if(turinmorph[(i/4)/s.x][(i/4)%s.x]){
                pixels[i]=c1.r;
                pixels[i+1]=c1.b;
                pixels[i+2]=c1.g;
                pixels[i+3]=c1.a;
            }else {
                pixels[i]=c2.r;
                pixels[i+1]=c2.b;
                pixels[i+2]=c2.g;
                pixels[i+3]=c2.a;
            }

        }
    }

    return pixels;
}

void Fish::sexualReproducction(Fish * f, std::set<Fish*> &fish){
    sf::Color c1 = (rand()%2 == 0?this->c1:f->c1), c2 = (rand()%2 == 0?this->c2:f->c2);
    if(rand()%100 < 33){
        this->c2 = sf::Color(c2.r + (rand()%17)-8, c2.b + (rand()%17)-8, c2.g + (rand()%17)-8, c2.a);
    }
    if(rand()%100 < 33){
        this->c1 = sf::Color(c1.r + (rand()%17)-8, c1.b + (rand()%17)-8, c1.g + (rand()%17)-8, c1.a);
    }
    Fish *temp = new Fish(
                x,y,rand()%360,
                (rand()%2 == 0?speed:f->speed)+(rand()%100 < 7? rand()%3-1:0),
                (rand()%2 == 0?vision:f->vision)+(rand()%100 < 7? rand()%11-5:0),
                (rand()%2 == 0?minSize:f->minSize)+(rand()%100 < 7? rand()%7-3:0)/10.0,
                (rand()%2 == 0?maxSize:f->maxSize)+(rand()%100 < 7? rand()%7-3:0)/10.0,
                (rand()%2 == 0?maxEnergy:f->maxEnergy)+(rand()%100 < 7? rand()%11-5:0),
                (rand()%2 == 0?liveExpectation:f->liveExpectation)+(rand()%100 < 7? rand()%101-50:0),
                (rand()%2 == 0?reproductive_percentage:f->reproductive_percentage)+(rand()%100 < 7? rand()%7-3:0)/10.0,
                (rand()%2 == 0?startLookingMate:f->startLookingMate)+(rand()%100 < 7? rand()%7-3:0)/10.0,
                (rand()%2 == 0?stopLookingMate:f->stopLookingMate)+(rand()%100 < 7? rand()%7-3:0)/10.0,
                (rand()%2 == 0?innerRadiousX:f->innerRadiousX)+(rand()%100 < 5? rand()%3-1:0),
                (rand()%2 == 0?innerRadiousY:f->innerRadiousY)+(rand()%100 < 5? rand()%3-1:0),
                (rand()%2 == 0?outterRadiousX:f->outterRadiousX)+(rand()%100 < 5? rand()%3-1:0),
                (rand()%2 == 0?outterRadiousY:f->outterRadiousY)+(rand()%100 < 5? rand()%3-1:0),
                (rand()%2 == 0?initialDensity:f->initialDensity)+(rand()%100 < 7? rand()%7-3:0)/10.0,
                (rand()%2 == 0?ratio:f->ratio)+(rand()%100 < 7? rand()%7-3:0)/10.0,
                (c1), (c2), false
                );
    fish.insert(temp);
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
    sprite.setScale(currSize,currSize);
    window.draw(sprite);

}

void Fish::fishFeed(std::vector<std::vector<short> > &food){
    int prevEnergy = energy;
    for(int i = std::max(0.0, y-(50*currSize)/2); i<std::min(food.size() +0.0,y+(50*currSize)/2);i++){
        for(int j = std::max(0.0, x-(50*currSize)/2); j<std::min(food[0].size() +0.0,x+(50*currSize)/2);j++){
            if(energy > maxEnergy)
                return;
            energy += food[i][j]*food[i][j]*30;
            food[i][j] = 0;
            //std::cout<<"feeded\n";
        }
    }
    if(energy > maxEnergy * startLookingMate and (1 - reproductive_percentage)*liveExpectation < life){
        if(!searchingMate)std::cout<<"now reproductive\n";
        searchingMate = true;
    }
    if(energy < maxEnergy * stopLookingMate)
        searchingMate = false;
    if(energy > prevEnergy)
        currSize = std::min(maxSize, currSize + 0.01);
}

void Fish::sharkFeed(std::set<Fish*> &fish){
    int prevEnergy = energy;
    for(auto &f : fish){
        if(abs(x-f->x)*abs(x-f->x) + abs(y-f->y)*abs(y-f->y) <=
                texture.getSize().y*currSize*0.25*texture.getSize().y*currSize ){
            energy = std::min(maxEnergy,f->energy*2 + energy);
            currSize = std::min(maxSize, currSize + 0.05);
            if(energy > maxEnergy * startLookingMate and (1 - reproductive_percentage)*liveExpectation < life)
                searchingMate = true;
            fish.erase(f);
            std::cerr<<"some shark killed a fish\n";
            return;
        }
    }
    if(energy < maxEnergy * stopLookingMate)
        searchingMate = false;
    if(energy > prevEnergy)
        currSize = std::min(maxSize, currSize + 0.01);

}

void Fish::fishSearchFood(std::vector<std::vector<short> > &food){
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

void Fish::sharkSearchFood(std::set<Fish*> &fish){
    int mindis = 2000000000;
    for(auto &f : fish){
        float temprot;
        int dis = distance(x, y, f->x, f->y, temprot);
        if(dis <= vision*vision and dis< mindis and (f->tagged == nullptr or f->tagged == this)){
            mindis = f->energy;
            rot = temprot;
            f->tagged = this;
        }
    }
}

void Fish::searchMate(std::set<Fish*> &fish){
    int mindis = 1000000000;
    for(auto f : fish){
        float temprot;
        int dis = distance(x, y, f->x, f->y, temprot);
        if(f->searchingMate and male != f->male and dis <= vision*vision and dis < mindis ){
            mindis = dis;
            rot = temprot;
            if(mindis < 250){
                //Fish * newfish = new Fish(x,y,0,speed,vision,currSize,maxSize,maxEnergy,liveExpectation,reproductive_percentage,startLookingMate,stopLookingMate);
                //fish.insert(newfish);
                sexualReproducction(f,fish);
                energy = maxEnergy*stopLookingMate;
                searchingMate = false;
                f->energy = f->maxEnergy*f->stopLookingMate;
                f->searchingMate = false;
                std::cout<<"added new fish\n";
            }
        }
    }
}

void Fish::avoid_obstacles(std::set<Fish*> &sharks, std::vector<std::pair<int,int> > &rockspos){
    for(auto &r : rockspos){
        int dis = abs(x-r.first)*abs(x-r.first)+abs(y-r.second)*abs(y-r.second);
        if(dis < 1000){
            rot = customAtan2(r.second-y, r.first - x) + 180;
            return;
        }
    }
    for(auto &f : sharks){
        float temprot;
        int dis = distance(x, y, f->x, f->y, temprot);
        if(dis < vision*vision){
            rot = temprot + 180;
        }
    }
}

void run_fish(sf::RenderWindow &window, int maxX, int maxY, std::set<Fish*> &fish, std::vector<std::vector<short> > &food,
              std::set<Fish*> &sharks, std::vector<std::pair<int,int> > rockspos){
    std::vector<Fish*> deadFish, deadSharks;
    for(auto &f : fish){
        //f->fishSearchFood(food);
        f->avoid_obstacles(sharks, rockspos);
        if(f->searchingMate)
            f->searchMate(fish);
        f->move(window, maxX, maxY);
        f->fishFeed(food);
        //std::cout<<f->energy <<"\n";
        if(f->life<0 or f->energy<0)
            deadFish.push_back(f);
    }
    for(auto &f : sharks){
        std::set<Fish*> fish_empty;
        f->sharkSearchFood(fish);
        f->avoid_obstacles(fish_empty, rockspos);
        if(f->searchingMate)
            f->searchMate(sharks);
        f->move(window, maxX, maxY);
        f->sharkFeed(fish);
        //std::cout<<f->energy <<"\n";
        if(f->life<0 or f->energy<0)
            deadSharks.push_back(f);
    }

    for(auto &f : deadFish){
        fish.erase(f);
        delete f;
    }
    for(auto &f : deadSharks){
        std::cerr<<"is here?\n";
        sharks.erase(f);
        delete f;
        std::cerr<<"nope\n";
    }
}

int Fish::distance_basic(int x1,int y1,int x2,int y2){
    return abs(x1-x2) * abs(x1-x2) + abs(y1-y2) * abs(y1-y2);
}

int Fish::distance(int x1,int y1,int x2,int y2, float &rot){
    int dis = distance_basic(x1,y1,x2,y2);
    rot = customAtan2(y2-y1,x2-x1);

    if(distance_basic(x1,y1,x2+1366,y2+768) < dis){
        dis = distance_basic(x1,y1,x2+1366,y2+768);
        rot = customAtan2(y2+768-y1,x2+1366-x1);
    }

    if(distance_basic(x1,y1,x2+1366,y2) < dis){
        dis = distance_basic(x1,y1,x2+1366,y2);
        rot = customAtan2(y2-y1,x2+1366-x1);
    }

    if(distance_basic(x1,y1,x2,y2+768) < dis){
        dis = distance_basic(x1,y1,x2,y2+768);
        rot = customAtan2(y2+768-y1,x2-x1);
    }

    x2 -= 1366;
    y2 -= 768;


    if(distance_basic(x1,y1,x2,y2)< dis){
        dis = distance_basic(x1,y1,x2,y2);
            rot = customAtan2(y2-y1,x2-x1);
    }
    if(distance_basic(x1,y1,x2+1366,y2+768) < dis){
        dis = distance_basic(x1,y1,x2+1366,y2+768);
        rot = customAtan2(y2+768-y1,x2+1366-x1);
    }

    if(distance_basic(x1,y1,x2+1366,y2) < dis){
        dis = distance_basic(x1,y1,x2+1366,y2);
        rot = customAtan2(y2-y1,x2+1366-x1);
    }

    if(distance_basic(x1,y1,x2,y2+768) < dis){
        dis = distance_basic(x1,y1,x2,y2+768);
        rot = customAtan2(y2+768-y1,x2-x1);
    }

    return dis;
}
