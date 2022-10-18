// Grant Guernsey

#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include <time.h>


void SIGINT_signal(int signum)
{ 
    // The child comes into this function after the signal has been changed in the ignore sigal to honor the second request
    printf("%d has received a %d that is being honored\n", getpid(), signum);
    exit(0);
}
void ignore_signal(int signum)
{ 
    // Child comes here after the first signal interupt and prints out that it is not being honored
    printf("%d has received a %d that is being ignored\n", getpid(), signum);

    // Changes it to honor the next signal and go to the function abobe
    signal(SIGINT, SIGINT_signal);
}


// Infiniate loop for the children to go to to hang out in
void child_hang()
{
    while(1);
}

int main(int argc, char **argv, char **envp)
{

    // Parent creates 5 children then waits
    // An array that will hold the id's of all of the chidren is created
    int child_id[5];

    // Iterates 5 times to create 5 children
    for(int x=0; x < 5; x++)
    {
        // The fork is called and saved into the array
        child_id[x] = fork();

        // Child process enters this statement after the fork
        if(child_id[x] == 0)
        {
            // Changes to be the ignore signal for sig int
            signal(SIGINT, ignore_signal);

            // Sends to child to the inifinte loop function
            child_hang();
        }
        
    }

    // Counts down from this number and prints it out
    int countdown = 5;

    // Keeps track of how many kill calls
    int kill_count = 0;

    // Infinte loop to run the code
    while(1){
        // couting down
        printf("\nCountdown at %d", countdown);
        sleep(1);
        countdown--;
        
        // If the coundown is zero runs this statement
        if(countdown == 0)
        {
            // Resets the countdown to 5
            countdown = 5;

            // Calls sigint to all of the children
            for(int i = 0; i < 5; i++){
                kill(child_id[i], SIGINT);
            }
            
            // If it is after the second call of kill it goes kere
            if(kill_count == 1)
            {
                // Waits for the processes to all die
                wait(NULL);
                // Prints a new line to look better in the terminal
                printf("\n");
                // parent process exits
                exit(0);
            }
            kill_count ++;
        }
    }
        

}