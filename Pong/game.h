#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>

enum states
{
	MENU,
	OPTIONS,
	PLAY,
	AI,
};

class game
{
public:
	game();
	void run();
private:
	void handleEvents();
	void update(sf::Time deltaTime);
	void draw();
	void resetGame(sf::RectangleShape& player, sf::CircleShape& ball);

	static const sf::Time timePerFrame;
	const int screenWidth{ 800 };
	const int screenHeight{ 500 };
	const float batWidth{ 30.f };
	const float batHeight{ 200. };
	const float ballRadius{ 12.f };
	float ballSpeed{ 300 };
	const float paddleSpeed{ 300.f };
	const float botPaddleSpeed{ 250.f };

	bool mouseButtonReleased;

	int leftMove{ 0 };
	int rightMove{ 0 };

	sf::Vector2f leftBatVel;
	sf::Vector2f rightBatVel;

	bool m_paused;
	states currentState = MENU;
	sf::RectangleShape pauseBackground;

	sf::RenderWindow window;

	//Main menu
	sf::Text title;
	sf::Text buttonPlay;
	sf::Text buttonPlayAI;
	sf::Text buttonOptions;
	sf::Text buttonExit;
	//in game
	sf::Text pauseTitle;
	sf::Text pauseMenu;
	sf::Text pauseExit;

	//in options
	sf::Text optionsTitle;
	sf::Text buttonBack;
	sf::Text buttonVolume;
	sf::Text buttonFullscreen;
	sf::RectangleShape sliderBar;
	sf::RectangleShape slider;
	bool selected;
	sf::View fullscreenView;

	int scoreLeft{ 0 };
	int scoreRight{ 0 };
	char scoreChar[20];

    float volume;

	sf::Font font;
	sf::Text scoreBoard;
	sf::Text ballSpeedText;

	sf::Vector2f ballVelocity;

	sf::CircleShape ball;
	sf::RectangleShape leftBat;
	sf::RectangleShape rightBat;
	sf::CircleShape predBall;
	sf::Vector2f predBallVel;

	sf::SoundBuffer menuSoundBuff;
	sf::SoundBuffer paddleSoundBuff;
	sf::SoundBuffer wallSoundBuff;
	sf::SoundBuffer scoreSoundBuff;

	sf::Sound menuSound;
	sf::Sound paddleCollision;
	sf::Sound wallCollision;
	sf::Sound scoreCollision;

	bool soundPlayed;
};
