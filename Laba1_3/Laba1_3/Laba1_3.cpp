#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

class Entity {
protected:
    std::string name;
    int health;
    int attackPower;
    int defense;
    int maxHealth;

public:
    Entity(const std::string& name, int health, int attackPower, int defense)
        : name(name), health(health), attackPower(attackPower), defense(defense), maxHealth(health) {
    } 

    virtual ~Entity() {}

    std::string getName() const { return name; }
    int getHealth() const { return health; }
    int getAttack() const { return attackPower; }
    int getDefense() const { return defense; }
    int getMaxHealth() const { return maxHealth; }

    void setHealth(int newHealth) {
        health = newHealth;
        if (health > maxHealth) { 
            health = maxHealth;
        }
    }


    virtual void attack(Entity& target) = 0;
    virtual void displayInfo() const = 0;

    void heal(int amount) {
        health += amount;
        if (health > maxHealth) {
            health = maxHealth;
        }
        std::cout << name << " heals for " << amount << " HP.\n";
    }
};

class Character : public Entity {
public:
    Character(const std::string& name, int health, int attackPower, int defense)
        : Entity(name, health, attackPower, defense) {
    }

    void attack(Entity& target) override {
        int damage = getAttack() - target.getDefense();
        if (damage > 0) {
            target.setHealth(target.getHealth() - damage);
            std::cout << getName() << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << getName() << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Character: " << name << ", HP: " << health << "/" << maxHealth << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }
};

class Monster : public Entity {
public:
    Monster(const std::string& name, int health, int attackPower, int defense)
        : Entity(name, health, attackPower, defense) {
    }

    void attack(Entity& target) override {
        int damage = getAttack() - target.getDefense();
        if (damage > 0) {
            target.setHealth(target.getHealth() - damage);
            std::cout << getName() << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << getName() << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Monster: " << name << ", HP: " << health << "/" << maxHealth << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }
};

class Boss : public Entity {
public:
    Boss(const std::string& name, int health, int attackPower, int defense)
        : Entity(name, health, attackPower, defense) {
    }

    void attack(Entity& target) override {
        int damage = getAttack() - target.getDefense();
        if (damage > 0) {
            if (rand() % 100 < 40) {
                damage += 10;
                std::cout << "Fire Strike! ";
            }
            target.setHealth(target.getHealth() - damage);
            std::cout << getName() << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << getName() << " attacks " << target.getName() << ", but attack failed!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Boss: " << name << ", HP: " << health << "/" << maxHealth << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    Character hero("Hero", 100, 25, 10);
    Monster goblin("Goblin", 50, 15, 10);
    Boss Knight("Dark Knight", 150, 25, 20);

    Entity* entities[] = { &hero, &goblin, &Knight };

    for (auto* entity : entities) {
        entity->displayInfo();
    }

    std::cout << "\nFight:\n";
    hero.attack(goblin);
    goblin.attack(hero);
    hero.attack(Knight);
    Knight.attack(hero);

    std::cout << "\n";
    for (auto* entity : entities) {
        entity->displayInfo();
    }

    std::cout << "\n";
    hero.heal(15);
    Knight.heal(10);

    for (auto* entity : entities) {
        entity->displayInfo();
    }

    return 0;
}
