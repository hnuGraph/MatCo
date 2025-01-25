# MatCo: Computing Match Cover of Subgraph Query over Graph Data

# Introduction
We propose a new framework, called MatCo, to compute the match cover.
This document provides users with detailed instructions on how to build, configure, and run the MatCo framework. By following these steps, users will be able to set up the environment, compile the project, and execute the system to compute the match cover between data graphs and query graphs.

## Requirements

\- CMake  3.28

\- g++ 9.3.0

The configuration version provided above is for reference only, and strict version requirements are not necessary.



## Directory Description
```bash
root/
├── example/
│   ├── experiment/
│   │   ├── datasets/ # The experiment datasets
│   │   └── query_graph/ # The experiment query graphs
│   └── query_graph/ # The examples of demonstration
├── graph/
│   ├── graph.cpp # The structure of graph
│   └── graph.h 
├── matching/
│   ├── MatchCover.cpp # Specific implementation of MatCo
│   ├── MatchCover.h
│   ├── matching.cpp # Inherited class of MatCo
│   └── matching.h
├── utils/
│   ├── CLI11.hpp # Command line parsing
│   ├── globals.cpp # Global variable
│   ├── globals.h
│   ├── timer.hpp # Timer
│   ├── types.h # Macro definition
│   └── utils.h 
├── CMakeLists.txt
├── main.cpp 
├── MatCo.sh # Execute the given example
└── README.md
         
```

## Build Instructions



To build the project, follow these steps:



1. Create a build directory and navigate to it:

```bash
mkdir build
cd build
```

2. Generate the build system using CMake and compile the project:

```bash
cmake ..
make
```

3. After building the project, you can execute the following scripts to run an example:

```bash
cd ..
sh MatCo.sh
```

## Command-line Option Descriptions

The following table outlines the command-line options supported by the application:

| Option                        | Description                                 | Type      | Default value | Required | 
|:-------------------------------:|:---------------------------------------------:|:-----------:|:---------------:|:----------:|
| `-d, --data_graph_path`        | Specifies the path to the data graph.       | String    | N/A           | Yes      | 
| `-q, --query_graph_path`       | Specifies the path to the query graph.      | String    | N/A           | Yes      | 
| `-e, --print_enum`             | Print the enumeration answers.    | Bool   | `false`        | No       | 
| `-p, --print_prep`             | Print preprocessing information.   | Bool   | `false`        | No       | 
| `-k, --print_key_vertice`      | Print the key vertices.          | Bool   | `false`        | No       | 
| `-v, --verify_correctness`      | Verify correctness (the file path of key vertices) .          | String   | N/A        | No       | 

## Command Example 

```bash
./build/MatCo -d example/experiment/datasets/yeast.graph -q example/query_graph/query_1.graph  -p false -e true -k true -v example/query_graph/query_1_kv.txt
```

## Experiment Datasets

| Dataset    |    \|V\|   |      \|E\|   | Degree |\|L(V)\||
| :--------: | :--------: | :----------: | :----: | :----: |
| Yeast      | 3,112      | 12,519       | 8      | 71     |
| Human      | 4,674      | 86,282       | 36.9   | 44     |
| WordNet    | 76,853     | 120,399      | 3.1    | 5      |
| DBLP       | 317,080    | 1,049,866    | 6.6    | 15     |
| YouTube    | 1,134,890  | 2,987,624    | 5.3    | 25     |
| EU-2005    | 862,644    | 16,138,468   | 37.4   | 40     |
| Orkut      | 3,072,441  | 117,185,083  | 76.2   | 100    |
| Twitter    | 41,652,230 | 1,468,365,182| 57     | 1000   |


The first six datasets can be found in the `example/experiment/datasets` directory.Due to the large size of the Orkut and Twitter datasets, they are not uploaded. You can download them directly from the respective websites: [Orkut](https://snap.stanford.edu/data/com-Orkut.html), [Twitter](https://law.di.unimi.it/webdata/twitter-2010/).
The query graphs used in the experiment in the paper can be found in the `example/experiment/query_graph` directory