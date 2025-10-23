#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <fstream>
#include <cctype>
#include <limits>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <thread>
#include <random>
#include <regex>
#include <set>
#include <queue>
#include <stack>
#include <functional>
#include <unordered_map>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

class Logger {
private:
    std::ofstream logFile;
    std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
public:
    Logger(const std::string& filename = "contact_manager.log") {
        logFile.open(filename, std::ios::app);
    }
    
    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }
    
    void log(const std::string& message, const std::string& level = "INFO") {
        if (logFile.is_open()) {
            logFile << "[" << getCurrentTime() << "] [" << level << "] " << message << std::endl;
        }
        // Also print to console for important messages
        if (level == "ERROR" || level == "WARNING") {
            std::cout << "[" << level << "] " << message << std::endl;
        }
    }
};

class InputValidator {
public:
    static bool isValidName(const std::string& name) {
        if (name.empty()) return false;
        return std::all_of(name.begin(), name.end(), [](char c) {
            return std::isalpha(c) || std::isspace(c) || c == '-' || c == '\'' || c == '.';
        });
    }
    
    static bool isValidPhone(const std::string& phone) {
        if (phone.empty()) return false;
        return std::regex_match(phone, std::regex(R"(^[\+]?[0-9\s\-\(\)]{10,}$)"));
    }
    
    static bool isValidEmail(const std::string& email) {
        if (email.empty()) return true;
        return std::regex_match(email, std::regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)"));
    }
    
    static bool isValidDate(const std::string& date) {
        if (date.empty()) return true;
        return std::regex_match(date, std::regex(R"(^\d{4}-\d{2}-\d{2}$)"));
    }
    
    static bool isValidURL(const std::string& url) {
        if (url.empty()) return true;
        return std::regex_match(url, std::regex(R"(^(https?|ftp)://[^\s/$.?#].[^\s]*$)"));
    }
};

class SimpleEncryption {
private:
    std::string key;
    
    std::string simpleXOR(const std::string& data, const std::string& key) {
        std::string result = data;
        for (size_t i = 0; i < data.length(); ++i) {
            result[i] = data[i] ^ key[i % key.length()];
        }
        return result;
    }
    
public:
    SimpleEncryption(const std::string& encryptionKey = "default_key_123") : key(encryptionKey) {}
    
    std::string encrypt(const std::string& data) {
        return simpleXOR(data, key);
    }
    
    std::string decrypt(const std::string& data) {
        return simpleXOR(data, key);
    }
    
    static std::string simpleHash(const std::string& password) {
        // Simple hash function for demonstration
        unsigned long hash = 5381;
        for (char c : password) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return std::to_string(hash);
    }
};

class BackupManager {
private:
    std::string backupDir;
    
    void createDirectory(const std::string& path) {
#ifdef _WIN32
        CreateDirectoryA(path.c_str(), NULL);
#else
        mkdir(path.c_str(), 0755);
#endif
    }
    
public:
    BackupManager(const std::string& dir = "backups") : backupDir(dir) {
        // Create backup directory if it doesn't exist
        createDirectory(backupDir);
    }
    
    bool createBackup(const std::string& sourceFile) {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream backupName;
        backupName << backupDir << "/backup_" << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S") << ".dat";
        
        std::ifstream src(sourceFile, std::ios::binary);
        std::ofstream dst(backupName.str(), std::ios::binary);
        
        if (!src.is_open() || !dst.is_open()) {
            return false;
        }
        
        dst << src.rdbuf();
        src.close();
        dst.close();
        return true;
    }
    
    std::vector<std::string> listBackups() {
        std::vector<std::string> backups;
        // Simple implementation - in production, you'd use filesystem library
        return backups;
    }
    
    bool restoreBackup(const std::string& backupFile, const std::string& targetFile) {
        std::ifstream src(backupFile, std::ios::binary);
        std::ofstream dst(targetFile, std::ios::binary);
        
        if (!src.is_open() || !dst.is_open()) {
            return false;
        }
        
        dst << src.rdbuf();
        src.close();
        dst.close();
        return true;
    }
};

class Contact {
private:
    std::string name;
    std::string phone;
    std::string email;
    std::string address;
    std::string notes;
    std::string company;
    std::string jobTitle;
    std::string birthday;
    std::string website;
    std::vector<std::string> tags;
    std::string socialMedia;
    std::time_t createdDate;
    std::time_t modifiedDate;
    bool isFavorite;
    int contactId;
    static int nextId;

public:
    Contact() : name(""), phone(""), email(""), address(""), notes(""), company(""), 
                jobTitle(""), birthday(""), website(""), socialMedia(""), 
                createdDate(std::time(nullptr)), modifiedDate(std::time(nullptr)), 
                isFavorite(false), contactId(nextId++) {}
    
    Contact(const std::string& name, const std::string& phone, 
            const std::string& email = "", const std::string& address = "",
            const std::string& company = "", const std::string& jobTitle = "")
        : name(name), phone(phone), email(email), address(address), 
          company(company), jobTitle(jobTitle), notes(""), birthday(""), website(""),
          socialMedia(""), createdDate(std::time(nullptr)), modifiedDate(std::time(nullptr)),
          isFavorite(false), contactId(nextId++) {}

    // Getters
    std::string getName() const { return name; }
    std::string getPhone() const { return phone; }
    std::string getEmail() const { return email; }
    std::string getAddress() const { return address; }
    std::string getNotes() const { return notes; }
    std::string getCompany() const { return company; }
    std::string getJobTitle() const { return jobTitle; }
    std::string getBirthday() const { return birthday; }
    std::string getWebsite() const { return website; }
    std::vector<std::string> getTags() const { return tags; }
    std::string getSocialMedia() const { return socialMedia; }
    std::time_t getCreatedDate() const { return createdDate; }
    std::time_t getModifiedDate() const { return modifiedDate; }
    bool getIsFavorite() const { return isFavorite; }
    int getContactId() const { return contactId; }

    // Setters
    void setName(const std::string& name) { this->name = name; updateModifiedDate(); }
    void setPhone(const std::string& phone) { this->phone = phone; updateModifiedDate(); }
    void setEmail(const std::string& email) { this->email = email; updateModifiedDate(); }
    void setAddress(const std::string& address) { this->address = address; updateModifiedDate(); }
    void setNotes(const std::string& notes) { this->notes = notes; updateModifiedDate(); }
    void setCompany(const std::string& company) { this->company = company; updateModifiedDate(); }
    void setJobTitle(const std::string& jobTitle) { this->jobTitle = jobTitle; updateModifiedDate(); }
    void setBirthday(const std::string& birthday) { this->birthday = birthday; updateModifiedDate(); }
    void setWebsite(const std::string& website) { this->website = website; updateModifiedDate(); }
    void setSocialMedia(const std::string& socialMedia) { this->socialMedia = socialMedia; updateModifiedDate(); }
    void setIsFavorite(bool favorite) { isFavorite = favorite; updateModifiedDate(); }
    
    void addTag(const std::string& tag) {
        if (std::find(tags.begin(), tags.end(), tag) == tags.end()) {
            tags.push_back(tag);
            updateModifiedDate();
        }
    }
    
    void removeTag(const std::string& tag) {
        tags.erase(std::remove(tags.begin(), tags.end(), tag), tags.end());
        updateModifiedDate();
    }

    void updateModifiedDate() {
        modifiedDate = std::time(nullptr);
    }

    void display() const {
        std::cout << "ID: " << contactId << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Phone: " << phone << std::endl;
        if (!email.empty()) std::cout << "Email: " << email << std::endl;
        if (!address.empty()) std::cout << "Address: " << address << std::endl;
        if (!company.empty()) std::cout << "Company: " << company << std::endl;
        if (!jobTitle.empty()) std::cout << "Job Title: " << jobTitle << std::endl;
        if (!birthday.empty()) std::cout << "Birthday: " << birthday << std::endl;
        if (!website.empty()) std::cout << "Website: " << website << std::endl;
        if (!socialMedia.empty()) std::cout << "Social Media: " << socialMedia << std::endl;
        if (!notes.empty()) std::cout << "Notes: " << notes << std::endl;
        if (!tags.empty()) {
            std::cout << "Tags: ";
            for (size_t i = 0; i < tags.size(); ++i) {
                std::cout << tags[i];
                if (i != tags.size() - 1) std::cout << ", ";
            }
            std::cout << std::endl;
        }
        std::cout << "Favorite: " << (isFavorite ? "Yes" : "No") << std::endl;
        
        char createdBuffer[80];
        std::strftime(createdBuffer, sizeof(createdBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&createdDate));
        std::cout << "Created: " << createdBuffer << std::endl;
        
        char modifiedBuffer[80];
        std::strftime(modifiedBuffer, sizeof(modifiedBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&modifiedDate));
        std::cout << "Modified: " << modifiedBuffer << std::endl;
        
        std::cout << "------------------------" << std::endl;
    }

    void displayCompact() const {
        std::cout << std::setw(4) << contactId << " | "
                  << std::setw(20) << std::left << (name.length() > 20 ? name.substr(0, 17) + "..." : name) << " | "
                  << std::setw(15) << phone << " | "
                  << std::setw(20) << (email.length() > 20 ? email.substr(0, 17) + "..." : email) << " | "
                  << (isFavorite ? "★" : " ") << std::endl;
    }

    int getAge() const {
        if (birthday.empty()) return -1;
        
        std::tm tm = {};
        std::istringstream ss(birthday);
        ss >> std::get_time(&tm, "%Y-%m-%d");
        if (ss.fail()) return -1;
        
        auto birthTime = std::mktime(&tm);
        auto now = std::time(nullptr);
        
        int age = std::difftime(now, birthTime) / (60 * 60 * 24 * 365.25);
        return age;
    }

    bool matchesSearch(const std::string& query) const {
        std::string searchQuery = query;
        std::transform(searchQuery.begin(), searchQuery.end(), searchQuery.begin(), ::tolower);
        
        auto checkField = [&](const std::string& field) {
            std::string fieldLower = field;
            std::transform(fieldLower.begin(), fieldLower.end(), fieldLower.begin(), ::tolower);
            return fieldLower.find(searchQuery) != std::string::npos;
        };
        
        return checkField(name) || checkField(phone) || checkField(email) || 
               checkField(address) || checkField(company) || checkField(jobTitle) ||
               checkField(notes) || checkField(website) || checkField(socialMedia) ||
               std::any_of(tags.begin(), tags.end(), [&](const std::string& tag) {
                   return checkField(tag);
               });
    }

    friend std::ostream& operator<<(std::ostream& os, const Contact& contact) {
        os << contact.contactId << "\n" << contact.name << "\n" << contact.phone << "\n" 
           << contact.email << "\n" << contact.address << "\n" << contact.company << "\n"
           << contact.jobTitle << "\n" << contact.birthday << "\n" << contact.website << "\n"
           << contact.socialMedia << "\n" << contact.notes << "\n" << contact.isFavorite << "\n"
           << contact.createdDate << "\n" << contact.modifiedDate << "\n";
        
        // Save tags
        os << contact.tags.size() << "\n";
        for (const auto& tag : contact.tags) {
            os << tag << "\n";
        }
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Contact& contact) {
        is >> contact.contactId;
        is.ignore();
        std::getline(is, contact.name);
        std::getline(is, contact.phone);
        std::getline(is, contact.email);
        std::getline(is, contact.address);
        std::getline(is, contact.company);
        std::getline(is, contact.jobTitle);
        std::getline(is, contact.birthday);
        std::getline(is, contact.website);
        std::getline(is, contact.socialMedia);
        std::getline(is, contact.notes);
        is >> contact.isFavorite;
        is >> contact.createdDate;
        is >> contact.modifiedDate;
        
        // Load tags
        size_t tagCount;
        is >> tagCount;
        is.ignore();
        contact.tags.clear();
        for (size_t i = 0; i < tagCount; ++i) {
            std::string tag;
            std::getline(is, tag);
            contact.tags.push_back(tag);
        }
        
        // Update nextId
        if (contact.contactId >= Contact::nextId) {
            Contact::nextId = contact.contactId + 1;
        }
        
        return is;
    }

    bool operator<(const Contact& other) const {
        return name < other.name;
    }

    bool operator==(const Contact& other) const {
        return contactId == other.contactId;
    }
};

int Contact::nextId = 1;

class Statistics {
private:
    std::map<std::string, int> tagCounts;
    std::map<std::string, int> companyCounts;
    int totalContacts;
    int favoritesCount;
    
public:
    Statistics() : totalContacts(0), favoritesCount(0) {}
    
    void update(const std::vector<Contact>& contacts) {
        totalContacts = contacts.size();
        tagCounts.clear();
        companyCounts.clear();
        favoritesCount = 0;
        
        for (const auto& contact : contacts) {
            if (contact.getIsFavorite()) {
                favoritesCount++;
            }
            
            if (!contact.getCompany().empty()) {
                companyCounts[contact.getCompany()]++;
            }
            
            for (const auto& tag : contact.getTags()) {
                tagCounts[tag]++;
            }
        }
    }
    
    void display() const {
        std::cout << "\n=== STATISTICS ===\n";
        std::cout << "Total Contacts: " << totalContacts << std::endl;
        std::cout << "Favorites: " << favoritesCount << std::endl;
        
        if (!companyCounts.empty()) {
            std::cout << "\nCompanies:\n";
            for (const auto& entry : companyCounts) {
                std::cout << "  " << entry.first << ": " << entry.second << std::endl;
            }
        }
        
        if (!tagCounts.empty()) {
            std::cout << "\nTags:\n";
            for (const auto& entry : tagCounts) {
                std::cout << "  " << entry.first << ": " << entry.second << std::endl;
            }
        }
    }
};

class ContactManager {
private:
    std::vector<Contact> contacts;
    std::map<std::string, Contact*> phoneIndex;
    std::map<int, Contact*> idIndex;
    std::string filename;
    Logger logger;
    SimpleEncryption encryptor;
    BackupManager backupManager;
    Statistics stats;
    std::unordered_map<std::string, std::vector<Contact*>> tagIndex;
    bool autoBackup;
    int autoBackupInterval;
    std::time_t lastBackupTime;
    
    void buildIndex() {
        phoneIndex.clear();
        idIndex.clear();
        tagIndex.clear();
        
        for (auto& contact : contacts) {
            phoneIndex[contact.getPhone()] = &contact;
            idIndex[contact.getContactId()] = &contact;
            
            // Build tag index
            for (const auto& tag : contact.getTags()) {
                tagIndex[tag].push_back(&contact);
            }
        }
    }

    void saveToFile() {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not save contacts to file!\n";
            logger.log("Failed to save contacts to file: " + filename, "ERROR");
            return;
        }
        
        // Encrypt data before saving
        std::stringstream buffer;
        for (const auto& contact : contacts) {
            buffer << contact;
        }
        
        std::string encryptedData = encryptor.encrypt(buffer.str());
        file << encryptedData;
        file.close();
        
        logger.log("Contacts saved successfully: " + std::to_string(contacts.size()) + " contacts", "INFO");
    }

    void loadFromFile() {
        std::ifstream file(filename);
        if (!file.is_open()) {
            logger.log("No existing contact file found, starting fresh", "INFO");
            return;
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        
        // Decrypt data
        std::string decryptedData = encryptor.decrypt(buffer.str());
        std::stringstream decryptedStream(decryptedData);
        
        Contact contact;
        while (decryptedStream >> contact) {
            contacts.push_back(contact);
        }
        
        buildIndex();
        stats.update(contacts);
        logger.log("Loaded " + std::to_string(contacts.size()) + " contacts from file", "INFO");
    }

    void displayContacts(const std::vector<Contact>& contactList, bool compact = false) const {
        if (compact) {
            std::cout << "\n" << std::setw(4) << "ID" << " | "
                      << std::setw(20) << std::left << "Name" << " | "
                      << std::setw(15) << "Phone" << " | "
                      << std::setw(20) << "Email" << " | "
                      << "Fav" << std::endl;
            std::cout << std::string(70, '-') << std::endl;
            
            for (size_t i = 0; i < contactList.size(); ++i) {
                contactList[i].displayCompact();
            }
        } else {
            std::cout << "\n=== CONTACTS (" << contactList.size() << ") ===\n";
            for (size_t i = 0; i < contactList.size(); ++i) {
                std::cout << "Contact #" << i + 1 << ":\n";
                contactList[i].display();
            }
        }
    }

    void checkAutoBackup() {
        if (autoBackup) {
            auto now = std::time(nullptr);
            if (std::difftime(now, lastBackupTime) >= autoBackupInterval) {
                if (backupManager.createBackup(filename)) {
                    logger.log("Auto-backup created successfully", "INFO");
                    lastBackupTime = now;
                }
            }
        }
    }

public:
    ContactManager(const std::string& filename = "contacts.dat", 
                   bool enableAutoBackup = true, int backupInterval = 3600) 
        : filename(filename), logger(), encryptor(), backupManager(),
          autoBackup(enableAutoBackup), autoBackupInterval(backupInterval),
          lastBackupTime(std::time(nullptr)) {
        loadFromFile();
        std::cout << "Loaded " << contacts.size() << " contacts.\n";
    }

    ~ContactManager() {
        saveToFile();
        std::cout << "Saved " << contacts.size() << " contacts.\n";
    }

    // Advanced contact addition with validation
    bool addContact(const Contact& contact) {
        if (!InputValidator::isValidName(contact.getName())) {
            std::cout << "Error: Invalid name format!\n";
            logger.log("Failed to add contact: Invalid name - " + contact.getName(), "WARNING");
            return false;
        }
        
        if (!InputValidator::isValidPhone(contact.getPhone())) {
            std::cout << "Error: Invalid phone number format!\n";
            logger.log("Failed to add contact: Invalid phone - " + contact.getPhone(), "WARNING");
            return false;
        }
        
        if (!InputValidator::isValidEmail(contact.getEmail())) {
            std::cout << "Error: Invalid email format!\n";
            logger.log("Failed to add contact: Invalid email - " + contact.getEmail(), "WARNING");
            return false;
        }
        
        if (phoneExists(contact.getPhone())) {
            std::cout << "Error: Contact with phone number " << contact.getPhone() 
                      << " already exists!\n";
            logger.log("Failed to add contact: Duplicate phone - " + contact.getPhone(), "WARNING");
            return false;
        }
        
        contacts.push_back(contact);
        phoneIndex[contact.getPhone()] = &contacts.back();
        idIndex[contact.getContactId()] = &contacts.back();
        
        // Update tag index
        for (const auto& tag : contact.getTags()) {
            tagIndex[tag].push_back(&contacts.back());
        }
        
        stats.update(contacts);
        logger.log("Contact added: " + contact.getName() + " (" + contact.getPhone() + ")", "INFO");
        std::cout << "Contact added successfully! (ID: " << contact.getContactId() << ")\n";
        
        checkAutoBackup();
        return true;
    }

    // Multiple deletion options
    bool deleteContact(const std::string& phone) {
        auto it = phoneIndex.find(phone);
        if (it == phoneIndex.end()) {
            std::cout << "Contact with phone number " << phone << " not found!\n";
            return false;
        }
        
        logger.log("Contact deleted: " + it->second->getName() + " (" + phone + ")", "INFO");
        int contactId = it->second->getContactId();
        
        contacts.erase(std::remove_if(contacts.begin(), contacts.end(),
            [&](const Contact& c) { return c.getPhone() == phone; }), contacts.end());
        
        phoneIndex.erase(phone);
        idIndex.erase(contactId);
        buildIndex(); // Rebuild tag index
        stats.update(contacts);
        
        std::cout << "Contact deleted successfully!\n";
        checkAutoBackup();
        return true;
    }
    
    bool deleteContactById(int id) {
        auto it = idIndex.find(id);
        if (it == idIndex.end()) {
            std::cout << "Contact with ID " << id << " not found!\n";
            return false;
        }
        return deleteContact(it->second->getPhone());
    }

    // Advanced editing with partial updates
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

        // Name
        std::cout << "New name [" << contact.getName() << "]: ";
        std::getline(std::cin, input);
        if (!input.empty()) {
            if (!InputValidator::isValidName(input)) {
                std::cout << "Error: Invalid name format!\n";
                return false;
            }
            contact.setName(input);
        }

        // Phone
        std::cout << "New phone [" << contact.getPhone() << "]: ";
        std::getline(std::cin, input);
        if (!input.empty()) {
            if (!InputValidator::isValidPhone(input)) {
                std::cout << "Error: Invalid phone format!\n";
                return false;
            }
            if (phoneExists(input) && input != phone) {
                std::cout << "Error: Phone number already exists!\n";
                return false;
            }
            phoneIndex.erase(phone);
            contact.setPhone(input);
            phoneIndex[input] = &contact;
        }

        // Email
        std::cout << "New email [" << contact.getEmail() << "]: ";
        std::getline(std::cin, input);
        if (!input.empty()) {
            if (!InputValidator::isValidEmail(input)) {
                std::cout << "Error: Invalid email format!\n";
                return false;
            }
            contact.setEmail(input);
        }

        // Additional fields
        std::cout << "New address [" << contact.getAddress() << "]: ";
        std::getline(std::cin, input);
        if (!input.empty()) contact.setAddress(input);

        std::cout << "New company [" << contact.getCompany() << "]: ";
        std::getline(std::cin, input);
        if (!input.empty()) contact.setCompany(input);

        std::cout << "New job title [" << contact.getJobTitle() << "]: ";
        std::getline(std::cin, input);
        if (!input.empty()) contact.setJobTitle(input);

        std::cout << "New birthday (YYYY-MM-DD) [" << contact.getBirthday() << "]: ";
        std::getline(std::cin, input);
        if (!input.empty()) {
            if (!InputValidator::isValidDate(input)) {
                std::cout << "Error: Invalid date format! Use YYYY-MM-DD.\n";
                return false;
            }
            contact.setBirthday(input);
        }

        std::cout << "New website [" << contact.getWebsite() << "]: ";
        std::getline(std::cin, input);
        if (!input.empty()) contact.setWebsite(input);

        std::cout << "New social media [" << contact.getSocialMedia() << "]: ";
        std::getline(std::cin, input);
        if (!input.empty()) contact.setSocialMedia(input);

        std::cout << "New notes [" << contact.getNotes() << "]: ";
        std::getline(std::cin, input);
        if (!input.empty()) contact.setNotes(input);

        std::cout << "Toggle favorite (current: " << (contact.getIsFavorite() ? "Yes" : "No") << ") [y/N]: ";
        std::getline(std::cin, input);
        if (!input.empty() && (input[0] == 'y' || input[0] == 'Y')) {
            contact.setIsFavorite(!contact.getIsFavorite());
        }

        buildIndex(); // Rebuild indexes
        logger.log("Contact updated: " + contact.getName() + " (" + contact.getPhone() + ")", "INFO");
        std::cout << "Contact updated successfully!\n";
        checkAutoBackup();
        return true;
    }

    // Multiple display options
    void displayAllContacts(bool compact = false) const {
        if (contacts.empty()) {
            std::cout << "No contacts found.\n";
            return;
        }
        displayContacts(contacts, compact);
    }

    void displayFavorites(bool compact = false) const {
        std::vector<Contact> favorites;
        std::copy_if(contacts.begin(), contacts.end(), std::back_inserter(favorites),
                    [](const Contact& c) { return c.getIsFavorite(); });
        
        if (favorites.empty()) {
            std::cout << "No favorite contacts found.\n";
            return;
        }
        displayContacts(favorites, compact);
    }

    void displayRecent(int count = 10, bool compact = true) const {
        std::vector<Contact> recentContacts = contacts;
        std::sort(recentContacts.begin(), recentContacts.end(),
                 [](const Contact& a, const Contact& b) {
                     return a.getModifiedDate() > b.getModifiedDate();
                 });
        
        if (count < recentContacts.size()) {
            recentContacts.resize(count);
        }
        
        displayContacts(recentContacts, compact);
    }

    // Advanced search with multiple criteria
    void searchByName(const std::string& name) const {
        std::vector<Contact> results;
        std::copy_if(contacts.begin(), contacts.end(), std::back_inserter(results),
                    [&](const Contact& c) { 
                        std::string contactName = c.getName();
                        std::string searchName = name;
                        std::transform(contactName.begin(), contactName.end(), contactName.begin(), ::tolower);
                        std::transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);
                        return contactName.find(searchName) != std::string::npos;
                    });

        if (results.empty()) {
            std::cout << "No contacts found with name containing: " << name << std::endl;
        } else {
            std::cout << "Found " << results.size() << " contact(s):\n";
            displayContacts(results, true);
        }
    }

    void searchByPhone(const std::string& phone) const {
        auto it = phoneIndex.find(phone);
        if (it != phoneIndex.end()) {
            std::cout << "Contact found:\n";
            it->second->display();
        } else {
            // Partial phone search
            std::vector<Contact> results;
            std::copy_if(contacts.begin(), contacts.end(), std::back_inserter(results),
                        [&](const Contact& c) { 
                            return c.getPhone().find(phone) != std::string::npos;
                        });
            
            if (results.empty()) {
                std::cout << "No contact found with phone: " << phone << std::endl;
            } else {
                std::cout << "Found " << results.size() << " contact(s):\n";
                displayContacts(results, true);
            }
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
            displayContacts(results, true);
        }
    }

    void searchByCompany(const std::string& company) const {
        std::vector<Contact> results;
        std::string searchCompany = company;
        std::transform(searchCompany.begin(), searchCompany.end(), searchCompany.begin(), ::tolower);

        for (const auto& contact : contacts) {
            std::string contactCompany = contact.getCompany();
            std::transform(contactCompany.begin(), contactCompany.end(), contactCompany.begin(), ::tolower);
            if (contactCompany.find(searchCompany) != std::string::npos) {
                results.push_back(contact);
            }
        }

        if (results.empty()) {
            std::cout << "No contacts found with company containing: " << company << std::endl;
        } else {
            std::cout << "Found " << results.size() << " contact(s):\n";
            displayContacts(results, true);
        }
    }

    void searchByTag(const std::string& tag) const {
        auto it = tagIndex.find(tag);
        if (it != tagIndex.end()) {
            std::vector<Contact> results;
            for (const auto& contactPtr : it->second) {
                results.push_back(*contactPtr);
            }
            std::cout << "Found " << results.size() << " contact(s) with tag '" << tag << "':\n";
            displayContacts(results, true);
        } else {
            std::cout << "No contacts found with tag: " << tag << std::endl;
        }
    }

    void globalSearch(const std::string& query) const {
        std::vector<Contact> results;
        std::copy_if(contacts.begin(), contacts.end(), std::back_inserter(results),
                    [&](const Contact& c) { return c.matchesSearch(query); });

        if (results.empty()) {
            std::cout << "No contacts found matching: " << query << std::endl;
        } else {
            std::cout << "Found " << results.size() << " contact(s):\n";
            displayContacts(results, true);
        }
    }

    // Advanced sorting
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

    void sortByCompany() {
        std::sort(contacts.begin(), contacts.end(), 
            [](const Contact& a, const Contact& b) {
                return a.getCompany() < b.getCompany();
            });
        buildIndex();
        std::cout << "Contacts sorted by company.\n";
    }

    void sortByRecent() {
        std::sort(contacts.begin(), contacts.end(), 
            [](const Contact& a, const Contact& b) {
                return a.getModifiedDate() > b.getModifiedDate();
            });
        buildIndex();
        std::cout << "Contacts sorted by recent modification.\n";
    }

    // Tag management
    void addTagToContact(const std::string& phone, const std::string& tag) {
        auto it = phoneIndex.find(phone);
        if (it == phoneIndex.end()) {
            std::cout << "Contact not found!\n";
            return;
        }
        it->second->addTag(tag);
        tagIndex[tag].push_back(it->second);
        std::cout << "Tag '" << tag << "' added to contact.\n";
        checkAutoBackup();
    }

    void removeTagFromContact(const std::string& phone, const std::string& tag) {
        auto it = phoneIndex.find(phone);
        if (it == phoneIndex.end()) {
            std::cout << "Contact not found!\n";
            return;
        }
        it->second->removeTag(tag);
        
        // Update tag index
        auto& tagContacts = tagIndex[tag];
        tagContacts.erase(std::remove(tagContacts.begin(), tagContacts.end(), it->second), tagContacts.end());
        if (tagContacts.empty()) {
            tagIndex.erase(tag);
        }
        
        std::cout << "Tag '" << tag << "' removed from contact.\n";
        checkAutoBackup();
    }

    void listAllTags() const {
        std::cout << "\n=== ALL TAGS ===\n";
        if (tagIndex.empty()) {
            std::cout << "No tags found.\n";
            return;
        }
        for (const auto& entry : tagIndex) {
            std::cout << entry.first << " (" << entry.second.size() << " contacts)\n";
        }
    }

    // Import/Export
    bool exportToCSV(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cout << "Error: Could not create file " << filename << std::endl;
            return false;
        }

        // CSV header
        file << "ID,Name,Phone,Email,Address,Company,JobTitle,Birthday,Website,SocialMedia,Notes,Favorite,Tags\n";
        
        for (const auto& contact : contacts) {
            file << contact.getContactId() << ","
                 << "\"" << contact.getName() << "\","
                 << "\"" << contact.getPhone() << "\","
                 << "\"" << contact.getEmail() << "\","
                 << "\"" << contact.getAddress() << "\","
                 << "\"" << contact.getCompany() << "\","
                 << "\"" << contact.getJobTitle() << "\","
                 << "\"" << contact.getBirthday() << "\","
                 << "\"" << contact.getWebsite() << "\","
                 << "\"" << contact.getSocialMedia() << "\","
                 << "\"" << contact.getNotes() << "\","
                 << (contact.getIsFavorite() ? "Yes" : "No") << ",";
            
            // Tags
            file << "\"";
            const auto& tags = contact.getTags();
            for (size_t i = 0; i < tags.size(); ++i) {
                file << tags[i];
                if (i != tags.size() - 1) file << ";";
            }
            file << "\"\n";
        }
        
        file.close();
        std::cout << "Contacts exported to " << filename << " successfully!\n";
        // Remove const cast for logger call
        const_cast<Logger&>(logger).log("Contacts exported to CSV: " + filename, "INFO");
        return true;
    }

    // Duplicate detection
    void findDuplicates() const {
        std::cout << "\n=== DUPLICATE DETECTION ===\n";
        std::map<std::string, std::vector<Contact>> phoneDuplicates;
        std::map<std::string, std::vector<Contact>> emailDuplicates;
        
        for (const auto& contact : contacts) {
            if (!contact.getPhone().empty()) {
                phoneDuplicates[contact.getPhone()].push_back(contact);
            }
            if (!contact.getEmail().empty()) {
                emailDuplicates[contact.getEmail()].push_back(contact);
            }
        }
        
        bool foundDuplicates = false;
        
        // Check phone duplicates
        for (const auto& entry : phoneDuplicates) {
            if (entry.second.size() > 1) {
                foundDuplicates = true;
                std::cout << "Duplicate phone " << entry.first << ":\n";
                for (const auto& contact : entry.second) {
                    std::cout << "  - " << contact.getName() << " (ID: " << contact.getContactId() << ")\n";
                }
            }
        }
        
        // Check email duplicates
        for (const auto& entry : emailDuplicates) {
            if (entry.second.size() > 1) {
                foundDuplicates = true;
                std::cout << "Duplicate email " << entry.first << ":\n";
                for (const auto& contact : entry.second) {
                    std::cout << "  - " << contact.getName() << " (ID: " << contact.getContactId() << ")\n";
                }
            }
        }
        
        if (!foundDuplicates) {
            std::cout << "No duplicates found!\n";
        }
    }

    // Birthday reminders
    void upcomingBirthdays(int days = 30) const {
        std::cout << "\n=== UPCOMING BIRTHDAYS (next " << days << " days) ===\n";
        auto now = std::time(nullptr);
        std::tm* currentTm = std::localtime(&now);
        int currentYear = currentTm->tm_year + 1900;
        
        bool found = false;
        
        for (const auto& contact : contacts) {
            if (contact.getBirthday().empty()) continue;
            
            std::tm birthTm = {};
            std::istringstream ss(contact.getBirthday());
            ss >> std::get_time(&birthTm, "%Y-%m-%d");
            if (ss.fail()) continue;
            
            // Set to current year
            birthTm.tm_year = currentYear - 1900;
            auto nextBirthday = std::mktime(&birthTm);
            
            double diff = std::difftime(nextBirthday, now) / (60 * 60 * 24);
            if (diff >= 0 && diff <= days) {
                std::cout << contact.getName() << " - " << contact.getBirthday();
                if (diff == 0) {
                    std::cout << " (Today!)";
                } else if (diff == 1) {
                    std::cout << " (Tomorrow!)";
                } else {
                    std::cout << " (in " << static_cast<int>(diff) << " days)";
                }
                std::cout << " - Age: " << contact.getAge() + 1 << std::endl;
                found = true;
            }
        }
        
        if (!found) {
            std::cout << "No upcoming birthdays in the next " << days << " days.\n";
        }
    }

    // Bulk operations
    void bulkAddTags(const std::vector<std::string>& phones, const std::string& tag) {
        int successCount = 0;
        for (const auto& phone : phones) {
            auto it = phoneIndex.find(phone);
            if (it != phoneIndex.end()) {
                it->second->addTag(tag);
                tagIndex[tag].push_back(it->second);
                successCount++;
            }
        }
        std::cout << "Tag '" << tag << "' added to " << successCount << " contacts.\n";
        checkAutoBackup();
    }

    // Data validation and cleanup
    void validateAllContacts() {
        std::cout << "\n=== CONTACT VALIDATION ===\n";
        int invalidCount = 0;
        
        for (const auto& contact : contacts) {
            if (!InputValidator::isValidName(contact.getName())) {
                std::cout << "Invalid name: " << contact.getName() << " (ID: " << contact.getContactId() << ")\n";
                invalidCount++;
            }
            if (!InputValidator::isValidPhone(contact.getPhone())) {
                std::cout << "Invalid phone: " << contact.getPhone() << " (ID: " << contact.getContactId() << ")\n";
                invalidCount++;
            }
            if (!InputValidator::isValidEmail(contact.getEmail())) {
                std::cout << "Invalid email: " << contact.getEmail() << " (ID: " << contact.getContactId() << ")\n";
                invalidCount++;
            }
        }
        
        if (invalidCount == 0) {
            std::cout << "All contacts are valid!\n";
        } else {
            std::cout << "Found " << invalidCount << " validation issues.\n";
        }
    }

    // Backup management
    void createBackup() {
        if (backupManager.createBackup(filename)) {
            std::cout << "Backup created successfully!\n";
            logger.log("Manual backup created", "INFO");
        } else {
            std::cout << "Backup creation failed!\n";
        }
    }

    // Quick actions
    void toggleFavorite(const std::string& phone) {
        auto it = phoneIndex.find(phone);
        if (it != phoneIndex.end()) {
            it->second->setIsFavorite(!it->second->getIsFavorite());
            std::cout << "Contact " << (it->second->getIsFavorite() ? "added to" : "removed from") << " favorites.\n";
            checkAutoBackup();
        } else {
            std::cout << "Contact not found!\n";
        }
    }

    // Getters and utility functions
    size_t getContactCount() const {
        return contacts.size();
    }

    bool phoneExists(const std::string& phone) const {
        return phoneIndex.find(phone) != phoneIndex.end();
    }

    void displayStats() const {
        stats.display();
    }

    // Get contact by ID for external use
    Contact* getContactById(int id) {
        auto it = idIndex.find(id);
        if (it != idIndex.end()) {
            return it->second;
        }
        return nullptr;
    }

    // Get all contacts (for external processing)
    const std::vector<Contact>& getAllContacts() const {
        return contacts;
    }
};

// Enhanced UI functions
void displayMainMenu() {
    std::cout << "\n=== ADVANCED CONTACT MANAGEMENT SYSTEM ===\n";
    std::cout << "1. Add Contact\n";
    std::cout << "2. Display Contacts\n";
    std::cout << "3. Search Contacts\n";
    std::cout << "4. Edit Contact\n";
    std::cout << "5. Delete Contact\n";
    std::cout << "6. Sort Contacts\n";
    std::cout << "7. Contact Statistics\n";
    std::cout << "8. Tag Management\n";
    std::cout << "9. Import/Export\n";
    std::cout << "10. Advanced Features\n";
    std::cout << "11. Exit\n";
    std::cout << "Choose an option (1-11): ";
}

void displayAdvancedMenu(ContactManager& manager) {
    int choice;
    std::cout << "\n=== ADVANCED FEATURES ===\n";
    std::cout << "1. Find Duplicates\n";
    std::cout << "2. Upcoming Birthdays\n";
    std::cout << "3. Validate All Contacts\n";
    std::cout << "4. Create Backup\n";
    std::cout << "5. Display Favorites\n";
    std::cout << "6. Display Recent Contacts\n";
    std::cout << "7. Toggle Favorite\n";
    std::cout << "8. Back to Main Menu\n";
    std::cout << "Choose an option (1-8): ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (choice) {
        case 1: manager.findDuplicates(); break;
        case 2: manager.upcomingBirthdays(); break;
        case 3: manager.validateAllContacts(); break;
        case 4: manager.createBackup(); break;
        case 5: 
            {
                int displayChoice;
                std::cout << "Display: 1. Detailed 2. Compact: ";
                std::cin >> displayChoice;
                std::cin.ignore();
                manager.displayFavorites(displayChoice == 2);
            }
            break;
        case 6: manager.displayRecent(); break;
        case 7: {
            std::string phone;
            std::cout << "Enter phone number to toggle favorite: ";
            std::getline(std::cin, phone);
            manager.toggleFavorite(phone);
            break;
        }
        case 8: return;
        default: std::cout << "Invalid choice!\n";
    }
}

void displaySearchMenu(ContactManager& manager) {
    int choice;
    std::string query;

    std::cout << "\n=== ADVANCED SEARCH ===\n";
    std::cout << "1. Search by Name\n";
    std::cout << "2. Search by Phone\n";
    std::cout << "3. Search by Email\n";
    std::cout << "4. Search by Company\n";
    std::cout << "5. Search by Tag\n";
    std::cout << "6. Global Search\n";
    std::cout << "Choose search type (1-6): ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter search term: ";
    std::getline(std::cin, query);

    switch (choice) {
        case 1: manager.searchByName(query); break;
        case 2: manager.searchByPhone(query); break;
        case 3: manager.searchByEmail(query); break;
        case 4: manager.searchByCompany(query); break;
        case 5: manager.searchByTag(query); break;
        case 6: manager.globalSearch(query); break;
        default: std::cout << "Invalid choice!\n";
    }
}

void displaySortMenu(ContactManager& manager) {
    int choice;
    std::cout << "\n=== SORT OPTIONS ===\n";
    std::cout << "1. Sort by Name\n";
    std::cout << "2. Sort by Phone\n";
    std::cout << "3. Sort by Company\n";
    std::cout << "4. Sort by Recent\n";
    std::cout << "Choose sort type (1-4): ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (choice) {
        case 1: manager.sortByName(); break;
        case 2: manager.sortByPhone(); break;
        case 3: manager.sortByCompany(); break;
        case 4: manager.sortByRecent(); break;
        default: std::cout << "Invalid choice!\n";
    }
}

void displayTagMenu(ContactManager& manager) {
    int choice;
    std::string phone, tag;

    std::cout << "\n=== TAG MANAGEMENT ===\n";
    std::cout << "1. Add Tag to Contact\n";
    std::cout << "2. Remove Tag from Contact\n";
    std::cout << "3. List All Tags\n";
    std::cout << "Choose option (1-3): ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (choice) {
        case 1:
            std::cout << "Enter phone number: ";
            std::getline(std::cin, phone);
            std::cout << "Enter tag: ";
            std::getline(std::cin, tag);
            manager.addTagToContact(phone, tag);
            break;
        case 2:
            std::cout << "Enter phone number: ";
            std::getline(std::cin, phone);
            std::cout << "Enter tag: ";
            std::getline(std::cin, tag);
            manager.removeTagFromContact(phone, tag);
            break;
        case 3:
            manager.listAllTags();
            break;
        default: std::cout << "Invalid choice!\n";
    }
}

void addContactMenu(ContactManager& manager) {
    std::string name, phone, email, address, company, jobTitle, birthday, website, socialMedia, notes;

    std::cout << "\n=== ADD NEW CONTACT ===\n";
    
    do {
        std::cout << "Name: ";
        std::getline(std::cin, name);
        if (!InputValidator::isValidName(name)) {
            std::cout << "Error: Invalid name format! Use only letters, spaces, hyphens, and apostrophes.\n";
        }
    } while (!InputValidator::isValidName(name));
    
    do {
        std::cout << "Phone: ";
        std::getline(std::cin, phone);
        if (!InputValidator::isValidPhone(phone)) {
            std::cout << "Error: Invalid phone format! Use numbers, spaces, hyphens, and parentheses.\n";
        }
    } while (!InputValidator::isValidPhone(phone));
    
    do {
        std::cout << "Email (optional): ";
        std::getline(std::cin, email);
        if (!email.empty() && !InputValidator::isValidEmail(email)) {
            std::cout << "Error: Invalid email format!\n";
        }
    } while (!email.empty() && !InputValidator::isValidEmail(email));
    
    std::cout << "Address (optional): ";
    std::getline(std::cin, address);
    
    std::cout << "Company (optional): ";
    std::getline(std::cin, company);
    
    std::cout << "Job Title (optional): ";
    std::getline(std::cin, jobTitle);
    
    do {
        std::cout << "Birthday (YYYY-MM-DD, optional): ";
        std::getline(std::cin, birthday);
        if (!birthday.empty() && !InputValidator::isValidDate(birthday)) {
            std::cout << "Error: Invalid date format! Use YYYY-MM-DD.\n";
        }
    } while (!birthday.empty() && !InputValidator::isValidDate(birthday));
    
    std::cout << "Website (optional): ";
    std::getline(std::cin, website);
    
    std::cout << "Social Media (optional): ";
    std::getline(std::cin, socialMedia);
    
    std::cout << "Notes (optional): ";
    std::getline(std::cin, notes);

    Contact newContact(name, phone, email, address, company, jobTitle);
    if (!birthday.empty()) newContact.setBirthday(birthday);
    if (!website.empty()) newContact.setWebsite(website);
    if (!socialMedia.empty()) newContact.setSocialMedia(socialMedia);
    if (!notes.empty()) newContact.setNotes(notes);

    manager.addContact(newContact);
}

int main() {
    ContactManager manager("contacts.dat", true, 3600); // Auto-backup every hour
    int choice;

    std::cout << "Welcome to Advanced Contact Management System!\n";
    std::cout << "Features: Auto-backup, Encryption, Tags, Statistics, and more!\n";

    while (true) {
        displayMainMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: addContactMenu(manager); break;
            case 2: 
                {
                    int displayChoice;
                    std::cout << "Display: 1. Detailed 2. Compact: ";
                    std::cin >> displayChoice;
                    std::cin.ignore();
                    manager.displayAllContacts(displayChoice == 2);
                }
                break;
            case 3: displaySearchMenu(manager); break;
            case 4: {
                std::string phone;
                std::cout << "Enter phone number to edit: ";
                std::getline(std::cin, phone);
                manager.editContact(phone);
                break;
            }
            case 5: {
                int deleteChoice;
                std::cout << "Delete by: 1. Phone 2. ID: ";
                std::cin >> deleteChoice;
                std::cin.ignore();
                if (deleteChoice == 1) {
                    std::string phone;
                    std::cout << "Enter phone number to delete: ";
                    std::getline(std::cin, phone);
                    manager.deleteContact(phone);
                } else {
                    int id;
                    std::cout << "Enter contact ID to delete: ";
                    std::cin >> id;
                    std::cin.ignore();
                    manager.deleteContactById(id);
                }
                break;
            }
            case 6: displaySortMenu(manager); break;
            case 7: manager.displayStats(); break;
            case 8: displayTagMenu(manager); break;
            case 9: 
                manager.exportToCSV("contacts_export.csv");
                break;
            case 10: displayAdvancedMenu(manager); break;
            case 11:
                std::cout << "Thank you for using Advanced Contact Management System!\n";
                return 0;
            default:
                std::cout << "Invalid choice! Please try again.\n";
        }
    }
    return 0;
}