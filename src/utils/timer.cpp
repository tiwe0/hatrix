#include "hatrix/utils/timer.hpp"

float
get_current_time()
{
    auto current_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<float>(current_time.time_since_epoch()).count();
}

std::string generate_timestamp_id() {
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    auto high_res_time = std::chrono::high_resolution_clock::now();
    auto fractional_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(high_res_time.time_since_epoch()).count() % 1000000;
    std::ostringstream oss;
    oss << timestamp << std::setfill('0') << std::setw(6) << fractional_ns;

    return oss.str();
}
