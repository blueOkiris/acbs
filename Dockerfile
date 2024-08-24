# Set up Debian as a build system

FROM debian:latest
RUN apt-get update && apt-get install -y \
    build-essential

# Set up environment to build for Debian
WORKDIR /proj
ENTRYPOINT [ "make -j$(nproc)" ]

