#!/bin/sh
mkdir server_logs 2> /dev/null
echo "usage example: ./initialize_servers 4"
echo "initializing $1 servers"
END=`expr $1 - 1`
for i in $(seq 0 $END); do 
	echo "initializing server $i"
	LOGFILE="log$1.txt"
	./server.o $i > ./server_logs/log$i.txt 2>&1 &
	PID=$!
	echo $PID >> server_pids.txt
	echo "server pid is $PID"
	echo "created logfile at ./server_logs/log$i.txt"
done
