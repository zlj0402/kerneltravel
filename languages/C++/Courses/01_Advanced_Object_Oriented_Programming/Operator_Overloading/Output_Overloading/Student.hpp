/**
 * @brief: 主要尝试二元运算符重载，第一参数和第二参数位置的必要性；
 * @date: 14/5/2026
 */

#include <string>
#include <ostream>

namespace overloading {

class Student {
private:
    int age_;
    std::string name_;

public:
    Student(int age = 0, std::string name) : age_(age), name_(name) {}

    int age() const {
        return age_;
    }

    std::string name() const {
        return name_;
    }
};

// inline std::ostream& operator << (const Student &stu, std::ostream &os) {
inline std::ostream& operator << (std::ostream &os, const Student &stu) {
    return os << stu.age() << ", " << stu.name();
}

}