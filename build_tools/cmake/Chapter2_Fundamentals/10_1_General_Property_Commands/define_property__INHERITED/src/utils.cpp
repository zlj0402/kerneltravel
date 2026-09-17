#include "utils.hpp"
#include <iostream>
#include <ctime>
#include <iomanip>

namespace timeutil {

void currentTime() {
	std::time_t now = std::time(nullptr);
	std::tm* local = std::localtime(&now);
	std::cout << "current time: " << std::put_time(local, "%Y-%m-%d %H:%M.%S") << std::endl;
}

} // namespace timeutil