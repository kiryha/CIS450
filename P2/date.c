#include "types.h"
#include "user.h"
#include "date.h"

int
main(int argc, char *argv[])
{
    struct rtcdate date_data;

    if (date(&date_data)) {
        printf(2, "date failed\n");
        exit();
    }

    // Print the time as a formatted string
    printf(1, "Current date-and-time is %d-%d-%d %d:%d:%d\n",
           date_data.month,
           date_data.day,
           date_data.year,
           date_data.hour,
           date_data.minute,
           date_data.second);
    exit();
}
int sys_date(void)
{
    struct rtcdate *date_data;
    if(argptr(0, (void*)&date_data, sizeof(date_data)) < 0)
        return -1;
    cmostime(date_data);
    return 0;
 }
