#include <iostream>
#include <string>
#include <memory>
#include <vector>

class Inventory {
private:
    std::vector<std::unique_ptr<std::string>> items; // Вектор умных указателей на строки

public:
    void addItem(const std::string& item) {
        items.push_back(std::make_unique<std::string>(item)); // Создаем умный указатель на новую строку
    }

    void displayInventory() const {
        std::cout << "Inventory:\n";
        if (items.empty()) {
            std::cout << "  (empty)\n";
            return;
        }
        for (size_t i = 0; i < items.size(); ++i) {
            std::cout << "  " << i + 1 << ": " << *items[i] << "\n";  // Разыменовываем умный указатель, чтобы получить строку
        }
    }
};

int main() {
    Inventory playerInventory;

    playerInventory.addItem("Sword");
    playerInventory.addItem("Potion of Healing");
    playerInventory.addItem("Bow");
    playerInventory.addItem("Axe");

    playerInventory.displayInventory();


    return 0;
}