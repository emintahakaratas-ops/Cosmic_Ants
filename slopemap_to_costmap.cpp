#include <bits/stdc++.h>
using namespace std;

int main() {
    FILE* infile = fopen("slopemap.txt", "r");
    FILE* outfile = fopen("costmap.txt", "w");
    
    if (infile == NULL || outfile == NULL) {
        printf("Error opening files\n");
        return 1;
    }
    
    int n, m;
    fscanf(infile, "%d %d", &n, &m);
    
    vector<vector<double>> slope(n, vector<double>(m));
    vector<vector<int>> cost(n, vector<int>(m));
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            fscanf(infile, "%lf", &slope[i][j]);
        }
    }
    
    double max_slope = 100.0;
    double scale = 0.3;
    int base = 1;
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            double s = slope[i][j];
            
            if(s > max_slope){
                cost[i][j] = -1;
            } else {
                cost[i][j] = base + (int)(s * scale);
                if(cost[i][j] < 1) cost[i][j] = 1;
            }
        }
    }
    
    fprintf(outfile, "%d %d\n", n, m);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            fprintf(outfile, "%d ", cost[i][j]);
        }
        fprintf(outfile, "\n");
    }
    
    fclose(infile);
    fclose(outfile);
    
    return 0;
}