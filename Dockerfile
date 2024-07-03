FROM ubuntu:24.04

RUN DEBIAN_FRONTEND=noninteractive \
	apt-get update \
 && apt-get install -qq build-essential cmake pkg-config libglib2.0-dev

COPY . /opt/xdia
WORKDIR /opt/xdia
RUN cmake -Bbuild -S. \
 && cmake --build build

FROM scratch
WORKDIR /opt/xdia
COPY --from=0 /opt/xdia/build/src/xdia .
COPY --from=0 /opt/xdia/msdia140.dll .
CMD ["/opt/xdia/xdia"]
