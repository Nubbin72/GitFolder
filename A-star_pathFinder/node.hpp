#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#define WORLD_SIZE 30


//Consider node as one square at a map for now ( node can be any shape or a place on a square )
class node
{
    public:
        sf::Vector2i m_tile;
        node* m_parent;
        int m_id;

        float G;
        float H;

        node() : m_parent(nullptr){}
        node(sf::Vector2i l_tile, node* l_parent = nullptr) :
                m_tile(l_tile),
                m_parent(l_parent),
                m_id(l_tile.y * WORLD_SIZE + l_tile.x),
                G(0),
                H(0) {}

        float getF() {return G + H; }

        //The absolute distance from start node to the end node
        float manhattanDistance(node* nodeEnd)
        {
            float x = (float)fabsf(this->m_tile.x - nodeEnd->m_tile.x);
            float y = (float)fabsf(this->m_tile.y - nodeEnd->m_tile.y);
            return x + y;
        }


};
