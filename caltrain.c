#include <pthread.h>
#include "caltrain.h"
void station_init(struct station *station){
	station->waiting_passengers = 0;
    station->onboard_passengers =0;
    station->no_free_seats = 0;
    pthread_cond_init(&station->passengers_cond, NULL); //cond var waiting for train to signal arrival
    pthread_cond_init(&station->train_toleave_cond, NULL); //cond var signal train is full and ready to leave
    pthread_cond_init(&station->passenger_seated_cond, NULL); //cond var signal that passseners are seated
    pthread_mutex_init(&station->st_mutex, NULL);
    
}

void station_load_train(struct station *station, int count){
    if (count == 0 || station ->waiting_passengers == 0) // train will not be loaded
        return;
    station->no_free_seats = count;
    pthread_mutex_lock(&station->st_mutex);
    if (station->no_free_seats > 0) {
        pthread_cond_broadcast(&station->passengers_cond);
    }
    while (station->no_free_seats > 0 && station->waiting_passengers > 0) {
        pthread_cond_wait(&station->passenger_seated_cond, &station->st_mutex);
    }
    if (station->onboard_passengers > 0) {
        pthread_cond_wait(&station->train_toleave_cond, &station->st_mutex);
    }
    pthread_mutex_unlock(&station->st_mutex);
}
void station_wait_for_train(struct station *station){
    pthread_mutex_lock(&station->st_mutex);
    station->waiting_passengers++;
    while (station->no_free_seats == 0 || station->waiting_passengers == 0) {
        pthread_cond_wait(&station->passengers_cond,&station->st_mutex);//passenger thread wait for train to arrive
    }
    station->onboard_passengers++;
	station->no_free_seats--;
    station->waiting_passengers--;
    pthread_mutex_unlock(&station->st_mutex);
    pthread_cond_signal(&station->passenger_seated_cond);
}

void station_on_board(struct station *station){
    station->onboard_passengers--;
    if (station->onboard_passengers == 0)
        pthread_cond_signal(&station->train_toleave_cond);
}
