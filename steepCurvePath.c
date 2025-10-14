/***
 * @file steepCurvePath.c
 * @author Francesco Bittasi (francesco.bittasi@studenti.unipd.it)
 * 
 * @brief Submission for the application to the driverless department
 * of the RaceUP unipd FormulaSAE team. (Assignment 1)
 * @date 14-10-2025
 */

#include <stdio.h>
#include <math.h>

// Parameters
#define I 7         // m of the inner radius
#define O 10        // m of the outer radius
#define H 24        // # of points in the radial direction
#define K 128       // # of points in the direction of travel

// Macros
#define UNDEF -1
#define FALSE 0
#define TRUE 1
#define PI 3.14

/// point
typedef struct coords {
    unsigned h;
    unsigned k;
} coords;

typedef struct point {
    char traversed;
    double distance;
    coords prev;
} point;

/// Retrives the index of the point with minimum distance from the source
int selectMinDist(point graph[K][H]) {
    double minDist = INFINITY;
    int minDistIdx = UNDEF;

    for (int i = 0; i < K; i++) {
        for (int j = 0; j < H; j++) {
            if (graph[i][j].distance < minDist) {
                minDistIdx = i * H + j;
            }
        }
    }

    if (minDistIdx == UNDEF) {
        printf("ERROR");
    }
    return minDistIdx;
}

void updateDistance(int h, int k, double edgeLenght, int prevH, int prevK, point graph[K][H]) {
    double updatedDistance = graph[prevK][prevH].distance + edgeLenght;
    if (updatedDistance < graph[k][h].distance) {
        graph[k][h].distance = updatedDistance;
        graph[k][h].prev.h = prevH;
        graph[k][h].prev.k = prevK;
    }
}

int main() {
    // Parameters setup
    double a = 180.0 / (double)K;       // angle of K subdivisions
    double t[H];                        // lenght of vertical edges (on the direction of the circumference, one for each h distance from the center)
    double d[H-1];                      // lenght of the diagonal edges (one for each trapezoid)
    double l = (double)(O - I) / 2.0;   // lenght of the radial edges (it's always the same)
    
    // Computing edges lenght
    for (int i = 0; i < H; i++) {
        t[i] = 2 * (I + l*i) * sin(a / 2);
    }
    for (int i = 0; i < H - 1; i++) {
        d[i] = sqrt(l*l + t[i]*t[i] + 2*t[i]*l*sin(a / 2));
    }

    // Setup graph structure (Dijkstra)
    point graph[K][H];                  // matrix of points in the graph, row: points on the same partition, column: points on the same semicircumference
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < H; j++) {
            graph[i][j].traversed = FALSE;
            graph[i][j].distance = INFINITY;
            graph[i][j].prev.h = UNDEF;
            graph[i][j].prev.k = UNDEF;
        }
    }
    graph[0][H-1].distance = 0.;            // setup source of the path


    // Pathfinding algorithm (Dijkstra)
    // we have to analyze each point, we know exactly how many points we have
    for (int i = 0; i < K*H; i++) {
        // select current minimum distance point
        int u = selectMinDist(graph);
        int k = u/H;
        int h = u%H;
        graph[k][h].traversed = TRUE;

        // analyze possible edges and update distances/prev.Points
        // - left (lenght l)
        if (h > 0) {
            updateDistance(h-1,k,l,h,k,graph);
        }
        // - right (lenght l)
        if (h < H - 1) {
            updateDistance(h+1,k,l,h,k,graph);
        }
        // - top (lenght t[h])
        if (k < K - 1) {
            updateDistance(h,k+1,t[h],h,k,graph);
        }
        // - bottom (lenght t[h]) [probably useless]
        if (k > 0) {
            updateDistance(h,k-1,t[h],h,k,graph);
        }
        // - top, left (lenght d[h-1])
        if (k < K - 1 && h > 0) {
            updateDistance(h-1,k+1,d[h-1],h,k,graph);
        }
        // - top, right (lenght d[h])
        if (k < K - 1 && h < H - 1) {
            updateDistance(h+1,k+1,d[h],h,k,graph);
        }        
        // - bottom, left (lenght d[h-1]) [probably useless]
        if (k > 0 && h > 0) {
            updateDistance(h-1,k-1,d[h-1],h,k,graph);
        }
        // - bottom, right (lenght d[h]) [probably useless]
        if (k > 0 && h < H - 1) {
            updateDistance(h+1,k-1,d[h],h,k,graph);
        }
    }

    // Result retrival and output
    // - starting from the endpoint we traverse back the path
    // Given we know the structure of the graph, we can assume that the number of points
    // traversed with always be lower than K*2 by a large margin... (unproven, but plausible)
    coords path[K*2];
    int h = H-1, k = K-1;
    int i = 0;
    path[i].k = k;
    path[i].h = h;
    while (graph[k][h].prev.h != UNDEF) {
        i++;
        path[i].h = graph[k][h].prev.h;
        path[i].k = graph[k][h].prev.k;
        h = path[i].h;
        k = path[i].k;
    }

    printf("Total path lenght: %fm\n",graph[K-1][H-1].distance);
    printf("Path:");
    for (int j=0; j<i; j++) {
        printf(" (%d,%d)",path[j].k, path[j].h);
    }
    printf("\n");

    return 0;
}