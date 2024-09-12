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

A thread pool is utilized to handle multiple connections simultaneously. Every time a connection request is made from a client it is added to the connection work queue. Each thread deques a connection request and services it till the connection no longer exists and then moves onto to the next request in the queue.

![Multithreaded Server Diagram](/images/multiThreadedServer.png)

## Data persistence

Persistence can be enabled to safetly store data. There are two ways this can be done, every x interval data is written to persistent storage or every time a write request is made to memory the request will be forked and also written to persistent storage.

### Fork to write to durable storage

From the client send a message in the following format.
<br>`Persistence Write:{Key} {Value}`

## Replication

Master-Slave Architecture
