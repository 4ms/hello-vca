# Hello VCA firmware builder
# mount source at /workspace/hello-vca.
# Output binary: build/main.uimg on host
# Usage on host:
#   git clone https://github.com/4ms/hello-vca.git
#   cd hello-vca
#   git submodule update --init --recursive
#   docker build -t hello-vca-fw .
#   docker run --rm -it -v $(pwd):/workspace/hello-vca hello-vca-fw

FROM debian:bookworm-slim
LABEL org.opencontainers.image.source=https://github.com/4ms/hello-vca
RUN apt-get update -qq && apt-get install -y -qq --no-install-recommends \
    git cmake ninja-build build-essential python3 wget file xxd tar xz-utils \
    ca-certificates flatbuffers-compiler libflatbuffers-dev \
    && rm -rf /var/lib/apt/lists/*
ARG ARM_TOOLCHAIN_VERSION=14.2.rel1
RUN ARCH=$(uname -m) && \
    case "$ARCH" in \
        x86_64)  TOOLCHAIN_ARCH="x86_64" ;; \
        aarch64) TOOLCHAIN_ARCH="aarch64" ;; \
        *) echo "Unsupported architecture: $ARCH"; exit 1 ;; \
    esac && \
    wget -q -O /tmp/arm-toolchain.tar.xz \
        "https://developer.arm.com/-/media/Files/downloads/gnu/${ARM_TOOLCHAIN_VERSION}/binrel/arm-gnu-toolchain-${ARM_TOOLCHAIN_VERSION}-${TOOLCHAIN_ARCH}-arm-none-eabi.tar.xz" && \
    mkdir -p /opt/arm-toolchain && \
    tar -xf /tmp/arm-toolchain.tar.xz -C /opt/arm-toolchain --strip-components=1 && \
    rm /tmp/arm-toolchain.tar.xz
ENV PATH="/opt/arm-toolchain/bin:${PATH}"
ENV TOOLCHAIN_BASE_DIR="/opt/arm-toolchain"
WORKDIR /workspace
CMD ["/bin/bash", "-c", "set -e; \
if [ ! -d /workspace/hello-vca ]; then exit 1; fi; \
cd /workspace/hello-vca; \
sed -i '73 s/^/#/' cmake/CheckGit.cmake 2>/dev/null || true; \
if [ ! -f build/build.ninja ]; then \
    rm -rf build; \
    cmake -S . -B build -GNinja \
        -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi-gcc.cmake \
        -DLOG_LEVEL=DEBUG; \
fi; \
cmake --build build"]
