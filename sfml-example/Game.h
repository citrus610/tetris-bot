#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Input.h"
#include "Board.h"

/*
The game class, the heart of a program
*/

class Game
{
public:
	Game();
	virtual ~Game();

	void start();
	void end();

	virtual void load();
	virtual void pollEvent();
	virtual void update(float dt);
	virtual void render();
	virtual void unload();

	bool isRunning();

	sf::RenderTexture canvas;

private:
	sf::RenderWindow *window;
	sf::Event event;
	sf::Clock clock;

	sf::Sprite sprite;

	float deltaTime = 0;

	bool running = false;
};

