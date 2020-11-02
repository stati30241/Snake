/*
Simple snake game.

Author: stati30241
Youtube: https://www.youtube.com/channel/UC7Lx000LLDX6CU0qsyHGfQw
*/

#include <SFML/Graphics.hpp>

#include <cstddef>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <vector>


// Window variables
constexpr int pixelSize = 25;
sf::VideoMode windowVideoMode{ 30u * pixelSize, 30u * pixelSize };

// Font Variables
sf::Font font;
sf::Text scoreText{ "Score: ", font, pixelSize }, fpsText{ "FPS: ", font, pixelSize };

// Snake and food variables
std::vector<sf::Vector2i> snakes = { { 5, 16 }, { 4, 16 }, { 3, 16 } };
sf::Vector2i food = { 25, 16 };
unsigned char dir = 1;

// Game variables
constexpr float tickDelay = 0.1f;
int score = 0;


// Updates the food position
void updateFoodPosition() {
	static std::mt19937 randomEngine{ static_cast<unsigned int>(std::time(nullptr)) };
	std::uniform_int_distribution randomDist{ 0, 29 };

	while (true) {
		food = { randomDist(randomEngine), randomDist(randomEngine) };

		for (const auto& snake : snakes) {
			if (food == snake) {
				continue;
			}
		}
		return;
	}
}


// Updates the positions of game entites and checks and resolves collision
bool tick(float deltaTime) {
	// Updating the text
	scoreText.setString("Score: " + std::to_string(score));
	fpsText.setString("FPS: " + std::to_string(1.0f / deltaTime));

	// Move the snake's body
	for (size_t i = snakes.size() - 1; i > 0; --i) {
		snakes.at(i) = snakes.at(i - 1);
	}

	// Move the snakes head
	switch (dir) {
	case 0:
		snakes.front().x--; break;
	case 1:
		snakes.front().x++; break;
	case 2:
		snakes.front().y--; break;
	case 3:
		snakes.front().y++; break;
	}

	// Check collision with the walls
	if (snakes.front().x < 0 || snakes.front().y < 0 ||
		snakes.front().x > 29 || snakes.front().y > 29) {
		return false;
	}

	// Check collisions with itself
	for (size_t i = 1; i < snakes.size(); ++i) {
		if (snakes.front() == snakes.at(i)) {
			return false;
		}
	}

	// Check for collision with the food
	if (snakes.front() == food) {
		updateFoodPosition();
		snakes.push_back(snakes.back());
		score++;
	}

	return true;
}


// Main function
int main() {
	// Window
	sf::RenderWindow window{ windowVideoMode, "Snake", sf::Style::Close };
	window.setVerticalSyncEnabled(true);

	// Clock
	sf::Clock clock;
	float timer = 0.0f;

	// Font and text
	if (!font.loadFromFile("cour.ttf")) return -1;
	scoreText.setPosition({ pixelSize / 4.0f, 0 });
	fpsText.setPosition({ windowVideoMode.width / 3.0f, 0 });

	// Main game loop
	while (window.isOpen()) {
		// Event loop
		sf::Event sfmlEvent;
		while (window.pollEvent(sfmlEvent)) {
			if (sfmlEvent.type == sf::Event::Closed) {
				window.close();
			} else if (sfmlEvent.type == sf::Event::KeyPressed) {
				dir = static_cast<unsigned char>(sfmlEvent.key.code) - 71;
			}
		}

		// Updating game entites
		float deltaTime = clock.restart().asSeconds();
		timer += deltaTime;
		
		if (timer >= tickDelay) {
			if (!tick(deltaTime)) {
				window.close();
			}

			timer = 0.0f;
		}

		// Clearning the screen
		window.clear();

		// Drawing the snakes
		for (const auto& snake : snakes) {
			sf::RectangleShape snakeRect{ { pixelSize, pixelSize } };
			snakeRect.setPosition(sf::Vector2f{ snake * pixelSize });
			snakeRect.setFillColor(sf::Color::Green);
			window.draw(snakeRect);
		}

		// Drawing the food
		sf::RectangleShape foodRect{ { pixelSize, pixelSize } };
		foodRect.setPosition(sf::Vector2f{ food * pixelSize });
		foodRect.setFillColor(sf::Color::Red);
		window.draw(foodRect);

		// Drawing the text
		window.draw(scoreText);
		window.draw(fpsText);

		window.display();
	}

	return 0;
}
