FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update

RUN apt-get -y install python3-pip build-essential manpages-dev software-properties-common git cmake

RUN add-apt-repository ppa:ubuntu-toolchain-r/test

RUN apt-get update && apt-get -y install gcc-11 g++-11

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 20

RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 20

RUN pip install conan

WORKDIR /home
RUN conan --version

# dotenv-cpp
WORKDIR /home
RUN rm -rf dotenv-cpp
RUN git clone --depth 1 --branch master https://github.com/laserpants/dotenv-cpp.git
WORKDIR /home/dotenv-cpp
RUN ls
WORKDIR /home/dotenv-cpp/build
RUN cmake ..
RUN make
RUN make install

COPY . /home/node
WORKDIR /home/node
RUN chmod -R 777 .
WORKDIR /home/node/build
RUN cmake ..
RUN make

CMD ./bin/node


