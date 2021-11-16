#include<iostream>
#include<vector>
#include<list>

using namespace std;

#define raws 15
#define columns 30
//UPDATE WITH RIGHT VALUES
#define end 3
#define wall 2
#define start 1
#define empty 0
#define ball 4

vector<int> findEnd(vector<vector<int>> map) {
    for(int i = 0; i < raws; i++) {
        for(int j = 0; j < columns; j++) { 
            if(map[i][j] == end)
                return {i, j};
        }
    }
    return {-1, -1};
}

int main(void) {
    vector<vector<int>> map= {
        {0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,2,0,0,0,0,0,0,0,0},
        {0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0},
        {0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,2,0,0,0,0,0,0,0,0},
        {0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,2,0,0,0,0,0,0,0,0},
        {0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0},
        {0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,2,0,0,2,0,0,0,0,0},
        {0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,2,0,0,2,0,0,0,0,0},
        {0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,2,0,0,2,0,0,0,0,0},
        {0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0},
        {0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0},
        {0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0},
        {0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,3,0,2,0,0,0,0,0},
        {0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0},
        {0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    };

    vector<vector<bool>> mapVis;

    for(int i=0; i < raws; i++) {
        mapVis.push_back(vector<bool> ());
        for(int j=0; j < raws; j++) {
            mapVis[i].push_back(false);
        }
    }

    list<vector<int>> haveTo;
    haveTo.push_front(findEnd(map));
    mapVis[haveTo.front()[0]][haveTo.front()[1]] = true;

    while(haveTo.size() != 0) {
        vector<int> now = haveTo.front();
        haveTo.pop_front();

        //HAVE TO ADD THE CHEC TO BOUNDS
        if(now[0] + 1 < raws && map[now[0] + 1][now[1]] != wall && !mapVis[now[0] + 1][now[1]])  {
            haveTo.push_back(vector<int> {now[0] + 1, now[1]});
            mapVis[now[0] + 1][now[1]] = true;
        }

        if(now[1] + 1 < columns && map[now[0]][now[1] + 1] != wall && !mapVis[now[0]][now[1] + 1])  {
            haveTo.push_back(vector<int> {now[0], now[1] + 1});
            mapVis[now[0]][now[1] + 1] = true;
        }

        if(now[0] - 1 >= 0 && map[now[0] - 1][now[1]] != wall && !mapVis[now[0] - 1][now[1]])  {
            haveTo.push_back(vector<int> {now[0] - 1, now[1]});
            mapVis[now[0] - 1][now[1]] = true;
        }

        if(now[1] - 1 >= 0 && map[now[0]][now[1] - 1] != wall && !mapVis[now[0]][now[1] - 1])  {
            haveTo.push_back(vector<int> {now[0], now[1] - 1});
            mapVis[now[0]][now[1] - 1] = true;
        }

        cout << "Cicle finished" <<endl;
    }

    for(int i=0; i < raws; i++) {
        cout <<endl;
        for(int j=0; j < columns; j++) {
            cout << mapVis[i][j] << " ";
        }
    }

    //WITH THIS MAP NOW I CAN CREATE THE GRAPH FOR DIJKISTRA

}