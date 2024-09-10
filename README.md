# in-memory-storage

# How to run

Building and running server

1. `mkdir build`
2. `cd build`
3. `cmake ..`
4. `make server`
5. `./server`

Parameters can be passed in by doing the following, `./server ${PORT} ${NODE_ID}`

To run the client open a new terminal and CD into the repo.

1. `cd build`
2. `make client`
3. `./client`

# Features

## Multithreaded server

A thread pool is utilized to handle multiple connections simultaneously. Every time a connection request is made from a client it is added to the a work queue. Each thread deques a connection request and services it till the connection no longer exists and then moves onto to the next request in the queue.

![Multithreaded Server Diagram](/images/multiThreadedServer.png)

## Data persistence

write data to some external source for safety. In this case, data is written to a file.

## Replication

Master-Slave Architecture
