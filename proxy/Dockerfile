FROM alpine:latest AS build

WORKDIR /proxy

RUN apk add --update apt

COPY ./main.c .
COPY ./headers ./headers/
COPY ./sources ./sources/
COPY ./CMakeLists.txt .
COPY ./Makefile .

RUN make

#ENTRYPOINT ["./proxy/build/proxy", "10", "8080"]
