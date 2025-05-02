#include <iostream>
#include <string>
class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
    }
    // Перегрузка оператора ==
    bool operator==(const Character& other) const {
        return name == other.name && health == other.health;
    }

    // Перегрузка оператора <<
    friend std::ostream& operator<<(std::ostream& os, const Character& character) {
        os << "Character: " << character.name << ", HP: " << character.health
            << ", Attack: " << character.attack << ", Defense: " << character.defense;
        return os;
    }

    // Перегрузка оператора +
    bool operator =(const Character& other) const {
    }
};

class Weapon {
private:
    std::string name;
    int damage;

public:
    Weapon(const std::string& n, int d) : name(n), damage(d) {
    }
    //Перегрузка оператора +
    Weapon operator+(const Weapon& other) const {
        return Weapon(name + "+" + other.name, damage + other.damage);
    }

    // Перегрузка оператора > для сравнения урона
    bool operator>(const Weapon& other) const {
        return damage > other.damage;
    }

    // Функция для вывода информации об оружии
    void printWeaponInfo() const {
        std::cout << "Name: " << name << ", damage: " << damage;
    }
};

int main() {
    Character hero1("Hero", 100, 20, 10);
    Character hero2("Hero", 100, 20, 10);
    Character hero3("Warrior", 150, 25, 15);

    Weapon sword1("Iron sword", 20);
    Weapon sword2("Old sword", 7);
    Weapon bow("Bow", 15);
    Weapon axe("Axe", 25);

    // Демонстрация перегрузки оператора +
    Weapon combinedWeapon = sword1 + bow;
    std::cout << "Combined Weapon: ";
    combinedWeapon.printWeaponInfo();
    std::cout << std::endl;

    // Демонстрация перегрузки оператора >
    if (axe > sword2) {
        std::cout << "Axe has more damage than Sword.\n";
    }
    else {
        std::cout << "Sword has more damage than Axe.\n";
    }

    if (bow > axe) {
        std::cout << "Bow has more damage than Axe.\n";
    }
    else {
        std::cout << "Axe has more damage than Bow.\n";
    }


    if (hero1 == hero2) {
        std::cout << "Hero1 and Hero2 are the same!\n";
    }
    if (!(hero1 == hero3)) {
        std::cout << "Hero1 and Hero3 are different!\n";
    }

    std::cout << hero1 << std::endl; // Вывод информации о персонаже
    return 0;
}
