#include "Student.hpp"
#include <iostream>
using overloading::Student;
using overloading::operator<<;

int main() {
    Student zlj(27, "Dong Bin");

    std::cout << zlj << std::endl;
}