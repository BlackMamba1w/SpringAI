![SpringAI Banner](https://github.com/user-attachments/assets/1d85595c-5ba6-4eb8-b6bb-df26354a13cd)
# Introduction

In this project, I am building my own AI coding assistant for projects. This project is still in a very early developmental stage as it is still only a solo passion project by a student. However, I am planning to implement distillation, fine-tuning and persistent memory as time passes.

```
╭──────────────────────────────────────────────────────────────╮
│                                                              │
│   ███████╗██████╗ ██████╗ ██╗███╗   ██╗ ██████╗  █████╗ ██╗  │
│   ██╔════╝██╔══██╗██╔══██╗██║████╗  ██║██╔════╝ ██╔══██╗██║  │
│   ███████╗██████╔╝██████╔╝██║██╔██╗ ██║██║  ███╗███████║██║  │
│   ╚════██║██╔═══╝ ██╔══██╗██║██║╚██╗██║██║   ██║██╔══██║██║  │
│   ███████║██║     ██║  ██║██║██║ ╚████║╚██████╔╝██║  ██║██║  │
│   ╚══════╝╚═╝     ╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝╚═╝  │
│                                                              │
│          AI-powered coding from your terminal.               │
│                                                              │
╰──────────────────────────────────────────────────────────────╯
```
As of now, I used HTTP RESTful APIs (OpenAI-compatible tool), calling, agent loop, and am integrating multiple tools into an AI assistant. I have also implemented a RAG (Retrieval-Augmented Generation) system, such that files are vector-embedded with the help of the nomic-embedding model from ollama.


# Programme structure and aim

The entry point for my `SpringAI` implementation is in `src/main.cpp`.
All header files can be found in `include` and tools are in `src/tools`.

# To run:

First, you need to build and install all necessary packages with
```sh
./build
```
Do make sure you have ollama installed and the required models serving for now. Will switch to an API key in future commits.
After that, run

```
./Spring.sh -p "Your prompt here"
```

and voila, your response should be there in a pretty long time (Spare me I'm still working on this D:)

# Set-up instructions:

1. Ensure you have `cmake` and `Ninja-build` installed locally
2. Run `./Spring.sh` to run your program, which is implemented in
   `src/main.cpp`.
3. Ensure you have ollama mistral and ollama nomic-embedded-text installed locally (or you can configure your model as you choose in `include/Configs.hpp` and have it running).

# Example usage of tool

```
chosen_one@Cyberspace:/mnt/c/Users/saina/Downloads/Claude_Code_CLI_API$ ./claude.sh -p "Create a python file with an implementation of djikstra's algorithm and A* algorithm in it as 2 functions"

    ╭──────────────────────────────────────────────────────────────╮
    │                                                              │
    │   ███████╗██████╗ ██████╗ ██╗███╗   ██╗ ██████╗  █████╗ ██╗  │
    │   ██╔════╝██╔══██╗██╔══██╗██║████╗  ██║██╔════╝ ██╔══██╗██║  │
    │   ███████╗██████╔╝██████╔╝██║██╔██╗ ██║██║  ███╗███████║██║  │
    │   ╚════██║██╔═══╝ ██╔══██╗██║██║╚██╗██║██║   ██║██╔══██║██║  │
    │   ███████║██║     ██║  ██║██║██║ ╚████║╚██████╔╝██║  ██║██║  │
    │   ╚══════╝╚═╝     ╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝╚═╝  │
    │                                                              │
    │          AI-powered coding from your terminal.               │
    │                                                              │
    ╰──────────────────────────────────────────────────────────────╯
     Below is an example of implementing Dijkstra's and A\★ algorithms in Python as separate functions. Note that the A\★ implementation assumes a heuristic function `h(n, goal)` is provided (which calculates the cost to go from current node `n` to destination goal node).

import heapq
def dijkstra(graph, start, goal=None):
    distances = {node: float('inf') for node in graph}
    distances[start] = 0
    unvisited = [(0, start)]
    visited = set()

    while unvisited:
        current_distance, current = heapq.heappop(unvisited)
        visited.add(current)

        if goal and current == goal:
            return distances[goal]

        for neighbor, edge_cost in graph[current].items():
            distance = current_distance + edge_cost

            if distance < distances[neighbor]:
                distances[neighbor] = distance
                heapq.heappush(unvisited, (distance, neighbor))

    return None

def astar(graph, start, goal, heuristic):
    def f(n):
        return distances[n] + heuristic(n, goal)

    distances = {node: float('inf') for node in graph}
    distances[start] = 0
    unvisited = [(f(n), n) for n in graph if n != goal]
    visited = set()

    while unvisited:
        _, current = heapq.heappop(unvisited)
        visited.add(current)

        if goal and current == goal:
            return distances[goal]

        for neighbor, edge_cost in graph[current].items():
            distance = distances[current] + edge_cost

            if distance < distances[neighbor]:
                distances[neighbor] = distance
                heapq.heappush(unvisited, (f(neighbor), neighbor))

    return None
```

The functions expect `graph` to be a dictionary that maps each node to its connected neighbors as another dictionary, where each neighbor is associated with its corresponding edge cost. You can call these functions in the following way:

```
def main():
    # Example graph representation
    graph = {
        'A': {'B': 1, 'C': 5},
        'B': {'A': 1, 'D': 2, 'E': 3},
        'C': {'A': 5, 'D': 4, 'F': 2},
        'D': {'B': 2, 'C': 4, 'E': 6},
        'E': {'B': 3, 'D': 6}
    }

    start = 'A'
    goal = 'F'

    shortest_path_length = dijkstra(graph, start, goal)
    a_star_shortest_path_length = astar(graph, start, goal, heuristic=lambda n, g: 7 if n == g else int(n[0][1]) ** 2)

    print("Dijkstra's Algorithm:", shortest_path_length)
    print("A* Algorithm:", a_star_shortest_path_length)

if __name__ == "__main__":
    main()

```
The respective files should be created in your project directory. Please do not install this tool yet, as it is still work in progress.

# Contact author
Please do email the author at *k.sainandan2011@gmail.com* if you wish to offer help, suggestions or would like to work on this project together by forming a team. Will respond in 3-5 days. This Project is in a very early stage. 
I am planning on implementing the following features over the coming months:
   1. Image handling capability
   2. Persistent Memory
   3. Distillation and Finetuning of model
   4. Small-Scale MLA implementation
   5. Interactive command line interface
