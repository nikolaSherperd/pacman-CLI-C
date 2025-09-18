#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>

enum GameState {
    TitleScreen,
    Playing,
    GameOver
};

class Ghost
{
public:
    sf::RectangleShape shape;
    int gridX;
    int gridY;

    Ghost(int startX, int startY, sf::Color color, float tileSize)
    {
        gridX = startX;
        gridY = startY;

        shape.setSize(sf::Vector2f(tileSize, tileSize));
        shape.setFillColor(color);
        shape.setPosition(gridX * tileSize, gridY * tileSize);
    }

    void updatePosition(float tileSize)
    {
        shape.setPosition(gridX * tileSize, gridY * tileSize);
    }
};

bool isWall(int x, int y, const std::vector<std::string>& map)
{
    if (y < 0 || y >= map.size() || x < 0 || x >= map[y].size())
    {
        return true;
    }
    return map[y][x] == '#';
}

float getDistance(int x1, int y1, int x2, int y2)
{
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

int main()
{
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pac-Man");

    std::vector<std::string> maze_map = {
        "####################",
        "#P.......#.........#",
        "#.##.##.##.##.##.#.#",
        "#........#.........#",
        "#.##.#.#####.#.##.#",
        "##.....#...#.....#",
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

    int pellets_left = 0;

    int score = 0;
    GameState current_state = GameState::TitleScreen;

    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        return -1;
    }

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.f, 10.f);

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::Red);
    sf::FloatRect textRect = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    gameOverText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("PAC-MAN");
    titleText.setCharacterSize(70);
    titleText.setFillColor(sf::Color::Yellow);
    sf::FloatRect titleRect = titleText.getLocalBounds();
    titleText.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
    titleText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 100.f);

    sf::Text instructionsText;
    instructionsText.setFont(font);
    instructionsText.setString("Press ENTER to start");
    instructionsText.setCharacterSize(30);
    instructionsText.setFillColor(sf::Color::White);
    sf::FloatRect instructionsRect = instructionsText.getLocalBounds();
    instructionsText.setOrigin(instructionsRect.left + instructionsRect.width / 2.0f, instructionsRect.top + instructionsRect.height / 2.0f);
    instructionsText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 50.f);

    const float TILE_SIZE = 40.0f;
    sf::RectangleShape wallTile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    wallTile.setFillColor(sf::Color::Blue);

    const float PACMAN_RADIUS = 15.0f;
    sf::CircleShape pacman(PACMAN_RADIUS);
    pacman.setFillColor(sf::Color::Yellow);

    const float PELLET_RADIUS = 5.0f;
    sf::CircleShape pellet(PELLET_RADIUS);
    pellet.setFillColor(sf::Color::White);

    std::vector<Ghost> ghosts;
    ghosts.push_back(Ghost(10, 6, sf::Color::Red, TILE_SIZE));
    ghosts.push_back(Ghost(9, 7, sf::Color::Cyan, TILE_SIZE));
    ghosts.push_back(Ghost(10, 7, sf::Color::Magenta, TILE_SIZE));
    ghosts.push_back(Ghost(11, 7, sf::Color::Yellow, TILE_SIZE));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (current_state == GameState::TitleScreen)
            {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
                {
                    current_state = GameState::Playing;

                    // Reset game variables for a new game
                    pacmanX = 1;
                    pacmanY = 1;
                    score = 0;
                    pellets_left = 0;

                    // Reset ghost positions
                    ghosts[0].gridX = 10;
                    ghosts[0].gridY = 6;
                    ghosts[1].gridX = 9;
                    ghosts[1].gridY = 7;
                    ghosts[2].gridX = 10;
                    ghosts[2].gridY = 7;
                    ghosts[3].gridX = 11;
                    ghosts[3].gridY = 7;

                    // Recount pellets from the original maze map
                    for (size_t y = 0; y < maze_map.size(); ++y)
                    {
                        for (size_t x = 0; x < maze_map[y].size(); ++x)
                        {
                            if (maze_map[y][x] == '.')
                            {
                                pellets_left++;
                            }
                        }
                    }
                }
            }
            else if (current_state == GameState::GameOver)
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    window.close();
                }
            }
        }

        if (current_state == GameState::Playing)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                int nextX = pacmanX - 1;
                int nextY = pacmanY;
                if (!isWall(nextX, nextY, maze_map))
                {
                    if (maze_map[nextY][nextX] == '.')
                    {
                        maze_map[nextY][nextX] = ' ';
                        score += 10;
                        pellets_left--;
                    }
                    pacmanX = nextX;
                    pacmanY = nextY;
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                int nextX = pacmanX + 1;
                int nextY = pacmanY;
                if (!isWall(nextX, nextY, maze_map))
                {
                    if (maze_map[nextY][nextX] == '.')
                    {
                        maze_map[nextY][nextX] = ' ';
                        score += 10;
                        pellets_left--;
                    }
                    pacmanX = nextX;
                    pacmanY = nextY;
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                int nextX = pacmanX;
                int nextY = pacmanY - 1;
                if (!isWall(nextX, nextY, maze_map))
                {
                    if (maze_map[nextY][nextX] == '.')
                    {
                        maze_map[nextY][nextX] = ' ';
                        score += 10;
                        pellets_left--;
                    }
                    pacmanX = nextX;
                    pacmanY = nextY;
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                int nextX = pacmanX;
                int nextY = pacmanY + 1;
                if (!isWall(nextX, nextY, maze_map))
                {
                    if (maze_map[nextY][nextX] == '.')
                    {
                        maze_map[nextY][nextX] = ' ';
                        score += 10;
                        pellets_left--;
                    }
                    pacmanX = nextX;
                    pacmanY = nextY;
                }
            }
            for (size_t i = 0; i < ghosts.size(); ++i)
            {
                float minDistance = -1.0f;
                int nextX = ghosts[i].gridX;
                int nextY = ghosts[i].gridY;

                if (!isWall(ghosts[i].gridX, ghosts[i].gridY - 1, maze_map))
                {
                    float distance = getDistance(ghosts[i].gridX, ghosts[i].gridY - 1, pacmanX, pacmanY);
                    if (minDistance == -1.0f || distance < minDistance)
                    {
                        minDistance = distance;
                        nextX = ghosts[i].gridX;
                        nextY = ghosts[i].gridY - 1;
                    }
                }
                if (!isWall(ghosts[i].gridX, ghosts[i].gridY + 1, maze_map))
                {
                    float distance = getDistance(ghosts[i].gridX, ghosts[i].gridY + 1, pacmanX, pacmanY);
                    if (minDistance == -1.0f || distance < minDistance)
                    {
                        minDistance = distance;
                        nextX = ghosts[i].gridX;
                        nextY = ghosts[i].gridY + 1;
                    }
                }
                if (!isWall(ghosts[i].gridX - 1, ghosts[i].gridY, maze_map))
                {
                    float distance = getDistance(ghosts[i].gridX - 1, ghosts[i].gridY, pacmanX, pacmanY);
                    if (minDistance == -1.0f || distance < minDistance)
                    {
                        minDistance = distance;
                        nextX = ghosts[i].gridX - 1;
                        nextY = ghosts[i].gridY;
                    }
                }
                if (!isWall(ghosts[i].gridX + 1, ghosts[i].gridY, maze_map))
                {
                    float distance = getDistance(ghosts[i].gridX + 1, ghosts[i].gridY, pacmanX, pacmanY);
                    if (minDistance == -1.0f || distance < minDistance)
                    {
                        minDistance = distance;
                        nextX = ghosts[i].gridX + 1;
                        nextY = ghosts[i].gridY;
                    }
                }
                ghosts[i].gridX = nextX;
                ghosts[i].gridY = nextY;
                ghosts[i].updatePosition(TILE_SIZE);
            }

            for (size_t i = 0; i < ghosts.size(); ++i)
            {
                if (ghosts[i].gridX == pacmanX && ghosts[i].gridY == pacmanY)
                {
                    current_state = GameState::GameOver;
                }
            }

            if (pellets_left == 0)
            {
                current_state = GameState::GameOver;
            }
        }

        window.clear(sf::Color::Black);

        if (current_state == GameState::TitleScreen)
        {
            window.draw(titleText);
            window.draw(instructionsText);
        }
        else if (current_state == GameState::Playing)
        {
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

            for (size_t i = 0; i < ghosts.size(); ++i)
            {
                window.draw(ghosts[i].shape);
            }

            scoreText.setString("Score: " + std::to_string(score));
            window.draw(scoreText);
        }
        else if (current_state == GameState::GameOver)
        {
            std::string final_message = "GAME OVER\n\nFinal Score: " + std::to_string(score) + "\n\nPress any key to exit.";
            gameOverText.setFillColor(sf::Color::Red);
            if (pellets_left == 0)
            {
                final_message = "YOU WIN!\n\nFinal Score: " + std::to_string(score) + "\n\nPress any key to exit.";
                gameOverText.setFillColor(sf::Color::Green);
            }

            gameOverText.setString(final_message);
            sf::FloatRect textRect = gameOverText.getLocalBounds();
            gameOverText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            gameOverText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
            window.draw(gameOverText);
        }

        window.display();
    }
    return 0;
}
