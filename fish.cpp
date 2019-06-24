#include "fish.h"
const bool MOVE = true;
Fish::Fish(int x, int y, double rot, double speed, int vision, double minSize, double maxSize,
           int maxEnergy, int liveExpectation, double reproductive_percentage, double startLookingMate,
           double stopLookingMate, int innerRadiousX, int innerRadiousY, int outterRadiousX, int outterRadiousY,
           int initialDensity, double ratio, sf::Color c1, sf::Color c2):
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
    c1(c1), c2(c2)
    {
    if (!texture.loadFromFile("fishfixed.png")){
            std::cerr << "Error while loading texture" << std::endl;
            return;
    }
    if(rand()%100 < 5){
        c1 = sf::Color(c1.r + (rand()%3)-1, c1.b + (rand()%3)-1, c1.g + (rand()%3)-1, c1.a);
        c2 = sf::Color(c2.r + (rand()%3)-1, c2.b + (rand()%3)-1, c2.g + (rand()%3)-1, c2.a);
    }
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
        /*for(int yy = 0; yy< s.y; yy++){
            for(int xx = 0; xx < s.x; xx++)
                std::cerr<<turinmorph[yy][xx];
            std::cerr<<'\n';
        }
        std::cerr<<"----------------------\n";
        */
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
                (rand()%2 == 0?c1:f->c1),
                (rand()%2 == 0?c2:f->c2)
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

void Fish::feed(std::vector<std::vector<short> > &food){
    int prevEnergy = energy;
    for(int i = std::max(0.0, y-(50*currSize)/2); i<std::min(food.size() +0.0,y+(50*currSize)/2);i++){
        for(int j = std::max(0.0, x-(50*currSize)/2); j<std::min(food[0].size() +0.0,x+(50*currSize)/2);j++){
            if(energy > maxEnergy)
                return;
            energy += food[i][j]*food[i][j]*50;
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
    if(energy > prevEnergy)
        currSize = std::min(maxSize, currSize + 0.01);
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
