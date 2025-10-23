#ifndef CONTACT_MANAGER_H
#define CONTACT_MANAGER_H

#include "Contact.h"
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <fstream>
#include <cctype>

class ContactManager {
private:
    std::vector<Contact> contacts;
    std::map<std::string, Contact*> phoneIndex;
    std::string filename;

    void buildIndex();
    void saveToFile();
    void loadFromFile();
    void displayContacts(const std::vector<Contact>& contactList) const;

public:
    ContactManager(const std::string& filename = "contacts.dat");
    ~ContactManager();

    void addContact(const Contact& contact);
    bool deleteContact(const std::string& phone);
    bool editContact(const std::string& phone);
    void displayAllContacts() const;
    
    void searchByName(const std::string& name) const;
    void searchByPhone(const std::string& phone) const;
    void searchByEmail(const std::string& email) const;

    void sortByName();
    void sortByPhone();

    size_t getContactCount() const;
    bool phoneExists(const std::string& phone) const;
};

#endif