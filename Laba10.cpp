#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <map>

class User {
protected:
    std::string name;
    int id;
    int accessLevel;

public:
    User(const std::string& name, int id, int accessLevel)
        : name(name), id(id), accessLevel(accessLevel) {
        if (name.empty()) {
            throw std::invalid_argument("User name cannot be empty");
        }
        if (accessLevel < 0) {
            throw std::invalid_argument("Access level cannot be negative");
        }
    }

    virtual ~User() {}

    std::string getName() const { return name; }
    int getId() const { return id; }
    int getAccessLevel() const { return accessLevel; }

    void setName(const std::string& newName) {
        if (newName.empty()) {
            throw std::invalid_argument("User name cannot be empty");
        }
        name = newName;
    }

    void setAccessLevel(int newAccessLevel) {
        if (newAccessLevel < 0) {
            throw std::invalid_argument("Access level cannot be negative");
        }
        accessLevel = newAccessLevel;
    }

    virtual void displayInfo() const {
        std::cout << "ID: " << id << ", Name: " << name
            << ", Access Level: " << accessLevel;
    }

    virtual void saveToFile(std::ofstream& out) const {
        out << name << '\n' << id << '\n' << accessLevel << '\n';
    }

    virtual void loadFromFile(std::ifstream& in) {
        std::getline(in, name);
        in >> id >> accessLevel;
        in.ignore();
    }
};

class Student : public User {
private:
    std::string group;

public:
    Student(const std::string& name, int id, int accessLevel, const std::string& group)
        : User(name, id, accessLevel), group(group) {
        if (group.empty()) {
            throw std::invalid_argument("Group cannot be empty");
        }
    }

    std::string getGroup() const { return group; }
    void setGroup(const std::string& newGroup) {
        if (newGroup.empty()) {
            throw std::invalid_argument("Group cannot be empty");
        }
        group = newGroup;
    }

    void displayInfo() const override {
        User::displayInfo();
        std::cout << ", Type: Student, Group: " << group << std::endl;
    }

    void saveToFile(std::ofstream& out) const override {
        out << "Student\n";
        User::saveToFile(out);
        out << group << '\n';
    }

    void loadFromFile(std::ifstream& in) override {
        User::loadFromFile(in);
        std::getline(in, group);
    }
};

class Teacher : public User {
private:
    std::string department;

public:
    Teacher(const std::string& name, int id, int accessLevel, const std::string& department)
        : User(name, id, accessLevel), department(department) {
        if (department.empty()) {
            throw std::invalid_argument("Department cannot be empty");
        }
    }

    std::string getDepartment() const { return department; }
    void setDepartment(const std::string& newDepartment) {
        if (newDepartment.empty()) {
            throw std::invalid_argument("Department cannot be empty");
        }
        department = newDepartment;
    }

    void displayInfo() const override {
        User::displayInfo();
        std::cout << ", Type: Teacher, Department: " << department << std::endl;
    }

    void saveToFile(std::ofstream& out) const override {
        out << "Teacher\n";
        User::saveToFile(out);
        out << department << '\n';
    }

    void loadFromFile(std::ifstream& in) override {
        User::loadFromFile(in);
        std::getline(in, department);
    }
};

class Administrator : public User {
private:
    std::string position;

public:
    Administrator(const std::string& name, int id, int accessLevel, const std::string& position)
        : User(name, id, accessLevel), position(position) {
        if (position.empty()) {
            throw std::invalid_argument("Position cannot be empty");
        }
    }

    std::string getPosition() const { return position; }
    void setPosition(const std::string& newPosition) {
        if (newPosition.empty()) {
            throw std::invalid_argument("Position cannot be empty");
        }
        position = newPosition;
    }

    void displayInfo() const override {
        User::displayInfo();
        std::cout << ", Type: Administrator, Position: " << position << std::endl;
    }

    void saveToFile(std::ofstream& out) const override {
        out << "Administrator\n";
        User::saveToFile(out);
        out << position << '\n';
    }

    void loadFromFile(std::ifstream& in) override {
        User::loadFromFile(in);
        std::getline(in, position);
    }
};

class Resource {
private:
    std::string name;
    int requiredAccessLevel;

public:
    Resource(const std::string& name, int requiredAccessLevel)
        : name(name), requiredAccessLevel(requiredAccessLevel) {
        if (name.empty()) {
            throw std::invalid_argument("Resource name cannot be empty");
        }
        if (requiredAccessLevel < 0) {
            throw std::invalid_argument("Required access level cannot be negative");
        }
    }

    std::string getName() const { return name; }
    int getRequiredAccessLevel() const { return requiredAccessLevel; }

    void setName(const std::string& newName) {
        if (newName.empty()) {
            throw std::invalid_argument("Resource name cannot be empty");
        }
        name = newName;
    }

    void setRequiredAccessLevel(int newLevel) {
        if (newLevel < 0) {
            throw std::invalid_argument("Required access level cannot be negative");
        }
        requiredAccessLevel = newLevel;
    }

    bool checkAccess(const User& user) const {
        return user.getAccessLevel() >= requiredAccessLevel;
    }

    void saveToFile(std::ofstream& out) const {
        out << name << '\n' << requiredAccessLevel << '\n';
    }

    void loadFromFile(std::ifstream& in) {
        std::getline(in, name);
        in >> requiredAccessLevel;
        in.ignore();
    }
};

template<typename T>
class AccessControlSystem {
private:
    std::vector<std::unique_ptr<User>> users;
    std::vector<T> resources;

public:
    template<typename U, typename... Args>
    void addUser(Args&&... args) {
        users.emplace_back(std::make_unique<U>(std::forward<Args>(args)...));
    }

    void addResource(const T& resource) {
        resources.push_back(resource);
    }

    std::string checkAccess(int userId, const std::string& resourceName) const {
        auto userIt = std::find_if(users.begin(), users.end(),
            [userId](const auto& user) { return user->getId() == userId; });

        auto resourceIt = std::find_if(resources.begin(), resources.end(),
            [resourceName](const auto& resource) { return resource.getName() == resourceName; });

        if (userIt == users.end()) {
            throw std::runtime_error("User not found");
        }
        if (resourceIt == resources.end()) {
            throw std::runtime_error("Resource not found");
        }

        bool accessGranted = resourceIt->checkAccess(**userIt);
        return (**userIt).getName() + " is trying to access '" + resourceIt->getName() + "': " + (accessGranted ? "Access granted" : "Access denied");
    }

    std::vector<User*> findUsersByName(const std::string& name) const {
        std::vector<User*> result;
        for (const auto& user : users) {
            if (user->getName() == name) {
                result.push_back(user.get());
            }
        }
        return result;
    }

    User* findUserById(int id) const {
        auto it = std::find_if(users.begin(), users.end(),
            [id](const auto& user) { return user->getId() == id; });
        return it != users.end() ? it->get() : nullptr;
    }

    void sortUsersByAccessLevel() {
        std::sort(users.begin(), users.end(),
            [](const auto& a, const auto& b) {
                return a->getAccessLevel() < b->getAccessLevel();
            });
    }

    void sortUsersById() {
        std::sort(users.begin(), users.end(),
            [](const auto& a, const auto& b) {
                return a->getId() < b->getId();
            });
    }

    void displayAllUsers() const {
        for (const auto& user : users) {
            user->displayInfo();
        }
    }

    void displayAllResources() const {
        for (const auto& resource : resources) {
            std::cout << "Resource: " << resource.getName()
                << ", Required access level: "
                << resource.getRequiredAccessLevel() << std::endl;
        }
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream out(filename);
        if (!out) {
            throw std::runtime_error("Failed to open file for writing");
        }

        out << users.size() << '\n';
        for (const auto& user : users) {
            user->saveToFile(out);
        }

        out << resources.size() << '\n';
        for (const auto& resource : resources) {
            resource.saveToFile(out);
        }
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream in(filename);
        if (!in) {
            throw std::runtime_error("Failed to open file for reading");
        }

        users.clear();
        resources.clear();

        size_t userCount;
        in >> userCount;
        in.ignore();

        for (size_t i = 0; i < userCount; ++i) {
            std::string userType;
            std::getline(in, userType);

            std::unique_ptr<User> user;
            if (userType == "Student") {
                user = std::make_unique<Student>("", 0, 0, "");
            }
            else if (userType == "Teacher") {
                user = std::make_unique<Teacher>("", 0, 0, "");
            }
            else if (userType == "Administrator") {
                user = std::make_unique<Administrator>("", 0, 0, "");
            }
            else {
                throw std::runtime_error("Unknown user type in file");
            }

            user->loadFromFile(in);
            users.push_back(std::move(user));
        }

        size_t resourceCount;
        in >> resourceCount;
        in.ignore();

        for (size_t i = 0; i < resourceCount; ++i) {
            T resource("", 0);
            resource.loadFromFile(in);
            resources.push_back(resource);
        }
    }
};

int main() {
    try {
        AccessControlSystem<Resource> system;

        system.addUser<Student>("Shevchenko Nikita", 1, 1, "Group 03");
        system.addUser<Student>("Kuviko Daniil", 431, 1, "Group 04");
        system.addUser<Student>("Khara Andrey", 33, 1, "Group 02");
        system.addUser<Student>("Vorobyova Veronica", 4, 1, "Group 05");
        system.addUser<Teacher>("Podkolzina Lyubov Alexandrovna", 157, 2, "Institute of Cross-Cutting Technologies");
        system.addUser<Administrator>("Zhidkova Valeria Yuryevna", 746, 3, "Assistant");
        system.addUser<Administrator>("Dibrov Alexander Alexandrovich", 107, 4, "Head of the Academic Support Sector");
        system.addUser<Administrator>("Markarian Yulia Artyomovna", 803, 5, "Director of Institute");

        system.addResource(Resource("Audience 1-384", 1));
        system.addResource(Resource("Laboratory 202", 3));
        system.addResource(Resource("Scientific library", 2));
        system.addResource(Resource("Dean's office", 4));
        system.addResource(Resource("Director's office", 5));

        std::cout << "All users:\n";
        system.displayAllUsers();

        std::cout << "\nAll resources:\n";
        system.displayAllResources();

        std::cout << "\nAccess check:\n";
        std::cout << system.checkAccess(001, "Audience 1-384") << std::endl;
        std::cout << system.checkAccess(431, "Laboratory 202") << std::endl;
        std::cout << system.checkAccess(107, "Scientific library") << std::endl;
        std::cout << system.checkAccess(803, "Director's office") << std::endl;
        std::cout << system.checkAccess(746, "Director's office") << std::endl;

        try {
            system.addUser<Student>("", 4, 1, "Group 102"); // Empty name
        }
        catch (const std::invalid_argument& e) {
            std::cout << "\nError creating user: " << e.what() << std::endl;
        }

        system.saveToFile("university_access_system.dat");
        std::cout << "\nData saved to file\n";

        AccessControlSystem<Resource> newSystem;
        try {
            newSystem.loadFromFile("university_access_system.dat");
            std::cout << "\nData loaded from file:\n";
            newSystem.displayAllUsers();
        }
        catch (const std::exception& e) {
            std::cerr << "Error loading from file: " << e.what() << std::endl;
        }

        std::cout << "\nSearching for user with ID 33:\n";
        User* user = system.findUserById(33);
        if (user) {
            user->displayInfo();
        }
        else {
            std::cout << "User with ID 33 not found." << std::endl;
        }

        std::cout << "\nSorting users by access level:\n";
        system.sortUsersByAccessLevel();
        system.displayAllUsers();

        std::cout << "\nSorting users by ID:\n";
        system.sortUsersById();
        system.displayAllUsers();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
