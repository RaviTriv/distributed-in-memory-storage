# distributed-in-memory-storage

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
3. `./client ${PORT}`

# Features

## Multithreaded server

A thread pool is utilized to handle multiple connections simultaneously. Every time a connection request is made from a client it is added to the connection work queue. Each thread deques a connection request and services it till the connection no longer exists and then moves onto to the next request in the queue.

![Multithreaded Server Diagram](/images/multiThreadedServer.png)

## Data persistence

Persistence can be enabled to safetly store data. Every time a write request is made to memory the request will be forked and also written to persistent storage.

![Forked Persistence Diagram](/images/ForkedPersistence.png)

### Fork to write to durable storage

Persistence can be enabled/disabled via command line by passing 1 to enable 0 to disable.
<br> `./server ${PORT} ${NODE_ID} ${Persistence}`

## Replication

Master-Slave Architecture

![Master Slave Diagram](/images/master-slave.png)

Several instances of the server will be running with one master and `x` slaves. Only the master will handle write requests, read requests will be sent to the slaves. Slaves will be updated every time a write request is made to master. When the master goes down a slave will be promoted to the role of master to a slave.
