# -Calltrain-problem-for-Synchronization-and-Mutual-Exclusion
 The code implements synchronization and mutual exclusion for a train station scenario using pthreads in C

# -1. station_init(struct station station):

**Initializes the station struct with counters for waiting passengers, onboard passengers, and free seats.**
**Initializes three condition variables:**

passengers_cond: Signals waiting passengers when a train arrives with free seats.

train_toleave_cond: Signals the train to leave when all passengers are seated.

passenger_seated_cond: Signals the station_load_train function that passengers are seated.

**Initializes a mutex st_mutex to control access to the shared station data.**

# -2. station_load_train(struct station station, int count):

Checks if there are any passengers waiting (station->waiting_passengers) or the incoming train has no free seats (count == 0). If so, the train won't be loaded (returns).

Updates no_free_seats with the number of free seats on the train.

Acquires the station mutex (pthread_mutex_lock).

Broadcasts a signal (pthread_cond_broadcast) to all waiting passengers that there might be free seats.

Loops while there are free seats and waiting passengers:
Waits (pthread_cond_wait) on the passenger_seated_cond variable with the mutex held. This ensures the function doesn't proceed until passengers are seated.

Optionally waits (pthread_cond_wait) on the train_toleave_cond variable if there are already onboard passengers. This ensures the train doesn't leave until all passengers have disembarked.

Releases the station mutex (pthread_mutex_unlock).

# -3. station_wait_for_train(struct station station):

Acquires the station mutex (pthread_mutex_lock).

Increments waiting_passengers to indicate a new waiting passenger.

Loops while there are no free seats or no waiting passengers (potential race condition):
Waits (pthread_cond_wait) on the passengers_cond variable with the mutex held. This ensures the thread waits for a signal that there might be free seats.

Updates onboard_passengers and no_free_seats to reflect the passenger boarding the train.

Decrements waiting_passengers as one less passenger is waiting.

Releases the station mutex (pthread_mutex_unlock).

Signals (pthread_cond_signal) on the passenger_seated_cond variable to inform station_load_train that a passenger is seated.

# -4. station_on_board(struct station station):

Decrements onboard_passengers to indicate a passenger disembarking.

If all passengers have disembarked (onboard_passengers == 0), signals (pthread_cond_signal) on the train_toleave_cond variable to notify the train that it can leave the station.

This code utilizes condition variables and a mutex to achieve synchronization and mutual exclusion:

The mutex (st_mutex) ensures exclusive access to the shared station data, preventing race conditions between threads.
Condition variables (passengers_cond, train_toleave_cond, and passenger_seated_cond) allow threads to wait for specific events (e.g., train arrival, all passengers seated) before proceeding.
