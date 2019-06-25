#include <SFML/Graphics.hpp>
#include<bits/stdc++.h>
#include<sandpile.h>
#include<LSystems.h>
#include<fish.h>
const int TREES = 6, FISH = 30, SHARKS = 5, ROCKS = 5, GRAINS = 10, INITIALGRAIN = 300;

float rot = 0;
sf::Sprite rock;
sf::Texture texture;

void initrock(){
    if (!texture.loadFromFile("rockfixed.png")){
            std::cerr << "Error while loading texture" << std::endl;
            return;
    }
    rock.setTexture(texture);
    sf::FloatRect spriteSize=rock.getGlobalBounds();
    rock.setOrigin(spriteSize.width/2,spriteSize.height/2);
}

void drawRocks(sf::RenderWindow &window, std::vector<std::pair<int,int> > &rockspos){
    //std::cerr<<rock.getRotation()<<'\n';
    rock.setRotation(++rot);
    for(auto pos : rockspos){
        rock.setPosition(pos.first, pos.second);
        window.draw(rock);
    }
}

int main()
{
    //basic SFML init
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    sf::RenderWindow window(sf::VideoMode(  desktopMode.width,
                                            desktopMode.height,
                                            desktopMode.bitsPerPixel),
                            "Simulación vida artificial",
                            sf::Style::Fullscreen);
    window.setVerticalSyncEnabled (true);
    window.setKeyRepeatEnabled(true);
    initrock();

    //needed for the program
    const int w = desktopMode.width, h = desktopMode.height;
    std::vector<std::vector<short> > food(h, std::vector<short> (w, 0));
    std::vector<std::pair<int,int> > rockspos;
    std::vector<std::pair<int,int> > treespos;
    for(int i = 0; i<TREES; i++)
        treespos.push_back({rand() % w , rand() % (h)});
    for(int i = 0; i<ROCKS; i++)
        rockspos.push_back({rand() % w , rand() % (h)});

    std::set<Fish*> fish;
    sf::Color c1(255,0,2,255), c2(2,255,255,255), c3(100,100,100,255), c4(200,200,200,255);
    for(int i = 0; i<FISH; i++){
        Fish* temp =  new Fish(rand() % w, rand() % h, 0, 5, 200, 0.5, 1.1, 1000, 600, 0.1, 0.8, 0.5,4,4,6,6,50,1.0, c1,c2,0);
        fish.insert(temp);
    }
    std::set<Fish*> sharks;
    for(int i = 0; i<SHARKS; i++){
        Fish* temp =  new Fish(rand() % w, rand() % h, 0, 8, 300, 0.3, 0.8, 400, 1800, 0.4, 0.8, 0.5,4,4,6,6,50,1.0, c3,c4,1);
        sharks.insert(temp);
    }

    addRandGrains(food,window, INITIALGRAIN);

    //main loop
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }
        window.clear(sf::Color::Transparent);
        addRandGrains(food, window, GRAINS); // add food
        addtrees(treespos, window); //l-systems
        drawRocks(window, rockspos);
        run_fish(window, desktopMode.width, desktopMode.height, fish, food, sharks, rockspos);

        window.display();
    }
    return 0;
}
