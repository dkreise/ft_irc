#!/bin/bash

# Check if correct number of arguments are provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <server_ip> <port>"
    exit 1
fi

SERVER_IP=$1
PORT=$2

# Function to send input with \r\n endings
send_input() {
    while IFS= read -r line; do
        printf "%s\r\n" "$line"
    done
}

# Connect to the server and send input
send_input | nc $SERVER_IP $PORT
