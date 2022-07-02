echo "INSTALLING DEPENDENCIES"

sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get -q update
sudo apt-get -y install clang
sudo apt-get install -y libc++-dev libc++abi-dev libboost-all-dev
