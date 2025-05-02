#include <iostream>
#include <vector>
#include <queue>
#include <string>

template <typename T>
class GameManager {
private:
    std::vector<T> entities;

public:
    void addEntity(const T& entity) {
        entities.push_back(entity);
    }

    void displayAll() const {
        for (const auto& entity : entities) {
            entity->displayInfo();
        }
    }
};

// Пример класса, поддерживающего метод displayInfo (для демонстрации GameManager)
class Entity {
public:
    virtual void displayInfo() const {
        std::cout << "Entity" << std::endl;
    }
};

class Player : public Entity {
private:
    std::string name;
    int health;
    int score;

public:
    Player(const std::string& name, int health, int score) : name(name), health(health), score(score) {}

    void displayInfo() const override {
        std::cout << "Player: " << name << ", Helth: " << health << ", Score: " << score << std::endl;
    }
};

class Enemy : public Entity {
private:
    std::string name;
    int health;
    std::string type;

public:
    Enemy(const std::string& name, int health, const std::string& type) : name(name), health(health), type(type) {}

    void displayInfo() const override {
        std::cout << "Enemy: " << name << " (" << type << "), Health: " << health << std::endl;
    }
};


// Шаблонный класс Queue
template <typename T>
class Queue {
private:
    std::queue<T> data;

public:
    void push(const T& item) {
        data.push(item);
    }

    void pop() {
        if (!data.empty()) {
            data.pop();
        }
        else {
            std::cout << "The queue is empty. The item cannot be deleted." << std::endl;
        }
    }

    T& front() {
        return data.front();
    }

    bool empty() const {
        return data.empty();
    }

    size_t size() const {
        return data.size();
    }

    void printQueue() const {
        std::queue<T> tempQueue = data; 
        while (!tempQueue.empty()) {
            std::cout << tempQueue.front() << " ";
            tempQueue.pop();
        }
        std::cout << std::endl;
    }
};

int main() {
    // Пример использования GameManager
    GameManager<Entity*> manager;
    manager.addEntity(new Player("Hero", 100, 0));
    manager.addEntity(new Enemy("Goblin", 50, "Goblin"));
    manager.displayAll();

    std::cout << std::endl;

    // Пример использования Queue для строк
    Queue<std::string> stringQueue;
    stringQueue.push("Bow");
    stringQueue.push("Axe");
    stringQueue.push("Shield");

    std::cout << "String queue: ";
    stringQueue.printQueue();


    std::cout << "Extracting the element: " << stringQueue.front() << std::endl;
    stringQueue.pop();

    std::cout << "String queue: ";
    stringQueue.printQueue();

    std::cout << std::endl;

    // Пример использования Queue для чисел
    Queue<int> intQueue;
    intQueue.push(1);
    intQueue.push(2);
    intQueue.push(3);

    std::cout << "Numerical queue: ";
    intQueue.printQueue();

    std::cout << "Extracting the element: " << intQueue.front() << std::endl;
    intQueue.pop();

    std::cout << "Numerical queue: ";
    intQueue.printQueue();

    return 0;
}

