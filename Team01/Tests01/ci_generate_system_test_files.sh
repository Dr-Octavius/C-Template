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
