FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update

RUN apt-get -y install python3-pip build-essential manpages-dev software-properties-common git cmake libpqxx-dev

RUN pip install conan

# dotenv-cpp
# This is a very simple library that wraps around functions provided
# In the standard library for accessing .env files.
WORKDIR /home
RUN rm -rf dotenv-cpp
RUN git clone --depth 1 --branch master git@github.com:laserpants/dotenv-cpp.git

# pegtl parser library
WORKDIR /home
RUN rm -rf pegtl
RUN git clone --depth 1 --branch master git@github.com:taocpp/PEGTL.git
RUN mkdir build
WORKDIR /home/PEGTL/build
RUN cmake ..
RUN make
RUN make install

#secp256k1
WORKDIR /home
RUN rm -rf secp256k1
RUN git clone --depth 1 --branch master https://github.com/Gigamonkey-BSV/secp256k1.git
WORKDIR /home/secp256k1
RUN conan install .
RUN CONAN_CPU_COUNT=1 conan create . proofofwork/stable

#data
WORKDIR /home
RUN rm -rf data
RUN git clone --depth 1 --branch sessions https://github.com/DanielKrawisz/data.git
WORKDIR /home/data
RUN conan install .
RUN CONAN_CPU_COUNT=1 conan create . proofofwork/stable

#gigamonkey
#WORKDIR /home
#RUN rm -rf Gigamonkey
#RUN git clone --depth 1 --branch sessions https://github.com/Gigamonkey-BSV/Gigamonkey.git
#WORKDIR /home/Gigamonkey
#RUN conan install .
#RUN CONAN_CPU_COUNT=1 conan create . proofofwork/stable

COPY . /home/node
WORKDIR /home/node
RUN chmod -R 777 .
RUN conan install . --output-folder=build --build=missing
WORKDIR /home/node/build
RUN cmake ..
RUN make

CMD ./bin/node


