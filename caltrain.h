#include <pthread.h>

struct station {
    // FILL ME IN
    int waiting_passengers;
    int onboard_passengers;
    int no_free_seats;
    pthread_cond_t passengers_cond;
    pthread_cond_t train_toleave_cond;
    pthread_cond_t passenger_seated_cond;
    pthread_mutex_t st_mutex;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);