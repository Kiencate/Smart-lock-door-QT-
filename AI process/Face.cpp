#include <string>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <chrono>
#include <string>

volatile int flag_ui = 0;
int getpidui()
{
    char line_face[30];
    FILE *cmd_face = popen("pidof button", "r");
    fgets(line_face, 30, cmd_face);
    pid_t pid_ui = strtoul(line_face, NULL, 10);
    pclose(cmd_face);
    return pid_ui;
}

void usrface(int sig)
{
    flag_ui = 1;
}

void do_infer(){

    for(int i = 0; i<10000000; i++)
    {
        if(i % 9000000 == 0)
        {
            kill(getpidui(), SIGUSR2);
        }
    }
    std::cout<<"face ok\n";
}
int main(int argc, char **argv)
{
    if (signal(SIGUSR1, usrface)==SIG_ERR){
        perror("\nSIGUSR1");
        exit(3);
    }
    while(flag_ui == 0)
    {
        usleep(50);
    }

    do_infer();
    return 0;
}