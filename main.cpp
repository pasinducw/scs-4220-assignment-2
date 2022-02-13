#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <chrono>
#include <random>
#include <stack>

#include "lib/VEBTree.h"
#include "lib/FibonacciHeap.h"

#define INF (1e9)

using namespace std;

chrono::steady_clock::time_point tBegin;
vector<pair<long long, string>> timestamps;
mt19937 generator;
bool generatorInitialized = false;

void tick() {
    tBegin = chrono::steady_clock::now();
}

void tock(string message) {
    chrono::steady_clock::time_point tEnd = chrono::steady_clock::now();
    auto duration = chrono::nanoseconds(tEnd - tBegin);
    timestamps.push_back({duration.count(), message});
}

int betterRand() {
    if (!generatorInitialized) {
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        generator = mt19937(seed);
    }
    int value = generator();
    return value < 0 ? -value : value;
}

void printResults(int N, int P, int W, bool withHeaders = false) {
    vector<string> headers;
    vector<long long> data;
    headers.push_back("N");
    headers.push_back("P");
    headers.push_back("W");

    data.push_back(N);
    data.push_back(P);
    data.push_back(W);

    for (auto entry:timestamps) headers.push_back(entry.second);
    for (auto entry:timestamps) data.push_back(entry.first);

    if (withHeaders) {
        for (auto entry:headers) cout << entry << ',';
        cout << endl;
    }

    for (auto entry:data) cout << entry << ',';
    cout << endl;
}

vector<int> dijkstra(const int V, vector<vector<pair<int, int>>> &E, const int source) {
    tick();
    // computes the minimum distance to all the reachable nodes from the given source node
    vector<bool> processed(V, false);
    vector<int> distance(V, INF);
    distance[source] = 0;

    priority_queue <pair<int, int>> pq;
    pq.push({0, source});

    tock("Prepare Standard Dijkstra");
    tick();
    while (!pq.empty()) {
        auto a = pq.top().second;
        pq.pop();
        if (processed[a]) continue;
        processed[a] = true;
        for (auto u:E[a]) {
            auto b = u.first, w = u.second;
            if (distance[a] + w < distance[b]) {
                distance[b] = distance[a] + w;
                pq.push({-distance[b], b});
            }
        }
    }

    tock("Standard Dijkstra");
    return distance;
}

vector<int> dijkstraWithFibonacciHeap(const int V, vector<vector<pair<int, int>>> &E, const int source) {
    tick();
    // computes the minimum distance to all the reachable nodes from the given source node
    vector<bool> processed(V, false);
    vector<int> distance(V, INF);
    distance[source] = 0;

    AugmentedFibonacciHeap<pair<int, int>> pq;
    pq.push(0, {0, source});

    tock("Prepare Dijkstra with Fibonacci Heap");
    tick();
    while (!pq.empty()) {
        auto a = pq.top().second;
        pq.pop();
        if (processed[a]) continue;
        processed[a] = true;
        for (auto u:E[a]) {
            auto b = u.first, w = u.second;
            if (distance[a] + w < distance[b]) {
                distance[b] = distance[a] + w;
                pq.push(distance[b], {distance[b], b});
            }
        }
    }

    tock("Dijkstra with Fibonacci Heap");
    return distance;
}

vector<int> dijkstraWithVEBTree(const int V, vector<vector<pair<int, int>>> &E, const int source) {
    tick();
    // computes the minimum distance to all the reachable nodes from the given source node
    vector<bool> processed(V, false);
    vector<int> distance(V, INF);
    distance[source] = 0;

    // compute max possible distance
    int maxPossibleDistance = 0;
    for (auto vE:E)
        for (auto e:vE) {
            maxPossibleDistance += e.second;
        }

    AugmentedVEBTree<pair<int, int>> pq(maxPossibleDistance);
    pq.push(0, {0, source});

    tock("Prepare Dijkstra with van Emde Boas Tree");
    tick();

    while (!pq.empty()) {
        auto a = pq.top().second;
        pq.pop();
        if (processed[a]) continue;
        processed[a] = true;
        for (auto u:E[a]) {
            auto b = u.first, w = u.second;
            if (distance[a] + w < distance[b]) {
                distance[b] = distance[a] + w;
                pq.push(distance[b], {distance[b], b});
            }
        }
    }

    tock("Dijkstra with van Emde Boas Tree");
    return distance;
}

// creates a directed graph with N nodes, with specified degree for each node
// with specified maximum weight
vector<vector<pair<int, int>>> generateGraph(int N, int P, int W) {
    if (P > N - 1) throw invalid_argument("P cannot be greater than N-1");

    vector<vector<pair<int, int>>> E(N);
    for (int source = 0; source < N; source++) {
        vector<bool> hasEdge(N, false);
        for (int j = 0; j < P; j++) {
            int target = betterRand() % N;
            int weight = betterRand() % W + 1;
            while (hasEdge[target] || target == source) target = betterRand() % N;
            hasEdge[target] = true;
            E[source].push_back({target, weight});
        }
    }

    // check for unreachable nodes and link them to root
    vector<bool> visited(N, false);
    stack<int> dfs;
    dfs.push(0);
    while (!dfs.empty()) {
        auto node = dfs.top();
        dfs.pop();
        if (visited[node]) continue;
        visited[node] = true;
        for (auto e:E[node]) dfs.push(e.first);
    }

    for (int i = 0; i < N; i++)
        if (!visited[i]) {
            int weight = betterRand() % W + 1;
            E[0].push_back({i, weight});
        }

    return E;
}

int main(int argc, const char **argv) {
    int N, P, W;
    bool printHeaders;

    if (argc != 5) {
        cout << "Invalid commandline arguments" << endl;
        return 0;
    }

    srand(time(NULL));

    N = atoi(argv[1]);
    P = betterRand() % min(atoi(argv[2]), N - 1) + 1;
    W = betterRand() % atoi(argv[3]) + 1;
    printHeaders = atoi(argv[4]);

    auto E = generateGraph(N, P, W);

    auto distances = dijkstra(N, E, 0);
    auto distancesWithFibonacciHeap = dijkstraWithFibonacciHeap(N, E, 0);
    auto distancesWithVEBTree = dijkstraWithVEBTree(N, E, 0);


    for (int i = 0; i < N; i++) {
        assert(distances[i] == distancesWithFibonacciHeap[i]);
        assert(distances[i] == distancesWithVEBTree[i]);
    }

    printResults(N, P, W, printHeaders);
    return 0;
}
