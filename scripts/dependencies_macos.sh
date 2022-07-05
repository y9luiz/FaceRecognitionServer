#!/bin/bash
echo "INSTALLING DEPENDENCIES"
brew update
brew install -y clang
brew install -y cmake
brew install -y libboost-dev
brew install -y libboost-system-dev
brew install -y libboost-date-time-dev
brew install -y ninja-build