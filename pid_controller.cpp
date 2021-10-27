#include "pid_controller.h"
#include "pid.h"

int main()
{
    pid_handle_t* pid = NULL;

    auto err = pid_create_new_default(&pid);

    PID_LOG("sizeof(*pid) = %ld\n", sizeof(*pid));

    PID_LOG("pid pv.max =%.2f\npid->err = %d", pid->control.pv.max, pid->err);

    return 0;
}