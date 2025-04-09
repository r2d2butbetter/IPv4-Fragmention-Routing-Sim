FROM debian:bookworm-slim	

RUN apt-get update && apt-get install -y \
    gcc \
    make \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN make

CMD ["./build/network_sim"]