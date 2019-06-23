#include <SFML/Graphics.hpp>
#include<bits/stdc++.h>
#include<sandpile.h>
#include<LSystems.h>
#include<fish.h>
const int TREES = 3, FISH = 1, SHARKS = 1;

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

    std::vector<std::pair<int,int> > treespos;
    for(int i = 0; i<TREES; i++)
        treespos.push_back({rand() % w , rand() % (h)});

    std::set<Fish*> fish;
    for(int i = 0; i<FISH; i++){
        Fish* temp =  new Fish(rand() % w, rand() % h,0, 5, 10, 0.5, 1, 100, 100, 0.4);
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
        addRandGrains(food, window, 10); // add food
        addtrees(treespos, window); //l-systems
        run_fish(window, desktopMode.width, desktopMode.height, fish);


        window.display();
    }
    return 0;
}
