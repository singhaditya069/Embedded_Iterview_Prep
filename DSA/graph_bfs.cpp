#include <iostream>
#include <vector>
#include <queue>
using namespace std;

void bfs(int start, vector<vector<int>>& adj, int n) {
    vector<bool> visited(n, false);
    queue<int> q;
    q.push(start);
    visited[start] = true;
    while(!q.empty()) {
        int node = q.front(); q.pop();
        cout << node << " ";
        for(int neighbor : adj[node]) {
            if(!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
}

int main() {
    int n = 5;
    vector<vector<int>> adj(n);
    adj[0] = {1, 2};
    adj[1] = {0, 3};
    adj[2] = {0, 4};
    adj[3] = {1};
    adj[4] = {2};
    cout << "BFS starting from node 0: ";
    bfs(0, adj, n);
    cout << endl;
    return 0;
}
