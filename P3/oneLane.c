#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define number_of_cars 50
#define bridge_capacity 3

pthread_mutex_t total_cars_mutex;
pthread_cond_t safe;

int total_cars = 0;
int current_direction = 0;

// Determine if a car can cross the bridge in a given direction
bool safe_to_cross(int direction){
	bool safe = false;

	// If the bridge is empty
	if(total_cars==0){
		safe = true;;
	}

	// If less then bridge_capacity and cars going the same direction
	else if ((total_cars < bridge_capacity) && (current_direction == direction)){
		safe = true;
	}

	// If bridge is full or has opposite traffic
	else{
		safe = false;
	}

	return safe;
}


// Vehicle arriving to the bridge
void arrive_at_bridge(int direction, int id){
	pthread_mutex_lock(&total_cars_mutex);

    // Wait if is not safe to cross the bridge
	while(!safe_to_cross(direction)){
		pthread_cond_wait(&safe, &total_cars_mutex);
	}

	printf("Car [ %d, %d ] arrived. current_direction, total_cars [ %d, %d ]\n", id, direction, current_direction, total_cars);

	total_cars++;
	current_direction = direction;
	pthread_mutex_unlock(&total_cars_mutex);
	sched_yield();

	return;
}


// Vehicle crossing the bridge
void cross_the_bridge(int direction, int id){
	pthread_mutex_lock(&total_cars_mutex);

	printf("Car [ %d, %d ] is crossing. current_direction, total_cars [ %d, %d ]\n", id, direction, current_direction, total_cars);

	pthread_mutex_unlock(&total_cars_mutex);
	sched_yield();

	return;
}


// Vehicle exiting the bridge
void exit_the_bridge(int direction, int id){
	pthread_mutex_lock(&total_cars_mutex);

	total_cars--;

	printf("Car [ %d, %d ] exited. current_direction, total_cars [ %d, %d ]\n", id, direction, current_direction, total_cars);

	pthread_cond_broadcast(&safe);
	pthread_mutex_unlock(&total_cars_mutex);
	sched_yield();

	return;
}


// Process vehicle crossing the bridge
void *handle_current_car(void *idv){
	int id = *((int*)(&idv));

	int direction = rand() % 2;  //Generate a random direction

	arrive_at_bridge(direction, id);
	cross_the_bridge(direction, id);
	exit_the_bridge(direction, id);

	pthread_exit(NULL);
}


int main(int argc, char *argv[]){
	int i;

	// Create thread for each car
	pthread_t threads[number_of_cars];
	pthread_attr_t attr;

	// Initialize mutex and conditional variables
	pthread_mutex_init(&total_cars_mutex, NULL);
	pthread_cond_init(&safe, NULL);

	// Create threads
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	srand(time(NULL));  // Get random number

	for(i=0; i<number_of_cars; i++){
		pthread_create(&threads[i], &attr, handle_current_car, (void*)i);
	}

	for(i=0; i<number_of_cars; i++){
		pthread_join(threads[i], NULL);
	}

	printf("%d cars crossed the bridge.\n", number_of_cars);

	// Clean up and exit
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&total_cars_mutex);
	pthread_cond_destroy(&safe);
	pthread_exit(NULL);
}