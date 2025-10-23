#include "ContactManager.h"
#include <iostream>
#include <algorithm>

ContactManager::ContactManager(const std::string& filename) : filename(filename) {
    loadFromFile();
    buildIndex();
}

ContactManager::~ContactManager() {
    saveToFile();
}

void ContactManager::loadFromFile() {
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

void ContactManager::saveToFile() {
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

void ContactManager::buildIndex() {
    phoneIndex.clear();
    for (auto& contact : contacts) {
        phoneIndex[contact.getPhone()] = &contact;
    }
}

void ContactManager::addContact(const Contact& contact) {
    if (phoneExists(contact.getPhone())) {
        std::cout << "Error: Contact with phone number " << contact.getPhone() 
                  << " already exists!\n";
        return;
    }

    contacts.push_back(contact);
    phoneIndex[contact.getPhone()] = &contacts.back();
    std::cout << "Contact added successfully!\n";
}

bool ContactManager::deleteContact(const std::string& phone) {
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

bool ContactManager::editContact(const std::string& phone) {
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

void ContactManager::displayAllContacts() const {
    if (contacts.empty()) {
        std::cout << "No contacts found.\n";
        return;
    }
    displayContacts(contacts);
}

void ContactManager::displayContacts(const std::vector<Contact>& contactList) const {
    std::cout << "\n=== CONTACTS (" << contactList.size() << ") ===\n";
    for (size_t i = 0; i < contactList.size(); ++i) {
        std::cout << "Contact #" << i + 1 << ":\n";
        contactList[i].display();
    }
}

void ContactManager::searchByName(const std::string& name) const {
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
        std::cout << "Found " << results.size() << " contact(s) with name containing: " << name << std::endl;
        displayContacts(results);
    }
}

void ContactManager::searchByPhone(const std::string& phone) const {
    auto it = phoneIndex.find(phone);
    if (it != phoneIndex.end()) {
        std::cout << "Contact found:\n";
        it->second->display();
    } else {
        std::cout << "No contact found with phone: " << phone << std::endl;
    }
}

void ContactManager::searchByEmail(const std::string& email) const {
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
        std::cout << "Found " << results.size() << " contact(s) with email containing: " << email << std::endl;
        displayContacts(results);
    }
}

void ContactManager::sortByName() {
    std::sort(contacts.begin(), contacts.end());
    buildIndex();
    std::cout << "Contacts sorted by name.\n";
}

void ContactManager::sortByPhone() {
    std::sort(contacts.begin(), contacts.end(), 
        [](const Contact& a, const Contact& b) {
            return a.getPhone() < b.getPhone();
        });
    buildIndex();
    std::cout << "Contacts sorted by phone number.\n";
}

size_t ContactManager::getContactCount() const {
    return contacts.size();
}

bool ContactManager::phoneExists(const std::string& phone) const {
    return phoneIndex.find(phone) != phoneIndex.end();
}


