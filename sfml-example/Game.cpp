#include "Game.h"

Game::Game()
{
	this->window = nullptr;
}

Game::~Game()
{
	delete this->window;
}

void Game::start()
{
	this->window = new sf::RenderWindow(sf::VideoMode(960, 540), "TETRIS");
	this->canvas.create(1920, 1080);
	this->window->setKeyRepeatEnabled(true);

	this->running = true;

	this->load();

	while (this->running)
	{
		this->pollEvent();
		this->deltaTime = clock.restart().asSeconds();
		this->update(this->deltaTime);
		this->window->clear();
		this->render(); // user's function
		this->canvas.display();
		const sf::Texture& canvas_texture = this->canvas.getTexture();
		this->sprite.setTexture(canvas_texture);
		float sprite_scale = 0; // find the scale of the main canvas
		if ((float)((float)this->window->getView().getSize().x / (float)this->window->getView().getSize().y) >= (float)((float)16 / (float)9)) {
			sprite_scale = (float)this->window->getView().getSize().y / 1080;
		}
		else {
			sprite_scale = (float)this->window->getView().getSize().x / 1920;
		}
		this->sprite.setScale(sprite_scale, sprite_scale);
		this->sprite.setPosition((this->window->getView().getSize().x - 1920 * sprite_scale) / 2, (this->window->getView().getSize().y - 1080 * sprite_scale) / 2);
		this->window->draw(this->sprite);
		this->window->display();
	}

	this->unload();
	this->window->close();
}

void Game::end()
{
	this->running = false;
}

void Game::pollEvent()
{
	while (this->window->pollEvent(this->event))
	{
		switch (this->event.type)
		{
		case sf::Event::Closed:
			this->running = false;
			break;
			// catch the resize events
		case sf::Event::Resized:
			// update the view to the new size of the window
			sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
			this->window->setView(sf::View(visibleArea));
			break;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		Input::up.pressCount++;
		Input::up.releaseCount = 0;
	}
	else {
		Input::up.releaseCount++;
		Input::up.pressCount = 0;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		Input::down.pressCount++;
		Input::down.releaseCount = 0;
	}
	else {
		Input::down.releaseCount++;
		Input::down.pressCount = 0;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		Input::right.pressCount++;
		Input::right.releaseCount = 0;
	}
	else {
		Input::right.releaseCount++;
		Input::right.pressCount = 0;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		Input::left.pressCount++;
		Input::left.releaseCount = 0;
	}
	else {
		Input::left.releaseCount++;
		Input::left.pressCount = 0;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		Input::space.pressCount++;
		Input::space.releaseCount = 0;
	}
	else {
		Input::space.releaseCount++;
		Input::space.pressCount = 0;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
		Input::c.pressCount++;
		Input::c.releaseCount = 0;
	}
	else {
		Input::c.releaseCount++;
		Input::c.pressCount = 0;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
		Input::x.pressCount++;
		Input::x.releaseCount = 0;
	}
	else {
		Input::x.releaseCount++;
		Input::x.pressCount = 0;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
		Input::z.pressCount++;
		Input::z.releaseCount = 0;
	}
	else {
		Input::z.releaseCount++;
		Input::z.pressCount = 0;
	}
}

bool Game::isRunning()
{
	return this->running;
}

void Game::load()
{

}

void Game::update(float dt)
{

}

void Game::render()
{

}

void Game::unload()
{

}

