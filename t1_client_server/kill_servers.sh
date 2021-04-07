#!/bin/sh
cat server_pids.txt | while read line; do
  echo "killing PID $line"
  kill -9 $line
done
rm server_pids.txt