#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    system("ls -la");
    sleep(5);
    system("ps");
    sleep(5);
    system("clear");

    return 0;
}
