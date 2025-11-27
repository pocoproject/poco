# MongoDB Replica Set Examples

This directory contains examples demonstrating Poco::MongoDB replica set support.

## ReplicaSetMonitor - Deployment Health Check Tool

A continuous monitoring tool that performs regular read/write operations and displays replica set topology status. Ideal for deployment verification and health monitoring.

### Features

- **Continuous Health Checks**: Performs writes to primary and reads from replica set at configurable intervals
- **Real-time Topology Display**: Shows current replica set status, server roles, and round-trip times
- **Statistics Tracking**: Monitors success rates for read and write operations
- **Deployment Verification**: Quickly verify replica set is functioning correctly
- **Automatic Failover Testing**: Continues working even during primary elections

### Usage

```bash
# Basic usage with defaults
./ReplicaSetMonitor

# Using MongoDB URI (recommended)
./ReplicaSetMonitor -u 'mongodb://mongo1:27017,mongo2:27017/?replicaSet=rs0'

# Specify replica set and hosts (traditional method)
./ReplicaSetMonitor -s rs0 -H mongo1:27017,mongo2:27017,mongo3:27017

# Run with 10-second intervals for 100 iterations
./ReplicaSetMonitor -i 10 -n 100

# Verbose mode for detailed operation output
./ReplicaSetMonitor -v

# Full example with URI
./ReplicaSetMonitor \
    --uri 'mongodb://mongo1:27017,mongo2:27017,mongo3:27017/?replicaSet=rs0&readPreference=primaryPreferred' \
    --interval 5 \
    --database test \
    --collection health_check \
    --verbose

# Full example with traditional options
./ReplicaSetMonitor \
    --set rs0 \
    --hosts mongo1:27017,mongo2:27017,mongo3:27017 \
    --interval 5 \
    --database test \
    --collection health_check \
    --verbose
```

### Command-Line Options

| Option | Description | Default |
|--------|-------------|---------|
| `-h, --help` | Show help message | - |
| `-u, --uri URI` | MongoDB connection URI (takes precedence) | - |
| `-s, --set NAME` | Replica set name | `rs0` |
| `-H, --hosts HOSTS` | Comma-separated host:port list | `localhost:27017,localhost:27018,localhost:27019` |
| `-i, --interval SECONDS` | Check interval in seconds | `5` |
| `-d, --database NAME` | Database name | `test` |
| `-c, --collection NAME` | Collection name | `poco_monitor` |
| `-v, --verbose` | Verbose output | `false` |
| `-n, --iterations N` | Number of iterations | unlimited |

**Note:** The `--uri` option takes precedence over `--set` and `--hosts` options.

### Environment Variables

- `MONGODB_URI`: MongoDB connection URI (takes precedence)
- `MONGODB_REPLICA_SET`: Replica set name
- `MONGODB_HOSTS`: Comma-separated host:port list

### Example Output

```
Connecting to replica set: rs0
Seed servers: mongo1:27017, mongo2:27017, mongo3:27017
Check interval: 5 seconds

Replica set connected successfully!
Background monitoring active.

================================================================================
TOPOLOGY STATUS
================================================================================
Replica Set: rs0
Type:        Replica Set (with Primary)
Has Primary: Yes

Servers: 3
--------------------------------------------------------------------------------
Address                        Type        RTT (ms)  Status
--------------------------------------------------------------------------------
mongo1:27017                   PRIMARY     2.34      OK
mongo2:27017                   SECONDARY   3.12      OK
mongo3:27017                   SECONDARY   2.89      OK
================================================================================

[2025-11-26T21:15:00Z] Check #1
Write (Primary): ✓ OK (12 ms)
Read (PrimaryPreferred): ✓ OK (8 ms)
Statistics: Writes: 1/1 (100.0%), Reads: 1/1 (100.0%)
--------------------------------------------------------------------------------
[2025-11-26T21:15:05Z] Check #2
Write (Primary): ✓ OK (9 ms)
Read (PrimaryPreferred): ✓ OK (7 ms)
Statistics: Writes: 2/2 (100.0%), Reads: 2/2 (100.0%)
--------------------------------------------------------------------------------
```

### Use Cases

#### 1. Deployment Verification

Run the monitor immediately after deploying a replica set:

```bash
# Run for 60 iterations (5 minutes with 5-second intervals)
./ReplicaSetMonitor -s production-rs -H prod1:27017,prod2:27017,prod3:27017 -n 60
```

#### 2. Load Testing

Combine with multiple instances to generate load:

```bash
# Run multiple monitors in parallel
for i in {1..10}; do
    ./ReplicaSetMonitor -i 1 &
done
```

#### 3. Failover Testing

Run the monitor while performing failover operations:

```bash
# Start monitor
./ReplicaSetMonitor -v

# In another terminal, step down the primary:
# mongo --eval "rs.stepDown()"

# Monitor will automatically failover and continue operations
```

#### 4. Continuous Monitoring

Run as a long-term health check:

```bash
# Run indefinitely with 30-second intervals
./ReplicaSetMonitor -i 30 > replica_set_health.log 2>&1
```

### Docker Compose Testing

Create a local replica set for testing:

```yaml
# docker-compose.yml
version: '3'
services:
  mongo1:
    image: mongo:7.0
    command: ["--replSet", "rs0", "--bind_ip_all", "--port", "27017"]
    ports: ["27017:27017"]

  mongo2:
    image: mongo:7.0
    command: ["--replSet", "rs0", "--bind_ip_all", "--port", "27017"]
    ports: ["27018:27017"]

  mongo3:
    image: mongo:7.0
    command: ["--replSet", "rs0", "--bind_ip_all", "--port", "27017"]
    ports: ["27019:27017"]
```

Initialize the replica set:

```bash
docker-compose up -d
docker exec -it $(docker ps -q -f name=mongo1) mongosh --eval "
rs.initiate({
  _id: 'rs0',
  members: [
    { _id: 0, host: 'localhost:27017' },
    { _id: 1, host: 'localhost:27018' },
    { _id: 2, host: 'localhost:27019' }
  ]
})"

# Wait a few seconds for election
sleep 5

# Run the monitor
./ReplicaSetMonitor -s rs0 -H localhost:27017,localhost:27018,localhost:27019
```

---

## ReplicaSet - Feature Examples

Demonstrates various replica set features with multiple commands.

### Usage

```bash
./ReplicaSet <command>
```

### Commands

| Command | Description |
|---------|-------------|
| `basic` | Basic replica set connection and operations |
| `readpref` | Read preference examples (primary, secondary, nearest) |
| `failover` | Automatic failover demonstration |
| `pool` | Connection pooling example |
| `topology` | Topology discovery and monitoring |

### Examples

```bash
# Basic connection
./ReplicaSet basic

# Try different read preferences
./ReplicaSet readpref

# Demonstrate automatic failover
./ReplicaSet failover

# Show connection pooling
./ReplicaSet pool

# Display topology information
./ReplicaSet topology
```

---

## URIExample - URI Parsing Demonstration

Demonstrates MongoDB URI parsing and connection to replica sets.

### Features

- Parse MongoDB connection URIs
- Display parsed configuration (hosts, replica set name, read preference, timeouts)
- Connect to replica set and show topology
- Query server information
- Validate URI format

### Usage

```bash
./URIExample <uri>
```

### Examples

```bash
# Basic replica set URI
./URIExample 'mongodb://localhost:27017,localhost:27018,localhost:27019/?replicaSet=rs0'

# With read preference
./URIExample 'mongodb://mongo1:27017,mongo2:27017/?replicaSet=rs0&readPreference=primaryPreferred'

# With custom timeouts and heartbeat
./URIExample 'mongodb://host1:27017,host2:27017/?replicaSet=rs0&connectTimeoutMS=5000&socketTimeoutMS=30000&heartbeatFrequencyMS=5000'
```

### Supported URI Options

- `replicaSet=name` - Replica set name
- `readPreference=mode` - Read preference (primary|primaryPreferred|secondary|secondaryPreferred|nearest)
- `connectTimeoutMS=ms` - Connection timeout in milliseconds
- `socketTimeoutMS=ms` - Socket timeout in milliseconds
- `heartbeatFrequencyMS=ms` - Heartbeat frequency in milliseconds

### Example Output

```
Parsing MongoDB Replica Set URI
================================================================================
URI: mongodb://localhost:27017,localhost:27018,localhost:27019/?replicaSet=rs0

✓ URI parsed successfully!

Configuration:
--------------------------------------------------------------------------------
Replica Set Name: rs0
Read Preference:  primary
Seed Servers:     localhost:27017, localhost:27018, localhost:27019
Monitoring:       Active

Connecting to replica set...
✓ Connected to primary: localhost:27017

Server Information:
--------------------------------------------------------------------------------
MongoDB Version: 7.0.5
Git Version:     7809d71e84e314b497f282ea52598668b08b84dd

Replica Set Topology:
--------------------------------------------------------------------------------
Set Name:    rs0
Has Primary: Yes
Servers:     3

  localhost:27017 [PRIMARY] RTT: 2.34 ms
  localhost:27018 [SECONDARY] RTT: 3.12 ms
  localhost:27019 [SECONDARY] RTT: 2.89 ms

✓ Success!
```

---

## Building the Examples

### With CMake

```bash
cd poco
mkdir build && cd build
cmake .. -DENABLE_MONGODB=ON -DENABLE_SAMPLES=ON
cmake --build . --target ReplicaSetMonitor
cmake --build . --target ReplicaSet
cmake --build . --target URIExample

# Executables are in bin/
./bin/ReplicaSetMonitor --help
./bin/ReplicaSet basic
./bin/URIExample 'mongodb://localhost:27017/?replicaSet=rs0'
```

### With Make

```bash
cd MongoDB/samples/ReplicaSet
make
./ReplicaSetMonitor --help
```

---

## Troubleshooting

### "No suitable server found in replica set"

- Verify MongoDB servers are running: `nc -zv localhost 27017`
- Check replica set is initialized: `mongosh --eval "rs.status()"`
- Verify network connectivity between hosts
- Check replica set name matches: `-s` option should match `rs.status()._id`

### "Connection failed" errors

- Ensure MongoDB is binding to the correct interface (`--bind_ip_all`)
- Check firewall rules allow connections to MongoDB ports
- Verify authentication is disabled or credentials are provided

### High latency or timeouts

- Check network conditions between client and MongoDB servers
- Increase timeout values in replica set configuration
- Verify MongoDB servers are not overloaded

---

## Advanced Configuration

### Custom Read Preference with Tags

```cpp
// Target servers in specific datacenter
Document tags;
tags.add("dc", "east");
tags.add("rack", "1");
ReadPreference pref(ReadPreference::Nearest, tags);
```

### Custom Heartbeat Frequency

```cpp
ReplicaSet::Config config;
config.heartbeatFrequency = Poco::Timespan(30, 0);  // 30 seconds
```

### Disable Background Monitoring

```cpp
ReplicaSet::Config config;
config.enableMonitoring = false;  // Manual topology refresh only
```

---

## Additional Resources

- [MongoDB Replica Set Documentation](https://www.mongodb.com/docs/manual/replication/)
- [Server Discovery and Monitoring Spec](https://github.com/mongodb/specifications/blob/master/source/server-discovery-and-monitoring/server-discovery-and-monitoring.rst)
- [Poco Documentation](https://pocoproject.org/docs/)
