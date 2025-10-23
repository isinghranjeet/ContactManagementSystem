cmake_minimum_required(VERSION 3.10)
project(ContactManagementSystem)

set(CMAKE_CXX_STANDARD 14)

add_executable(ContactManagementSystem
    main.cpp
    ContactManager.cpp
    Contact.cpp
)