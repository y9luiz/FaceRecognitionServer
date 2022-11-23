from dkimg/opencv:4.5.5-ubuntu

LABEL maintainer="y9luiz"

RUN groupadd -g 1000 default

RUN useradd -ou 0 -g 0 runner

RUN usermod -a -G default runner

USER runner

RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    libboost-dev \
    libboost-system-dev \
    libboost-date-time-dev \
    ninja-build \
    lcov

WORKDIR /home/runner/faceRecognitionServer/

COPY . .

RUN chown -R runner:default /home/runner/faceRecognitionServer/
