FROM ubuntu:latest
RUN apt-get update && apt-get install -q -y --no-install-recommends \
   make \
   gcc \ 
   && rm -rf /var/lib/apt/lists/*
WORKDIR /container
COPY . .

RUN make all
ENTRYPOINT [ "source" ]