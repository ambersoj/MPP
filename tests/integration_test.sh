#!/bin/bash

set -e  # Exit on first failure

echo "Running MPP Integration Tests..."

# Start core in the background
./main core &
CORE_PID=$!
sleep 1  # Give it time to start

# Start net in the background
./main net &
NET_PID=$!
sleep 1  # Give it time to start

# Send test commands
echo "start" | nc -U /tmp/mpp_core.sock
sleep 1
echo "stop" | nc -U /tmp/mpp_core.sock
sleep 1

# Check if core processed the commands
grep "Received command: start" logs/mpp.log
grep "Received command: stop" logs/mpp.log

echo "All tests passed!"

# Kill background processes
kill $CORE_PID $NET_PID
