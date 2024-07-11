FROM alpine:latest

RUN apk update && apk add build-base musl-dev cmake icu-dev icu-static
