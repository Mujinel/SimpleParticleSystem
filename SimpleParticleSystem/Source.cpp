#include<iostream>
#include"SFML\Graphics.hpp"
#include"SFML\Window.hpp"
#include"SFML\System.hpp"
#include <random>
#include <array>

using namespace sf;

struct Particle
{
	Vector2f position;
	Vector2f velocity;
	Vector2f acceleration;
	float mass;
	float size;

	Color color;

	float timeElapsed;
	bool isAlive;
	bool isRocket;
};

int main()
{
	std::random_device rd;
	std::mt19937 mt(rd()); // Mersenne Twister
	std::uniform_real_distribution<float> randPositionX(200.0f, 1400.0f);
	std::uniform_real_distribution<float> randVelocityX(-100.0f, 100.0f);
	std::uniform_real_distribution<float> randVelocityY(-700.0f, -300.0f);
	std::uniform_real_distribution<float> randSize(1, 10);
	std::uniform_int_distribution<int> randColorChannel(0, 255);

	RenderWindow window(VideoMode(1600, 900), "Particle system", Style::Default);

	Vector2f gravity = Vector2f(0, 250);

	std::array<Particle, 1000> particles;
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].isAlive = false;
	}

	int nrParticles = 0;

	/*Particle p;

	p.position = Vector2f(800, 900);
	p.velocity = Vector2f(randVelocityX(mt), randVelocityY(mt));
	p.acceleration = Vector2f(0, 0);
	p.mass = 1;
	p.size = randSize(mt);
	p.color = sf::Color::Cyan;
	p.timeElapsed = 0.0f;
	p.isAlive = true;*/

	float dt = 0;
	sf::Clock frameClock;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		dt = frameClock.getElapsedTime().asSeconds();
		frameClock.restart();
		// update logic

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
		{
			for (int i = 0; i < particles.size(); i++)
			{
				if (particles[i].isAlive == false)
				{
					// initialize a new particle in the place of a dead one
					particles[i].position = Vector2f(randPositionX(mt), 900);
					particles[i].velocity = Vector2f(randVelocityX(mt), randVelocityY(mt));
					particles[i].acceleration = Vector2f(0, 0);
					particles[i].mass = 1;
					particles[i].size = 5;
					particles[i].color = sf::Color(randColorChannel(mt) / 4, randColorChannel(mt) / 4, randColorChannel(mt) / 4, 255);
					particles[i].timeElapsed = 0.0f;
					particles[i].isAlive = true;
					particles[i].isRocket = true;
					nrParticles++;
					break;
				}
			}
		}

		for (int i = 0; i < particles.size(); i++)
		{
			if (particles[i].isAlive)
			{
				// update position using Newton's second law: F = m*a; => a = F/m;
				particles[i].acceleration = gravity / particles[i].mass;
				particles[i].velocity += particles[i].acceleration * dt;
				particles[i].position += particles[i].velocity * dt;

				//update particle time
				particles[i].timeElapsed += dt;

				// kill the particle if sufficient time passed
				if (particles[i].timeElapsed > 2.0f)
				{
					if (particles[i].isRocket)
					{
						for (int k = 0; k < 10; k++)
						{
							for (int j = 0; j < particles.size(); j++)
							{
								if (particles[j].isAlive == false)
								{
									// initialize a new particle in the place of a dead one
									particles[j].position = particles[i].position;
									particles[j].velocity = Vector2f(randVelocityX(mt), randVelocityY(mt) / 3);
									particles[j].acceleration = Vector2f(0, 0);
									particles[j].mass = 1;
									particles[j].size = randSize(mt);
									particles[j].color = sf::Color(200 + randColorChannel(mt) / 5, 120 + randColorChannel(mt) / 2, randColorChannel(mt) / 2, 255);
									particles[j].timeElapsed = 0.0f;
									particles[j].isAlive = true;
									particles[j].isRocket = false;
									nrParticles++;
									break;
								}
							}
						}
					}
					nrParticles--;
					particles[i].isAlive = false;
				}
			}
		}
		// draw everything
		window.clear();
		// draw stuff

		for (int i = 0; i < particles.size(); i++)
		{
			if (particles[i].isAlive)
			{
				sf::CircleShape c(particles[i].size / 2);
				c.setOrigin(particles[i].size / 2, particles[i].size / 2);
				c.setPosition(particles[i].position);
				c.setFillColor(particles[i].color);
				window.draw(c);
			}
		}

		window.display();
	}

	return 0;
}