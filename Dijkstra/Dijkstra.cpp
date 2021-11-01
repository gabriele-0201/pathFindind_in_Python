#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

using namespace std;
namespace py = pybind11;

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

constexpr int raws = 15;
constexpr int columns = 30;

void removeUnvisited(vector<NodeD*> &unvisited, NodeD* node);
NodeD* minDistance(vector<NodeD*> &unvisited);
bool alreadyVisited(vector<NodeD*> &visited, NodeD* node);

vector<vector<int>> dijkstra(vector<vector<int>> map) {
    //Initialize Graph

    NodeD (*nodes)[columns] = new NodeD[raws][columns];

    for(int i = 0; i < raws; i++) {
        for(int j = 0; j < columns; j++) {
            if(map[i][j] != 1)
                nodes[i][j] = NodeD(i, j);
        }
    }

    for(int i = 0; i < raws; i++) {
        for(int j = 0; j < columns; j++) {
            if(i > 0)
                    if(map[i][j] != 1)
                        nodes[i][j].addAttachedNode(&nodes[i - 1][j]);
            if(i < (raws - 1))
                    if(map[i][j] != 1)
                        nodes[i][j].addAttachedNode(&nodes[i + 1][j]);

            if(j > 0)
                    if(map[i][j] != 1)
                        nodes[i][j].addAttachedNode(&nodes[i][j - 1]);
            if(j < (columns - 1))
                    if(map[i][j] != 1)
                        nodes[i][j].addAttachedNode(&nodes[i][j + 1]);
        }
    }

    //Algorithm

    NodeD* start = &nodes[0][0];
    NodeD* end = &nodes[raws - 1][columns - 1];

    vector<NodeD*> visited;
    vector<NodeD*> unvisited;

    for(int i = 0; i < raws; i++) {
        for(int j = 0; j < columns; j++) {
            if(map[i][j] != 1)
                unvisited.push_back(&nodes[i][j]);
        }
    }

    NodeD* selected = end;
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

    NodeD* path = start;

    vector<vector<int>> lengthMap;
    
    for(int i = 0; i < raws; i++) {
        vector<int> lineVec;
        for(int j = 0; j < columns; j++) { 
            if(map[i][j] != 1)
                lineVec.push_back(nodes[i][j].getDistance());
            else
                lineVec.push_back(-1);
        }
        lengthMap.push_back(lineVec);
    }

   return lengthMap;
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

PYBIND11_MODULE(FindPathDijkstra, handle) {
    handle.def("dijkstra", &dijkstra);
}