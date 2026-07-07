[![progress-banner](https://camo.githubusercontent.com/f4f22828b0f14d4b9eab189bcd1df9caa7414ceb16e5cf64420a717df5ac4716/68747470733a2f2f6261636b656e642e636f646563726166746572732e696f2f70726f67726573732f636c617564652d636f64652f36666662373030312d616639632d343537322d386166342d653366306334366661346633)](https://app.codecrafters.io/users/BlackMamba1w?r=2qF)

# Introduction

Claude Code is an AI coding assistant that uses Large Language Models (LLMs) to
understand code and perform actions through tool calls. In this challenge,
I will build my own Claude Code from scratch by implementing an LLM-powered
coding assistant.

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
I will be using HTTP RESTful APIs (OpenAI-compatible tool), calling, agent loop, and how to integrate multiple tools into an AI assistant.


# Programme structure and aim

The entry point for my `claude-code` implementation is in `src/main.cpp`.
This project is mainly for me to learn json parsing, and to learn how to pull API requests from openAI. Will complete by 7/16/2026.

```sh
./claude.sh
```

# Programme structure

The entry point for my `shell` implementation is in `src/main.cpp`.
All functions that I use in `src/main.cpp` can be found in `src/funcs.cpp` and are defined in `funcs.hpp`.
One can run the shell via *WSL* or *Terminal* by travelling to the directory and running:

```sh
./build.sh
./claude.sh
```

# Set-up instructions:

1. Ensure you have `cmake` and `Ninja-build` installed locally
2. Run `./claude.sh` to run your program, which is implemented in
   `src/main.cpp`.
3. Ensure you have ollama mistral installed locally (or you can configure your model as you choose in `src/main.cpp` and have it running).

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

# Contact author
Contact the author of this agent to suggest any improvements @ *k.sainandan2011@gmail.com* ! Will respond in 3-5 business days. This Project is about 20% complete, and I am planning to add my own features soon upon completing the challenge.
