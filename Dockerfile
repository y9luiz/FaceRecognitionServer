from dkimg/opencv:4.5.5-ubuntu

LABEL maintainer="y9luiz"

RUN apt-get update && apt-get install -y \
    clang \
    cmake \
    libboost-dev \
    libboost-system-dev \
    libboost-date-time-dev \
    ninja-build

RUN mkdir faceRecognitionServer

WORKDIR faceRecognitionServer

COPY . .

RUN bash scripts/build_debug.sh
