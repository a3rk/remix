# Multistage docker build, requires docker 17.05

# builder stage
FROM ubuntu:16.04 as builder

RUN apt-get update && \
    apt-get --no-install-recommends --yes install \
        ca-certificates \
        cmake \
        g++ \
        libboost1.58-all-dev \
        libssl-dev \
        libzmq3-dev \
        libreadline-dev \
        libsodium-dev \
        make \
        pkg-config \
        graphviz \
        doxygen \
        git

WORKDIR /src
COPY . .
RUN rm -rf build && \
    make -j$(nproc) release-static

# runtime stage
FROM ubuntu:16.04

RUN apt-get update && \
    apt-get --no-install-recommends --yes install ca-certificates && \
    apt-get clean && \
    rm -rf /var/lib/apt

COPY --from=builder /src/build/release/bin/* /usr/local/bin/

# Contains the blockchain
VOLUME /root/.remix

# Generate your wallet via accessing the container and run:
# cd /wallet
# remix-wallet-cli
VOLUME /wallet

EXPOSE 11330
EXPOSE 11331

ENTRYPOINT ["remixd", "--p2p-bind-ip=0.0.0.0", "--p2p-bind-port=18080", "--rpc-bind-ip=127.0.0.1", "--rpc-bind-port=18081", "--non-interactive"] 
