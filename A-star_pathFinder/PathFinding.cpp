#include "PathFinding.hpp"

PathFinding::PathFinding() : m_initStartAndGoal(false), m_foundGoal(false), m_searching(true)
{
}

void PathFinding::getPath(std::vector<sf::Vector2f>& l_path)
{
    for(int i = m_pathToGoal.size()-1; i > 0; --i)
    {
        sf::Vector2f position;
        position.x = m_pathToGoal[i]->x;
        position.y = m_pathToGoal[i]->y;
        l_path.push_back(position);
    }
}

void PathFinding::findPath(sf::Vector2i l_currentPosition, sf::Vector2i l_goalPosition)
{
    for(const auto itr : m_unwalkable)
    {
        if(l_goalPosition == itr)
        {
            std::cout << "Target is not a walkable tile" << std::endl;
            clearOpenList();
            clearClosedList();
            clearPathToGoalList();
            clearUnwalkableList();
            m_initStartAndGoal = false;
            m_foundGoal = false;
            m_searching = true;

        }
    }

    if(!m_initStartAndGoal)
    {
        //Delete all elements from open vector
        for(const auto itr : m_openList)
            delete itr;
        m_openList.clear();

        //Delete all elements from closed vector
        for(const auto itr : m_closedList)
            delete itr;
        m_closedList.clear();

        //Delete all elements from path to goal vector
        for(const auto itr : m_pathToGoal)
            delete itr;
        m_pathToGoal.clear();

        //Initialize start and goal
        node start;
        start.m_tile = l_currentPosition;

        node goal;
        goal.m_tile = l_goalPosition;

        setStartAndGoal(start, goal);
        m_initStartAndGoal = true;
        m_foundGoal = false;
        m_searching = true;
        std::cout << "goal set" << std::endl;
    }


    if(m_initStartAndGoal)
    {
        continiuePath();
    }

}


void PathFinding::clearOpenList()
{
    for(const auto itr : m_openList)
            delete itr;
        m_openList.clear();
}

void PathFinding::clearClosedList()
{
    for(const auto itr : m_closedList)
            delete itr;
        m_closedList.clear();
}

void PathFinding::clearPathToGoalList()
{
    for(const auto itr : m_pathToGoal)
            delete itr;
        m_pathToGoal.clear();
}

void PathFinding::clearUnwalkableList()
{
    m_unwalkable.clear();
}

void PathFinding::setUnwalkable(std::vector<sf::RectangleShape> l_unwalkable)
{
    for(unsigned int i = 0; i < l_unwalkable.size(); ++i)
    {
        if(l_unwalkable[i].getFillColor() == sf::Color::Red)
        m_unwalkable.push_back(sf::Vector2i(l_unwalkable[i].getPosition().x / WORLD_SIZE, l_unwalkable[i].getPosition().y / WORLD_SIZE));
    }
}

void PathFinding::setStartAndGoal(node l_start, node l_goal)
{
    m_startNode = new node(l_start.m_tile, nullptr);
    m_goalNode = new node(l_goal.m_tile, &l_goal);

    m_startNode->G = 0;
    m_startNode->H = m_startNode->manhattanDistance(m_goalNode);
    m_startNode->m_parent = nullptr;

    m_openList.push_back(m_startNode);
}

void PathFinding::pathOpened(sf::Vector2i l_position, float l_newCost, node* l_parent)
{
    for(unsigned int i = 0; i < m_unwalkable.size(); ++i)
        if(l_position == m_unwalkable[i])
            return;

    int id = l_position.y * WORLD_SIZE + l_position.x;
    for(unsigned int i = 0; i < m_closedList.size(); ++i)
        if(id == m_closedList[i]->m_id)
            return;

    node* newChild = new node(l_position, l_parent);
    newChild->G = l_newCost;
    newChild->H = l_parent->manhattanDistance(m_goalNode);

    for(unsigned int i = 0; i < m_openList.size(); ++i)
    {
        if(id == m_openList[i]->m_id)
        {
            float newF = newChild->G + l_newCost + m_openList[i]->H;

            if(m_openList[i]->getF() > newF)
            {
                m_openList[i]->G = newChild->G + l_newCost;
                m_openList[i]->m_parent = newChild;
            }
            else
            {
                delete newChild;
                return;
            }
        }
    }
    m_openList.push_back(newChild);

}

node* PathFinding::getNextCell()
{
    float bestF = 9999;
    int cellIndex = -1;
    node* nextCell = nullptr;

    for(unsigned int i=0; i < m_openList.size(); ++i)
    {
        if(m_openList[i]->getF() < bestF)
        {
            bestF = m_openList[i]->getF();
            cellIndex = i;
        }
    }

    if(cellIndex >= 0)
    {
        nextCell = m_openList[cellIndex];
        m_closedList.push_back(nextCell);
        m_openList.erase(m_openList.begin() + cellIndex);
    }
    return nextCell;
}

void PathFinding::continiuePath()
{
    //8 sides to check
    for(int i = 0; i < 8; ++i)
    {
        if(m_openList.empty())
            return;
        node* currentCell = getNextCell();
        if(currentCell->m_id == m_goalNode->m_id)
        {
            m_goalNode->m_parent = currentCell->m_parent;

            node* getPath;
            for(getPath = m_goalNode; getPath != nullptr; getPath = getPath->m_parent)
                m_pathToGoal.push_back(new sf::Vector2i(getPath->m_tile));

            m_foundGoal = true;
            m_searching = false;
            std::cout << "Path found" << std::endl;
            return;
        }
        else
        {

            //Right cell
            pathOpened(sf::Vector2i(currentCell->m_tile.x + 1, currentCell->m_tile.y), currentCell->G +1, currentCell);
            //Left cell
            pathOpened(sf::Vector2i(currentCell->m_tile.x - 1, currentCell->m_tile.y), currentCell->G +1, currentCell);
            //Top cell
            pathOpened(sf::Vector2i(currentCell->m_tile.x, currentCell->m_tile.y + 1), currentCell->G +1, currentCell);
            //Bottom cell
            pathOpened(sf::Vector2i(currentCell->m_tile.x, currentCell->m_tile.y - 1), currentCell->G +1, currentCell);

            for(unsigned int i = 0; i < m_openList.size(); ++i)
            {
                if(currentCell->m_id == m_openList[i]->m_id)
                {
                    m_openList.erase(m_openList.begin() + i);
                }
            }
            if(m_openList.size() == 0 && !m_foundGoal)
            {
                std::cout << "Can't find a path" << std::endl;
                m_foundGoal = false;
                m_searching = false;
                return;
            }

        }
    }
}














