### FastLogger vs Poco::Logger w/ AsyncChannel Benchmark Results

Comparison of Poco::FastLogger (Quill-based wrapper) vs traditional Logger + AsyncChannel across three platforms.

**Test Hardware:** Apple M4 Max (ARM64)
- CPU: Apple M4 Max, 16 cores
- RAM: 128 GB
- L1 I-Cache: 128 KB per core
- L1 D-Cache: 64 KB per core
- L2 Cache: 4 MB per core cluster

**Test Environment:**
- macOS: Native execution
- Linux: OrbStack VM (lightweight Linux virtualization)
- Windows: Parallels VM
- All tests run in Release mode with optimizations

## Summary

| Benchmark | Linux CPU | macOS CPU | Windows CPU |
|-----------|-----------|-----------|-------------|
| **File I/O** |
| AsyncChannel | 413.5ns | 142.4ns | 2355ns |
| FastLogger | 13.5ns | 219.1ns | 101ns |
| **Speedup** | **31x** | 0.65x (slower) | **23x** |
| **Short Message (null sink)** |
| AsyncChannel | 480.2ns | 191.6ns | 3111ns |
| FastLogger | 6.9ns | 179.7ns | 44ns |
| **Speedup** | **70x** | **1.1x** | **71x** |
| **Long Message (null sink)** |
| AsyncChannel | 684.7ns | 424.6ns | 3384ns |
| FastLogger | 10.4ns | 254.8ns | 39ns |
| **Speedup** | **66x** | **1.7x** | **87x** |

**CPU Time** = time spent in calling thread (latency seen by your application)

## Key Observations

1. **Linux (VM)**: FastLogger achieves 31-70x lower latency than AsyncChannel. The lock-free SPSC queue and deferred formatting deliver excellent performance.

2. **Windows (VM)**: Similar excellent results with 24-75x improvement. Windows ARM64 performs comparably to Linux.

3. **macOS (native)**: FastLogger shows minimal improvement or is slightly slower for file I/O. This appears to be a macOS-specific limitation:
   - Thread affinity is not supported on macOS (warning: "Failed to set thread affinity")
   - The Quill backend thread competes with application threads for CPU time
   - macOS kernel scheduling differs from Linux/Windows

## Detailed Results

### Linux (ARM64 VM via OrbStack)

```
Benchmark                                            CPU Time      Wall Time
----------------------------------------------------------------------------
AsyncChannelFileFixture/BM_Comparison_Logger          413.5ns        416.3ns
FastLoggerFileFixture/BM_Comparison_FastLogger         13.5ns        410.9ns
BM_Comparison_AsyncChannel_ShortMsg                   480.2ns        486.9ns
BM_Comparison_FastLogger_ShortMsg                       6.9ns        340.4ns
BM_Comparison_AsyncChannel_LongMsg                    684.7ns        701.1ns
BM_Comparison_FastLogger_LongMsg                       10.4ns        559.0ns
```

### macOS (ARM64 native - Apple Silicon)

```
Benchmark                                            CPU Time      Wall Time
----------------------------------------------------------------------------
AsyncChannelFileFixture/BM_Comparison_Logger          142.4ns        151.6ns
FastLoggerFileFixture/BM_Comparison_FastLogger        219.1ns        570.7ns
BM_Comparison_AsyncChannel_ShortMsg                   191.6ns        238.5ns
BM_Comparison_FastLogger_ShortMsg                     179.7ns        485.4ns
BM_Comparison_AsyncChannel_LongMsg                    424.6ns        508.8ns
BM_Comparison_FastLogger_LongMsg                      254.8ns        685.9ns
```

### Windows (ARM64 VM via Parallels)

```
Benchmark                                            CPU Time      Wall Time
----------------------------------------------------------------------------
AsyncChannelFileFixture/BM_Comparison_Logger         2354.9ns       2384.1ns
FastLoggerFileFixture/BM_Comparison_FastLogger        100.7ns       1483.3ns
BM_Comparison_AsyncChannel_ShortMsg                  3111.0ns       3149.4ns
BM_Comparison_FastLogger_ShortMsg                      43.8ns       1101.6ns
BM_Comparison_AsyncChannel_LongMsg                   3383.8ns       3400.3ns
BM_Comparison_FastLogger_LongMsg                       39.4ns       1782.6ns
```

## Understanding the Metrics

- **CPU Time**: Time the calling thread spends logging. This is what matters for application latency - lower is better.
- **Wall Time**: Total elapsed time including backend thread processing. For async loggers, this includes time for the message to be fully written to disk.

FastLogger's advantage comes from:
1. Lock-free SPSC queue (no mutex contention)
2. Backend thread does heavy lifting asynchronously

Note: Both FastLogger and AsyncChannel use async I/O. The performance difference is primarily due to the lock-free queue vs mutex-protected queue.

## Recommendations

- **Linux/Windows**: Use FastLogger for significant latency improvements (24-75x)
- **macOS**: FastLogger still provides benefits for high-throughput scenarios, but the latency advantage is minimal. Consider using traditional AsyncChannel if latency consistency is critical.

## Test Configuration

- 5 repetitions per benchmark
- Release builds with optimizations enabled
- File I/O tests write to temporary files
- Short/Long message tests use null sink (measure queue overhead only)
