#define SCREEN_HEIGHT 500
#define SCREEN_WIDTH 500

#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>
#include <random>
#include <iostream>

using namespace std::chrono_literals;

struct particle
{
    particle(sf::Vector2f p, int posA_, int posB_)
        : pos(p), posA(posA_), posB(posB_)
    {
        s.setFillColor(sf::Color::White);
        s.setPosition(pos);
    }
    sf::RectangleShape s{ sf::Vector2f(10, 10) };
    sf::Vector2f pos;
    int posA{};
    int posB{};
};

void moveDown(particle& x, std::vector<std::vector<int>>& grid)
{
    if (x.posA < (SCREEN_HEIGHT / 10 - 1) && grid[x.posA + 1][x.posB] == 0) // Check bounds and if the space below is empty
    {
        grid[x.posA][x.posB] = 0;
        grid[x.posA + 1][x.posB] = 1;
        x.posA++;
        x.s.move(0, 10); // Moving down, so y increases
        std::cout << "Moved down to (" << x.posA << ", " << x.posB << ")\n";
    }
    else
    {
        // There is something underneath, try to move left or right
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, 1); // distribution in range [0, 1]
        int random = dist(rng);
        if (random == 0 && x.posB > 0 && grid[x.posA][x.posB - 1] == 0)
        {
            grid[x.posA][x.posB] = 0;
            grid[x.posA][x.posB - 1] = 1;
            x.posB--;
            x.s.move(-10, 0); // Moving left
        }
        else if (random == 1 && x.posB < (SCREEN_WIDTH / 10 - 1) && grid[x.posA][x.posB + 1] == 0)
        {
            grid[x.posA][x.posB] = 0;
            grid[x.posA][x.posB + 1] = 1;
            x.posB++;
            x.s.move(10, 0); // Moving right
        }
    }
}

int main()
{
    std::chrono::system_clock::time_point t = std::chrono::system_clock::now();

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML Particle Simulation");
    std::vector<particle> sand_particles{};
    std::vector<std::vector<int>> grid(SCREEN_HEIGHT / 10, std::vector<int>(SCREEN_WIDTH / 10, 0));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    int mouseX = event.mouseButton.x / 10;
                    int mouseY = 0;

                    sand_particles.push_back(particle(sf::Vector2f(mouseX * 10, mouseY * 10), mouseY, mouseX));
                    grid[mouseY][mouseX] = 1;
                    std::cout << "Particle added at: " << mouseX << ", " << mouseY << "\n";
                }
            }
        }

        if (std::chrono::system_clock::now() - t >= 25ms)
        {
            for (auto& x : sand_particles)
            {
                if (!x.s.getPosition().y == 0)
                {
                    moveDown(x, grid);
                }
            }
            t = std::chrono::system_clock::now();
        }

        window.clear();
        for (auto& x : sand_particles)
        {
            window.draw(x.s);
        }
        window.display();
    }

    return 0;
}
