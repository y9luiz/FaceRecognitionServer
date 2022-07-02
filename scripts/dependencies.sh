#!/bin/bash
echo "INSTALLING DEPENDENCIES"

apt-get update
apt-get install -y clang
apt-get install -y cmake
apt-get install -y libboost-dev
apt-get install -y libboost-system-dev
apt-get install -y libboost-date-time-dev
apt-get install -y ninja-build