# FaceRecognitionServer

You can watch the development of this project on my Youtube Channel [Youtube Channel](https://www.youtube.com/channel/UCSSw2imkxpZ-_8PPds_5sQA)

## Enviroment Setup

### Dependencies
  
  - C++17
  - Boost Library 1.65+
  
### Ubuntu 18.04+

On ubuntu 18.04+ run the following command lines: 

#### Installing dependencies
```
sudo apt-get update
sudo apt-get install -y git
sudo apt-get install -y cmake
sudo apt-get install -y build-essential
sudo apt-get install -y libboost-dev
sudo apt-get install -y libboost-system-dev
sudo apt-get install -y libboost-date-time-dev
```
#### Build from souce 

```
git clone https://github.com/y9luiz/FaceRecognitionServer.git
cd  FaceRecognitionServer
mkdir build
cd build
cmake ..
cmake --build .
```

### Windows

For windows you need download cmake and boost from source or use chocolatey

### Installing dependencies via chocolatey

<b>You have to install Microsoft Visual Studio first with C++ Development support</b>

```
choco install choco install boost-msvc-14.1 -y
choco install cmake -y
```

#### Build from souce 

```
git clone https://github.com/y9luiz/FaceRecognitionServer.git
cd  FaceRecognitionServer
mkdir build
cd build
cmake ..
cmake --build .
```

