# Set up Debian as a build system

FROM debian:latest
RUN apt-get update && apt-get install -y \
    build-essential \
    curl \
    jq

# Set up environment to build for Debian
WORKDIR /proj
ENV COMMAND="build"
RUN \
    LATEST_TAG=$(curl -s https://api.github.com/repos/blueOkiris/acbs/releases/latest | jq -r .tag_name) \
    && curl \
        -L -o acbs-${LATEST_TAG}.linux-x86_64 \
        https://github.com/blueOkiris/acbs/releases/download/${LATEST_TAG}/acbs-${LATEST_TAG}.linux-x86_64 \
    && chmod +x acbs-${LATEST_TAG}.linux-x86_64 \
    && mv acbs-${LATEST_TAG}.linux-x86_64 /usr/bin/acbs
ENTRYPOINT [ "sh", "-c", "acbs ${COMMAND}" ]

