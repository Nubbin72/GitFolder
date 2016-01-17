#include <SFML/Graphics.hpp>
#include <iostream>
#include "PathFinding.hpp"


int main()
{
    const int screen_width{ 800 };
    const int screen_height{ 600 };;

    float grid_columns = screen_width / WORLD_SIZE;
    float grid_rows = screen_height / WORLD_SIZE;
    int position = 0;

    sf::Vector2i startPosition(100, 150);
    sf::Vector2i goalPosition(600, 400);

    unsigned int tile = 0;
    bool pathLoaded = false;
    bool isDirectionSet = false;
    sf::Vector2f moveDirection;

    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), " A* Pathfinding");
    window.setFramerateLimit(60);


    //Creating the grid
    std::vector<sf::VertexArray> grid;
    //Horizontal lines
    for(int i = 0; i < grid_rows; ++i)
    {
        sf::VertexArray line(sf::Lines, 2);
        line[0].position = sf::Vector2f(0,position);
        line[1].position = sf::Vector2f(screen_width, position);

        line[0].color = sf::Color::Black;
        line[1].color = sf::Color::Black;
        grid.push_back(line);
        position += WORLD_SIZE;
    }
    //Vertical lines
    position = 0;
    for(int i = 0; i < grid_columns; ++i)
    {
        sf::VertexArray line(sf::Lines, 2);
        line[0].position = sf::Vector2f(position, 0);
        line[1].position = sf::Vector2f(position, screen_height);

        line[0].color = sf::Color::Black;
        line[1].color = sf::Color::Black;
        grid.push_back(line);
        position += WORLD_SIZE;
    }

    //Creating the blocks
    std::vector<sf::RectangleShape> blocks;
    sf::Vector2f blockPosition(0,0);
    for(int i = 0; i < grid_rows; ++i)
    {
        for(int j = 0; j < grid_columns; ++j)
        {
            sf::RectangleShape shape;
            shape.setSize(sf::Vector2f(WORLD_SIZE, WORLD_SIZE));
            shape.setPosition(blockPosition.x, blockPosition.y);
            blockPosition.x += WORLD_SIZE;
            //Fill the blocks with white color, if it is an edge block, fill it with red
            shape.setFillColor(sf::Color::White);
            if((shape.getPosition().x == 0 && shape.getPosition().y <= screen_height) || //Left
                   (shape.getPosition().x <= screen_width && shape.getPosition().y == 0) || //Top
                   (shape.getPosition().x == screen_width - (screen_width % WORLD_SIZE) - WORLD_SIZE  && shape.getPosition().y <= screen_height) || //Right
                   (shape.getPosition().x <= screen_width  && shape.getPosition().y >= screen_height - WORLD_SIZE)) // bottom
                    shape.setFillColor(sf::Color::Red);

            blocks.push_back(shape);
        }
        blockPosition.x = 0;
        blockPosition.y += WORLD_SIZE;
    }


    //Initialize Start block
    sf::RectangleShape start;
    start.setFillColor(sf::Color::Green);
    start.setSize(sf::Vector2f(WORLD_SIZE,WORLD_SIZE));
    start.setPosition(startPosition.x - (startPosition.x % WORLD_SIZE),
                      startPosition.y - (startPosition.y % WORLD_SIZE));

    //Initialize Goal block
    sf::RectangleShape goal;
    goal.setFillColor(sf::Color::Blue);
    goal.setSize(sf::Vector2f(WORLD_SIZE,WORLD_SIZE));
    goal.setPosition(goalPosition.x - (goalPosition.x % WORLD_SIZE),
                      goalPosition.y - (goalPosition.y % WORLD_SIZE));


    //PathFinder object and vector of path positions
    PathFinding m_pathFinder;
    std::vector<sf::Vector2f> pathToGoal;

    while (window.isOpen())
    {
        //If player closes the screen, close the window
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        //Left mouse button to create a wall
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            for(auto& itr : blocks)
                if(itr.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(window)))
                    itr.setFillColor(sf::Color::Red);
        }
        //Right mouse button to destroy a wall
        else if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            for(auto& itr : blocks)
                if(itr.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(window)))
                    itr.setFillColor(sf::Color::White);
        }
        //If press Enter, search for the path
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
        {
            m_pathFinder.setUnwalkable(blocks);
            while(m_pathFinder.m_searching)
            {
                m_pathFinder.findPath((sf::Vector2i)start.getPosition() / WORLD_SIZE, (sf::Vector2i)goal.getPosition() / WORLD_SIZE);
            }
        }
        //Reset the level and make a border around the screen by pressing the letter R
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        {
            if(!m_pathFinder.m_searching)
                std::cout << "level reset" << std::endl;

            for(auto& itr : blocks)
            {
                //Reset only the tiles with path on it
                if(itr.getFillColor() == sf::Color(80,80,80,80))
                   itr.setFillColor(sf::Color::White);
            }

            pathToGoal.clear();
            isDirectionSet = false;
            pathLoaded = false;
            tile = 0;
            moveDirection = sf::Vector2f(0,0);
            m_pathFinder.clearOpenList();
            m_pathFinder.clearClosedList();
            m_pathFinder.clearPathToGoalList();
            m_pathFinder.clearUnwalkableList();
            m_pathFinder.m_initStartAndGoal = false;
            m_pathFinder.m_foundGoal = false;
            m_pathFinder.m_searching = true;

            //If we reset and the player is between two tiles, place within the nearest block
            start.setPosition(start.getPosition().x - ((int)start.getPosition().x % WORLD_SIZE),
                              start.getPosition().y - ((int)start.getPosition().y % WORLD_SIZE));


        }
        //Move start by pressing the letter S
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            start.setPosition(sf::Vector2f(sf::Mouse::getPosition(window).x - (sf::Mouse::getPosition(window).x % WORLD_SIZE),
                                          sf::Mouse::getPosition(window).y - (sf::Mouse::getPosition(window).y % WORLD_SIZE)));
        }
        //Move goal by pressing the letter G
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::G))
        {
            goal.setPosition(sf::Vector2f(sf::Mouse::getPosition(window).x - (sf::Mouse::getPosition(window).x % WORLD_SIZE),
                                          sf::Mouse::getPosition(window).y - (sf::Mouse::getPosition(window).y % WORLD_SIZE)));
        }

        if(m_pathFinder.m_foundGoal && !pathLoaded)
        {
            m_pathFinder.getPath(pathToGoal);
            for(auto& itr : blocks)
                for(unsigned int i = 0; i < pathToGoal.size(); ++i)
                    if(itr.getGlobalBounds().contains(sf::Vector2f(pathToGoal[i].x * WORLD_SIZE, pathToGoal[i].y * WORLD_SIZE)))
                        itr.setFillColor(sf::Color(80,80,80, 80));
            pathLoaded = true;

        }
        if(m_pathFinder.m_foundGoal && start.getPosition() != goal.getPosition() && !pathToGoal.empty())
        {

            if(!isDirectionSet)
            {
                moveDirection = sf::Vector2f(pathToGoal[tile].x * WORLD_SIZE - start.getPosition().x,
                                                      pathToGoal[tile].y * WORLD_SIZE - start.getPosition().y);
                isDirectionSet = true;

            }
            start.move(moveDirection.x / 10, moveDirection.y / 10 );
            if(start.getPosition().x / WORLD_SIZE == pathToGoal[tile].x && start.getPosition().y / WORLD_SIZE == pathToGoal[tile].y)
            {
                tile++;

                if(tile >= pathToGoal.size())
                    tile--;

                moveDirection = sf::Vector2f(pathToGoal[tile].x * WORLD_SIZE - start.getPosition().x,
                                                      pathToGoal[tile].y * WORLD_SIZE - start.getPosition().y);
            }
        }

        //Fill the screen with white color
        window.clear(sf::Color::White);
        //Draw blocks
        for(const auto itr : blocks)
            window.draw(itr);

        window.draw(goal);
        window.draw(start);

        //Draw grid
        for(const auto itr : grid)
            window.draw(itr);

        //Flip the buffers
        window.display();
    }

    return 0;
}
