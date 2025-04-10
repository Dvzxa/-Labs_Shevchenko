#include <iostream>
#include <string>

class Character {
private:
    std::string name;  // Приватное поле: имя персонажа
    int health;        // Приватное поле: уровень здоровья
    int maxHealth;     // Приватное поле: максимальный уровень здоровья
    int minHealth;     // Приватное поле: минимальный уровень здоровья
    int attack;        // Приватное поле: уровень атаки
    int defense;       // Приватное поле: уровень защиты
public:
    // Конструктор для инициализации данных
    Character(const std::string& n, int h, int maxH, int minH, int a, int d)
        : name(n), health(h), maxHealth(maxH), minHealth(minH), attack(a), defense(d) {
        // Гарантируем, что начальное здоровье находится в допустимых пределах.
        if (health > maxHealth) {
            health = maxHealth;
        }
        if (health < minHealth) {
            health = minHealth;
        }
    }
    // Метод для получения уровня здоровья
    int getHealth() const {
        return health;
    }
    // Метод для вывода информации о персонаже
    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << "/" << maxHealth
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
    // Метод для атаки другого персонажа
    void attackEnemy(Character& enemy) {
        int damage = attack - enemy.defense;
        if (damage > 0) {
            enemy.health -= damage;
            if (enemy.health < minHealth) enemy.health = minHealth;
            std::cout << name << " attacks " << enemy.name << " for " << damage << " damage!" << std::endl;
        }
        else {
            std::cout << name << " attacks " << enemy.name << ", but it has no effect!" << std::endl;
        }
    }
    // Вылечить персонажа
    void heal(int amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth; // Ограничение здоровья максимумом
        std::cout << name << " heals for " << amount << " HP!" << std::endl;
    }
    // Получить урон
    void takeDamage(int amount) {
        health -= amount;
        if (health < minHealth) health = minHealth; // Ограничение здоровья минимумом
        std::cout << name << " takes " << amount << " damage!" << std::endl;
    }
};

int main() {
    // Создание персонажей
    Character hero("Hero", 100, 100, 0, 20, 10);
    Character monster("Goblin", 50, 50, 0, 15, 5);

    // Выводим информацию о персонажах
    std::cout << "Initial state:" << std::endl;
    hero.displayInfo();
    monster.displayInfo();

    // Атака героя на монстра
    hero.attackEnemy(monster);
    monster.displayInfo();

    // Проверка takeDamage
    hero.takeDamage(30);
    hero.displayInfo();

    // Проверка heal
    hero.heal(20);
    hero.displayInfo();

    // Проверка ограничения здоровья (максимум)
    hero.heal(60);  // Попытка лечения больше максимума
    hero.displayInfo();

    // Проверка ограничения здоровья (минимум)
    monster.takeDamage(65); // Урон больше текущего здоровья
    monster.displayInfo();
    return 0;
}
