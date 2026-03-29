#include <bits/stdc++.h>
using namespace std;

const long long INF = 4e18;

int n, m;
vector<vector<int>> cost;
vector<vector<long long>> dist;
vector<vector<pair<int,int>>> parent;

int dx[4]={1,-1,0,0};
int dy[4]={0,0,1,-1};

long long heuristic(int x, int y, int gx, int gy){
    return abs(x-gx)+abs(y-gy);
}

void astar(int sx, int sy, int gx, int gy){
    priority_queue<
        pair<long long,pair<int,int>>,
        vector<pair<long long,pair<int,int>>>,
        greater<pair<long long,pair<int,int>>>
    > pq;

    dist[sx][sy]=0;
    parent[sx][sy]={-1,-1};

    pq.push({heuristic(sx,sy,gx,gy),{sx,sy}});

    while(!pq.empty()){
        auto cur=pq.top();
        pq.pop();

        int x=cur.second.first;
        int y=cur.second.second;

        if(x==gx && y==gy) return;

        for(int i=0;i<4;i++){
            int nx=x+dx[i];
            int ny=y+dy[i];

            if(nx<0 || ny<0 || nx>=n || ny>=m) continue;
            if(cost[nx][ny]==-1) continue;

            long long nd = dist[x][y] + cost[nx][ny];

            if(nd < dist[nx][ny]){
                dist[nx][ny]=nd;
                parent[nx][ny]={x,y};
                long long f = nd + heuristic(nx,ny,gx,gy);
                pq.push({f,{nx,ny}});
            }
        }
    }
}

int main(){
    FILE* infile = fopen("costmap.txt", "r");
    FILE* outfile = fopen("rota.txt", "w");
    
    if (infile == NULL || outfile == NULL) {
        printf("Error opening files\n");
        return 1;
    }
    
    fscanf(infile, "%d %d", &n, &m);

    cost.assign(n,vector<int>(m));
    dist.assign(n,vector<long long>(m,INF));
    parent.assign(n,vector<pair<int,int>>(m,{-2,-2}));

    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            fscanf(infile, "%d", &cost[i][j]);
        }
    }

    // Başlangıç ve bitiş noktaları - ~100 adım civarı
    int sx = 100, sy = 100;    // Başlangıç: (100, 100)
    int gx = 150, gy = 150;    // Bitiş: (150, 150)
    
    if (sx == -1 || gx == -1) {
        fprintf(outfile, "-1\n");
        fclose(infile);
        fclose(outfile);
        return 0;
    }
    
    fprintf(outfile, "Start: (%d, %d) -> %d\n", sx, sy, cost[sx][sy]);
    fprintf(outfile, "Goal: (%d, %d) -> %d\n", gx, gy, cost[gx][gy]);
    
    astar(sx, sy, gx, gy);

    if(dist[gx][gy]==INF){
        fprintf(outfile, "-1\n");
        fclose(infile);
        fclose(outfile);
        return 0;
    }

    fprintf(outfile, "%lld\n", dist[gx][gy]);

    vector<pair<int,int>> path;
    int x=gx,y=gy;

    while(!(x==sx && y==sy)){
        path.push_back({x,y});
        auto p=parent[x][y];
        if(p.first==-2){
            fprintf(outfile, "-1\n");
            fclose(infile);
            fclose(outfile);
            return 0;
        }
        x=p.first;
        y=p.second;
    }

    path.push_back({sx,sy});
    reverse(path.begin(),path.end());

    fprintf(outfile, "%lu\n", path.size());
    for(auto u:path){
        fprintf(outfile, "%d %d\n", u.first, u.second);
    }
    
    fclose(infile);
    fclose(outfile);
    
    return 0;
}