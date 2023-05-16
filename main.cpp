#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>

sf::Color getRainbowColor(float time) {
    time = fmod(time/1000.0, 1.f);

    int r = 0;
    int g = 0;
    int b = 0;

    int sector = time / 0.1667f;
    float t = (time / 0.1667f) - sector;

    switch (sector) {
    case 0:
        r = 255;
        g = 255 * t;
        b = 0;
        break;
    case 1:
        r = 255 * (1.f - t);
        g = 255;
        b = 0;
        break;
    case 2:
        r = 0;
        g = 255;
        b = 255 * t;
        break;
    case 3:
        r = 0;
        g = 255 * (1.f - t);
        b = 255;
        break;
    case 4:
        r = 255 * t;
        g = 0;
        b = 255;
        break;
    case 5:
        r = 255;
        g = 0;
        b = 255 * (1.f - t);
        break;
    }

    return sf::Color(r, g, b);
}

int main()
{
    unsigned int MAX_TRY_BEFORE_GIVEUP = std::numeric_limits<unsigned int>::max()/100000;
    std::cout << MAX_TRY_BEFORE_GIVEUP << std::endl;
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 800), "Random Walk");

    // Seed the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 1);

    // Create a vertex array to hold the path of the random walk
    sf::VertexArray path(sf::LinesStrip);

    // Start at the middle of the window
    sf::Vector2f position(window.getSize().x / 2.f, window.getSize().y / 2.f);
    path.append(sf::Vertex(position));

    // Load a font
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        // error handling
        std::cout << "Error loading font\n";
        return 1;
    }

    float time = 0.f;
    const float EPSILON = 1e-2f;

    sf::View view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));

    sf::Text verticesText("Vertices: ", font, 50);
    verticesText.setFillColor(sf::Color::White);
    verticesText.setPosition(2, 2);

    sf::RectangleShape button(sf::Vector2f(100, 50));
    button.setFillColor(sf::Color::Green);
    button.setPosition(window.getSize().x - 110.f, 10.f);

    sf::Text buttonText("OK", font, 20);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setPosition(window.getSize().x - 90.f, 20.f);


    while (window.isOpen())
    {
        bool returnedToOrigin=false;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (event.mouseWheelScroll.delta > 0)
                    view.zoom(0.9f);
                else if (event.mouseWheelScroll.delta < 0)
                    view.zoom(1.1f);
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (button.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        // Button was clicked
                        path.clear();
                        position = sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f);
                        path.append(sf::Vertex(position));
                        time = 0.f;
                    }
                }
            }
        }

        // Move the view with the arrow keys
        float moveSpeed = 0.1f;  // or any other value you choose
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            view.move(0, -moveSpeed);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            view.move(0, moveSpeed);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            view.move(-moveSpeed, 0);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            view.move(moveSpeed, 0);

        // ...

        window.setView(view);
        // If we are back at the origin, we just stop moving
        if (path.getVertexCount() >= MAX_TRY_BEFORE_GIVEUP){
            window.draw(button);
            buttonText.setString("KO");
            button.setFillColor(sf::Color::Red);
            window.draw(buttonText);
            returnedToOrigin = true;
        }else if (std::abs(position.x - window.getSize().x / 2.f) > EPSILON || std::abs(position.y - window.getSize().y / 2.f) > EPSILON || path.getVertexCount() <= 1)
        { 
            //text.setString(std::to_string(path.getVertexCount()));
            // Choose a random direction and step size
            int direction = distrib(gen) ? 1 : -1;
            bool stepInX = distrib(gen);

            if (stepInX)
                position.x += direction;
            else
                position.y += direction;

            // Add the new position to the path
            path.append(sf::Vertex(position, getRainbowColor(time)));
            time += 0.01f;
        } else {
            window.draw(button);
            buttonText.setString("OK");
            button.setFillColor(sf::Color::Green);
            window.draw(buttonText);
            returnedToOrigin = true;
        }

        // Clear the window
        if(!returnedToOrigin)
            window.clear();

        // Draw the path
        window.draw(path);
        verticesText.setString("Vertices: " + std::to_string(path.getVertexCount()));
        //std::cout<< "Vertices: " << path.getVertexCount() << std::endl;
        window.draw(verticesText);

        // Display what was drawn
        window.display();
        
    }

    return 0;
}
