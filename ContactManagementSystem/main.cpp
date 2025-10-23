// #include "ContactManager.h"
// #include <iostream>
// #include <limits>

// void displayMenu() {
//     std::cout << "\n=== CONTACT MANAGEMENT SYSTEM ===\n";
//     std::cout << "1. Add Contact\n";
//     std::cout << "2. Display All Contacts\n";
//     std::cout << "3. Search Contact\n";
//     std::cout << "4. Edit Contact\n";
//     std::cout << "5. Delete Contact\n";
//     std::cout << "6. Sort Contacts\n";
//     std::cout << "7. Contact Count\n";
//     std::cout << "8. Exit\n";
//     std::cout << "Choose an option (1-8): ";
// }

// void searchMenu(ContactManager& manager) {
//     int choice;
//     std::string query;

//     std::cout << "\n=== SEARCH OPTIONS ===\n";
//     std::cout << "1. Search by Name\n";
//     std::cout << "2. Search by Phone\n";
//     std::cout << "3. Search by Email\n";
//     std::cout << "Choose search type (1-3): ";
//     std::cin >> choice;
//     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

//     std::cout << "Enter search term: ";
//     std::getline(std::cin, query);

//     switch (choice) {
//         case 1:
//             manager.searchByName(query);
//             break;
//         case 2:
//             manager.searchByPhone(query);
//             break;
//         case 3:
//             manager.searchByEmail(query);
//             break;
//         default:
//             std::cout << "Invalid choice!\n";
//     }
// }

// void sortMenu(ContactManager& manager) {
//     int choice;
    
//     std::cout << "\n=== SORT OPTIONS ===\n";
//     std::cout << "1. Sort by Name\n";
//     std::cout << "2. Sort by Phone\n";
//     std::cout << "Choose sort type (1-2): ";
//     std::cin >> choice;
//     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

//     switch (choice) {
//         case 1:
//             manager.sortByName();
//             break;
//         case 2:
//             manager.sortByPhone();
//             break;
//         default:
//             std::cout << "Invalid choice!\n";
//     }
// }

// void addContactMenu(ContactManager& manager) {
//     std::string name, phone, email, address;

//     std::cout << "\n=== ADD NEW CONTACT ===\n";
//     std::cout << "Name: ";
//     std::getline(std::cin, name);
    
//     std::cout << "Phone: ";
//     std::getline(std::cin, phone);
    
//     std::cout << "Email (optional): ";
//     std::getline(std::cin, email);
    
//     std::cout << "Address (optional): ";
//     std::getline(std::cin, address);

//     Contact newContact(name, phone, email, address);
//     manager.addContact(newContact);
// }

// void editContactMenu(ContactManager& manager) {
//     std::string phone;
    
//     std::cout << "\n=== EDIT CONTACT ===\n";
//     std::cout << "Enter phone number of contact to edit: ";
//     std::getline(std::cin, phone);
    
//     manager.editContact(phone);
// }

// void deleteContactMenu(ContactManager& manager) {
//     std::string phone;
    
//     std::cout << "\n=== DELETE CONTACT ===\n";
//     std::cout << "Enter phone number of contact to delete: ";
//     std::getline(std::cin, phone);
    
//     manager.deleteContact(phone);
// }

// int main() {
//     ContactManager manager;
//     int choice;

//     std::cout << "Welcome to Contact Management System!\n";
//     std::cout << "Data will be automatically saved to 'contacts.dat'\n";

//     while (true) {
//         displayMenu();
//         std::cin >> choice;
//         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

//         switch (choice) {
//             case 1:
//                 addContactMenu(manager);
//                 break;
//             case 2:
//                 manager.displayAllContacts();
//                 break;
//             case 3:
//                 searchMenu(manager);
//                 break;
//             case 4:
//                 editContactMenu(manager);
//                 break;
//             case 5:
//                 deleteContactMenu(manager);
//                 break;
//             case 6:
//                 sortMenu(manager);
//                 break;
//             case 7:
//                 std::cout << "\nTotal contacts: " << manager.getContactCount() << std::endl;
//                 break;
//             case 8:
//                 std::cout << "Thank you for using Contact Management System!\n";
//                 std::cout << "Goodbye!\n";
//                 return 0;
//             default:
//                 std::cout << "Invalid choice! Please try again.\n";
//         }
//     }

//     return 0;
// }














#include "ContactManager.h"
#include <iostream>
#include <limits>
#include <iomanip>

class AdvancedContactSystem {
private:
    ContactManager manager;
    
    void displayMainMenu();
    void displaySearchMenu();
    void displaySortMenu();
    void displayStatsMenu();
    void displayImportExportMenu();
    
    void addContact();
    void editContact();
    void deleteContact();
    void viewContacts();
    void searchContacts();
    void sortContacts();
    void showStatistics();
    void importExportMenu();
    void backupRestore();
    void tagManagement();
    
public:
    void run();
};

void AdvancedContactSystem::run() {
    std::cout << "=== ADVANCED CONTACT MANAGEMENT SYSTEM ===\n";
    std::cout << "Version 2.0 - Professional Edition\n\n";
    
    while (true) {
        displayMainMenu();
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: addContact(); break;
            case 2: viewContacts(); break;
            case 3: searchContacts(); break;
            case 4: editContact(); break;
            case 5: deleteContact(); break;
            case 6: sortContacts(); break;
            case 7: showStatistics(); break;
            case 8: importExportMenu(); break;
            case 9: backupRestore(); break;
            case 10: tagManagement(); break;
            case 0: 
                std::cout << "Thank you for using Advanced Contact Management System!\n";
                return;
            default:
                std::cout << "Invalid choice! Please try again.\n";
        }
    }
}

void AdvancedContactSystem::displayMainMenu() {
    std::cout << "\n=== MAIN MENU ===\n";
    std::cout << "1. Add Contact\n";
    std::cout << "2. View All Contacts\n";
    std::cout << "3. Search Contacts\n";
    std::cout << "4. Edit Contact\n";
    std::cout << "5. Delete Contact\n";
    std::cout << "6. Sort Contacts\n";
    std::cout << "7. Statistics\n";
    std::cout << "8. Import/Export\n";
    std::cout << "9. Backup/Restore\n";
    std::cout << "10. Tag Management\n";
    std::cout << "0. Exit\n";
    std::cout << "Choose an option: ";
}

void AdvancedContactSystem::addContact() {
    std::cout << "\n=== ADD NEW CONTACT ===\n";
    
    std::string firstName, lastName, phone, email, address, company, jobTitle, notes;
    
    std::cout << "First Name: ";
    std::getline(std::cin, firstName);
    std::cout << "Last Name: ";
    std::getline(std::cin, lastName);
    std::cout << "Phone: ";
    std::getline(std::cin, phone);
    std::cout << "Email (optional): ";
    std::getline(std::cin, email);
    std::cout << "Address (optional): ";
    std::getline(std::cin, address);
    std::cout << "Company (optional): ";
    std::getline(std::cin, company);
    std::cout << "Job Title (optional): ";
    std::getline(std::cin, jobTitle);
    std::cout << "Notes (optional): ";
    std::getline(std::cin, notes);
    
    Contact contact(firstName, lastName, phone);
    contact.setEmail(email);
    contact.setAddress(address);
    contact.setCompany(company);
    contact.setJobTitle(jobTitle);
    contact.setNotes(notes);
    
    // Set category and priority
    std::cout << "Category (0=Personal, 1=Work, 2=Family, 3=Friends, 4=Business, 5=Emergency): ";
    int category;
    std::cin >> category;
    contact.setCategory(static_cast<ContactCategory>(category));
    
    std::cout << "Priority (0=Low, 1=Medium, 2=High, 3=Urgent): ";
    int priority;
    std::cin >> priority;
    contact.setPriority(static_cast<ContactPriority>(priority));
    
    std::cin.ignore();
    
    if (manager.addContact(contact)) {
        std::cout << "Contact added successfully!\n";
    } else {
        std::cout << "Error: Contact with this phone number already exists!\n";
    }
}

void AdvancedContactSystem::viewContacts() {
    auto contacts = manager.getAllContacts();
    if (contacts.empty()) {
        std::cout << "No contacts found.\n";
        return;
    }
    
    std::cout << "\n=== ALL CONTACTS (" << contacts.size() << ") ===\n";
    for (const auto& contact : contacts) {
        contact.displaySummary();
    }
}

// ... (implement other methods similarly)

int main() {
    AdvancedContactSystem system;
    system.run();
    return 0;
}