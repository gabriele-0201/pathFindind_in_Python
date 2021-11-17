#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include<iostream>
#include<vector>
#include<list>

using namespace std;

constexpr int raws = 15;
constexpr int columns = 30;
constexpr int endCode = 3;
constexpr int wall = 2;
constexpr int start = 1;
constexpr int empty = 0;
constexpr int ball = 4;

class NodeD {
    private:

        int raw;
        int column;
        int distance; //Distance from the end
        vector<NodeD*> attacchedNode;
        NodeD* prevNode;

    public:

        NodeD();
        NodeD (int _raw, int _column);

        int getRaw();
        int getColumn();
        bool calcDistance(int prevDistance);
        int getDistance();
        void setStartDistance();
        void addAttachedNode(NodeD *att);
        vector<NodeD*> getAttacchedNode(); 
        void setPrevNode(NodeD* pNode);
        NodeD* getPrevNode();
};

vector<int> findEnd(vector<vector<int>> map) {
    for(int i = 0; i < raws; i++) {
        for(int j = 0; j < columns; j++) { 
            if(map[i][j] == endCode)
                return {i, j};
        }
    }
    return {-1, -1};
}

void removeUnvisited(vector<NodeD*> &unvisited, NodeD* node) {
    for(vector<NodeD*>::iterator itr = unvisited.begin(); itr != unvisited.end(); ++itr) {
        //Have to do this because the iterator is a pointer to the value of the vector and I have to use the value pointed by the value in the vector and this pointere is pointed from the iterator
        if((*itr) -> getRaw() == node -> getRaw() && (*itr) -> getColumn() == node -> getColumn()) {
            unvisited.erase(itr);
            break;
        }
    }
}

NodeD* minDistance(vector<NodeD*> &unvisited) {
    vector<NodeD*>::iterator itrMinD;

    int minD;
    bool foundFirst = false;

    for(vector<NodeD*>::iterator itr = unvisited.begin(); itr != unvisited.end(); ++itr) {
        int thisDistance = (*itr) -> getDistance();

        if(!foundFirst && thisDistance != -1) {
            foundFirst = true;
            minD = thisDistance;
            itrMinD = itr;
        }else if(foundFirst && thisDistance < minD && thisDistance != -1) {
            minD = (*itr) -> getDistance();
            itrMinD = itr;
        }
    }
    return *itrMinD;
}

bool alreadyVisited(vector<NodeD*> &visited, NodeD* node) {
    for(vector<NodeD*>::iterator itr = visited.begin(); itr != visited.end(); ++itr) {
        //Have to do this because the iterator is a pointer to the value of the vector and I have to use the value pointed by the value in the vector and this pointere is pointed from the iterator
        if((*itr) -> getRaw() == node -> getRaw() && (*itr) -> getColumn() == node -> getColumn()) {
            return true;    
        }
    }
    return false;
}

//Class implementation
NodeD::NodeD() {};
NodeD::NodeD(int _raw, int _column) : raw{_raw}, column{_column}, distance{-1} {};

int NodeD::getRaw() { return raw; }
int NodeD::getColumn() { return column; }

bool NodeD::calcDistance(int prevDistance) {
    int newD = prevDistance + 1;

    if(distance = -1 || newD < distance) {
        distance = newD;
        return true;
    } else 
        return false;

}

int NodeD::getDistance() { return distance; }

void NodeD::setStartDistance() {
    distance = 0;
}

void NodeD::addAttachedNode(NodeD *att) {
    attacchedNode.push_back(att);
}

vector<NodeD*> NodeD::getAttacchedNode() { return attacchedNode; }

void NodeD::setPrevNode(NodeD* pNode) {
    prevNode = pNode;
}

NodeD* NodeD::getPrevNode() {return prevNode; }

vector<vector<int>> dijkstra(vector<vector<bool>> map, vector<int> posEnd) {
    //Initialize Graph

    NodeD (*nodes)[columns] = new NodeD[raws][columns];

    for(int i = 0; i < raws; i++) {
        for(int j = 0; j < columns; j++) {
            if(map[i][j])
                nodes[i][j] = NodeD(i, j);
        }
    }

    for(int i = 0; i < raws; i++) {
        for(int j = 0; j < columns; j++) {
            if(i > 0)
                    if(map[i][j])
                        nodes[i][j].addAttachedNode(&nodes[i - 1][j]);
            if(i < (raws - 1))
                    if(map[i][j])
                        nodes[i][j].addAttachedNode(&nodes[i + 1][j]);

            if(j > 0)
                    if(map[i][j])
                        nodes[i][j].addAttachedNode(&nodes[i][j - 1]);
            if(j < (columns - 1))
                    if(map[i][j])
                        nodes[i][j].addAttachedNode(&nodes[i][j + 1]);
        }
    }


    NodeD* endNode = &nodes[posEnd[0]][posEnd[1]];

    vector<NodeD*> visited;
    vector<NodeD*> unvisited;

    for(int i = 0; i < raws; i++) {
        for(int j = 0; j < columns; j++) {
            if(map[i][j] != wall)
                unvisited.push_back(&nodes[i][j]);
        }
    }

    NodeD* selected = endNode;
    selected -> setStartDistance();

    int counter = 0;

    while (unvisited.size() != 0) {
        
        vector<NodeD*> attnodes = selected -> getAttacchedNode();

        for(NodeD* node : attnodes) {
            
            if(alreadyVisited(visited, node))
                continue;

            bool modified = node -> calcDistance(selected -> getDistance());
            if(modified)
                node-> setPrevNode(selected);
           
        }


        visited.push_back(selected);
        removeUnvisited(unvisited, selected);
        if(!unvisited.empty())
            selected = minDistance(unvisited);
    }


    vector<vector<int>> lengthMap;
    
    for(int i = 0; i < raws; i++) {
        vector<int> lineVec;
        for(int j = 0; j < columns; j++) { 
            if(map[i][j])
                lineVec.push_back(nodes[i][j].getDistance());
            else
                lineVec.push_back(-1);
        }
        lengthMap.push_back(lineVec);
    }

   return lengthMap;
}

vector<vector<bool>> getBfs(vector<vector<int>>& map, vector<int>& endP) {
    vector<vector<bool>> mapVis(raws);

    for(int i=0; i < raws; i++) {
        //mapVis -> push_back(vector<bool>());
        for(int j=0; j < columns; j++) {
            mapVis[i].push_back(false);
        }
    }

    list<vector<int>> haveTo;
    haveTo.push_front(endP);
    mapVis[haveTo.front()[0]][haveTo.front()[1]] = true;

    while(haveTo.size() != 0) {
        vector<int> now = haveTo.front();
        haveTo.pop_front();

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
    }
    return mapVis;
}

vector<vector<int>> bfsDijkstra(vector<vector<int>> map) {
    vector<int> endPosition = findEnd(map);

    vector<vector<bool>> mapVis = getBfs(map, endPosition);

    vector<vector<int>> dij = dijkstra(mapVis, endPosition);

    return dij;
}

PYBIND11_MODULE(bfsDijkstraModule, handle) {
    handle.def("bfsDijkstra", &bfsDijkstra);
}

/*
int main(void) {
    vector<vector<int>> map= {
        {0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,2,0,0,0,0,0,0,0,0},
        {0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0},
        {0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,2,0,0,0,0,0,3,0,0},
        {0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,2,0,0,0,0,0,0,0,0},
        {0,2,0,0,0,0,0,2,0,0,0,0,0,0,2,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0},
        {0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,2,0,0,2,0,0,0,0,0},
        {0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,2,0,0,2,0,0,0,0,0},
        {0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,2,0,0,2,0,0,0,0,0},
        {0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0},
        {0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0},
        {0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0},
        {0,2,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0},
        {0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0},
        {0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    };

    vector<int> endPosition = findEnd(map);

    vector<vector<bool>> mapVis = getBfs(map, endPosition);

    vector<vector<int>> dij = bfsDijkstra(mapVis, endPosition);
    
    for(int i = 0; i < raws; i++) {
        cout <<endl;
        for(int j = 0; j < columns; j++) { 
            cout <<dij[i][j] <<" ";
        }
    }
}
*/
