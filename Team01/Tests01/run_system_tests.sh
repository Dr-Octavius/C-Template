#!/bin/bash
is_port_in_use() {
    if lsof -i:$1 > /dev/null; then
        # Port is in use
        return 0
    else
        # Port is not in use
        return 1
    fi
}

# Function to show a simple loading indicator
show_loading() {
    mypid=$!
    loadingText=$1

    echo -ne "$loadingText\r"

    while kill -0 $mypid 2>/dev/null; do
        for i in '/' '-' '|' '\'; do
            echo -ne "\r$loadingText$i"
            sleep .1
        done
    done
    echo -ne "\r$loadingText...Completed.\n"
}

# Check if Node is installed
if ! command -v node &> /dev/null
then
    echo "Node could not be found, attempting to install..."
    # Adapt this part based on your system's package manager, e.g., apt for Ubuntu, brew for macOS
    curl -sL https://deb.nodesource.com/setup_14.x | sudo -E bash - & show_loading "Installing Node.js"
    sudo apt-get install -y nodejs & show_loading "Installing Node.js"
fi

# Run testRunner.js
echo "Running SPA tests..."
node testRunner.js & show_loading "Running tests"

# Start a Python HTTP server
PORT=8000
SERVER_STARTED=0

start_server() {
    if command -v python3 &> /dev/null; then
        python3 -m http.server $1 & show_loading &
        SERVER_STARTED=1
    elif command -v python &> /dev/null; then
        python -m http.server $1 & show_loading &
        SERVER_STARTED=1
    else
        echo "Neither python3 nor python could be found"
        exit 1
    fi
}

while [ $SERVER_STARTED -eq 0 ]; do
    if is_port_in_use 8000; then
        echo "Server is running on Port $PORT. Don't Need to start another one."
        SERVER_STARTED=1
    else
        echo "Starting HTTP server on port $PORT..."
        start_server $PORT
    fi
done

echo "Server started on port $PORT. Visit http://localhost:$PORT"

# Open the browser
if [ "$(uname)" == "Darwin" ]; then
    open http://localhost:$PORT
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    xdg-open http://localhost:$PORT
fi

wait
