FROM ubuntu

MAINTAINER Soltan Gheorghe

WORKDIR /root/build

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update -y
RUN apt-get upgrade -y

RUN apt-get install -y git
RUN apt-get install -y g++ make cmake libprotobuf-dev libboost-all-dev protobuf-compiler

RUN apt-get install -y vim
RUN apt-get install -y doxygen graphviz

RUN apt-get install -y software-properties-common

RUN apt-get install -y rapidjson-dev
RUN apt-get install -y curl
RUN apt-get install -y libcurl4-openssl-dev
RUN apt-get install -y libgtest-dev

RUN apt-get install sqlite3
RUN apt-get install libsqlite3-dev

RUN apt install -y nlohmann-json3-dev

RUN add-apt-repository ppa:pistache+team/unstable
RUN apt update
RUN apt install -y libpistache-dev

RUN apt-get install -y mosquitto
RUN apt-get install -y mosquitto-clients
RUN apt install -y net-tools

RUN apt-get install -y libmosquitto-dev


# install the paho c library
WORKDIR /home/libraries
RUN git clone https://github.com/eclipse/paho.mqtt.c.git
WORKDIR /home/libraries/paho.mqtt.c
RUN git checkout v1.3.8
RUN cmake -Bbuild -H. -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_STATIC=ON -DPAHO_WITH_SSL=ON -DPAHO_HIGH_PERFORMANCE=ON
RUN cmake --build build/ --target install
RUN ldconfig

#install the paho c++ library
WORKDIR /home/libraries
RUN git clone https://github.com/eclipse/paho.mqtt.cpp
WORKDIR /home/libraries/paho.mqtt.cpp
RUN cmake -Bbuild -H. -DPAHO_BUILD_STATIC=ON -DPAHO_BUILD_DOCUMENTATION=TRUE -DPAHO_BUILD_SAMPLES=TRUE
RUN cmake --build build/ --target install
RUN ldconfig

WORKDIR /home/pistache
COPY . /home/pistache


EXPOSE 9080
EXPOSE 1883/tcp

RUN cmake .
RUN make

CMD mosquitto



