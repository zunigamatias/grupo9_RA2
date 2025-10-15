# Text Cache System - Empresa Texto é Vida

A high-performance text caching application designed to minimize loading times for legal documents stored on slow forensic disk systems.

## 📖 Project Overview

**Empresa Texto é Vida** needed a fast and efficient way to read their 100 most important legal texts. Due to legal requirements, these documents must be stored on a specialized forensic disk system that is very slow to access. This application solves the problem by implementing an intelligent caching system that keeps frequently accessed documents in memory for instant retrieval.

### The Challenge
- 100 important legal texts (1000+ words each)
- Documents stored on slow forensic disk (legal requirement)
- Lawyers need fast access to documents
- Memory can only hold 10 texts at once
- Need to determine the best caching strategy

### The Solution
A terminal-based text reader with three different caching algorithms that learns which strategy works best for the company's access patterns.

## 🚀 Features

### Core Functionality
- **Fast Text Access**: Read any of 100 numbered texts instantly when cached
- **Intelligent Caching**: Automatically manages which texts stay in memory
- **Performance Optimization**: Tests and selects the best caching algorithm
- **Memory Efficient**: Stores maximum 10 texts in memory simultaneously

### Caching Algorithms
1. **LFU (Least Frequently Used)**: Keeps texts that are accessed most often
2. **Random Replacement**: Randomly removes texts when memory is full
3. **S3-FIFO**: Smart queue system that gives recently accessed texts a "second chance"

### Simulation Mode
- Tests all three algorithms with 3 different user patterns
- Analyzes 5,400+ document access scenarios
- Generates visual performance reports
- Automatically selects the winning algorithm

## 🛠️ Installation & Setup

### Prerequisites
```bash
# Install required tools
sudo apt update
sudo apt install cmake build-essential gnuplot
```

### Building the Project
```bash
# Clone or download the project
cd your-project-directory

# Build the application
mkdir build
cd build
cmake ..
make
```

### Running the Application
```bash
./cache_simulator
```

## 🎯 How to Use

### Main Interface
When you start the application, you'll see:
```
=== Text Cache System ===

Enter text number (1-100), 0 to exit, -1 for simulation mode:
>
```

### Commands
- **1-100**: Read a specific text file
- **0**: Exit the program
- **-1**: Run simulation mode to determine best algorithm

### Example Usage
```
> 25
[Cache MISS]
Text 25 loaded in 0.0234 ms.
---- Full text ----
Content of file 25. This is important legal document...
---- End of text ----

> 25
[Cache HIT]
Text 25 loaded in 0.0012 ms.
```

## 📊 Simulation Mode

### What It Does
The simulation mode automatically:
1. Tests each caching algorithm with 3 virtual users
2. Each user makes 200 document requests
3. Uses three different access patterns:
   - **Uniform**: Random access to any document
   - **Poisson**: Bell-curve distribution (some docs more popular)
   - **Biased**: 43% chance of accessing documents 30-40

### Generated Reports
After simulation, you get:
- **Terminal Report**: Detailed statistics and comparison tables
- **Visual Charts**: PNG files showing performance graphs
  - `hit_rates.png` - Cache hit rates by algorithm
  - `response_times.png` - Average response times
  - `distribution_analysis.png` - Performance by access pattern
  - `miss_analysis.png` - Cache miss analysis

### Sample Results
```
=== ALGORITHM COMPARISON ===
lfu:
  Average Hit Rate: 26.9%
  Average Time: 0.0045 ms
  Score: 264.55

random:
  Average Hit Rate: 21.5%
  Average Time: 0.0052 ms
  Score: 209.48

s3_fifo:
  Average Hit Rate: 23.2%
  Average Time: 0.0048 ms
  Score: 227.20

WINNER: lfu
```

## 📁 Project Structure

```
├── algorithms/           # Cache implementations
│   ├── Cache.h          # Base cache class
│   ├── lfu/            # LFU algorithm
│   ├── random_replacement/  # Random replacement
│   └── s3_fifo/        # S3-FIFO algorithm
├── core/               # Core functionality
│   └── disk_io/        # File reading operations
├── simulation/         # Performance testing
├── CMakeLists.txt      # Build configuration
├── ra2_main.cpp       # Main application
└── README.md          # This file
```

## 🔧 Technical Details

### Cache Capacity
- **Maximum**: 10 documents in memory
- **Smart Eviction**: Removes least important documents when full
- **Instant Access**: Cached documents load in microseconds

### Performance Metrics
- **Cache Hit Rate**: Percentage of requests served from memory
- **Response Time**: Time from request to display
- **Miss Analysis**: Which documents cause cache misses

### Algorithm Selection
The system automatically chooses the best algorithm based on:
- Highest cache hit rate
- Lowest average response time
- Combined performance score

## 📈 Understanding the Results

### Cache Hit Rate
- **High (>30%)**: Algorithm works well for your access patterns
- **Medium (20-30%)**: Decent performance
- **Low (<20%)**: Algorithm not optimal for your usage

### Response Time
- **Cache Hit**: ~0.001-0.01 ms (memory access)
- **Cache Miss**: ~0.02-0.1 ms (includes file loading)

### When to Re-run Simulation
- Access patterns change significantly
- New types of documents added
- Performance seems slower than expected

## 🎯 For Legal Teams

### Daily Usage
1. Start the application
2. Enter document numbers as needed
3. Documents load instantly if recently accessed
4. System learns your usage patterns

### Best Practices
- Run simulation mode periodically
- Numbers 30-40 are often accessed faster (optimized range)
- First access of any document will be slower (cache miss)
- Frequently used documents stay in memory longer

### Performance Benefits
- **First Access**: Normal speed (disk read required)
- **Subsequent Access**: Up to 10x faster (memory retrieval)
- **Optimal Algorithm**: Automatically selected for your team's patterns

## 🤝 Support

The system is designed to be self-optimizing and requires minimal maintenance. The simulation mode helps ensure you're always using the most efficient caching strategy for your specific document access patterns.

---

*Built with C++17, CMake, and gnuplot for Empresa Texto é Vida's legal document management needs.*