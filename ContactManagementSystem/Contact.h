#ifndef CONTACT_H
#define CONTACT_H

#include <string>
#include <iostream>

class Contact {
private:
    std::string name;
    std::string phone;
    std::string email;
    std::string address;

public:
    // Constructors
    Contact();
    Contact(const std::string& name, const std::string& phone, 
            const std::string& email = "", const std::string& address = "");

    // Getters
    std::string getName() const;
    std::string getPhone() const;
    std::string getEmail() const;
    std::string getAddress() const;

    // Setters
    void setName(const std::string& name);
    void setPhone(const std::string& phone);
    void setEmail(const std::string& email);
    void setAddress(const std::string& address);

    // Display contact information
    void display() const;

    // Serialization for file I/O
    friend std::ostream& operator<<(std::ostream& os, const Contact& contact);
    friend std::istream& operator>>(std::istream& is, Contact& contact);

    // Comparison operators for sorting
    bool operator<(const Contact& other) const;
    bool operator==(const Contact& other) const;
};

#endif

