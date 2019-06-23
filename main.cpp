#include <SFML/Graphics.hpp>
#include<bits/stdc++.h>
#include<sandpile.h>
#include<LSystems.h>
#include<fish.h>
const int TREES = 3, FISH = 1, SHARKS = 1, ROCKS = 5, GRAINS = 1;

sf::Sprite rock;
sf::Texture texture;

void initrock(){
    if (!texture.loadFromFile("rockfixed.png")){
            std::cerr << "Error while loading texture" << std::endl;
            return;
    }
    rock.setTexture(texture);
    sf::FloatRect spriteSize=rock.getGlobalBounds();
    rock.setOrigin(spriteSize.width/2.,spriteSize.height/2.);
}

void drawRocks(sf::RenderWindow &window, std::vector<std::pair<int,int> > &rockspos){
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

    //needed for the program
    const int w = desktopMode.width, h = desktopMode.height;
    std::vector<std::vector<short> > food(h, std::vector<short> (w, 0));
    std::vector<std::pair<int,int> > rockspos;
    std::vector<std::pair<int,int> > treespos;
    initrock();
    for(int i = 0; i<TREES; i++)
        treespos.push_back({rand() % w , rand() % (h)});
    for(int i = 0; i<ROCKS; i++)
        rockspos.push_back({rand() % w , rand() % (h)});

    std::set<Fish*> fish;
    for(int i = 0; i<FISH; i++){
        Fish* temp =  new Fish(rand() % w, rand() % h,0, 5, 10, 0.5, 1, 1000, 10000, 0.4);
        fish.insert(temp);
    }


    //main loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }
        window.clear(sf::Color::Transparent);
        addRandGrains(food, window, GRAINS); // add food
        addtrees(treespos, window); //l-systems
        run_fish(window, desktopMode.width, desktopMode.height, fish, food);
        drawRocks(window, rockspos);

        window.display();
    }
    return 0;
}
