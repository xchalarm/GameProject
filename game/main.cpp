#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <sstream>

enum class GameState
{
    MENU,
    GAME,
    MINIGAME,
    EVENT,
    RESULT,
    END
};

std::wstring utf8ToUtf32(const std::string &utf8Str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(utf8Str);
}

// Function to wrap text into lines that fit within a specified width
std::vector<std::wstring> wrapText(const std::wstring &text, sf::Font &font, unsigned int maxWidth)
{
    std::vector<std::wstring> lines;
    std::wstring currentLine;

    std::wstringstream wordStream(text);
    std::wstring word;

    while (wordStream >> word)
    {
        std::wstring testLine = currentLine.empty() ? word : currentLine + L" " + word;

        sf::Text tempText(testLine, font, 24); // Use the same font and size as the text
        if (tempText.getLocalBounds().width <= maxWidth)
        {
            currentLine = testLine; // Add the word to the current line
        }
        else
        {
            if (!currentLine.empty())
            {
                lines.push_back(currentLine); // Push the current line to the lines
            }
            currentLine = word; // Start a new line with the current word
        }
    }

    if (!currentLine.empty()) // Push the last line
    {
        lines.push_back(currentLine);
    }

    return lines;
}

class Player
{
public:
    Player() : hp(100), score(0) {}
    void action(int amount) { amount < 0 ? takeDamage(-amount) : heal(amount); }
    void takeDamage(int damage) { hp = std::max(0, hp - damage); }
    void heal(int amount) { hp = std::min(100, hp + amount); }
    void addScore(int points) { score += points; }
    void collectItem(const std::string &item) { inventory.push_back(item); }
    void setEnding(std::string ending) { end = ending; }
    int getHP() const { return hp; }
    int getScore() const { return score; }
    std::string getEnding() const { return end; }
    const std::vector<std::string> &getInventory() const { return inventory; }
    bool hasItem(const std::string &itemName) const
    {
        // Assuming the player has an inventory (vector or set of items)
        for (const auto &item : inventory)
        {
            if (item == itemName)
            {
                return true;
            }
        }
        return false;
    }

private:
    int hp, score;
    std::vector<std::string> inventory;
    std::string end;
};

class Game
{
public:
    Game() : window(sf::VideoMode(800, 600), "SFML Window"), currentState(GameState::MENU)
    {

        item_dict["Medical Kit"] = "to heal wounds.";
        item_dict["Ancient Map"] = "to discover a hidden safe route.";
        item_dict["Updated Map"] = "to reveal secrets about the current location.";

        // Load font
        if (!font.loadFromFile("font/Prompt-Regular.ttf"))
        {
            std::cerr << "Error loading font" << std::endl;
            exit(-1);
        }

        for (int i = 0; i < 43; i++)
        {
            if (!Screen[i].loadFromFile("asset/Event/" + std::to_string(i + 1) + ".png"))
            {
                std::cerr << "Error loading texture" << std::endl;
                exit(-1);
            }
        }
        BG.setTexture(Screen[0]);
        BG.setScale(800.f / Screen[0].getSize().x, 600.f / Screen[0].getSize().y);

        setupButton(startButton, sf::Vector2f(200.f, 50.f), sf::Color::Green, sf::Vector2f(300.f, 400.f));
        setupText(buttonText, "Start", 30, sf::Color::Black, startButton);
    }

    void run()
    {
        while (window.isOpen())
        {
            processEvents();
            update();
            render();
        }
    }

private:
    Player player;
    sf::RenderWindow window;
    GameState currentState;
    sf::RectangleShape startButton;
    sf::RectangleShape decisionButton[4];
    sf::RectangleShape minigameButton[5];
    sf::RectangleShape minigameScreen;
    sf::RectangleShape dialogueScreen;
    sf::RectangleShape eventScreen;
    sf::RectangleShape decisionScreen;
    sf::RectangleShape resultScreen;
    sf::RectangleShape resultMinigameScreen;
    sf::RectangleShape resultEventScreen;
    sf::Text buttonText;
    sf::Font font;
    sf::Sprite BG;
    sf::Texture Screen[43];
    bool minigameVisible = false;
    bool decisionVisible = false;
    bool resultVisible = false;
    bool resultEventVisible = false;
    int currentChapter = 1;
    int event_random = 0;
    std::string chapter_arr[6] = {"Waking Up in Confusion", "Encountering a Survivor", "Changing Fate with Items (Special Chapter)", "Confronting the Zombie Horde", "The Final Decision", "The Final Battle"};
    std::string dialogue_arr[6] = {"The player wakes up in an abandoned house that has been untouched for years. The air is thick with dust, and darkness fills the room. The player is confused and must make a decision about what to do next.", "As the player continues their journey, they encounter another survivor who appears to have experience in this chaotic world. The survivor offers help, and the player must decide how to respond.", "At this stage, the player is given a chance to use collected items to change the course of the story.", "The player finds themselves in a terrifying situation—a massive horde of zombies is approaching. There is no easy escape, and the player must make a crucial decision.", "The player has survived through many trials and now reaches a critical moment—the final decision that will determine their fate.", "The final challenge has arrived. The player must face the strongest and most ruthless enemy in this apocalyptic world."};
    std::string decision_arr[6][3] = {
        {"Leave the house immediately to escape the overwhelming darkness.", "Hide inside the house and wait for things to calm down.", "Carefully inspect the house for useful items."},
        {"Accept help and travel together.", "Travel alone to prove your ability in this harsh world.", "Ask for more information to gain clues and knowledge."},
        {"Use a \"Medical Kit\" to heal wounds.", "Use an \"Ancient Map\" to discover a hidden safe route.", "Use an \"Updated Map\" to reveal secrets about the current location."},
        {"Use a weapon to fight the zombies.", "Run away from the horde.", "Hide in the darkness and wait for the danger to pass."},
        {"Use a parked car to escape the city.", "Walk through the zombie-infested area to find shelter.", "Search for a secret path using the map obtained earlier."},
        {"Engage the enemy head-on with bravery.", "Use evasive strategy and attack at the right moment.", "Use collected items to adjust your strategy."}};
    std::map<std::string, std::string> item_dict;
    std::string minigame_arr[6] = {"You find an old desk in the room with three drawers. Try to guess which drawer contains the key (1, 2, or 3)", "Choose a number between 1 and 4 to select the best attack timing", "", " Pick a number between 1 and 3 to find a safe route.", "The lock system requires a number between 1 and 5. Try guessing the code", "Choose a number between 1 and 3 to decide your defensive strategy"};
    std::string event_arr[3] = {"You found a mysterious letter revealing clues about the outbreak.", "You were suddenly attacked by a small group of zombies!", "You stumbled upon a hidden supply cache filled with valuable items!"};

    void setupButton(sf::RectangleShape &button, sf::Vector2f size, sf::Color color, sf::Vector2f position)
    {
        button.setSize(size);
        button.setFillColor(color);
        button.setPosition(position);
    }

    void setupText(sf::Text &text, const std::string &str, unsigned int size, sf::Color color, sf::RectangleShape &button)
    {
        text.setFont(font);
        text.setString(str);
        text.setCharacterSize(size);
        text.setFillColor(color);
        text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
        text.setPosition(button.getPosition() + sf::Vector2f(button.getSize().x / 2, button.getSize().y / 3));
    }

    void processEvents()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                handleMouseClick(event.mouseButton.x, event.mouseButton.y);
            }
        }
    }

    void handleMouseClick(int x, int y)
    {
        if (currentState == GameState::MENU && startButton.getGlobalBounds().contains(x, y))
        {
            currentState = GameState::GAME;
        }
        else if (currentState == GameState::GAME)
        {
            if (decisionVisible)
            {
                handleDecisionClick(x, y);
            }
            else if (resultVisible)
            {
                handleResultClick(x, y);
            }
            else
            {
                decisionVisible = true;
            }
        }
        else if (currentState == GameState::MINIGAME)
        {
            handleMinigameClick(x, y);
        }
        else if (currentState == GameState::RESULT)
        {
            handleResutMinigameClick(x, y);
        }
        else if (currentState == GameState::EVENT)
        {
            handleEventClick(x, y);
        }
    }

    void handleDecisionClick(int x, int y)
    {
        for (int i = 0; i < 4; i++)
        {
            if (decisionButton[i].getGlobalBounds().contains(x, y))
            {
                applyDecisionEffects(i);
                decisionVisible = false;
                resultVisible = true;
            }
        }
    }

    void applyDecisionEffects(int choice)
    {
        if (currentChapter == 1)
        {
            switch (choice)
            {
            case 0:
                BG.setTexture(Screen[1]);
                BG.setScale(800.f / Screen[1].getSize().x, 600.f / Screen[1].getSize().y);
                player.action(-10);
                player.addScore(10);
                break;
            case 1:
                BG.setTexture(Screen[3]);
                BG.setScale(800.f / Screen[3].getSize().x, 600.f / Screen[3].getSize().y);
                player.action(-5);
                player.addScore(5);
                break;
            case 2:
                BG.setTexture(Screen[2]);
                BG.setScale(800.f / Screen[2].getSize().x, 600.f / Screen[2].getSize().y);
                player.action(5);
                player.addScore(15);
                player.collectItem("Ancient Map");
                break;
            }
        }
        else if (currentChapter == 2)
        {
            switch (choice)
            {
            case 0:
                BG.setTexture(Screen[7]);
                BG.setScale(800.f / Screen[7].getSize().x, 600.f / Screen[7].getSize().y);
                player.collectItem("Weapon");
                player.collectItem("Medical Kit");
                player.addScore(20);
                break;
            case 1:
                BG.setTexture(Screen[4]);
                BG.setScale(800.f / Screen[4].getSize().x, 600.f / Screen[4].getSize().y);
                player.action(-15);
                break;
            case 2:
                BG.setTexture(Screen[6]);
                BG.setScale(800.f / Screen[6].getSize().x, 600.f / Screen[6].getSize().y);
                player.addScore(10);
                player.collectItem("Updated Map");
                break;
            }
        }
        else if (currentChapter == 3)
        {
            std::vector<std::string> availableItems;

            for (const auto &item : player.getInventory())
            {
                if (item != "Weapon") // กรองไม่ให้แสดง "Weapon"
                {
                    availableItems.push_back(item);
                }
            }

            if (choice >= 0 && choice < availableItems.size())
            {
                // Check the item chosen by the player
                std::string itemName = availableItems[choice];

                if (itemName == "Medical Kit") // Use "Medical Kit" to heal wounds
                {
                    BG.setTexture(Screen[25]);
                    BG.setScale(800.f / Screen[25].getSize().x, 600.f / Screen[25].getSize().y);
                    player.heal(15);     // Increase HP by 15
                    player.addScore(10); // Add 10 points to the score
                }
                else if (itemName == "Ancient Map") // Use "Ancient Map" to discover a hidden safe route
                {
                    BG.setTexture(Screen[27]);
                    BG.setScale(800.f / Screen[27].getSize().x, 600.f / Screen[27].getSize().y);
                    player.addScore(20); // Add 20 points to the score
                    // No change to HP
                }
                else if (itemName == "Updated Map") // Use "Updated Map" to reveal secrets about the current location
                {
                    BG.setTexture(Screen[26]);
                    BG.setScale(800.f / Screen[26].getSize().x, 600.f / Screen[26].getSize().y);
                    player.addScore(15); // Add 15 points to the score
                    // No change to HP
                }
                else
                {
                    // No changes to HP or score
                }
            }
        }
        else if (currentChapter == 4)
        {
            // Check if the player is armed
            bool isArmed = player.hasItem("Weapon"); // Assuming player has an `hasItem` function

            switch (choice)
            {
            case 0: // Choose to fight the horde
                BG.setTexture(Screen[23]);
                BG.setScale(800.f / Screen[23].getSize().x, 600.f / Screen[23].getSize().y);
                if (isArmed)
                {
                    player.addScore(30); // Add 30 points if armed
                    // No additional HP loss
                }
                else
                {
                    player.action(-30); // Lose 30 HP if unarmed
                }
                break;
            case 1: // Choose to run away from the horde
                BG.setTexture(Screen[22]);
                BG.setScale(800.f / Screen[22].getSize().x, 600.f / Screen[22].getSize().y);
                player.action(-10); // Lose 10 HP
                break;
            case 2: // Choose to hide in the darkness and wait
                BG.setTexture(Screen[10]);
                BG.setScale(800.f / Screen[10].getSize().x, 600.f / Screen[10].getSize().y);
                player.action(-5);  // Lose 5 HP
                player.addScore(5); // Add 5 points for hiding successfully
                break;
            }
        }
        else if (currentChapter == 5)
        {
            switch (choice)
            {
            case 0:
                BG.setTexture(Screen[14]);
                BG.setScale(800.f / Screen[14].getSize().x, 600.f / Screen[14].getSize().y);
                player.addScore(40);
                player.setEnding("Car");
                break;
            case 1:
                BG.setTexture(Screen[20]);
                BG.setScale(800.f / Screen[20].getSize().x, 600.f / Screen[20].getSize().y);
                player.action(-10);
                player.addScore(30);
                break;
            case 2:
                BG.setTexture(Screen[18]);
                BG.setScale(800.f / Screen[18].getSize().x, 600.f / Screen[18].getSize().y);
                player.addScore(50);
                player.setEnding("Secret");
                break;
            }
        }
        else if (currentChapter == 6)
        {
            switch (choice)
            {
            case 0:
                BG.setTexture(Screen[28]);
                BG.setScale(800.f / Screen[28].getSize().x, 600.f / Screen[28].getSize().y);
                player.addScore(35);
                player.action(-20);
                player.setEnding("Fight");
                break;
            case 1:
                BG.setTexture(Screen[16]);
                BG.setScale(800.f / Screen[16].getSize().x, 600.f / Screen[16].getSize().y);
                player.addScore(25);
                player.action(-10);
                player.setEnding("Strategy");
                break;
            case 2:
                BG.setTexture(Screen[9]);
                BG.setScale(800.f / Screen[9].getSize().x, 600.f / Screen[9].getSize().y);
                player.addScore(45);
                player.action(-5);
                player.action(10);
                player.setEnding("Greatness");
                break;
            }
        }
    }
    void handleResultClick(int x, int y)
    {
        resultVisible = false;
        if (currentChapter == 3)
        {
            currentChapter++;
            if (currentChapter == 4)
            {
                BG.setTexture(Screen[24]);
                BG.setScale(800.f / Screen[24].getSize().x, 600.f / Screen[24].getSize().y);
            }
        }
        else
        {
            currentState = GameState::MINIGAME;
            minigameVisible = true;
            if (currentChapter == 1)
            {
                BG.setTexture(Screen[32]);
                BG.setScale(800.f / Screen[32].getSize().x, 600.f / Screen[32].getSize().y);
            }
            else if (currentChapter == 2)
            {
                BG.setTexture(Screen[35]);
                BG.setScale(800.f / Screen[35].getSize().x, 600.f / Screen[33].getSize().y);
            }
            else if (currentChapter == 4)
            {
                BG.setTexture(Screen[33]);
                BG.setScale(800.f / Screen[33].getSize().x, 600.f / Screen[33].getSize().y);
            }
            else if (currentChapter == 5)
            {
                BG.setTexture(Screen[34]);
                BG.setScale(800.f / Screen[34].getSize().x, 600.f / Screen[34].getSize().y);
            }
            else if (currentChapter == 6)
            {
                BG.setTexture(Screen[36]);
                BG.setScale(800.f / Screen[36].getSize().x, 600.f / Screen[36].getSize().y);
            }
        }
    }

    void handleMinigameClick(int x, int y)
    {
        if (minigameVisible)
        {
            if (minigameScreen.getGlobalBounds().contains(x, y))
            {
                minigameVisible = false;
                resultVisible = true;
            }
        }
        else
        {
            for (int i = 0; i < 5; i++)
            {
                if (minigameButton[i].getGlobalBounds().contains(x, y))
                {
                    applyMinigameEffect(i);
                    currentState = GameState::RESULT;
                }
            }
        }
    }

    void applyMinigameEffect(int choice)
    {
        if (currentChapter == 1)
        {
            int correctAnswer = rand() % 3 + 1;
            if (choice == correctAnswer)
            {
                player.action(5);
                player.addScore(10);
            }
            else
            {
                player.action(-5);
                player.action(5);
            }
        }
        else if (currentChapter == 2)
        {
            int correctAnswer = rand() % 4 + 1;
            if (choice == correctAnswer)
            {
                player.addScore(15);
            }
            else
            {
                player.action(-10);
            }
        }
        else if (currentChapter == 4)
        {
            int correctAnswer = rand() % 3 + 1;
            if (choice == correctAnswer)
            {
                player.addScore(20);
            }
            else
            {
                player.action(-15);
            }
        }
        else if (currentChapter == 5)
        {
            int correctAnswer = rand() % 5 + 1;
            if (choice == correctAnswer)
            {
                player.addScore(25);
            }
            else
            {
                player.action(-20);
            }
        }
        else if (currentChapter == 6)
        {
            int correctAnswer = rand() % 3 + 1;
            if (choice == correctAnswer)
            {
                player.addScore(30);
            }
            else
            {
                player.action(-25);
            }
        }
    }

    void handleResutMinigameClick(int x, int y)
    {
        if (resultMinigameScreen.getGlobalBounds().contains(x, y))
        {
            event_random = rand() % 3;
            if (event_random == 0)
            {
                BG.setTexture(Screen[31]);
                BG.setScale(800.f / Screen[31].getSize().x, 600.f / Screen[31].getSize().y);
            }
            else if (event_random == 1)
            {
                BG.setTexture(Screen[29]);
                BG.setScale(800.f / Screen[29].getSize().x, 600.f / Screen[29].getSize().y);
            }
            else
            {
                BG.setTexture(Screen[30]);
                BG.setScale(800.f / Screen[30].getSize().x, 600.f / Screen[30].getSize().y);
            }
            currentState = GameState::EVENT;
        }
    }

    void handleEventClick(int x, int y)
    {
        if (!resultEventVisible)
        {
            if (eventScreen.getGlobalBounds().contains(x, y))
            {
                switch (event_random)
                {
                case 0:
                    player.action(3);
                    player.addScore(5);
                    break;
                case 1:
                    player.action(-7);
                    break;
                case 2:
                    player.action(5);
                    player.addScore(10);
                    break;
                }
                resultEventVisible = true;
            }
        }
        else
        {
            if (resultEventScreen.getGlobalBounds().contains(x, y))
            {
                currentChapter++;
                if (currentChapter == 2)
                {
                    BG.setTexture(Screen[5]);
                    BG.setScale(800.f / Screen[5].getSize().x, 600.f / Screen[5].getSize().y);
                }
                else if (currentChapter == 5)
                {
                    BG.setTexture(Screen[19]);
                    BG.setScale(800.f / Screen[19].getSize().x, 600.f / Screen[19].getSize().y);
                }
                else if (currentChapter == 6)
                {
                    BG.setTexture(Screen[17]);
                    BG.setScale(800.f / Screen[17].getSize().x, 600.f / Screen[17].getSize().y);
                }
                currentState = GameState::GAME;
                resultVisible = false;
                resultEventVisible = false;
                if (currentChapter > 6)
                {
                    currentState = GameState::END;
                }
            }
        }
    }

    void update()
    {
        if (player.getHP() == 0)
        {
            currentState = GameState::END;
            player.setEnding("Bad");
        }
        if (player.getEnding() != "")
        {
            currentState = GameState::END;
        }
    }

    void render()
    {
        window.clear(sf::Color::Black);

        if (currentState == GameState::MENU)
        {
            // Draw menu (start button)
            window.draw(startButton);
            window.draw(buttonText);
        }
        else if (currentState == GameState::GAME)
        {
            // Draw the game scene
            drawGameScene();
        }
        else if (currentState == GameState::MINIGAME)
        {
            drawMiniGame();
        }
        else if (currentState == GameState::RESULT)
        {
            drawResult();
        }
        else if (currentState == GameState::EVENT)
        {
            drawEvent();
        }
        else if (currentState == GameState::END)
        {
            drawEnd();
        }

        window.display();
    }

    void drawGameScene()
    {
        // Draw a block (this could represent a "game block")
        dialogueScreen.setSize(sf::Vector2f(800.f, 600.f));
        resultScreen.setSize(sf::Vector2f(800.f, 600.f));
        sf::RectangleShape gameBlock(sf::Vector2f(800.f, 600.f));
        setupButton(gameBlock, sf::Vector2f(800.f, 600.f), sf::Color(169, 181, 223), sf::Vector2f(0.f, 0.f));
        window.draw(gameBlock);

        window.draw(BG);

        sf::RectangleShape charecterBox;
        sf::RectangleShape dialogueBox;
        setupButton(charecterBox, sf::Vector2f(200.f, 50.f), sf::Color::Black, sf::Vector2f(25.f, 375.f));
        setupButton(dialogueBox, sf::Vector2f(800.f, 200.f), sf::Color::Black, sf::Vector2f(0.f, 400.f));
        window.draw(dialogueBox);
        window.draw(charecterBox);

        // Character text setup
        sf::Text charecterText;
        setupText(charecterText, "[Main Event]", 24, sf::Color::White, charecterBox);
        window.draw(charecterText);

        std::wstring dialogueMSG;
        if (resultVisible)
        {
            charecterText.setString("[Player]");
            std::stringstream ss;
            ss << "HP: " << player.getHP() << "\n";
            ss << "Score: " << player.getScore() << "\n";
            ss << "Item: ";

            if (!player.getInventory().empty())
            {
                for (size_t i = 0; i < player.getInventory().size(); i++)
                {
                    ss << player.getInventory()[i];
                    if (i < player.getInventory().size() - 1)
                        ss << ", ";
                }
            }
            else
            {
                ss << "None";
            }

            dialogueMSG = utf8ToUtf32(ss.str());
            sf::Text resultText;
            resultText.setFont(font);
            resultText.setString(dialogueMSG);
            resultText.setCharacterSize(24);
            resultText.setFillColor(sf::Color::White);
            resultText.setPosition(10.f, 435.f); // Adjust position as needed

            window.draw(resultText); // Now drawing sf::Text instead of std::wstring
        }
        else
        {
            dialogueMSG = utf8ToUtf32(dialogue_arr[currentChapter - 1]);
            std::vector<std::wstring> wrappedLines = wrapText(dialogueMSG, font, 780);
            float yOffset = 35.f;
            for (const auto &line : wrappedLines)
            {
                sf::Text lineText(line, font, 24);
                lineText.setFillColor(sf::Color::White);
                lineText.setPosition(10.f, 400 + yOffset);
                window.draw(lineText);
                yOffset += 35.f; // Adjust the space between lines
            }
        }

        sf::RectangleShape chapterBlock(sf::Vector2f(700.f, 50.f));
        setupButton(chapterBlock, sf::Vector2f(700.f, 50.f), sf::Color::Green, sf::Vector2f(50.f, 0.f));
        window.draw(chapterBlock);

        sf::Text chapterText;
        setupText(chapterText, "Chapeter " + std::to_string(currentChapter) + ": [" + chapter_arr[currentChapter - 1] + "]", 24, sf::Color::Black, chapterBlock);
        window.draw(chapterText);

        // Draw dialogue if visible
        if (decisionVisible)
        {

            setupButton(decisionScreen, sf::Vector2f(800.f, 600.f), sf::Color(0, 0, 0, 150), sf::Vector2f(0.f, 0.f));
            window.draw(decisionScreen);

            sf::Text decisionText[4];
            std::vector<std::string> availableItems;

            for (const auto &item : player.getInventory())
            {
                if (item != "Weapon") // กรองไม่ให้แสดง "Weapon"
                {
                    availableItems.push_back(item);
                }
            }

            if (currentChapter == 3)
            {
                for (size_t i = 0; i < availableItems.size() + 1 && i < 3; i++)
                {
                    if (i == availableItems.size()) // If we are at the last item, which is "Do not use any items"
                    {
                        setupButton(decisionButton[i], sf::Vector2f(650.f, 50.f), sf::Color::White, sf::Vector2f(75.f, 100.f + (i * 100.f)));
                        setupText(decisionText[i], "Do not use any items", 18, sf::Color::Black, decisionButton[i]);
                        window.draw(decisionButton[i]);
                        window.draw(decisionText[i]);
                    }
                    else // If it's an actual item, get its description from item_dict
                    {
                        std::string itemName = availableItems[i];
                        std::string itemDescription = item_dict[itemName]; // Get description from the dictionary

                        setupButton(decisionButton[i], sf::Vector2f(650.f, 50.f), sf::Color::White, sf::Vector2f(75.f, 100.f + (i * 100.f)));
                        setupText(decisionText[i], "Use a \"" + itemName + "\" " + itemDescription, 18, sf::Color::Black, decisionButton[i]);
                        window.draw(decisionButton[i]);
                        window.draw(decisionText[i]);
                    }
                }
            }
            else
            {
                for (int i = 0; i < 3; i++)
                {
                    setupButton(decisionButton[i], sf::Vector2f(650.f, 50.f), sf::Color::White, sf::Vector2f(75.f, 100.f + (i * 100.f)));
                    setupText(decisionText[i], decision_arr[currentChapter - 1][i], 18, sf::Color::Black, decisionButton[i]);
                    window.draw(decisionButton[i]);
                    window.draw(decisionText[i]);
                }
            }
        }
    }

    void drawMiniGame()
    {
        // Draw the mini-game scene
        minigameScreen.setSize(sf::Vector2f(800.f, 600.f));
        sf::RectangleShape miniGameBlock(sf::Vector2f(800.f, 600.f));
        setupButton(miniGameBlock, sf::Vector2f(800.f, 600.f), sf::Color::Black, sf::Vector2f(0.f, 0.f));
        window.draw(miniGameBlock);

        window.draw(BG);

        sf::RectangleShape chapterMinigameBlock(sf::Vector2f(700.f, 50.f));
        setupButton(chapterMinigameBlock, sf::Vector2f(700.f, 50.f), sf::Color::Green, sf::Vector2f(50.f, 0.f));
        window.draw(chapterMinigameBlock);

        sf::Text chapterMinigameText;
        setupText(chapterMinigameText, "Chapeter ???: [Minigame]", 24, sf::Color::Black, chapterMinigameBlock);
        window.draw(chapterMinigameText);

        if (minigameVisible)
        {
            minigameScreen.setFillColor(sf::Color(0, 0, 0, 150));
            window.draw(minigameScreen);
            std::wstring minigameeMSG;
            minigameeMSG = utf8ToUtf32(minigame_arr[currentChapter - 1]);
            std::vector<std::wstring> wrappedLines = wrapText(minigameeMSG, font, 780);
            float yOffset = 35.f;
            for (const auto &line : wrappedLines)
            {
                sf::Text lineText(line, font, 24);
                lineText.setFillColor(sf::Color::White);
                lineText.setPosition(40.f, 50 + yOffset);
                window.draw(lineText);
                yOffset += 35.f; // Adjust the space between lines
            }
        }
        else
        {
            if (currentChapter == 1)
            {
                sf::Text minigameText[3];
                for (int i = 0; i < 3; i++)
                {
                    setupButton(minigameButton[i], sf::Vector2f(650.f, 50.f), sf::Color::White, sf::Vector2f(75.f, 100.f + (i * 100.f)));
                    setupText(minigameText[i], std::to_string(i + 1), 18, sf::Color::Black, minigameButton[i]);
                    window.draw(minigameButton[i]);
                    window.draw(minigameText[i]);
                }
            }
            else if (currentChapter == 2)
            {
                sf::Text minigameText[4];
                for (int i = 0; i < 4; i++)
                {
                    setupButton(minigameButton[i], sf::Vector2f(650.f, 50.f), sf::Color::White, sf::Vector2f(75.f, 100.f + (i * 100.f)));
                    setupText(minigameText[i], std::to_string(i + 1), 18, sf::Color::Black, minigameButton[i]);
                    window.draw(minigameButton[i]);
                    window.draw(minigameText[i]);
                }
            }
            else if (currentChapter == 3)
            {
                sf::Text minigameText[4];
                for (int i = 0; i < 4; i++)
                {
                    setupButton(minigameButton[i], sf::Vector2f(650.f, 50.f), sf::Color::White, sf::Vector2f(75.f, 100.f + (i * 100.f)));
                    setupText(minigameText[i], std::to_string(i + 1), 18, sf::Color::Black, minigameButton[i]);
                    window.draw(minigameButton[i]);
                    window.draw(minigameText[i]);
                }
            }
            else if (currentChapter == 4)
            {
                sf::Text minigameText[3];
                for (int i = 0; i < 3; i++)
                {
                    setupButton(minigameButton[i], sf::Vector2f(650.f, 50.f), sf::Color::White, sf::Vector2f(75.f, 100.f + (i * 100.f)));
                    setupText(minigameText[i], std::to_string(i + 1), 18, sf::Color::Black, minigameButton[i]);
                    window.draw(minigameButton[i]);
                    window.draw(minigameText[i]);
                }
            }
            else if (currentChapter == 5)
            {
                sf::Text minigameText[5];
                for (int i = 0; i < 5; i++)
                {
                    setupButton(minigameButton[i], sf::Vector2f(650.f, 50.f), sf::Color::White, sf::Vector2f(75.f, 100.f + (i * 100.f)));
                    setupText(minigameText[i], std::to_string(i + 1), 18, sf::Color::Black, minigameButton[i]);
                    window.draw(minigameButton[i]);
                    window.draw(minigameText[i]);
                }
            }
            else if (currentChapter == 6)
            {
                sf::Text minigameText[3];
                for (int i = 0; i < 3; i++)
                {
                    setupButton(minigameButton[i], sf::Vector2f(650.f, 50.f), sf::Color::White, sf::Vector2f(75.f, 100.f + (i * 100.f)));
                    setupText(minigameText[i], std::to_string(i + 1), 18, sf::Color::Black, minigameButton[i]);
                    window.draw(minigameButton[i]);
                    window.draw(minigameText[i]);
                }
            }
        }
    }

    void drawResult()
    {
        resultMinigameScreen.setSize(sf::Vector2f(800.f, 600.f));
        sf::RectangleShape gameBlock(sf::Vector2f(800.f, 600.f));
        setupButton(gameBlock, sf::Vector2f(800.f, 600.f), sf::Color(169, 181, 223), sf::Vector2f(0.f, 0.f));
        window.draw(gameBlock);

        window.draw(BG);

        sf::RectangleShape charecterResultBox;
        sf::RectangleShape dialogueResultBox;
        setupButton(charecterResultBox, sf::Vector2f(200.f, 50.f), sf::Color::Black, sf::Vector2f(25.f, 375.f));
        setupButton(dialogueResultBox, sf::Vector2f(800.f, 200.f), sf::Color::Black, sf::Vector2f(0.f, 400.f));
        window.draw(dialogueResultBox);
        window.draw(charecterResultBox);

        // Character text setup
        sf::Text charecterResultText;
        setupText(charecterResultText, "[Player]", 24, sf::Color::White, charecterResultBox);
        window.draw(charecterResultText);

        std::wstring resultMSG;
        std::stringstream ss;
        ss << "HP: " << player.getHP() << "\n";
        ss << "Score: " << player.getScore() << "\n";
        ss << "Item: ";

        if (!player.getInventory().empty())
        {
            for (size_t i = 0; i < player.getInventory().size(); i++)
            {
                ss << player.getInventory()[i];
                if (i < player.getInventory().size() - 1)
                    ss << ", ";
            }
        }
        else
        {
            ss << "None";
        }

        resultMSG = utf8ToUtf32(ss.str());
        sf::Text resultText2;
        resultText2.setFont(font);
        resultText2.setString(resultMSG);
        resultText2.setCharacterSize(24);
        resultText2.setFillColor(sf::Color::White);
        resultText2.setPosition(10.f, 435.f); // Adjust position as needed

        window.draw(resultText2); // Now drawing sf::Text instead of std::wstring
    }
    void drawEvent()
    {
        eventScreen.setSize(sf::Vector2f(800.f, 600.f));
        resultEventScreen.setSize(sf::Vector2f(800.f, 600.f));
        sf::RectangleShape gameBlock(sf::Vector2f(800.f, 600.f));
        setupButton(gameBlock, sf::Vector2f(800.f, 600.f), sf::Color(169, 181, 223), sf::Vector2f(0.f, 0.f));
        window.draw(gameBlock);

        window.draw(BG);

        sf::RectangleShape charecterBox;
        sf::RectangleShape dialogueBox;
        setupButton(charecterBox, sf::Vector2f(200.f, 50.f), sf::Color::Black, sf::Vector2f(25.f, 375.f));
        setupButton(dialogueBox, sf::Vector2f(800.f, 200.f), sf::Color::Black, sf::Vector2f(0.f, 400.f));
        window.draw(dialogueBox);
        window.draw(charecterBox);

        // Character text setup
        sf::Text charecterText;
        setupText(charecterText, "[Random Event]", 24, sf::Color::White, charecterBox);
        window.draw(charecterText);

        std::wstring dialogueMSG;
        if (resultEventVisible)
        {
            charecterText.setString("[Player]");
            std::stringstream ss;
            ss << "HP: " << player.getHP() << "\n";
            ss << "Score: " << player.getScore() << "\n";
            ss << "Item: ";

            if (!player.getInventory().empty())
            {
                for (size_t i = 0; i < player.getInventory().size(); i++)
                {
                    ss << player.getInventory()[i];
                    if (i < player.getInventory().size() - 1)
                        ss << ", ";
                }
            }
            else
            {
                ss << "None";
            }

            dialogueMSG = utf8ToUtf32(ss.str());
            sf::Text resultText;
            resultText.setFont(font);
            resultText.setString(dialogueMSG);
            resultText.setCharacterSize(24);
            resultText.setFillColor(sf::Color::White);
            resultText.setPosition(10.f, 435.f); // Adjust position as needed

            window.draw(resultText); // Now drawing sf::Text instead of std::wstring
        }
        else
        {
            dialogueMSG = utf8ToUtf32(event_arr[event_random]);
            std::vector<std::wstring> wrappedLines = wrapText(dialogueMSG, font, 780);
            float yOffset = 35.f;
            for (const auto &line : wrappedLines)
            {
                sf::Text lineText(line, font, 24);
                lineText.setFillColor(sf::Color::White);
                lineText.setPosition(10.f, 400 + yOffset);
                window.draw(lineText);
                yOffset += 35.f; // Adjust the space between lines
            }
        }

        sf::RectangleShape chapterBlock(sf::Vector2f(700.f, 50.f));
        setupButton(chapterBlock, sf::Vector2f(700.f, 50.f), sf::Color::Green, sf::Vector2f(50.f, 0.f));
        window.draw(chapterBlock);

        sf::Text chapterText;
        setupText(chapterText, "Chapeter !!!: [Random Event]", 24, sf::Color::Black, chapterBlock);
        window.draw(chapterText);
    }

    void drawEnd()
    {
        if (player.getEnding() == "Car")
        {
            BG.setTexture(Screen[39]);
            BG.setScale(800.f / Screen[39].getSize().x, 600.f / Screen[39].getSize().y);
        }
        else if (player.getEnding() == "Bad")
        {
            BG.setTexture(Screen[38]);
            BG.setScale(800.f / Screen[38].getSize().x, 600.f / Screen[38].getSize().y);
        }
        else if (player.getEnding() == "Secret")
        {
            BG.setTexture(Screen[40]);
            BG.setScale(800.f / Screen[40].getSize().x, 600.f / Screen[40].getSize().y);
        }
        else if (player.getEnding() == "Fight")
        {
            BG.setTexture(Screen[41]);
            BG.setScale(800.f / Screen[41].getSize().x, 600.f / Screen[41].getSize().y);
        }
        else if (player.getEnding() == "Strategy")
        {
            BG.setTexture(Screen[37]);
            BG.setScale(800.f / Screen[37].getSize().x, 600.f / Screen[37].getSize().y);
        }
        else if (player.getEnding() == "Greatness")
        {
            BG.setTexture(Screen[42]);
            BG.setScale(800.f / Screen[42].getSize().x, 600.f / Screen[42].getSize().y);
        }
        window.draw(BG);

        sf::RectangleShape chapterBlock(sf::Vector2f(400.f, 50.f));
        setupButton(chapterBlock, sf::Vector2f(400.f, 50.f), sf::Color::Green, sf::Vector2f(200.f, 0.f));
        window.draw(chapterBlock);

        sf::Text chapterText;
        setupText(chapterText, "Scroce: " + std::to_string(player.getScore()), 24, sf::Color::Black, chapterBlock);
        window.draw(chapterText);
    }
};

int main()
{
    Game game;
    game.run();
    return 0;
}
