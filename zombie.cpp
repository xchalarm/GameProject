#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>  // สำหรับ std::find, std::remove
#include <windows.h>
using namespace std;

// โครงสร้างเก็บสถานะของผู้เล่น พร้อมชื่อ
struct Player {
    string name;
    int HP;
    int score;
    bool hasWeapon;
    vector<string> inventory;
};
// ฟังก์ชันรอให้ผู้เล่นกด Enter เพื่อดำเนินการต่อ
void pressEnterToContinue() {
     cout << "\nPress Enter to continue...";
     //cin.ignore();
     cin.get();
     cout << "\n";
 }
// ฟังก์ชันสำหรับแสดง "ภาพ" (ASCII art) ของแต่ละฉาก
void displayScene(const string &sceneTitle, const string &asciiArt) {
    system("cls"); // สำหรับ Linux/Unix; หากใช้ Windows ให้เปลี่ยนเป็น system("cls")
    cout << "=====================================" << endl;
    cout << sceneTitle << endl;
    cout << "=====================================" << endl;
    cout << asciiArt << endl;
    pressEnterToContinue();
}
// ฟังก์ชันสำหรับพิมพ์ ASCII art รูปต่าง ๆ
void printAsciiArt(const string &artName) {
    if (artName == "zombie") {
       // Zombie 1, 2, and 3 in a row
// Zombie 1, 2, and 3 in a row
cout << "       .-.             .-.             .-.\n";
cout << "      (o.o)           (o.o)           (o.o)\n";
cout << "       |=|             |=|             |=|\n";
cout << "       _|_             _|_            __|__\n";
cout << "    //.=|=.\\\\       //.=|=.\\\\       //.=|=.\\\\\n";
cout << "   // .=|=. \\\\     // .=|=. \\\\     // .=|=. \\\\ \n";
cout << "   \\\\ .=|=. //     \\\\ .=|=. //     \\\\ .=|=. //\n";
cout << "    \\\\(=)=.//       \\\\(=)=.//       \\\\(=)=.//\n";
cout << "     (😐 |:)         (X_X|:)         (💀|:)\n";
cout << "      || ||           || ||           || ||\n";
cout << "      () ()           () ()           () ()\n";
cout << "      || ||           || ||           || ||\n";
cout << "      || ||           || ||           || ||\n";
cout << "     ==' '=='        ==' '=='        ==' '=='\n";

    } else if (artName == "weapon") {
        cout << "        \n";
        cout << "        />__________________________________\n";
        cout << "[########[]_________________________________\\\n";
        cout << "        \\>__________________________________/\n";
        cout << "        \n";
    } else if (artName == "blood") {
        cout << "  ____\n";
        cout << " /\\' .\\    _____\n";
        cout << "/: \\___\\  / .  /\\\n";
        cout << "\\' / . / /____/..\\\n";
        cout << " \\/___/  \\'  '\\  /\n";
        cout << "          '\\  '\\/\n";
        cout << "            '\\_\\\n";
    } else if (artName == "map") {
        cout << "      _______\n";
        cout << "     /       \\\n";
        cout << "    /  MAP    \\\n";
        cout << "   /___________\\\n";
        cout << "   |           |\n";
        cout << "   |  X marks  |\n";
        cout << "   |  the spot |\n";
        cout << "   |___________|\n";
    } else {
        cout << "(ไม่มีภาพสำหรับ " << artName << ")\n";
    }
}
// ฟังก์ชันแสดงสถานะของผู้เล่น (รวมชื่อด้วย)
void displayStats(const Player &player) {
     cout << "==========================\n";
     cout << " Character Name: " << player.name << "\n";
     cout << " HP: " << player.HP << "\n";
     cout << " Score: " << player.score << "\n";
     cout << " Weapon Status: " << (player.hasWeapon ? "Armed" : "Unarmed") << "\n";
     cout << " Inventory: ";
     if (player.inventory.empty()) {
         cout << "Empty";
     } else {
         for (const auto &item : player.inventory) {
             cout << item << " ";
         }
     }
     cout << "\n==========================\n\n";
 }
// =======================
// ฟังก์ชันเหตุการณ์สุ่ม (Random Events)
// =======================
void randomEvent(Player &player) {
     int eventType = rand() % 3;
     if (eventType == 0) {
         cout << "\n[Random Event] You found a mysterious letter revealing clues about the outbreak.\n";
         cout << "Result: +3 HP, +5 Score\n";
         player.HP += 3;
         player.score += 5;
         printAsciiArt("map");
     } else if (eventType == 1) {
         cout << "\n[Random Event] You were suddenly attacked by a small group of zombies!\n";
         cout << "Result: -7 HP\n";
         player.HP -= 7;
         printAsciiArt("zombie");
     } else {
         cout << "\n[Random Event] You stumbled upon a hidden supply cache filled with valuable items!\n";
         cout << "Result: +5 HP, +10 Score\n";
         player.HP += 5;
         player.score += 10;
         printAsciiArt("weapon");
     }
     pressEnterToContinue();
 }
// =======================
// ฟังก์ชันมินิเกมในแต่ละบท (พร้อมบอกผลเลือด)
// =======================
void miniGameChapter1(Player &player) {
     cout << "Mini-game: Find the Hidden Key in the Drawer\n";
     cout << "You find an old desk in the room with three drawers. Try to guess which drawer contains the key (1, 2, or 3): ";
     int guess;
     cin >> guess;
     cin.ignore();
     int correct = rand() % 3 + 1;
     if (guess == correct) {
         cout << "\nYou opened the correct drawer and found a key with a mysterious symbol!\n";
         cout << "Result: +5 HP, +10 Score\n";
         player.HP += 5;
         player.score += 10;
         cout << "You gained 5 HP.\n";
         printAsciiArt("map");
     } else {
         cout << "\nYou opened the drawer but found nothing. Disappointment sets in.\n";
         cout << "Result: -5 HP, +5 Score\n";
         player.HP -= 5;
         player.score += 5;
         cout << "You lost 5 HP.\n";
         printAsciiArt("blood");
     }
     pressEnterToContinue();
 }
void miniGameChapter2(Player &player) {
     cout << "Mini-game: Test Your Combat Sharpness\n";
     cout << "Choose a number between 1 and 4 to select the best attack timing: ";
     int guess;
     cin >> guess;
     cin.ignore();
     int correct = rand() % 4 + 1;
     if (guess == correct) {
         cout << "\nYour timing was perfect! You successfully attacked and drove the zombies away.\n";
         cout << "Result: +15 Score\n";
         player.score += 15;
         cout << "Your HP remains strong (No HP loss).\n";
     } else {
         cout << "\nYour timing was off! The zombies counterattacked fiercely.\n";
         cout << "Result: -10 HP\n";
         player.HP -= 10;
         cout << "You lost 10 HP.\n";
         printAsciiArt("blood");
     }
     pressEnterToContinue();
 }
void miniGameChapter3(Player &player) {
     cout << "Mini-game: Choose a Path to Escape the Zombie Horde\n";
     cout << "Pick a number between 1 and 3 to find a safe route: ";
     int guess;
     cin >> guess;
     cin.ignore();
     int correct = rand() % 3 + 1;
     if (guess == correct) {
         cout << "\nYou chose a safe path and escaped without injury.\n";
         cout << "Result: +20 Score\n";
         player.score += 20;
         cout << "Your HP remains unchanged.\n";
     } else {
         cout << "\nThe path you chose led straight into a horde of zombies! You were attacked along the way.\n";
         cout << "Result: -15 HP\n";
         player.HP -= 15;
         cout << "You lost 15 HP.\n";
         printAsciiArt("zombie");
     }
     pressEnterToContinue();
 }
void miniGameChapter4(Player &player) {
     cout << "Mini-game: Guess the Code to Unlock the Escape Door\n";
     cout << "The lock system requires a number between 1 and 5. Try guessing the code: ";
     int guess;
     cin >> guess;
     cin.ignore();
     int correct = rand() % 5 + 1;
     if (guess == correct) {
         cout << "\nCorrect code! The door unlocks, and a ray of light fills your heart with hope.\n";
         cout << "Result: +25 Score\n";
         player.score += 25;
         cout << "Your health is restored by 25 HP.\n";
         printAsciiArt("weapon");
     } else {
         cout << "\nIncorrect code! The lock system is delayed, allowing zombies to get closer.\n";
         cout << "Result: -20 HP\n";
         player.HP -= 20;
         cout << "You lost 20 HP.\n";
         printAsciiArt("blood");
     }
     pressEnterToContinue();
 }
// มินิเกมสำหรับบทที่ 5 (Final Battle)
void miniGameChapter5(Player &player) {
     cout << "Mini-game: Test Your Endurance in the Final Battle\n";
     cout << "Choose a number between 1 and 3 to decide your defensive strategy: ";
     int guess;
     cin >> guess;
     cin.ignore();
     int correct = rand() % 3 + 1;
     if (guess == correct) {
         cout << "\nYour defense was precise! You successfully reduced the enemy's attack.\n";
         cout << "Result: +30 Score\n";
         player.score += 30;
         cout << "Your HP remains strong (No HP loss in this mini-game).\n";
     } else {
         cout << "\nYour defense was inaccurate! The enemy's attack was fierce.\n";
         cout << "Result: -25 HP\n";
         player.HP -= 25;
         cout << "You lost 25 HP.\n";
         printAsciiArt("blood");
     }
     pressEnterToContinue();
 }
// =======================
// ฟังก์ชันใช้ไอเท็ม (Item Usage) สำหรับเปลี่ยนแปลงเนื้อเรื่อง
// =======================
void useItems(Player &player) {
     if(player.inventory.empty()) {
         cout << "You have no items in your inventory that can be used.\n";
         pressEnterToContinue();
         return;
     }
     cout << "Your available items:\n";
     for (size_t i = 0; i < player.inventory.size(); i++) {
         cout << i+1 << ". " << player.inventory[i] << "\n";
     }
     cout << "Enter the item number you want to use (or 0 if you don't want to use any): ";
     int itemChoice;
     cin >> itemChoice;
     cin.ignore();
     if(itemChoice == 0) {
         cout << "You chose not to use any item.\n";
         pressEnterToContinue();
         return;
     }
     if(itemChoice < 1 || itemChoice > (int)player.inventory.size()) {
         cout << "Invalid choice.\n";
         pressEnterToContinue();
         return;
     }
     string chosenItem = player.inventory[itemChoice - 1];
     if(chosenItem == "Medical Kit") {
         cout << "You used a Medical Kit to heal your wounds, restoring 20 HP.\n";
         player.HP += 20;
         cout << "You gained 20 HP.\n";
         player.inventory.erase(remove(player.inventory.begin(), player.inventory.end(), "Medical Kit"), player.inventory.end());
         printAsciiArt("blood");
     } else if(chosenItem == "Ancient Map") {
         cout << "You used an Ancient Map to uncover secrets in the zombie world, gaining 15 points.\n";
         player.score += 15;
         cout << "Your HP remains unchanged.\n";
         player.inventory.erase(remove(player.inventory.begin(), player.inventory.end(), "Ancient Map"), player.inventory.end());
         printAsciiArt("map");
     } else if(chosenItem == "Updated Map") {
         cout << "You used an Updated Map to find a secret safe path, gaining 15 points.\n";
         player.score += 15;
         cout << "Your HP remains unchanged.\n";
         player.inventory.erase(remove(player.inventory.begin(), player.inventory.end(), "Updated Map"), player.inventory.end());
         printAsciiArt("map");
     } else {
         cout << "This item has no effect.\n";
     }
     pressEnterToContinue();
 }
// =======================
// เนื้อเรื่องและการตัดสินใจในแต่ละบท (Extended Narrative + ตัวเลือก 3 ตัวขึ้นไป)
// =======================
void displayIntro() {
     string asciiArt =
     "                                                               \n"
     " ██     ██ ███████  ██       ██████   ██████  ███    ███ ███████ \n"
     " ██     ██ ██       ██      ██       ██    ██ ████  ████ ██      \n"     
     " ██  █  ██ █████    ██      ██       ██    ██ ██ ████ ██ █████  \n" 
     " ██ ███ ██ ██       ██      ██       ██    ██ ██  ██  ██ ██      \n"
     "  ███ ███  ███████  ███████  ██████   ██████  ██      ██ ███████ \n";
     

     // Display "masterpiece" ASCII art as the opening scene
     printAsciiArt("masterpiece");
 
     displayScene("Welcome to the World of Zombies!", asciiArt);
 
     cout << "Your world has been overrun by ruthless and merciless zombies.\n";
     cout << "With every step you take, you will face choices filled with fear, sacrifice,\n";
     cout << "and unexpected random events.\n";
     cout << "If you wish to survive, you must rely on your knowledge, courage,\n";
     cout << "wit, and the items you gather along the way.\n\n";
 
     pressEnterToContinue();
 }
void chapter1(Player &player) {
     string asciiArt =
        "                                                               \n"
        "  ██████╗██╗  ██╗ █████╗ ██████╗ ████████╗███████╗██████╗     ██╗\n"
        " ██╔════╝██║  ██║██╔══██╗██╔══██╗╚══██╔══╝██╔════╝██╔══██╗    ██║\n"
        " ██║     ███████║███████║██████╔╝   ██║   █████╗  ██████╔╝    ██║\n"
        " ██║     ██╔══██║██╔══██║██╔═══╝    ██║   ██╔══╝  ██╔══██╗    ██║\n"
        " ╚██████╗██║  ██║██║  ██║██║        ██║   ███████╗██║  ██║    ██║\n"
        "  ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝        ╚═╝   ╚══════╝╚═╝  ╚═╝    ╚═╝\n";

     displayScene("Chapter 1: Waking Up in Confusion", asciiArt);
 
     cout << "You wake up in an abandoned house that has been left untouched for years.\n";
     cout << "The floor is covered in dust, and darkness looms around you.\n";
     cout << "----------------------------------------------------------\n";
     cout << "Options:\n";
     cout << "   \n";
     cout << "1. Leave the house immediately to escape the overwhelming darkness.\n";
     cout << "   Result: HP -10, Score +10\n";
     cout << "   (You lose 10 HP)\n";
     cout << "   \n";
     cout << "2. Hide inside the house and wait for things to calm down.\n";
     cout << "   Result: HP -5, Score +5\n";
     cout << "   (You lose 5 HP)\n";
     cout << "   \n";
     cout << "3. Carefully inspect the house for useful items.\n";
     cout << "   Result: HP +5, Score +15, Gain \"Ancient Map\"\n";
     cout << "   (You regain 5 HP)\n";
     cout << "   \n";
     cout << "Enter your choice: ";
     int choice;
     cin >> choice;
     cin.ignore();
     cout << "\n";
     if (choice == 1) {
          cout << "You decide to leave the house immediately with courage.\n";
          player.HP -= 10;
          player.score += 10;
          cout << "You lose 10 HP.\n";
     } else if (choice == 2) {
          cout << "You choose to hide inside, waiting for the situation to settle down.\n";
          player.HP -= 5;
          player.score += 5;
          cout << "You lose 5 HP.\n";
     } else if (choice == 3) {
          cout << "You carefully search the house and find an old journal along with a map.\n";
          player.HP += 5;
          player.score += 15;
          player.inventory.push_back("Ancient Map");
          cout << "You regain 5 HP.\n";
          printAsciiArt("map");
     } else {
          cout << "Invalid choice!\n";
          exit(0);
     }
     displayStats(player);
     pressEnterToContinue();
     miniGameChapter1(player);
     displayStats(player);
     randomEvent(player);
 }
void chapter2(Player &player) {
     string asciiArt =
 
     "                                                               \n"
     "  ██████╗██╗  ██╗ █████╗ ██████╗ ████████╗███████╗██████╗     ██████╗ \n"
     " ██╔════╝██║  ██║██╔══██╗██╔══██╗╚══██╔══╝██╔════╝██╔══██╗    ╚════██╗\n"
     " ██║     ███████║███████║██████╔╝   ██║   █████╗  ██████╔╝     █████╔╝\n"
     " ██║     ██╔══██║██╔══██║██╔═══╝    ██║   ██╔══╝  ██╔══██╗    ██╔═══╝ \n" 
     " ╚██████╗██║  ██║██║  ██║██║        ██║   ███████╗██║  ██║    ███████╗\n"
     "  ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝        ╚═╝   ╚══════╝╚═╝  ╚═╝    ╚══════╝\n";
 
     displayScene("Chapter 2: Encountering a Survivor", asciiArt);
 
     cout << "During your journey in the darkness, you encounter another survivor with vast experience.\n";
     cout << "----------------------------------------------------------\n";
     cout << "Options:\n";
     cout << "1. Accept help and travel together.\n";
     cout << "   Result: Gain a weapon, receive a \"Medical Kit\", Score +20\n";
     cout << "   (Your wounds begin to heal and stop bleeding)\n";
     cout << "   \n";
     cout << "2. Travel alone to prove your ability in this harsh world.\n";
     cout << "   Result: HP -15\n";
     cout << "   (You lose 15 HP)\n";
     cout << "   \n";
     cout << "3. Ask for more information to gain clues and knowledge.\n";
     cout << "   Result: Score +10, Receive \"Updated Map\"\n";
     cout << "   (Your health remains unchanged)\n";
     cout << "   \n";
     cout << "Enter your choice number: ";
     int choice;
     cin >> choice;
     cin.ignore();
     cout << "\n";
     if (choice == 1) {
          cout << "You decide to join him.\n";
          player.hasWeapon = true;
          player.inventory.push_back("Medical Kit");
          player.score += 20;
          cout << "Your wounds heal (No additional blood loss)\n";
          printAsciiArt("weapon");
     } else if (choice == 2) {
          cout << "You choose to travel alone.\n";
          player.HP -= 15;
          cout << "You lose 15 HP.\n";
          printAsciiArt("blood");
     } else if (choice == 3) {
          cout << "You ask questions and receive additional clues.\n";
          player.score += 10;
          player.inventory.push_back("Updated Map");
          cout << "Your health remains unchanged.\n";
          printAsciiArt("map");
     } else {
          cout << "Invalid choice!\n";
          exit(0);
     }
     displayStats(player);
     pressEnterToContinue();
     miniGameChapter2(player);
     displayStats(player);
     randomEvent(player);
 }
void chapterItemImpact(Player &player) {
     string asciiArt =
 
          "                                                      \n"
          "  ███████╗██████╗ ███████╗ ██████╗██╗ █████╗ ██╗      \n"
          "  ██╔════╝██╔══██╗██╔════╝██╔════╝██║██╔══██╗██║      \n"
          "  ███████╗██████╔╝█████╗  ██║     ██║███████║██║      \n"
          "  ╚════██║██╔═══╝ ██╔══╝  ██║     ██║██╔══██║██║      \n"
          "  ███████║██║     ███████╗╚██████╗██║██║  ██║███████╗ \n"
          "  ╚══════╝╚═╝     ╚══════╝ ╚═════╝╚═╝╚═╝  ╚═╝╚══════╝ \n";
 
     displayScene("Chapter 2.5: Changing Fate with Items(SPECIAL CHAPTER)", asciiArt);
     cout << "During your journey, you encounter a challenging situation and have the opportunity to use an item to change your fate.\n";
     cout << "You have the following options:\n";
     cout << "   \n";
     cout << "1. Use \"Medical Kit\" (if available) to heal wounds and restore health.\n";
     cout << "   Result: HP +15, Score +10\n";
     cout << "   \n";
     cout << "2. Use \"Ancient Map\" (if available) to discover a hidden safe route.\n";
     cout << "   Result: Score +20 (Your health remains unchanged)\n";
     cout << "   \n";
     cout << "3. Use \"Updated Map\" (if available) to reveal secrets of this location.\n";
     cout << "   Result: Score +15 (Your health remains unchanged)\n";
     cout << "   \n";
     cout << "4. Do not use any items.\n";
     cout << "   \n";
     cout << "Enter your choice number: ";
     int option;
     cin >> option;
     cin.ignore();
     cout << "\n";
     switch(option) {
          case 1: {
               if(find(player.inventory.begin(), player.inventory.end(), "Medical Kit") != player.inventory.end()){
                    cout << "You use the Medical Kit to heal your wounds.\n";
                    player.HP += 15;
                    player.score += 10;
                    cout << "Result: HP +15, Score +10 (Your health increased by 15 points)\n";
                    player.inventory.erase(remove(player.inventory.begin(), player.inventory.end(), "Medical Kit"), player.inventory.end());
                    printAsciiArt("blood");
               } else {
                    cout << "You don't have a \"Medical Kit\" to use.\n";
               }
               break;
          }
          case 2: {
               if(find(player.inventory.begin(), player.inventory.end(), "Ancient Map") != player.inventory.end()){
                    cout << "You use the Ancient Map to discover a hidden safe route.\n";
                    player.score += 20;
                    cout << "Result: Score +20 (Your health remains unchanged)\n";
                    player.inventory.erase(remove(player.inventory.begin(), player.inventory.end(), "Ancient Map"), player.inventory.end());
                    printAsciiArt("map");
               } else {
                    cout << "You don't have an \"Ancient Map\" to use.\n";
               }
               break;
          }
          case 3: {
               if(find(player.inventory.begin(), player.inventory.end(), "Updated Map") != player.inventory.end()){
                    cout << "You use the Updated Map to reveal secrets of this location.\n";
                    player.score += 15;
                    cout << "Result: Score +15 (Your health remains unchanged)\n";
                    player.inventory.erase(remove(player.inventory.begin(), player.inventory.end(), "Updated Map"), player.inventory.end());
                    printAsciiArt("map");
               } else {
                    cout << "You don't have an \"Updated Map\" to use.\n";
               }
               break;
          }
          case 4: {
               cout << "You choose not to use any items to change your fate.\n";
               break;
          }
          default:
               cout << "Invalid choice.\n";
               break;
     }
     pressEnterToContinue();
 }
void chapter3(Player &player) {
     string asciiArt =
        "                                                               \n"
        "  ██████╗██╗  ██╗ █████╗ ██████╗ ████████╗███████╗██████╗     ██████╗ \n"
        " ██╔════╝██║  ██║██╔══██╗██╔══██╗╚══██╔══╝██╔════╝██╔══██╗    ╚════██╗\n"
        " ██║     ███████║███████║██████╔╝   ██║   █████╗  ██████╔╝     █████╔╝\n"
        " ██║     ██╔══██║██╔══██║██╔═══╝    ██║   ██╔══╝  ██╔══██╗     ╚═══██╗\n"
        " ╚██████╗██║  ██║██║  ██║██║        ██║   ███████╗██║  ██║    ██████╔╝\n"
        "  ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝        ╚═╝   ╚══════╝╚═╝  ╚═╝    ╚═════╝ \n";

     displayScene("Chapter 3: Confronting the Zombie Horde", asciiArt);
     cout << "As you travel through the darkness, you encounter a fierce horde of zombies.\n";
     cout << "----------------------------------------------------------\n";
     cout << "Choices:\n";
     cout << "1. Use a weapon to fight the zombies.\n";
     cout << "   Result: If you have a weapon – Score +30 (No additional health loss)\n";
     cout << "           If you don't have a weapon – HP -30 (You lose 30 HP)\n";
     cout << "   \n";
     cout << "2. Run away.\n";
     cout << "   Result: HP -10 (You lose 10 HP)\n";
     cout << "   \n";
     cout << "3. Hide in the darkness.\n";
     cout << "   Result: HP -5, Score +5 (You lose 5 HP)\n";
     cout << "   \n";
     cout << "Enter your choice number: ";
     int choice;
     cin >> choice;
     cin.ignore();
     cout << "\n";
     if (choice == 1) {
          if (player.hasWeapon) {
               cout << "You use your skills and weapon to fight off the zombies.\n";
               player.score += 30;
               cout << "Result: Score +30, No additional health loss.\n";
               printAsciiArt("weapon");
          } else {
               cout << "You try to fight with your bare hands, but without a weapon, you suffer severe injuries.\n";
               player.HP -= 30;
               cout << "Result: HP -30, You lost 30 HP.\n";
               printAsciiArt("blood");
          }
          } else if (choice == 2) {
          cout << "You choose to run away.\n";
          player.HP -= 10;
          cout << "Result: HP -10, You lost 10 HP.\n";
          printAsciiArt("zombie");
          } else if (choice == 3) {
          cout << "You choose to hide in the darkness and wait for the danger to pass.\n";
          player.HP -= 5;
          player.score += 5;
          cout << "Result: HP -5, Score +5, You lost 5 HP.\n";
          } else {
          cout << "Invalid choice!\n";
          exit(0);
          }
    displayStats(player);
    pressEnterToContinue();
    miniGameChapter3(player);
    displayStats(player);
    randomEvent(player);
}
string chapter4(Player &player) {
    string asciiArt =

        "                                                               \n"
        "  ██████╗██╗  ██╗ █████╗ ██████╗ ████████╗███████╗██████╗     ██   ██╗\n"
        " ██╔════╝██║  ██║██╔══██╗██╔══██╗╚══██╔══╝██╔════╝██╔══██╗    ██   ██║ \n"
        " ██║     ███████║███████║██████╔╝   ██║   █████╗  ██████╔╝    ███████║ \n"
        " ██║     ██╔══██║██╔══██║██╔═══╝    ██║   ██╔══╝  ██╔══██╗     ╚═══██║ \n"
        " ╚██████╗██║  ██║██║  ██║██║        ██║   ███████╗██║  ██║         ██║ \n"
        "  ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝        ╚═╝   ╚══════╝╚═╝  ╚═╝         ╚═╝ \n";

    displayScene("Chapter 4: The Final Decision", asciiArt);
    cout << "After overcoming many trials, you have reached the final decision point.\n";
    cout << "----------------------------------------------------------\n";
    cout << "Choices:\n";
    cout << "1. Use a parked car to escape the city.\n";
    cout << "   Result: If survived - Score +40, (Your health remains strong)\n";
    cout << "   \n";
    cout << "2. Walk through the zombie-infested area to find shelter.\n";
    cout << "   Result: If survived - Score +30, (You lose some health)\n";
    cout << "   \n";
    cout << "3. Search for a secret path using the map you obtained.\n";
    cout << "   Result: If survived - Score +50, (Your health recovers)\n";
    cout << "   \n";
    cout << "Enter your choice number: ";
    int choice;
    cin >> choice;
    cin.ignore();
    cout << "\n";
    string endingText;
    if (choice == 1) {
        cout << "You chose to escape the city by car with speed and determination.\n";
        player.score += 40;
        cout << "Result: Score +40, Your health remains strong (No additional HP loss).\n";
        endingText = "Good Ending: Escape with Speed";
    } else if (choice == 2) {
        cout << "You chose to walk through the zombie-infested area with endurance and courage.\n";
        player.score += 30;
        player.HP -= 10;
        cout << "Result: Score +30, HP decreased by 10 (You lost 10 HP).\n";
        endingText = "Neutral Ending: The Journey of Courage";
    } else if (choice == 3) {
        cout << "You chose to find a secret path using a map, relying on your wit and accumulated knowledge.\n";
        player.score += 50;
        cout << "Result: Score +50, Your health remains in a good recovery state (No additional HP loss).\n";
        endingText = "Secret Ending: The Discovery of Salvation";
    } else {
        cout << "Invalid choice!\n";
        exit(0);
    }    
    displayStats(player);
    pressEnterToContinue();
    miniGameChapter4(player);
    displayStats(player);
    return endingText;
}
string chapter5(Player &player) {
    string asciiArt =

        "                                                               \n"
        "  ██████╗██╗  ██╗ █████╗ ██████╗ ████████╗███████╗██████╗     ███████╗ \n"
        " ██╔════╝██║  ██║██╔══██╗██╔══██╗╚══██╔══╝██╔════╝██╔══██╗    ██╔════╝ \n"
        " ██║     ███████║███████║██████╔╝   ██║   █████╗  ██████╔╝    █████╗   \n"
        " ██║     ██╔══██║██╔══██║██╔═══╝    ██║   ██╔══╝  ██╔══██╗    ╚════██╗  \n"
        " ╚██████╗██║  ██║██║  ██║██║        ██║   ███████╗██║  ██║    ██████╔╝  \n"
        "  ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝        ╚═╝   ╚══════╝╚═╝  ╚═╝    ╚═════╝   \n";

    displayScene("Chapter 5: The Final Battle", asciiArt);
    cout << "In the final chapter of your journey, you must face the biggest and most ruthless enemy.\n";
    cout << "This final battle is a true test of courage and endurance.\n";
    cout << "----------------------------------------------------------\n";
    cout << "Choices:\n";
    cout << "1. Engage the enemy head-on.\n";
    cout << "   Result: If survived: Score +35, HP Decreased by 20 (You lost 20 HP)\n";
    cout << "   \n";
    cout << "2. Use an evasive strategy and find the right moment to strike.\n";
    cout << "   Result: If survived: Score +25, HP Decreased by 10 (You lost 10 HP)\n";
    cout << "   \n";
    cout << "3. Use the collected item and adjust your strategy.\n";
    cout << "   Result: If survived: Score +45, Net HP Increase +5 (You lost 5 HP but recovered 10 HP)\n";
    cout << "   \n";
    cout << "Enter your choice number: ";
    int choice;
    cin >> choice;
    cin.ignore();
    cout << "\n";
    string endingText;
    if (choice == 1) {
         cout << "You chose to engage the enemy head-on with decisiveness and courage.\n";
         player.score += 35;
         player.HP -= 20;
         cout << "Result: Score +35, HP Decreased by 20 (You lost 20 HP)\n";
         endingText = "Final Ending: Fight with a Courageous Heart";
    } else if (choice == 2) {
         cout << "You chose to use an evasive strategy and strike at the right moment wisely.\n";
         player.score += 25;
         player.HP -= 10;
         cout << "Result: Score +25, HP Decreased by 10 (You lost 10 HP)\n";
         endingText = "Final Ending: Strategy of Victory";
    } else if (choice == 3) {
         cout << "You chose to use an item and adjusted your strategy carefully.\n";
         player.score += 45;
         // ฟื้นตัวสุทธิ 5 แต้ม: ลดไป 5 แต้มแต่ฟื้นเพิ่ม 10
         player.HP = player.HP - 5 + 10;
         cout << "Result: Score +45, Net HP Increase +5 (You lost 5 HP but recovered 10 HP)\n";
         endingText = "Final Ending: The Battle of Greatness";
    } else {
         cout << "Invalid choice!\n";
         exit(0);
    }
    displayStats(player);
    pressEnterToContinue();
    miniGameChapter5(player);
    displayStats(player);
    return endingText;
}
// =======================
// ฟังก์ชันสรุปตอนจบ (Final Summary & Item Usage)
// =======================

void finalSummary(Player &player, const string &endingText) {
    system("cls");
    cout << "=====================================\n";
    cout << "         Final Game Summary         \n";
    cout << "=====================================\n";
    cout << "Character Name: " << player.name << "\n";
    cout << "HP: " << player.HP << "\n";
    cout << "Score: " << player.score << "\n";
    cout << "Ending: " << endingText << "\n";
    cout << "Items remaining in the bag: ";
    if(player.inventory.empty()){
         cout << "Empty";
    } else {
         for(auto &item : player.inventory){
             cout << item << " ";
         }
    }
    cout << "\n=====================================\n";
    // โบนัสคะแนนจากเลือด (HP)
    int bonus = player.HP / 2;
    cout << "Bonus points from remaining health: " << bonus << "\n";
    player.score += bonus;
    pressEnterToContinue();
    char use;
    do {
         cout << "Do you want to use an item? (y/n): ";
         cin >> use;
         cin.ignore();
         if(use == 'y' || use == 'Y'){
             useItems(player);
             cout << "Status updated after using the item:\n";
             cout << "HP: " << player.HP << "\n";
             cout << "Score: " << player.score << "\n";
         }
    } while(use == 'y' || use == 'Y');
    cout << "\n---------- Final Results ----------\n";
    displayStats(player);
    cout << 
        "  ████████╗██╗  ██╗███████╗    ███████╗███╗   ██╗██████╗ \n"
        "  ╚══██╔══╝██║  ██║██╔════╝    ██╔════╝████╗  ██║██╔══██╗\n"
        "     ██║   ███████║█████╗      █████╗  ██╔██╗ ██║██║  ██║\n"
        "     ██║   ██╔══██║██╔══╝      ██╔══╝  ██║╚██╗██║██║  ██║\n"
        "     ██║   ██║  ██║███████╗    ███████╗██║ ╚████║██████╔╝\n"
        "     ╚═╝   ╚═╝  ╚═╝╚══════╝    ╚══════╝╚═╝  ╚═══╝╚═════╝ \n"
        "                                                               \n"
        "             Thank you for playing this game!\n";

    pressEnterToContinue();
}
// =======================
// ฟังก์ชันหลัก
// =======================

int main() {
    SetConsoleOutputCP(65001); 
    srand(time(0)); // Seed สำหรับสุ่มตัวเลข
    Player player;
    // กำหนดสถานะเริ่มต้นของผู้เล่น
    player.HP = 100;
    player.score = 0;
    player.hasWeapon = false;
    // รับชื่อตัวละคร
    system("cls");
    cout << "Please enter your character's name: ";
    getline(cin, player.name);
    // เริ่มต้นเกม
    displayIntro();
    chapter1(player);
    if (player.HP <= 0) { system("cls"); cout << "The game ends in Chapter 1\n"; exit(0); }
    chapter2(player);
    if (player.HP <= 0) { system("cls"); cout << "The game ends in Chapter 2\n"; exit(0); }
    chapterItemImpact(player);
    if (player.HP <= 0) { system("cls"); cout << "The game ends in Chapter 2.5\n"; exit(0); }
    chapter3(player);
    if (player.HP <= 0) { system("cls"); cout << "The game ends in Chapter 3\n"; exit(0); }
    string ending4 = chapter4(player);
    if (player.HP <= 0) { system("cls"); cout << "The game ends in Chapter 4\n"; exit(0); }
    string ending5 = chapter5(player);
    if (player.HP <= 0) { system("cls"); cout << "The game ends in Chapter 5\n"; exit(0); }
    // รวมข้อความฉากจบจากบทที่ 4 และ 5เข้าด้วยกัน
    string finalEnding = ending4 + " / " + ending5;
    finalSummary(player, finalEnding);
    return 0;
}