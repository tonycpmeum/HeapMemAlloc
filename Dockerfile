FROM ubuntu:latest

RUN apt-get update && apt-get install -q -y --no-install-recommends \
   build-essential \ 
   make \
   libc6-dev \
   && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY app .

RUN make -C .
CMD [ "bash", "-i" ]