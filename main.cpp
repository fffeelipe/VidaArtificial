#include <SFML/Graphics.hpp>
#include<bits/stdc++.h>
#include<sandpile.cpp>
#include<LSystems.h>

const int SPRITE_SPEED = 5;
const int TREES = 10;

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

    std::vector<std::vector<short> > food(desktopMode.height, std::vector<short> (desktopMode.width, 0));

    std::vector<std::pair<int,int> > treespos;
    for(int i = 0; i<TREES; i++)
        treespos.push_back({rand() % desktopMode.width , rand() % (desktopMode.height)});

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }
        window.clear(sf::Color::Transparent);
        addRandGrains(food, window, 10); // add food
        addtrees(treespos, window);
        window.display();
    }
    return 0;
}
