from dkimg/opencv:4.5.5-ubuntu

LABEL maintainer="y9luiz"

RUN groupadd -g 1000 default

RUN adduser --disabled-password --gecos "" --force-badname --gid 1000 ubuntu

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

RUN chown -R ubuntu:default /home/faceRecognitionServer/

USER ubuntu
