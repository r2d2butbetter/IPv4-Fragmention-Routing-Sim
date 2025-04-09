# IPv4 Fragmentation with Dijkstra Routing

This program simulates IPv4 packet fragmentation and routes each fragment using Dijkstra's algorithm to find the shortest path through a network.

## Features

- Create and visualize network topologies (predefined or custom)
- Generate IPv4 packets with custom payload sizes
- Fragment packets based on MTU constraints
- Calculate shortest paths for each fragment using Dijkstra's algorithm
- Support for dynamic network topology changes between fragment transmissions
- Detailed display of fragmentation and routing information

## Project Structure

- `main.c`: Main program logic and workflow
- `network.h/c`: Network topology representation and management
- `ipv4.h/c`: IPv4 packet structures and fragmentation functions
- `dijkstra.h/c`: Implementation of Dijkstra's shortest path algorithm
- `ui.h/c`: User interface functions
- `Makefile`: Compilation instructions

## Compilation

Use the provided Makefile to compile the program:

```
make
```

This will create an executable named `ipv4_router`.

## Usage

Run the compiled program:

```
./ipv4_router
```

Follow the interactive prompts to:
1. Choose or create a network topology
2. Specify source and destination nodes
3. Set MTU and payload size
4. View the fragmentation results and routing paths

## Docker Support

You can also run the application using Docker, which ensures consistent execution across different systems:

### Building the Docker Image

```
docker build -t ipv4-router .
```

### Running the Application in a Docker Container

```
docker run -it ipv4-router
```

The `-it` flags ensure you can interact with the program's interface.

## Module Details

### Network Module
- Represents the network as an adjacency matrix
- Supports directed graphs with weighted edges
- Allows dynamic modification of the topology

### IPv4 Module
- Creates IPv4 packets with proper headers
- Fragments packets according to IPv4 standards
- Handles fragment offset calculation in 8-byte units

### Dijkstra Module
- Implements Dijkstra's algorithm for shortest path finding
- Constructs the complete path from source to destination
- Detects unreachable destinations

### UI Module
- Provides user interface for input and visualization
- Displays network topology in multiple formats
- Shows detailed information about packets and fragments

## Limitations

- Does not implement actual packet transmission
- Uses simplified addressing (node numbers instead of IP addresses)
- Does not handle IPv4 options
- Simplified checksum calculation

## Extension Possibilities

- Add fragment reassembly at the destination
- Implement actual network simulation with packet transmission
- Add support for network failures and fault tolerance
- Extend with alternative routing algorithms