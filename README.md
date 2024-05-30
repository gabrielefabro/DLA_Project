# DLA Simulator

## Introduction

DLA Simulator is a C program that simulates the aggregation of particles moving in a 2D surface following Brownian motion. We present multiple implementations: single core, multi-thread using the pthread library, and multi-thread using OpenMP. Our implementations also render the simulation stages to show how particles aggregate.

## How to Run

**Note: This project is intended to run on Linux. Compatibility with other platforms is not guaranteed due to the use of multi-threading libraries and other dependencies.**

After cloning the repository, you can run the program, provided the dependencies are met. (See the [Dependencies](#dependencies) section.)

To compile, use the following command:

```sh
gcc -o dla_single_thread.out dla_single_thread.c -lgd
```

Here, `-lgd` specifies the GD library, which we use for image rendering.

For multi-threaded versions, add the following options:

- `-fopenmp` for the OpenMP implementation
- `-pthread` for the pthread implementation

To run the single-threaded version, execute the command below with the appropriate arguments:

- `n,m` - matrix size, rows, and columns respectively
- `num_particles` - number of particles to instantiate
- `x,y` - seed position with respective coordinates in the matrix [*optional*] \ If not specified, it will be randomly generated within the surface.

```sh
./dla_single_thread.out n,m num_particles x,y
```

For multi-threaded versions:

- OpenMP: `./dla_openmp.out n,m num_particles [-OPTIONS]`
- pthread: `./dla_pthread.out n,m num_particles [-OPTIONS]`

Where `n,m` are the matrix dimensions, and `num_particles` is the number of particles to instantiate.

Options are:

- `-n` - number of threads to use [default: 4]
- `-s` - seed position [default: randomly generated]
- `-t` - simulation horizon [default: 1000]

## How It Works

The program's core logic remains consistent across all three implementations, with minor differences. The initialization phase involves storing the command-line arguments, allocating memory for the matrix (filled with zeros), and allocating memory for the particle list, with particles generated at random positions. The simulation then proceeds as follows:

1. **Check Adjacent Cells**: For each particle, check the adjacent cells. If an aggregated particle (or the initial seed) is found, the current particle aggregates and is no longer considered. Otherwise, proceed to step 2.
2. **Move Particle**: The particle moves in a random direction to simulate Brownian motion. Then, return to step 1.

The simulation continues for the specified horizon, after which the result is saved as a JPEG image. All allocated memory is freed, and necessary finalization steps are performed for multi-threaded implementations.

## Dependencies

For image rendering in C, we use: [GD Graphics Library](https://libgd.github.io/).

To install it on Ubuntu:

```sh
sudo apt install libgd-dev
```
