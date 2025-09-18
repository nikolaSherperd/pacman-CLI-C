#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// Add these new includes
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

// isWall function remains the same
bool isWall(int x, int y, const std::vector<std::string>& map)
{
    if (y < 0 || y >= map.size() || x < 0 || x >= map[y].size())
    {
        return true;
    }
    return map[y][x] == '#';
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pac-Man");

    // The maze map
    std::vector<std::string> maze_map = {
        "####################",
        "#P.......#.........#",
        "#.##.##.##.##.##.#.#",
        "#........#.........#",
        "#.##.#.#####.#.##.#",
        "#......#...#.....#",
        "######.###.###.#####",
        "..........#........",
        "######.###.###.#####",
        "#........#.........#",
        "#.##.#####.#####.##.#",
        "#....#.........#....#",
        "####################"
    };

    // Pac-Man's position on the grid
    int pacmanX = 1;
    int pacmanY = 1;

    // The score variable
    int score = 0;

    // Set up the font and text for the score
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        return -1; // Error loading font
    }

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.f, 10.f);

    // Constants for drawing
    const float TILE_SIZE = 40.0f;
    sf::RectangleShape wallTile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    wallTile.setFillColor(sf::Color::Blue);

    // Pac-Man's properties
    const float PACMAN_RADIUS = 15.0f;
    sf::CircleShape pacman(PACMAN_RADIUS);
    pacman.setFillColor(sf::Color::Yellow);

    // Pellet properties
    const float PELLET_RADIUS = 5.0f;
    sf::CircleShape pellet(PELLET_RADIUS);
    pellet.setFillColor(sf::Color::White);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                int nextX = pacmanX;
                int nextY = pacmanY;

                if (event.key.code == sf::Keyboard::Left)
                {
                    nextX--;
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    nextX++;
                }
                else if (event.key.code == sf::Keyboard::Up)
                {
                    nextY--;
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    nextY++;
                }

                if (!isWall(nextX, nextY, maze_map))
                {
                    if (maze_map[nextY][nextX] == '.')
                    {
                        maze_map[nextY][nextX] = ' ';
                        score += 10;
                    }

                    pacmanX = nextX;
                    pacmanY = nextY;
                }
            }
        }

        window.clear(sf::Color::Black);

        for (size_t y = 0; y < maze_map.size(); ++y)
        {
            for (size_t x = 0; x < maze_map[y].size(); ++x)
            {
                switch (maze_map[y][x])
                {
                    case '#':
                        wallTile.setPosition(x * TILE_SIZE, y * TILE_SIZE);
                        window.draw(wallTile);
                        break;
                    case '.':
                        pellet.setPosition(x * TILE_SIZE + (TILE_SIZE / 2.0f) - PELLET_RADIUS,
                                           y * TILE_SIZE + (TILE_SIZE / 2.0f) - PELLET_RADIUS);
                        window.draw(pellet);
                        break;
                    case ' ':
                        break;
                }
            }
        }

        pacman.setPosition(pacmanX * TILE_SIZE + (TILE_SIZE / 2.0f) - PACMAN_RADIUS,
                           pacmanY * TILE_SIZE + (TILE_SIZE / 2.0f) - PACMAN_RADIUS);
        window.draw(pacman);

        // Update and draw the score
        scoreText.setString("Score: " + std::to_string(score));
        window.draw(scoreText);

        window.display();
    }

    return 0;
}
