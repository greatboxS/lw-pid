#include "pid_controller.h"
#include "pid.h"

int main()
{
    auto pid = pid_create_new();

    PID_LOG("sizeof(*pid) = %ld\n", sizeof(*pid));

    return 0;
}