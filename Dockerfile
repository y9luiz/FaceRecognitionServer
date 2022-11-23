from dkimg/opencv:4.5.5-ubuntu

LABEL maintainer="y9luiz"

RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    libboost-dev \
    libboost-system-dev \
    libboost-date-time-dev \
    ninja-build \
    lcov

WORKDIR /home/faceRecognitionServer

COPY . .
