#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <typeinfo>
#include <ctime>
#include <sstream>
#include <map>
#include <filesystem>

template<typename T>
class Logger {
private:
    std::ofstream logFile;
public:
    Logger(const std::string& filename) {
        logFile.open(filename, std::ios::app);
        if (!logFile.is_open()) {
            throw std::runtime_error("Unable to open log file");
        }
    }

    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void log(const T& message) {
        time_t now = time(0);
        tm ltm;
        localtime_s(&ltm, &now);

        logFile << "[" << 1900 + ltm.tm_year << "-"
            << 1 + ltm.tm_mon << "-"
            << ltm.tm_mday << " "
            << ltm.tm_hour << ":"
            << ltm.tm_min << ":"
            << ltm.tm_sec << "] "
            << message << std::endl;
    }
};

class Item {
protected:
    std::string name;
public:
    Item(const std::string& n) : name(n) {}
    virtual ~Item() {}

    virtual void use() = 0;
    virtual std::string getInfo() const {
        return name;
    }

    virtual std::string serialize() const {
        std::ostringstream oss;
        oss << typeid(*this).name() << "\n" << name;
        return oss.str();
    }

    virtual void deserialize(std::istream& iss) {
        std::getline(iss, name);
    }

    std::string getName() const { return name; }
};

class Weapon : public Item {
private:
    int attackBonus;
public:
    Weapon(const std::string& n, int atk)
        : Item(n), attackBonus(atk) {
    }

    void use() override {
        std::cout << "Equipped " << name << " (+" << attackBonus << " attack)" << std::endl;
    }

    std::string getInfo() const override {
        return name + " (+" + std::to_string(attackBonus) + " attack)";
    }

    std::string serialize() const override {
        std::ostringstream oss;
        oss << Item::serialize() << "\n" << attackBonus;
        return oss.str();
    }

    void deserialize(std::istream& iss) override {
        Item::deserialize(iss);
        iss >> attackBonus;
        iss.ignore();
    }

    int getAttackBonus() const { return attackBonus; }
};

class HealthPotion : public Item {
private:
    int healAmount;
public:
    HealthPotion(const std::string& n, int heal)
        : Item(n), healAmount(heal) {
    }
    void use() override {
        std::cout << "Drank " << name << " and healed " << healAmount << " HP" << std::endl;
    }
    std::string getInfo() const override {
        return name + " (heals " + std::to_string(healAmount) + " HP)";
    }

    std::string serialize() const override {
        std::ostringstream oss;
        oss << Item::serialize() << "\n" << healAmount;
        return oss.str();
    }

    void deserialize(std::istream& iss) override {
        Item::deserialize(iss);
        iss >> healAmount;
        iss.ignore();
    }

    int getHealAmount() const { return healAmount; }
};
class Inventory {
private:
    std::vector<std::shared_ptr<Item>> items;
public:
    void addItem(std::shared_ptr<Item> item) {
        items.push_back(item);
    }
    void removeItem(const std::string& itemName) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if ((*it)->getName() == itemName) {
                items.erase(it);
                return;
            }
        }
        throw std::runtime_error("Item not found in inventory");
    }

    void display() const {
        if (items.empty()) {
            std::cout << "Inventory is empty" << std::endl;
            return;
        }

        std::cout << "Inventory:" << std::endl;
        for (const auto& item : items) {
            std::cout << "- " << item->getInfo() << std::endl;
        }
    }

    std::shared_ptr<Item> getItem(const std::string& itemName) {
        for (auto& item : items) {
            if (item->getName() == itemName) {
                return item;
            }
        }
        return nullptr;
    }

    bool isEmpty() const {
        return items.empty();
    }

    std::string serialize() const {
        std::ostringstream oss;
        oss << items.size() << "\n";
        for (const auto& item : items) {
            oss << item->serialize() << "\n";
        }
        return oss.str();
    }

    void deserialize(std::istream& iss) {
        items.clear();
        int count;
        iss >> count;
        iss.ignore();

        for (int i = 0; i < count; ++i) {
            std::string type;
            std::getline(iss, type);

            std::shared_ptr<Item> item;
            if (type.find("Weapon") != std::string::npos) {
                item = std::make_shared<Weapon>("", 0); 
            }
            else if (type.find("HealthPotion") != std::string::npos) {
                item = std::make_shared<HealthPotion>("", 0); 
            }

            if (item) {
                item->deserialize(iss);
                items.push_back(item);
            }
        }
    }
};

class Entity {
protected:
    std::string name;
    int health;
    int maxHealth;
    int attack;
    int defense;
public:
    Entity(const std::string& n, int h, int a, int d)
        : name(n), health(h), maxHealth(h), attack(a), defense(d) {
    }

    virtual ~Entity() {}

    void takeDamage(int damage) {
        if (damage > 0) {
            health -= damage;
            if (health < 0) health = 0;
        }
    }

    bool isAlive() const {
        return health > 0;
    }

    virtual void attackTarget(Entity& target) {
        int damage = attack - target.defense;
        if (damage > 0) {
            target.takeDamage(damage);
            std::cout << name << " attacks " << target.name << " for " << damage << " damage!" << std::endl;
        }
        else {
            std::cout << name << " attacks " << target.name << ", but it has no effect!" << std::endl;
        }
    }

    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << "/" << maxHealth
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    std::string getName() const { return name; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }

    void heal(int amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
        std::cout << name << " heals for " << amount << " HP!" << std::endl;
    }

    void setAttack(int a) { attack = a; }
    void setDefense(int d) { defense = d; }
};

class Character : public Entity {
private:
    int level;
    int experience;
    Inventory inventory;
public:
    Character(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d), level(1), experience(0) {
    }

    void gainExperience(int exp) {
        experience += exp;
        std::cout << name << " gained " << exp << " experience points!" << std::endl;

        while (experience >= 100) {
            levelUp();
            experience -= 100;
        }
    }

    void levelUp() {
        level++;
        maxHealth += 10;
        health = maxHealth;
        attack += 2;
        defense += 2;
        std::cout << name << " leveled up to level " << level << "!" << std::endl;
    }

    void displayInfo() const override {
        std::cout << "Name: " << name << ", HP: " << health << "/" << maxHealth
            << ", Attack: " << attack << ", Defense: " << defense
            << ", Level: " << level << ", Experience: " << experience << "/100" << std::endl;
    }

    void useItem(const std::string& itemName) {
        auto item = inventory.getItem(itemName);
        if (item) {
            item->use();

            if (auto potion = std::dynamic_pointer_cast<HealthPotion>(item)) {
                heal(potion->getHealAmount());
                inventory.removeItem(itemName);
            }
            else if (auto weapon = std::dynamic_pointer_cast<Weapon>(item)) {
                attack += weapon->getAttackBonus();
                std::cout << "Attack increased by " << weapon->getAttackBonus() << std::endl;
            }
        }
        else {
            throw std::runtime_error("Item not found in inventory");
        }
    }

    void addToInventory(std::shared_ptr<Item> item) {
        inventory.addItem(item);
    }

    void showInventory() const {
        inventory.display();
    }

    Inventory& getInventory() { return inventory; }

    int getLevel() const { return level; }
    int getExperience() const { return experience; }

    std::string serialize() const {
        std::ostringstream oss;
        oss << name << "\n"
            << health << "\n"
            << maxHealth << "\n"
            << attack << "\n"
            << defense << "\n"
            << level << "\n"
            << experience << "\n";
        oss << inventory.serialize();
        return oss.str();
    }

    void deserialize(std::istream& iss) {
        std::string line;
        std::getline(iss, name);
        iss >> health >> maxHealth >> attack >> defense >> level >> experience;
        iss.ignore();
        inventory.deserialize(iss);
    }
};

class Monster : public Entity {
public:
    Monster(const std::string& n, int h, int a, int d) : Entity(n, h, a, d) {}

    virtual int getExperienceReward() const = 0;
};

class Goblin : public Monster {
public:
    Goblin() : Monster("Goblin", 30, 8, 3) {}

    int getExperienceReward() const override { return 20; }
};

class Dragon : public Monster {
public:
    Dragon() : Monster("Dragon", 70, 10, 8) {}

    void attackTarget(Entity& target) override {
        if (rand() % 100 < 20) {
            int damage = (attack * 2) - target.getDefense();
            if (damage > 0) {
                target.takeDamage(damage);
                std::cout << name << " breathes fire on " << target.getName()
                    << " for " << damage << " critical damage!" << std::endl;
            }
            else {
                std::cout << name << " breathes fire on " << target.getName()
                    << ", but it has no effect!" << std::endl;
            }
        }
        else {
            Monster::attackTarget(target);
        }
    }
    int getExperienceReward() const override { return 100; }
};

class Skeleton : public Monster {
public:
    Skeleton() : Monster("Skeleton", 40, 7, 5) {}

    void attackTarget(Entity& target) override {
        Monster::attackTarget(target);
        if (rand() % 100 < 10) {
            std::cout << name << " attacks again!" << std::endl;
            Monster::attackTarget(target);
        }
    }
    int getExperienceReward() const override { return 10; }
};

class Game {
private:
    std::unique_ptr<Character> player;
    Logger<std::string> logger;
    bool gameRunning;

    void saveGame() {
        std::ofstream saveFile("savegame.txt");
        if (!saveFile.is_open()) {
            throw std::runtime_error("Unable to open save file");
        }

        saveFile << player->serialize();
        saveFile.close();
        std::cout << "Game saved successfully!" << std::endl;
        logger.log("Game saved");
    }

    void loadGame() {
        std::ifstream saveFile("savegame.txt");
        if (!saveFile.is_open()) {
            throw std::runtime_error("No save game found");
        }

        player = std::make_unique<Character>("", 0, 0, 0);
        player->deserialize(saveFile);
        saveFile.close();

        std::cout << "Game loaded successfully!" << std::endl;
        logger.log("Game loaded");
    }

    void deleteProgress() {
        if (std::remove("savegame.txt") == 0) {
            std::cout << "Progress deleted successfully!" << std::endl;
            logger.log("Game progress deleted");
        }
        else {
            std::cout << "No save file found to delete or error occurred." << std::endl;
        }
    }

    void createCharacter() {
        std::string name;
        std::cout << "Enter your character's name: ";
        std::getline(std::cin, name);

        player = std::make_unique<Character>(name, 100, 10, 7);

        player->addToInventory(std::make_shared<Weapon>("Sword", 3));
        player->addToInventory(std::make_shared<HealthPotion>("Health Potion", 20));

        std::cout << "Character created successfully!" << std::endl;
        logger.log("New character created: " + name);
    }

    void battle() {
        std::unique_ptr<Monster> monster;
        int monsterType = rand() % 3;

        switch (monsterType) {
        case 0: monster = std::make_unique<Goblin>(); break;
        case 1: monster = std::make_unique<Dragon>(); break;
        case 2: monster = std::make_unique<Skeleton>(); break;
        }

        std::cout << "A wild " << monster->getName() << " appears!" << std::endl;
        logger.log("Battle started with " + monster->getName());

        while (player->isAlive() && monster->isAlive()) {
            std::cout << "\nYour turn:" << std::endl;
            player->displayInfo();
            monster->displayInfo();

            std::cout << "\n1. Attack\n2. Use item\n3. Try to flee\nChoose action: ";
            int choice;
            std::cin >> choice;
            std::cin.ignore();

            try {
                switch (choice) {
                case 1: {
                    player->attackTarget(*monster);
                    logger.log(player->getName() + " attacks " + monster->getName());
                    break;
                }
                case 2: {
                    if (player->getInventory().isEmpty()) {
                        std::cout << "Your inventory is empty!" << std::endl;
                        continue;
                    }

                    std::cout << "Your inventory:" << std::endl;
                    player->showInventory();

                    std::cout << "Enter item name to use: ";
                    std::string itemName;
                    std::getline(std::cin, itemName);

                    player->useItem(itemName);
                    logger.log(player->getName() + " used item: " + itemName);
                    continue;
                }
                case 3: {
                    if (rand() % 100 < 30) {
                        std::cout << "You successfully fled from battle!" << std::endl;
                        logger.log(player->getName() + " fled from battle");
                        return;
                    }
                    else {
                        std::cout << "You failed to flee!" << std::endl;
                        logger.log(player->getName() + " failed to flee");
                    }
                    break;
                default:
                    std::cout << "Invalid choice! You hesitate and lose your turn." << std::endl;
                    continue;
                }
                }
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
                continue;
            }

            if (monster->isAlive()) {
                std::cout << "\nEnemy's turn:" << std::endl;
                monster->attackTarget(*player);
                logger.log(monster->getName() + " attacks " + player->getName());
            }
        }

        if (!player->isAlive()) {
            std::cout << "You were defeated by the " << monster->getName() << "!" << std::endl;
            logger.log(player->getName() + " was defeated by " + monster->getName());
            gameRunning = false;
        }
        else {
            std::cout << "You defeated the " << monster->getName() << "!" << std::endl;
            int exp = monster->getExperienceReward();
            player->gainExperience(exp);
            logger.log(player->getName() + " defeated " + monster->getName() + " and gained " + std::to_string(exp) + " XP");

            // Восстановление здоровья после боя (5-25 HP)
            int healAmount = 5 + rand() % 21; // 5-25 случайное число
            player->heal(healAmount);
            logger.log(player->getName() + " recovered " + std::to_string(healAmount) + " HP after battle");
        }
    }

    void showMenu() {
        while (true) {
            std::cout << "\nMain Menu:" << std::endl;
            std::cout << "1. Start new game\n2. Load game\n3. Delete progress\n4. Exit\nChoose option: ";
            int choice;
            std::cin >> choice;
            std::cin.ignore();

            try {
                switch (choice) {
                case 1:
                    createCharacter();
                    gameRunning = true;
                    mainLoop();
                    return;
                case 2:
                    loadGame();
                    gameRunning = true;
                    mainLoop();
                    return;
                case 3:
                    deleteProgress();
                    break;
                case 4:
                    std::cout << "Goodbye!" << std::endl;
                    return;
                default:
                    std::cout << "Invalid choice!" << std::endl;
                }
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
        }
    }

    void mainLoop() {
        while (gameRunning) {
            std::cout << "\nWhat would you like to do?" << std::endl;
            std::cout << "1. Explore\n2. Check character\n3. Check inventory\n4. Save game\n5. Quit\nChoose option: ";
            int choice;
            std::cin >> choice;
            std::cin.ignore();

            try {
                switch (choice) {
                case 1:
                    battle();
                    break;
                case 2:
                    player->displayInfo();
                    break;
                case 3:
                    player->showInventory();

                    if (!player->getInventory().isEmpty()) {
                        std::cout << "Do you want to use an item? (y/n): ";
                        char useItem;
                        std::cin >> useItem;
                        std::cin.ignore();

                        if (useItem == 'y' || useItem == 'Y') {
                            std::cout << "Enter item name to use: ";
                            std::string itemName;
                            std::getline(std::cin, itemName);
                            player->useItem(itemName);
                        }
                    }
                    break;
                case 4:
                    saveGame();
                    break;
                case 5:
                    gameRunning = false;
                    std::cout << "Game exited." << std::endl;
                    break;
                default:
                    std::cout << "Invalid choice!" << std::endl;
                }
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }

            if (!player->isAlive()) {
                gameRunning = false;
            }
        }
    }

public:
    Game() : logger("game_log.txt"), gameRunning(false) {
        srand(time(0));
    }

    void start() {
        std::cout << "Welcome to Text Adventure!" << std::endl;
        showMenu();
    }
};

int main() {
    try {
        Game game;
        game.start();
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}