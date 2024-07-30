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
    sf::RectangleShape s{ sf::Vector2f(1, 1) };
    sf::Vector2f pos;
    int posA{};
    int posB{};
};

void moveDown(particle& x, std::vector<std::vector<int>>& grid)
{
    if (x.posA < (SCREEN_HEIGHT - 1) && grid[x.posA + 1][x.posB] == 0)
    {
        // Move down if possible
        grid[x.posA][x.posB] = 0;
        grid[x.posA + 1][x.posB] = 1;
        x.posA++;
        x.s.move(0, 1);
        std::cout << "Moved down to (" << x.posA << ", " << x.posB << ")\n";
    }
    else if (x.posA < (SCREEN_HEIGHT - 1))
    {
        // Try to move diagonally if not at the bottom
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, 1);
        int random = dist(rng);

        if (random == 0 && x.posB > 0 && grid[x.posA + 1][x.posB - 1] == 0)
        {
            // Move diagonally down-left
            grid[x.posA][x.posB] = 0;
            grid[x.posA + 1][x.posB - 1] = 1;
            x.posA++;
            x.posB--;
            x.s.move(-1, 1);
        }
        else if (random == 1 && x.posB < (SCREEN_WIDTH - 1) && grid[x.posA + 1][x.posB + 1] == 0)
        {
            // Move diagonally down-right
            grid[x.posA][x.posB] = 0;
            grid[x.posA + 1][x.posB + 1] = 1;
            x.posA++;
            x.posB++;
            x.s.move(1, 1);
        }
        // If can't move diagonally, the particle stays in place
    }
    // If at the bottom or can't move, do nothing
}

int main()
{
    std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point t2 = std::chrono::system_clock::now();
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML Particle Simulation");
    std::vector<particle> sand_particles{};
    std::vector<std::vector<int>> grid(SCREEN_HEIGHT, std::vector<int>(SCREEN_WIDTH, 0));

    int lastMouseX = SCREEN_WIDTH / 2;  // Default to center of screen

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseMoved)
            {
                lastMouseX = event.mouseMove.x;
            }
        }

        if (std::chrono::system_clock::now() - t >= 0.001ms)
        {
            for (auto& x : sand_particles)
            {
                moveDown(x, grid);
            }
            t = std::chrono::system_clock::now();
        }

        if (std::chrono::system_clock::now() - t2 >= 50ms)
        {
            int mouseX = lastMouseX;
            int mouseY = 0;
            if (grid[mouseY][mouseX] == 0)  // Only add if the spot is empty
            {
                sand_particles.push_back(particle(sf::Vector2f(mouseX, mouseY), mouseY, mouseX));
                grid[mouseY][mouseX] = 1;
                std::cout << "Particle added at: " << mouseX << ", " << mouseY << "\n";
            }
            t2 = std::chrono::system_clock::now();
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