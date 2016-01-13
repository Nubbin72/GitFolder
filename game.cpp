#include "game.h"

const sf::Time game::timePerFrame = sf::seconds(1.f / 60.f);

game::game() : ball(ballRadius), ballVelocity(sf::Vector2f(ballSpeed, 0.f)), leftBat(sf::Vector2f(30.f, 200.f)), rightBat(sf::Vector2f(30.f, 200.f)),
                m_paused(false), soundPlayed(false), predBall(ballRadius), predBallVel(sf::Vector2f(ballSpeed, 0.f)), selected(false)
{

	window.create(sf::VideoMode(screenWidth, screenHeight), "Pong");
    volume = 100;

	sprintf(scoreChar, "%d : %d", scoreLeft, scoreRight);

	font.loadFromFile("data/arial.ttf");

	scoreBoard.setFont(font);
	scoreBoard.setColor(sf::Color::White);
	scoreBoard.setString(static_cast<std::string>(scoreChar));
	scoreBoard.setPosition(screenWidth / 2.f - scoreBoard.getGlobalBounds().width / 2.f, 10.f);

	ballSpeedText.setFont(font);
	ballSpeedText.setString("Speed: " + std::to_string(static_cast<int>(ballSpeed / 10)));
	ballSpeedText.setColor(sf::Color::White);
	ballSpeedText.setCharacterSize(20);
	ballSpeedText.setPosition(screenWidth / 2.f - ballSpeedText.getGlobalBounds().width / 2.f, screenHeight - 50.f);


	ball.setFillColor(sf::Color::Red);
	leftBat.setFillColor(sf::Color::White);
	rightBat.setFillColor(sf::Color::White);
	predBall.setFillColor(sf::Color::Blue);

	ball.setPosition(sf::Vector2f(screenWidth / 2.f, screenHeight / 2.f));
	leftBat.setPosition(sf::Vector2f(0.f, screenHeight / 2.f - batHeight / 2.f + 2));
	rightBat.setPosition(sf::Vector2f(screenWidth - batWidth, screenHeight / 2.f - batHeight / 2.f + 2));
	predBall.setPosition(sf::Vector2f(screenWidth - 20.f, screenHeight / 2.f));

	title.setFont(font);
	title.setString("Pong");
	title.setColor(sf::Color::White);
	title.setCharacterSize(50);
	title.setPosition(screenWidth / 2.f - (title.getGlobalBounds().width / 2.f), 50);
	buttonPlay.setFont(font);
	buttonPlay.setString("1 vs 1");
	buttonPlay.setColor(sf::Color::White);
	buttonPlay.setPosition(screenWidth / 2.f - (buttonPlay.getGlobalBounds().width / 2.f), 180.f);

	buttonPlayAI.setFont(font);
	buttonPlayAI.setString("vs AI");
	buttonPlayAI.setColor(sf::Color::White);
	buttonPlayAI.setPosition(screenWidth / 2.f - (buttonPlayAI.getGlobalBounds().width / 2.f), 240.f);

	buttonOptions.setFont(font);
	buttonOptions.setString("OPTIONS");
	buttonOptions.setColor(sf::Color::White);
	buttonOptions.setPosition(screenWidth / 2.f - (buttonOptions.getGlobalBounds().width / 2.f), 300.f);

	buttonExit.setFont(font);
	buttonExit.setString("EXIT");
	buttonExit.setColor(sf::Color::White);
	buttonExit.setPosition(screenWidth / 2.f - (buttonExit.getGlobalBounds().width / 2.f), 360.f);


	pauseBackground.setFillColor(sf::Color::Black);
	pauseBackground.setOutlineColor(sf::Color::Red);
	pauseBackground.setOutlineThickness(2.f);
	pauseBackground.setSize(sf::Vector2f(300.f, 150.f));
	pauseBackground.setPosition(screenWidth / 2.f - pauseBackground.getGlobalBounds().width / 2.f, screenHeight / 2.f - pauseBackground.getGlobalBounds().height / 2.f);

	pauseTitle.setFont(font);
	pauseTitle.setString("Pause");
	pauseTitle.setColor(sf::Color::White);
	pauseTitle.setPosition(screenWidth / 2.f - (pauseTitle.getGlobalBounds().width / 2.f), 180.f);

	pauseMenu.setFont(font);
	pauseMenu.setString("MENU");
	pauseMenu.setColor(sf::Color::White);
	pauseMenu.setCharacterSize(20);
	pauseMenu.setPosition(screenWidth / 2.f - (pauseMenu.getGlobalBounds().width / 2), 240.f);

	pauseExit.setFont(font);
	pauseExit.setString("EXIT");
	pauseExit.setColor(sf::Color::White);
	pauseExit.setCharacterSize(20);
	pauseExit.setPosition(screenWidth / 2.f - (pauseExit.getGlobalBounds().width / 2.f), 280.f);

	optionsTitle.setFont(font);
	optionsTitle.setString("OPTIONS");
	optionsTitle.setColor(sf::Color::White);
	optionsTitle.setCharacterSize(40);
	optionsTitle.setPosition(screenWidth / 2.f - optionsTitle.getGlobalBounds().width / 2.f, 50);

	buttonVolume.setFont(font);
	buttonVolume.setString("Volume: ");
	buttonVolume.setColor(sf::Color::White);
	buttonVolume.setCharacterSize(20);
	buttonVolume.setPosition(screenWidth / 2.f - 200.f, 150.f);

	buttonFullscreen.setFont(font);
	buttonFullscreen.setString("Fullscreen: OFF");
	buttonFullscreen.setColor(sf::Color::White);
	buttonFullscreen.setCharacterSize(20);
	buttonFullscreen.setPosition(screenWidth / 2.f - 200.f, 200.f);

	buttonBack.setFont(font);
	buttonBack.setString("Back");
	buttonBack.setColor(sf::Color::White);
	buttonBack.setPosition(30.f, screenHeight - buttonBack.getGlobalBounds().height - 40.f);

	sliderBar.setSize(sf::Vector2f(150.f, 10.f));
	sliderBar.setOrigin(0.f, 5.f);
	sliderBar.setFillColor(sf::Color::White);
	sliderBar.setPosition(screenWidth / 2.f - 190 + buttonVolume.getGlobalBounds().width, buttonVolume.getPosition().y + buttonVolume.getGlobalBounds().height);

	slider.setSize(sf::Vector2f(10.f, 30.f));
	slider.setOrigin(5.f, 15.f);
	slider.setFillColor(sf::Color::White);
	slider.setOutlineColor(sf::Color::Red);
	slider.setOutlineThickness(2);
	slider.setPosition(sliderBar.getPosition().x + sliderBar.getGlobalBounds().width , sliderBar.getPosition().y);

	menuSoundBuff.loadFromFile("data/mouseOver.wav");
	paddleSoundBuff.loadFromFile("data/collisionPaddle.wav");
	wallSoundBuff.loadFromFile("data/collisionWall.wav");
	scoreSoundBuff.loadFromFile("data/collisionScore.wav");

	menuSound.setBuffer(menuSoundBuff);
	paddleCollision.setBuffer(paddleSoundBuff);
	wallCollision.setBuffer(wallSoundBuff);
	scoreCollision.setBuffer(scoreSoundBuff);

	menuSound.setVolume(volume);
	paddleCollision.setVolume(volume);
	wallCollision.setVolume(volume);
	scoreCollision.setVolume(volume);
}

void game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (window.isOpen())
	{
		handleEvents();
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			handleEvents();
			timeSinceLastUpdate -= timePerFrame;
			if (!m_paused)
				update(timePerFrame);
		}
		draw();
	}

}

void game::handleEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (currentState == MENU)
		{
			//Menu play 1v1
			if (buttonPlay.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
			{
				if (!soundPlayed)
				{
					menuSound.play();
					soundPlayed = true;
				}
				buttonPlay.setColor(sf::Color::Red);
				if (event.type == sf::Event::MouseButtonReleased)
					if (event.mouseButton.button == sf::Mouse::Left)
						currentState = PLAY;
			}
			else
			{
				buttonPlay.setColor(sf::Color::White);
				if (!buttonExit.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)) &&
					!buttonPlayAI.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)) &&
					!buttonOptions.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
				{
					soundPlayed = false;
				}

			}
			//Menu play vs AI
			if (buttonPlayAI.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
			{
				if (!soundPlayed)
				{
					menuSound.play();
					soundPlayed = true;
				}
				buttonPlayAI.setColor(sf::Color::Red);
				if (event.type == sf::Event::MouseButtonReleased)
					if (event.mouseButton.button == sf::Mouse::Left)
						currentState = AI;
			}
			else
			{
				buttonPlayAI.setColor(sf::Color::White);
				if (!buttonPlay.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)) &&
					!buttonOptions.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)) &&
					!buttonExit.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
				{
					soundPlayed = false;
				}
			}
			//Menu options
			if (buttonOptions.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
			{
				if (!soundPlayed)
				{
					menuSound.play();
					soundPlayed = true;
				}
				buttonOptions.setColor(sf::Color::Red);
				if (event.type == sf::Event::MouseButtonReleased)
					if (event.mouseButton.button == sf::Mouse::Left)
						currentState = OPTIONS;
			}
			else
			{
				buttonOptions.setColor(sf::Color::White);
				if (!buttonPlay.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)) &&
					!buttonPlayAI.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)) &&
					!buttonExit.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
				{
					soundPlayed = false;
				}
			}
			//Menu exit
			if (buttonExit.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
			{
				if (!soundPlayed)
				{
					menuSound.play();
					soundPlayed = true;
				}
				buttonExit.setColor(sf::Color::Red);
				if (event.type == sf::Event::MouseButtonReleased)
					if (event.mouseButton.button == sf::Mouse::Left)
					window.close();
			}
			else
			{
				buttonExit.setColor(sf::Color::White);
				if (!buttonPlay.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)) &&
					!buttonPlayAI.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)) &&
					!buttonOptions.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
				{
					soundPlayed = false;
				}
			}

		}
		else if (currentState == OPTIONS)
		{
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				currentState = MENU;

			if (buttonBack.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
			{
				if (!soundPlayed)
				{
					menuSound.play();
					soundPlayed = true;
				}
				buttonBack.setColor(sf::Color::Red);
				if (event.type == sf::Event::MouseButtonReleased)
					if (event.mouseButton.button == sf::Mouse::Left)
						currentState = MENU;
			}
			else
			{
				buttonBack.setColor(sf::Color::White);
				if (!slider.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)) &&
					!buttonFullscreen.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
					soundPlayed = false;
			}
			//volume control
			if (slider.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
			{
				slider.setFillColor(sf::Color::Red);
				slider.setOutlineColor(sf::Color::White);
				if (event.type == sf::Event::MouseButtonPressed)
				{
					selected = true;
				}
			}
			else
			{
				if (!selected)
				{
					slider.setFillColor(sf::Color::White);
					slider.setOutlineColor(sf::Color::Red);
				}
			}
			//volume
			if (selected && event.type == sf::Event::MouseButtonReleased)
				if (event.mouseButton.button == sf::Mouse::Left)
				{
				    float tempDistance = slider.getPosition().x - sliderBar.getPosition().x;
				    float tempVolume = tempDistance / sliderBar.getSize().x;
				    volume = tempVolume * 100;
                    menuSound.setVolume(volume);
                    paddleCollision.setVolume(volume);
                    wallCollision.setVolume(volume);
                    scoreCollision.setVolume(volume);

					selected = false;
					menuSound.play();
				}
			//fullscreen
			if (buttonFullscreen.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
			{
				buttonFullscreen.setColor(sf::Color::Red);
				if (event.type == sf::Event::MouseButtonReleased)
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						if (buttonFullscreen.getString() == "Fullscreen: OFF")
						{
							buttonFullscreen.setString("Fullscreen: ON");
							window.create(sf::VideoMode(screenWidth, screenHeight), "Pong", sf::Style::Fullscreen);
						}
						else if (buttonFullscreen.getString() == "Fullscreen: ON")
						{
							buttonFullscreen.setString("Fullscreen: OFF");
							window.create(sf::VideoMode(screenWidth, screenHeight), "Pong");
						}
					}
			}
			else
			{
				buttonFullscreen.setColor(sf::Color::White);
			}


		}
		else if (currentState == PLAY)
		{
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				m_paused = !m_paused;
			if (!m_paused)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				{
					leftMove = 1;

				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				{
					leftMove = 2;

				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					rightMove = 1;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				{
					rightMove = 2;

				}

				if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S))
					leftMove = 0;

				if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
					rightMove = 0;
			}
			else
			{
				if (pauseMenu.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
				{
					if (!soundPlayed)
					{
						menuSound.play();
						soundPlayed = true;
					}
					pauseMenu.setColor(sf::Color::Red);
					if (event.type == sf::Event::MouseButtonReleased)
						if (event.mouseButton.button == sf::Mouse::Left)
						{
							scoreLeft = 0;
							scoreRight = 0;
							sprintf(scoreChar, "%d : %d", scoreLeft, scoreRight);
							scoreBoard.setString(static_cast<std::string>(scoreChar));
							ballSpeed = 300;
							ballVelocity = sf::Vector2f(ballSpeed, 0.f);
							ball.setPosition(sf::Vector2f(screenWidth / 2.f, screenHeight / 2.f));
							leftBat.setPosition(sf::Vector2f(0.f, screenHeight / 2.f - batHeight / 2.f + 2));
							rightBat.setPosition(sf::Vector2f(screenWidth - batWidth, screenHeight / 2.f - batHeight / 2.f + 2));

							currentState = MENU;
							m_paused = false;

						}
				}
				else
				{
					pauseMenu.setColor(sf::Color::White);
					if (!pauseExit.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
					{
						soundPlayed = false;
					}
				}

				if (pauseExit.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
				{
					if (!soundPlayed)
					{
						menuSound.play();
						soundPlayed = true;
					}
					pauseExit.setColor(sf::Color::Red);
					if (event.type == sf::Event::MouseButtonReleased)
						if (event.mouseButton.button == sf::Mouse::Left)
							window.close();
				}
				else
				{
					pauseExit.setColor(sf::Color::White);
					if (!pauseMenu.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
					{
						soundPlayed = false;
					}
				}
			}
		}
		else if (currentState == AI)
		{
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				m_paused = !m_paused;

			if (!m_paused)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				{
					leftMove = 1;

				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				{
					leftMove = 2;

				}

				if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S))
					leftMove = 0;

			}
			else
			{
				if (pauseMenu.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
				{
					if (!soundPlayed)
					{
						menuSound.play();
						soundPlayed = true;
					}
					pauseMenu.setColor(sf::Color::Red);
					if (event.type == sf::Event::MouseButtonReleased)
						if (event.mouseButton.button == sf::Mouse::Left)
						{
							scoreLeft = 0;
							scoreRight = 0;
							sprintf(scoreChar, "%d : %d", scoreLeft, scoreRight);
							scoreBoard.setString(static_cast<std::string>(scoreChar));
							ballSpeed = 300;
							ballVelocity = sf::Vector2f(ballSpeed, 0.f);
							ball.setPosition(sf::Vector2f(screenWidth / 2.f, screenHeight / 2.f));
							leftBat.setPosition(sf::Vector2f(0.f, screenHeight / 2.f - batHeight / 2.f + 2));
							rightBat.setPosition(sf::Vector2f(screenWidth - batWidth, screenHeight / 2.f - batHeight / 2.f + 2));
							predBall.setPosition(sf::Vector2f(screenWidth - 20.f, screenHeight / 2.f + 50));

							currentState = MENU;
							m_paused = false;

						}
				}
				else
				{
					pauseMenu.setColor(sf::Color::White);
					if (!pauseExit.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
					{
						soundPlayed = false;
					}
				}

				if (pauseExit.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
				{
					if (!soundPlayed)
					{
						menuSound.play();
						soundPlayed = true;
					}
					pauseExit.setColor(sf::Color::Red);
					if (event.type == sf::Event::MouseButtonReleased)
						if (event.mouseButton.button == sf::Mouse::Left)
							window.close();
				}
				else
				{
					pauseExit.setColor(sf::Color::White);
					if (!pauseMenu.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
					{
						soundPlayed = false;
					}
				}
			}
		}
		if (event.type == sf::Event::Closed)
			window.close();
	}


}

void game::update(sf::Time deltaTime)
{

	if (currentState == PLAY)
	{
		//Left bat
		//paddle center
		if (ball.getGlobalBounds().intersects(leftBat.getGlobalBounds()) && ball.getGlobalBounds().top == leftBat.getGlobalBounds().top + leftBat.getGlobalBounds().height / 2.f)
		{
			paddleCollision.play();
			if (ballSpeed == 300.)
				ballSpeed = 500.;
			ballVelocity = sf::Vector2f(ballSpeed, 0.f);
			ballSpeed += 200;
			ball.setFillColor(sf::Color::Yellow);
		}
		//paddle top
		else if (ball.getGlobalBounds().intersects(leftBat.getGlobalBounds()) && ball.getGlobalBounds().top < leftBat.getGlobalBounds().top + batHeight / 2.f)
		{
			paddleCollision.play();
			if (ballSpeed == 300.)
				ballSpeed = 500.;
			float ballHitPoint = (leftBat.getGlobalBounds().top + (leftBat.getGlobalBounds().height / 2.f)) - (ball.getGlobalBounds().top + (ball.getGlobalBounds().height / 2.f));
			ballVelocity = sf::Vector2f(ballSpeed, -ballHitPoint * 2.f);
			ballSpeed += 10.;
		}
		//paddle bot
		else if (ball.getGlobalBounds().intersects(leftBat.getGlobalBounds()) && ball.getGlobalBounds().top > leftBat.getGlobalBounds().top + batHeight / 2.f)
		{
			paddleCollision.play();
			if (ballSpeed == 300)
				ballSpeed = 500;
			float ballHitPoint = (leftBat.getGlobalBounds().top + (leftBat.getGlobalBounds().height / 2.f)) - (ball.getGlobalBounds().top + (ball.getGlobalBounds().height / 2.f));
			ballVelocity = sf::Vector2f(ballSpeed, ballHitPoint * -2.f);
			ballSpeed += 10.;
		}


		//Right bat
		//paddle center
		if (ball.getGlobalBounds().intersects(rightBat.getGlobalBounds()) && ball.getGlobalBounds().top == rightBat.getGlobalBounds().top + rightBat.getGlobalBounds().height / 2.f)
		{
			paddleCollision.play();
			if (ballSpeed == 300.)
				ballSpeed = 500.;
			ballVelocity = sf::Vector2f(-ballSpeed, 0.f);
			ballSpeed += 200.;
			ball.setFillColor(sf::Color::Yellow);

		}
		else if (ball.getGlobalBounds().intersects(rightBat.getGlobalBounds()) && ball.getGlobalBounds().top < rightBat.getGlobalBounds().top + batHeight / 2.f)
		{
			paddleCollision.play();
			if (ballSpeed == 300.)
				ballSpeed = 500.;
			float ballHitPoint = (rightBat.getGlobalBounds().top + (rightBat.getGlobalBounds().height / 2.f)) - (ball.getGlobalBounds().top + (ball.getGlobalBounds().height / 2.f));
			ballVelocity = sf::Vector2f(-ballSpeed, -ballHitPoint * 2.f);
			ballSpeed += 10.;

		}
		//Paddle bot
		else if (ball.getGlobalBounds().intersects(rightBat.getGlobalBounds()) && ball.getGlobalBounds().top > rightBat.getGlobalBounds().top + batHeight / 2.f)
		{
			paddleCollision.play();
			if (ballSpeed == 300)
				ballSpeed = 500;
			float ballHitPoint = (rightBat.getGlobalBounds().top + (rightBat.getGlobalBounds().height / 2.f)) - (ball.getGlobalBounds().top + (ball.getGlobalBounds().height / 2.f));
			ballVelocity = sf::Vector2f(-ballSpeed, ballHitPoint * -2.f);
			ballSpeed += 10.;

		}





		//Top side bounce
		if (ball.getGlobalBounds().top <= 0)
		{
			wallCollision.play();
			ballVelocity.y *= -1.f;
		}
		//Bottom side bounce
		else if (ball.getGlobalBounds().top + ball.getGlobalBounds().height >= screenHeight)
		{
			wallCollision.play();
			ballVelocity.y *= -1.f;
		}

		//Left side collision
		if (ball.getGlobalBounds().left <= 0)
		{
			scoreCollision.play();
			scoreRight++;
			sprintf(scoreChar, "%d : %d", scoreLeft, scoreRight);
			scoreBoard.setString(static_cast<std::string>(scoreChar));
			ball.setPosition(sf::Vector2f(screenWidth / 2.f, screenHeight / 2.f));
			ball.setFillColor(sf::Color::Red);
			ballSpeed = 300;
			ballVelocity = sf::Vector2f(-ballSpeed, 0.f);

		}
		//Right side collision
		if (ball.getGlobalBounds().left + ball.getGlobalBounds().width >= screenWidth)
		{
			scoreCollision.play();
			scoreLeft++;
			sprintf(scoreChar, "%d : %d", scoreLeft, scoreRight);
			scoreBoard.setString(static_cast<std::string>(scoreChar));
			ball.setPosition(sf::Vector2f(screenWidth / 2.f, screenHeight / 2.f));
			ball.setFillColor(sf::Color::Red);
			ballSpeed = 300;
			ballVelocity = sf::Vector2f(ballSpeed, 0.f);
		}


		ball.move(ballVelocity.x * deltaTime.asSeconds(), ballVelocity.y * deltaTime.asSeconds());

		//paddle move

		if (leftMove == 1)
		{
			if (leftBat.getPosition().y > 0)
				leftBat.move(0.f, -paddleSpeed * deltaTime.asSeconds());
		}
		else if (leftMove == 2)
		{
			if (leftBat.getPosition().y + rightBat.getGlobalBounds().height < screenHeight)
				leftBat.move(0.f, paddleSpeed * deltaTime.asSeconds());
		}
		else if (leftMove == 0)
		{
			leftBatVel = sf::Vector2f(0, 0);
		}



		if (rightMove == 1)
		{
			if (rightBat.getPosition().y > 0)
				rightBat.move(0.f, -paddleSpeed * deltaTime.asSeconds());
		}
		else if (rightMove == 2)
		{
			if (rightBat.getPosition().y + rightBat.getGlobalBounds().height < screenHeight)
				rightBat.move(0.f, paddleSpeed * deltaTime.asSeconds());
		}
		else if (rightMove == 0)
		{
			rightBatVel = sf::Vector2f(0, 0);
		}


		ballSpeedText.setString("Speed: " + std::to_string(static_cast<int>(ballSpeed / 10)));
	}
	else if (currentState == AI)
	{
		//Left bat
		//paddle center
		if (ball.getGlobalBounds().intersects(leftBat.getGlobalBounds()) && ball.getGlobalBounds().top == leftBat.getGlobalBounds().top + leftBat.getGlobalBounds().height / 2.f)
		{
			paddleCollision.play();
			if (ballSpeed == 300.f)
				ballSpeed = 500.f;
			ballVelocity = sf::Vector2f(ballSpeed, 0.f);
			ballSpeed += 200.f;
			ball.setFillColor(sf::Color::Yellow);
			predBall.setPosition(ball.getPosition());
			predBallVel = ballVelocity;
		}
		//paddle top
		else if (ball.getGlobalBounds().intersects(leftBat.getGlobalBounds()) && ball.getGlobalBounds().top < leftBat.getGlobalBounds().top + batHeight / 2.f)
		{
			paddleCollision.play();
			if (ballSpeed == 300.f)
				ballSpeed = 500.f;
			float ballHitPoint = (leftBat.getGlobalBounds().top + (leftBat.getGlobalBounds().height / 2.f)) - (ball.getGlobalBounds().top + (ball.getGlobalBounds().height / 2.f));
			ballVelocity = sf::Vector2f(ballSpeed, -ballHitPoint * 2.f);
			ballSpeed += 10.f;
			predBall.setPosition(ball.getPosition());
			predBallVel = ballVelocity;
		}
		//paddle bot
		else if (ball.getGlobalBounds().intersects(leftBat.getGlobalBounds()) && ball.getGlobalBounds().top > leftBat.getGlobalBounds().top + batHeight / 2.f)
		{
			paddleCollision.play();
			if (ballSpeed == 300.f)
				ballSpeed = 500.f;
			float ballHitPoint = (leftBat.getGlobalBounds().top + (leftBat.getGlobalBounds().height / 2.f)) - (ball.getGlobalBounds().top + (ball.getGlobalBounds().height / 2.f));
			ballVelocity = sf::Vector2f(ballSpeed, ballHitPoint * -2.f);
			ballSpeed += 10.f;
			predBall.setPosition(ball.getPosition());
			predBallVel = ballVelocity;
		}


		//Right bat
		//paddle center
		if (ball.getGlobalBounds().intersects(rightBat.getGlobalBounds()) && ball.getGlobalBounds().top == rightBat.getGlobalBounds().top + rightBat.getGlobalBounds().height / 2.f)
		{
			paddleCollision.play();
			if (ballSpeed == 300.)
				ballSpeed = 500.;
			ballVelocity = sf::Vector2f(-ballSpeed, 0.f);
			ballSpeed += 200.;
			ball.setFillColor(sf::Color::Yellow);

		}
		else if (ball.getGlobalBounds().intersects(rightBat.getGlobalBounds()) && ball.getGlobalBounds().top < rightBat.getGlobalBounds().top + batHeight / 2.f)
		{
			paddleCollision.play();
			if (ballSpeed == 300.)
				ballSpeed = 500.;
			float ballHitPoint = (rightBat.getGlobalBounds().top + (rightBat.getGlobalBounds().height / 2.f)) - (ball.getGlobalBounds().top + (ball.getGlobalBounds().height / 2.f));
			ballVelocity = sf::Vector2f(-ballSpeed, -ballHitPoint * 2.f);
			ballSpeed += 10.;

		}
		//Paddle bot
		else if (ball.getGlobalBounds().intersects(rightBat.getGlobalBounds()) && ball.getGlobalBounds().top > rightBat.getGlobalBounds().top + batHeight / 2.f)
		{
			paddleCollision.play();
			if (ballSpeed == 300)
				ballSpeed = 500;
			float ballHitPoint = (rightBat.getGlobalBounds().top + (rightBat.getGlobalBounds().height / 2.f)) - (ball.getGlobalBounds().top + (ball.getGlobalBounds().height / 2.f));
			ballVelocity = sf::Vector2f(-ballSpeed, ballHitPoint * -2.f);
			ballSpeed += 10.;

		}





		//Top side bounce
		if (ball.getGlobalBounds().top <= 0)
		{
			wallCollision.play();
			ballVelocity.y *= -1.f;
		}
		//Bottom side bounce
		else if (ball.getGlobalBounds().top + ball.getGlobalBounds().height >= screenHeight)
		{
			wallCollision.play();
			ballVelocity.y *= -1.f;
		}
		//Predball
		if (predBall.getGlobalBounds().top <= 0)
		{
			predBallVel.y *= -1.f;
		}
		else if (predBall.getGlobalBounds().top + predBall.getGlobalBounds().height >= screenHeight)
		{
			predBallVel.y *= -1.f;
		}

		//Left side collision
		if (ball.getGlobalBounds().left <= 0)
		{
			scoreCollision.play();
			scoreRight++;
			sprintf(scoreChar, "%d : %d", scoreLeft, scoreRight);
			scoreBoard.setString(static_cast<std::string>(scoreChar));
			ball.setPosition(sf::Vector2f(screenWidth / 2.f, screenHeight / 2.f));
			ball.setFillColor(sf::Color::Red);
			ballSpeed = 300;
			ballVelocity = sf::Vector2f(-ballSpeed, 0.f);

		}
		//Right side collision
		if (ball.getGlobalBounds().left + ball.getGlobalBounds().width >= screenWidth)
		{
			scoreCollision.play();
			scoreLeft++;
			sprintf(scoreChar, "%d : %d", scoreLeft, scoreRight);
			scoreBoard.setString(static_cast<std::string>(scoreChar));
			ball.setPosition(sf::Vector2f(screenWidth / 2.f, screenHeight / 2.f));
			ball.setFillColor(sf::Color::Red);
			ballSpeed = 300;
			ballVelocity = sf::Vector2f(ballSpeed, 0.f);
			predBall.setPosition(sf::Vector2f(screenWidth - 20.f, screenHeight / 2.f + 100));
		}


		ball.move(ballVelocity.x * deltaTime.asSeconds(), ballVelocity.y * deltaTime.asSeconds());


		//paddle move

		if (leftMove == 1)
		{
			if (leftBat.getPosition().y > 0)
				leftBat.move(0.f, -paddleSpeed * deltaTime.asSeconds());
		}
		else if (leftMove == 2)
		{
			if (leftBat.getPosition().y + rightBat.getGlobalBounds().height < screenHeight)
				leftBat.move(0.f, paddleSpeed * deltaTime.asSeconds());
		}
		else if (leftMove == 0)
		{
			leftBatVel = sf::Vector2f(0, 0);
		}

		//AI CODE HERE

		if (predBall.getGlobalBounds().left > 0 && predBall.getGlobalBounds().left + predBall.getGlobalBounds().width + 25.f < screenWidth)
		{
			predBall.move(predBallVel.x * deltaTime.asSeconds() * 1.3f, predBallVel.y * deltaTime.asSeconds() * 1.3f);
		}

			if (ball.getPosition().x >= screenWidth / 2.f)
			{
				if (rightBat.getGlobalBounds().top + rightBat.getGlobalBounds().height / 2.f < predBall.getPosition().y && !rightBat.getGlobalBounds().intersects(predBall.getGlobalBounds()))
				{
					rightBat.move(0, paddleSpeed * deltaTime.asSeconds());
					if (rightBat.getGlobalBounds().top + rightBat.getGlobalBounds().height >= screenHeight)
						rightBat.move(0, -paddleSpeed * deltaTime.asSeconds());
				}
				else if (rightBat.getGlobalBounds().top + rightBat.getGlobalBounds().height / 2.f > predBall.getPosition().y && !rightBat.getGlobalBounds().intersects(predBall.getGlobalBounds()))
				{
					rightBat.move(0, -botPaddleSpeed  * deltaTime.asSeconds());
					if (rightBat.getGlobalBounds().top <= 0)
						rightBat.move(0, botPaddleSpeed * deltaTime.asSeconds());
				}
			}




		ballSpeedText.setString("Speed: " + std::to_string(static_cast<int>(ballSpeed / 10)));

	}
	else if (currentState == OPTIONS)
	{
		if (selected)
		{
			slider.setPosition(static_cast<float>(sf::Mouse::getPosition(window).x), slider.getPosition().y);
			if (slider.getGlobalBounds().left + slider.getGlobalBounds().width / 2.f <= sliderBar.getGlobalBounds().left)
				slider.setPosition(sliderBar.getPosition().x, slider.getPosition().y);
			else if (slider.getGlobalBounds().left + slider.getGlobalBounds().width / 2.f >= sliderBar.getGlobalBounds().left + sliderBar.getGlobalBounds().width)
				slider.setPosition(sliderBar.getPosition().x + sliderBar.getGlobalBounds().width, slider.getPosition().y);
		}
	}


}

void game::draw()
{
	if (currentState == MENU)
	{
		window.clear();
		window.draw(title);
		window.draw(buttonPlay);
		window.draw(buttonPlayAI);
		window.draw(buttonOptions);
		window.draw(buttonExit);
		window.display();
	}
	else if (currentState == OPTIONS)
	{
		window.clear();
		window.draw(optionsTitle);
		window.draw(buttonVolume);
		window.draw(sliderBar);
		window.draw(slider);
		window.draw(buttonFullscreen);
		window.draw(buttonBack);
		window.display();
	}
	else if (currentState == PLAY)
	{
		window.clear();
		window.draw(ballSpeedText);
		window.draw(scoreBoard);
		window.draw(leftBat);
		window.draw(rightBat);
		window.draw(ball);
		if (m_paused)
		{
			window.draw(pauseBackground);
			window.draw(pauseTitle);
			window.draw(pauseMenu);
			window.draw(pauseExit);
		}
		window.display();
	}
	else if (currentState == AI)
	{
		window.clear();
		window.draw(ballSpeedText);
		window.draw(scoreBoard);
		window.draw(leftBat);
		window.draw(rightBat);
		//window.draw(predBall);
		window.draw(ball);
		if (m_paused)
		{
			window.draw(pauseBackground);
			window.draw(pauseTitle);
			window.draw(pauseMenu);
			window.draw(pauseExit);
		}
		window.display();
	}
}

void game::resetGame(sf::RectangleShape& player, sf::CircleShape& ball)
{
	//If left player
	if (player.getGlobalBounds().left <= screenWidth / 2)
	{
		ball.setPosition(sf::Vector2f(player.getGlobalBounds().left + player.getGlobalBounds().width, player.getGlobalBounds().top + player.getGlobalBounds().height / 2));
	}
	//If right player
	if (player.getGlobalBounds().left >= screenWidth / 2)
	{
		ball.setPosition(sf::Vector2f(player.getGlobalBounds().left - ball.getGlobalBounds().width, player.getGlobalBounds().top + player.getGlobalBounds().height / 2));
	}

}
