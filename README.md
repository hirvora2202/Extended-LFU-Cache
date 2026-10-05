# Extended LFU Cache

A C++ implementation of an Extended Least Frequently Used (LFU) Cache that manages key-value data based on access frequency.

## Project Overview

This project implements an LFU cache using C++ data structures such as `unordered_map` and `unordered_set`.

The cache keeps track of how frequently each key is accessed. When the cache reaches its capacity, the item with the minimum access frequency is selected for eviction.

## Features

- Configurable cache capacity
- Key-value insertion
- Key access and frequency updates
- Frequency-based cache eviction
- Tracking of minimum-frequency items
- Display of current cache contents
- Display of the frequency map
- Re-insertion handling for previously evicted keys
- Exception handling for invalid or duplicate operations

## Data Structures Used

The implementation uses:

- `unordered_map` for key-to-item mapping
- `unordered_map` for frequency-to-items mapping
- `unordered_set` for storing items belonging to each frequency
- Custom C++ structures for cache items and frequency nodes

## How It Works

Each item in the cache stores:

- Key
- Data/value
- Access frequency
- Cache status

When an item is accessed, its frequency is increased and the item is moved to the corresponding frequency group.

When the cache becomes full, an item belonging to the minimum-frequency group is removed to make space for a new item.

## Menu Options

The program provides an interactive menu:

1. **Insertion** - Insert a key and value into the cache.
2. **Access** - Access an existing key and increase its frequency.
3. **Print** - Display the current cache contents and frequency mapping.

## Technologies Used

- C++
- Standard Template Library (STL)
- `unordered_map`
- `unordered_set`
- Exception Handling
- Dynamic Memory Allocation

## Compilation and Execution

### Windows

Make sure a C++ compiler such as MinGW/G++ is installed.

Compile the program using:

```bash
g++ ExtendedLFUCache.cpp -o ExtendedLFUCache.exe