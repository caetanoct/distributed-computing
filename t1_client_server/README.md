# Use guide

## Compilation

```bash
make memory=1024 n=4
```
The number of servers and the memory for each server will be defined in **compilation** time.

## Running Servers

First you will need to initialize all servers, according to the number you gave on `make memory=1024 n=4`, for this example **n=4** which means 4 servers. you can use the **initialize_servers.sh** shell script or initilize them manually.

### Using initialize_servers.sh

To initialize with n=4:

```bash
./initialize_servers.sh 4
```

To kill servers use
```bash
./kill_servers.sh
```
### Manually

To initialize with n=4:

```./server.o 0```
```./server.o 1```
```./server.o 2```
```./server.o 3```

## Running Client

Read 8 characters from server starting at position 1024, first char from the second server address space, because we set memory=1024 at compilation.

```$ ./client.o 1 1024 8```

Write the word 'rice' starting at memory address 2050, 4 is the size of the word 'rice' and address 2050 will target the third server in our example.

```$ ./client.o 0 2050 4 rice```

In our example:

| 1st_server | 2nd_server   | 3rd_server   | 4th_server   |
|---------|-----------|-----------|-----------|
| 0-1023  | 1024-2047 | 2048-3071 | 3072-4095 |
