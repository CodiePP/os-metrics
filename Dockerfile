FROM alpine:latest

RUN addgroup -g 1000 user \
    && adduser -u 1000 -G user -s /bin/bash -D user \
    && apk update \
    && apk add --no-cache bash cmake git alpine-sdk

COPY --chown=1000 . /home/user/

USER user

WORKDIR /home/user/

RUN mkdir -vp build && cd build && cmake --fresh -DCMAKE_BUILD_TYPE=Debug .. && make && ./test_linux_Debug

CMD bash
