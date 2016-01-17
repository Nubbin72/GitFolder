#pragma once
#include "node.hpp"
#include <vector>
#include <iostream>


class PathFinding
{
    public:
        PathFinding();
        void findPath(sf::Vector2i l_currentPosition, sf::Vector2i l_goalPosition);

        void clearOpenList();
        void clearClosedList();
        void clearPathToGoalList();
        void clearUnwalkableList();

        bool m_initStartAndGoal;
        bool m_foundGoal;
        bool m_searching;

        void setUnwalkable(std::vector<sf::RectangleShape> l_unwalkable);

        void getPath(std::vector<sf::Vector2f>& l_path);
    private:
        void setStartAndGoal(node l_start, node l_goal);
        void pathOpened(sf::Vector2i l_position, float l_newCost, node* l_parent);
        node* getNextCell();
        void continiuePath();

        node* m_startNode;
        node* m_goalNode;

        std::vector<node*> m_openList;
        std::vector<node*> m_closedList;

        //In this vector we will store our path
        std::vector<sf::Vector2i*> m_pathToGoal;
        //Tiles that are walls or other not walkable places
        std::vector<sf::Vector2i> m_unwalkable;



};
