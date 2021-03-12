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

WORKDIR /home/pistache
COPY . /home/pistache


EXPOSE 9080
RUN cmake .
RUN make
CMD ./smart_fridge_api



