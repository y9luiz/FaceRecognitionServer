from dkimg/opencv:4.5.5-ubuntu

LABEL maintainer="y9luiz"

ARG UID=1000

RUN useradd -m -u $UID -g root -o -s /bin/bash runner

RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    libboost-dev \
    libboost-system-dev \
    libboost-date-time-dev \
    ninja-build \
    lcov


RUN mkdir /faceRecognitionServer

RUN chown $UID:$UID /faceRecognitionServer

USER $UID

WORKDIR /faceRecognitionServer

COPY --chown=$UID . .