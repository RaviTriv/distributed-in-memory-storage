# in-memory-storage

# How to run
Building and running server
1. `mkdir build`
2. `cd build`
3. `cmake ..`
4. `make server`
5. `./server`

To run the client open a new terminal and CD into the repo.
1. `cd build`
2. `make client`
3. `./client`

# Features

## Multiclient support
support via multithreading (will use a thread pool soon)

## Data persistence
write data to some external source for safety. In this case, data is written to a file.

## Replication
Master-Slave Architecture