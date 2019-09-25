/*
    Dining philosophers problem:
    Rules:
    Each philosopher behaves as follows:
    -Think until the left fork is available; when it is, pick it up;
    -Think until the right fork is avaliable; when it is, pick it up;
    -When both forks are held, eat for a fixed amount of time;
    -Then put the right fork down;
    -Then put the left fork down
    -repeat from the beginning

    Design:
        Philosophers = threads
        Forks = mutexes 
        
*/

/*
        P0
      F4  F0
   P4        P1
   F3        F1
     P3    P2
        F2


*/


#include <pthread.h> 
#include <unistd.h>
#include <stdio.h>

const int NUM_PHIL = 5;
pthread_mutex_t forks[NUM_PHIL];

void pick_up_left_fork(int phil_number){
    int fork_number;
    if (phil_number == NUM_PHIL-1)
        fork_number = 0;
    else
        fork_number = phil_number;
    printf("Phil number %d waiting to pick up left fork \n", phil_number);
    pthread_mutex_lock(&forks[fork_number]); 
    printf("Phil number %d picked up left fork \n", phil_number);
}

void put_down_left_fork(int phil_number){
    int fork_number;
    if (phil_number == NUM_PHIL-1)
        fork_number = 0;
    else
        fork_number = phil_number;
    printf("Phil number %d putting down left \n", phil_number);
    pthread_mutex_unlock(&forks[fork_number]); 
    printf("Phil number %d has put down left fork \n", phil_number);
}

void pick_up_right_fork(int phil_number){
    int fork_number;
    if (phil_number == 0)
        fork_number = NUM_PHIL-1;
    else
        fork_number = phil_number -1;

    printf("Phil number %d waiting to pick up right fork \n", phil_number);
    pthread_mutex_lock(&forks[fork_number]); 
    printf("Phil number %d picked up right fork \n", phil_number);
}

void put_down_right_fork(int phil_number){
  int fork_number;
    if (phil_number == 0)
        fork_number = NUM_PHIL-1;
    else
        fork_number = phil_number -1;

    printf("Phil number %d putting down right fork \n", phil_number);
    pthread_mutex_unlock(&forks[fork_number]); 
    printf("Phil number %d has put down right fork \n", phil_number);
}

void eat(int phil_number){
    printf("-----Philosopher %d is eating \n", phil_number);
    sleep(1);
}

void* philBehaviour(void* phil_number){
    while(1){
        pick_up_left_fork((int)phil_number);
        pick_up_right_fork((int)phil_number);
        eat((int)phil_number);
        put_down_right_fork((int)phil_number);
        put_down_left_fork((int)phil_number);
    }
}

int main(){
    
    for (int i = 0; i < NUM_PHIL; i++){
        pthread_mutex_init(&forks[i], NULL); // Init all forks
    }

    pthread_t philosophers[NUM_PHIL];

    for (long i = 0; i < NUM_PHIL; i++) {
		pthread_create(&philosophers[i], NULL, philBehaviour, (void*) i);
	}
	
	for (int i = 0; i < NUM_PHIL; i++) {
		pthread_join(philosophers[i], NULL);
	}
    

     for (int i = 0; i < NUM_PHIL; i++){
        pthread_mutex_destroy(&forks[i]); // Destroy all forks
    }

}

