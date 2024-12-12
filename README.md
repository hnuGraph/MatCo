# Project Build Instructions



## Requirements

\- CMake 3.28

\- A compatible C++ compiler 9.3.0



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

3. After building the project, you can execute the MatCo.sh script to run an example:

```bash
cd ..
sh MatCo.sh
```

## Command-Line Option Descriptions

The following table outlines the command-line options supported by the application:

| Option                        | Description                                 | Type      | Default Value | Required | 
|-------------------------------|---------------------------------------------|-----------|---------------|----------|
| `-d, --data_graph_path`        | Specifies the path to the data graph.       | String    | N/A           | Yes      | 
| `-q, --query_graph_path`       | Specifies the path to the query graph.      | String    | N/A           | Yes      | 
| `-e, --print_enum`             | print the enumeration values.    | Boolean   | `false`        | No       | 
| `-p, --print_prep`             | print preparation information.   | Boolean   | `false`        | No       | 
| `-k, --print_key_vertice`      | print the key vertices.          | Boolean   | `false`        | No       | 
| `-v, --verify_correctness`      | verify correctness (a file of key vertices) .          | String   | ``        | No       | 

## Example Command

```bash
./build/MatCo -d example/data_graph/yeast.graph -q example/query_graph/query_1.graph  -p false -e true -k true -v example/query_graph/query_1_kv.txt
```
