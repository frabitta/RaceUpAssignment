# RaceUpAssignment
Submission for the application to the Driverless department of Race UP FSAE Unipd team.

## Compile and Run
To compile the code, run the makefile or run the command:
```
gcc -std=c99 -Wall -Wpedantic -Werror steepCurvePath.c -o steepCurvePath.exe -lm
```

And execute it by simply calling `steepCurvePath.exe`:
```
./steepCurvePath.exe
```

## Structure of the program
The source file is `steepCurvePath.c`, which contains the main function and all the necessary functions to solve the problem.

The program implements the Dijkstra algorithm to find the shortest path in a graph.
Since we know how the graph is structured, we treat the graph as a grid, where each cell is a node and the edges are the connections between adjacent nodes.

### Graph structure
Given the structure of the problem, we represent the graph as a grid of nodes disposed on K rows and H columns, where K is the number of partitions of the curve and H is the number of horizontal partitions.

![Grid image](https://github.com/frabitta/RaceUpAssignment/blob/main/media/SmartSelect_20251015_171803_Samsung%20Notes.jpg)

Each node is connected to its adjacent ones by horizontal, vertical or diagonal edges.
Given the regularity of the structure, a lot of the edges have the same lenght, so we can compute them once and use them multiple times. In the image a representation of the edges of equal lenght by color:

![Edges image](https://github.com/frabitta/RaceUpAssignment/blob/main/media/SmartSelect_20251015_171929_Samsung%20Notes.jpg)

Since we know the parameteres, in the first part of the program we compute the lenght of all the edges:
- horizontal edges ($`l`$) have lenght of $`(O-I)/(H-1)`$;
- vertical edges ($`t`$) have lenght of $`2r*sin(a/2)`$, with $`r`$ the radius and $`a`$ the angle of a single partition of the curve;
- diagonal edges ($`d`$) have lenght of $`\sqrt{l^2 + t^2 + 2t*l*sin(a/2)}`$ for the diagonal of the trapezoid having $`t`$ as the smallest base.

### Dijkstra algorithm
The Dijkstra algorithm is implemented by analyzing the available edges from the current node and updating the distance to the adjacent nodes if a shorter path is found. Since the number of nodes is not very high, we use a simple array to keep track of the visited nodes and the distances, instead of using a priority queue.
The algorithm continues until all nodes have been visited.

### Output
The program outputs the minimum distance to reach the end of the curve and prints the entire path.

## Comments
My implementation, given the parameters (H=5 and K=14), proposes a different path than the one provided in the assignment. However, measuring the difference lengths of the two paths, it shows that for my implementation the path can be 1.84mm shorter, equivalent to the 0.007% of the total length of it.



