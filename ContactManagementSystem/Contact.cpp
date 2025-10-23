#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <fstream>
#include <cctype>
#include <limits>

class Contact {
private:
    std::string name;
    std::string phone;
    std::string email;
    std::string address;

public:
    Contact() : name(""), phone(""), email(""), address("") {}
    
    Contact(const std::string& name, const std::string& phone, 
            const std::string& email = "", const std::string& address = "")
        : name(name), phone(phone), email(email), address(address) {}

    std::string getName() const { return name; }
    std::string getPhone() const { return phone; }
    std::string getEmail() const { return email; }
    std::string getAddress() const { return address; }

    void setName(const std::string& name) { this->name = name; }
    void setPhone(const std::string& phone) { this->phone = phone; }
    void setEmail(const std::string& email) { this->email = email; }
    void setAddress(const std::string& address) { this->address = address; }

    void display() const {
        std::cout << "Name: " << name << std::endl;
        std::cout << "Phone: " << phone << std::endl;
        if (!email.empty()) std::cout << "Email: " << email << std::endl;
        if (!address.empty()) std::cout << "Address: " << address << std::endl;
        std::cout << "------------------------" << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& os, const Contact& contact) {
        os << contact.name << "\n" << contact.phone << "\n" 
           << contact.email << "\n" << contact.address << "\n";
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Contact& contact) {
        std::getline(is, contact.name);
        std::getline(is, contact.phone);
        std::getline(is, contact.email);
        std::getline(is, contact.address);
        return is;
    }

    bool operator<(const Contact& other) const {
        return name < other.name;
    }

    bool operator==(const Contact& other) const {
        return name == other.name && phone == other.phone;
    }
};

class ContactManager {
private:
    std::vector<Contact> contacts;
    std::map<std::string, Contact*> phoneIndex;
    std::string filename;

    void buildIndex() {
        phoneIndex.clear();
        for (auto& contact : contacts) {
            phoneIndex[contact.getPhone()] = &contact;
        }
    }

    void saveToFile() {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not save contacts to file!\n";
            return;
        }
        for (const auto& contact : contacts) {
            file << contact;
        }
        file.close();
    }

    void loadFromFile() {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return;
        }
        Contact contact;
        while (file >> contact) {
            contacts.push_back(contact);
        }
        file.close();
    }

    void displayContacts(const std::vector<Contact>& contactList) const {
        std::cout << "\n=== CONTACTS (" << contactList.size() << ") ===\n";
        for (size_t i = 0; i < contactList.size(); ++i) {
            std::cout << "Contact #" << i + 1 << ":\n";
            contactList[i].display();
        }
    }

public:
    ContactManager(const std::string& filename = "contacts.dat") : filename(filename) {
        loadFromFile();
        buildIndex();
        std::cout << "Loaded " << contacts.size() << " contacts.\n";
    }

    ~ContactManager() {
        saveToFile();
        std::cout << "Saved " << contacts.size() << " contacts.\n";
    }

    void addContact(const Contact& contact) {
        if (phoneExists(contact.getPhone())) {
            std::cout << "Error: Contact with phone number " << contact.getPhone() 
                      << " already exists!\n";
            return;
        }
        contacts.push_back(contact);
        phoneIndex[contact.getPhone()] = &contacts.back();
        std::cout << "Contact added successfully!\n";
    }

    bool deleteContact(const std::string& phone) {
        auto it = phoneIndex.find(phone);
        if (it == phoneIndex.end()) {
            std::cout << "Contact with phone number " << phone << " not found!\n";
            return false;
        }
        contacts.erase(std::remove_if(contacts.begin(), contacts.end(),
            [&](const Contact& c) { return c.getPhone() == phone; }), contacts.end());
        phoneIndex.erase(phone);
        std::cout << "Contact deleted successfully!\n";
        return true;
    }

    bool editContact(const std::string& phone) {
        auto it = phoneIndex.find(phone);
        if (it == phoneIndex.end()) {
            std::cout << "Contact with phone number " << phone << " not found!\n";
            return false;
        }

        Contact& contact = *(it->second);
        std::string input;

        std::cout << "Editing contact: " << contact.getName() << std::endl;
        std::cout << "Leave field blank to keep current value.\n";

        std::cout << "New name [" << contact.getName() << "]: ";
        std::getline(std::cin, input);
        if (!input.empty()) contact.setName(input);

        std::cout << "New phone [" << contact.getPhone() << "]: ";
        std::getline(std::cin, input);
        if (!input.empty()) {
            if (phoneExists(input) && input != phone) {
                std::cout << "Error: Phone number already exists!\n";
                return false;
            }
            phoneIndex.erase(phone);
            contact.setPhone(input);
            phoneIndex[input] = &contact;
        }

        std::cout << "New email [" << contact.getEmail() << "]: ";
        std::getline(std::cin, input);
        if (!input.empty()) contact.setEmail(input);

        std::cout << "New address [" << contact.getAddress() << "]: ";
        std::getline(std::cin, input);
        if (!input.empty()) contact.setAddress(input);

        std::cout << "Contact updated successfully!\n";
        return true;
    }

    void displayAllContacts() const {
        if (contacts.empty()) {
            std::cout << "No contacts found.\n";
            return;
        }
        displayContacts(contacts);
    }

    void searchByName(const std::string& name) const {
        std::vector<Contact> results;
        std::string searchName = name;
        std::transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);

        for (const auto& contact : contacts) {
            std::string contactName = contact.getName();
            std::transform(contactName.begin(), contactName.end(), contactName.begin(), ::tolower);
            if (contactName.find(searchName) != std::string::npos) {
                results.push_back(contact);
            }
        }

        if (results.empty()) {
            std::cout << "No contacts found with name containing: " << name << std::endl;
        } else {
            std::cout << "Found " << results.size() << " contact(s):\n";
            displayContacts(results);
        }
    }

    void searchByPhone(const std::string& phone) const {
        auto it = phoneIndex.find(phone);
        if (it != phoneIndex.end()) {
            std::cout << "Contact found:\n";
            it->second->display();
        } else {
            std::cout << "No contact found with phone: " << phone << std::endl;
        }
    }

    void searchByEmail(const std::string& email) const {
        std::vector<Contact> results;
        std::string searchEmail = email;
        std::transform(searchEmail.begin(), searchEmail.end(), searchEmail.begin(), ::tolower);

        for (const auto& contact : contacts) {
            std::string contactEmail = contact.getEmail();
            std::transform(contactEmail.begin(), contactEmail.end(), contactEmail.begin(), ::tolower);
            if (contactEmail.find(searchEmail) != std::string::npos) {
                results.push_back(contact);
            }
        }

        if (results.empty()) {
            std::cout << "No contacts found with email containing: " << email << std::endl;
        } else {
            std::cout << "Found " << results.size() << " contact(s):\n";
            displayContacts(results);
        }
    }

    void sortByName() {
        std::sort(contacts.begin(), contacts.end());
        buildIndex();
        std::cout << "Contacts sorted by name.\n";
    }

    void sortByPhone() {
        std::sort(contacts.begin(), contacts.end(), 
            [](const Contact& a, const Contact& b) {
                return a.getPhone() < b.getPhone();
            });
        buildIndex();
        std::cout << "Contacts sorted by phone number.\n";
    }

    size_t getContactCount() const {
        return contacts.size();
    }

    bool phoneExists(const std::string& phone) const {
        return phoneIndex.find(phone) != phoneIndex.end();
    }
};

void displayMenu() {
    std::cout << "\n=== CONTACT MANAGEMENT SYSTEM ===\n";
    std::cout << "1. Add Contact\n";
    std::cout << "2. Display All Contacts\n";
    std::cout << "3. Search Contact\n";
    std::cout << "4. Edit Contact\n";
    std::cout << "5. Delete Contact\n";
    std::cout << "6. Sort Contacts\n";
    std::cout << "7. Contact Count\n";
    std::cout << "8. Exit\n";
    std::cout << "Choose an option (1-8): ";
}

void searchMenu(ContactManager& manager) {
    int choice;
    std::string query;

    std::cout << "\n=== SEARCH OPTIONS ===\n";
    std::cout << "1. Search by Name\n";
    std::cout << "2. Search by Phone\n";
    std::cout << "3. Search by Email\n";
    std::cout << "Choose search type (1-3): ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter search term: ";
    std::getline(std::cin, query);

    switch (choice) {
        case 1: manager.searchByName(query); break;
        case 2: manager.searchByPhone(query); break;
        case 3: manager.searchByEmail(query); break;
        default: std::cout << "Invalid choice!\n";
    }
}

void sortMenu(ContactManager& manager) {
    int choice;
    std::cout << "\n=== SORT OPTIONS ===\n";
    std::cout << "1. Sort by Name\n";
    std::cout << "2. Sort by Phone\n";
    std::cout << "Choose sort type (1-2): ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (choice) {
        case 1: manager.sortByName(); break;
        case 2: manager.sortByPhone(); break;
        default: std::cout << "Invalid choice!\n";
    }
}

void addContactMenu(ContactManager& manager) {
    std::string name, phone, email, address;

    std::cout << "\n=== ADD NEW CONTACT ===\n";
    std::cout << "Name: ";
    std::getline(std::cin, name);
    
    std::cout << "Phone: ";
    std::getline(std::cin, phone);
    
    std::cout << "Email (optional): ";
    std::getline(std::cin, email);
    
    std::cout << "Address (optional): ";
    std::getline(std::cin, address);

    Contact newContact(name, phone, email, address);
    manager.addContact(newContact);
}

int main() {
    ContactManager manager;
    int choice;

    std::cout << "Welcome to Contact Management System!\n";
    std::cout << "Data will be automatically saved to 'contacts.dat'\n";

    while (true) {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: addContactMenu(manager); break;
            case 2: manager.displayAllContacts(); break;
            case 3: searchMenu(manager); break;
            case 4: {
                std::string phone;
                std::cout << "Enter phone number to edit: ";
                std::getline(std::cin, phone);
                manager.editContact(phone);
                break;
            }
            case 5: {
                std::string phone;
                std::cout << "Enter phone number to delete: ";
                std::getline(std::cin, phone);
                manager.deleteContact(phone);
                break;
            }
            case 6: sortMenu(manager); break;
            case 7: 
                std::cout << "\nTotal contacts: " << manager.getContactCount() << std::endl;
                break;
            case 8:
                std::cout << "Thank you for using Contact Management System!\n";
                return 0;
            default:
                std::cout << "Invalid choice! Please try again.\n";
        }
    }
    return 0;
}



