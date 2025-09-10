#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// Add the isWall function here
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

    int pacmanX = 1;
    int pacmanY = 1;

    const float TILE_SIZE = 40.0f;
    sf::RectangleShape wallTile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    wallTile.setFillColor(sf::Color::Blue);

    const float PACMAN_RADIUS = 15.0f;
    sf::CircleShape pacman(PACMAN_RADIUS);
    pacman.setFillColor(sf::Color::Yellow);

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
                }
            }
        }

        pacman.setPosition(pacmanX * TILE_SIZE + (TILE_SIZE / 2.0f) - PACMAN_RADIUS,
                           pacmanY * TILE_SIZE + (TILE_SIZE / 2.0f) - PACMAN_RADIUS);
        window.draw(pacman);

        window.display();
    }

    return 0;
}
