#include <chrono>

void exec_time(std::chrono::high_resolution_clock::time_point* prev, char* proces_name)
{
    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    printf_s("%s: %dms\n",proces_name, (int)std::chrono::duration_cast<std::chrono::milliseconds>(now - *prev).count());
    *prev = now;
}

void print_speeds(uint8_t* speeds){
    printf_s("\t%d\t%d\t%d\t%d\t%d\n%d\t\t\t\t\t\t%d\n%d\t\t\t\t\t\t%d\n%d\t\t\t\t\t\t%d\n\t%d\t%d\t%d\t%d\t%d\n",
    speeds[0], speeds[1], speeds[2], speeds[3], speeds[4], speeds[15], speeds[5], speeds[14], speeds[6],
    speeds[13], speeds[7], speeds[12], speeds[11], speeds[10], speeds[9], speeds[8]);
}