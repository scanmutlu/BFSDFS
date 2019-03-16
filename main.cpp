#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;



void matrixCopy(char **parentArray, char **chidArray, int u, int v);

class Graph {
    int n;                ///number of verticiles
    int **A;             ///Matrix- lines between verticiles

public:
    Graph(int size = 2);

    ~Graph();

    bool isConnected(int, int);

    void addEdge(int u, int v);

    void printState(int k);
};

Graph::Graph(int size) {
    int i, j;
    if (size < 2) n = 2;
    else n = size;
    A = new int *[n];
    for (i = 0; i < n; ++i) {
        A[i] = new int[n];
    }
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            A[i][j] = 0;                /// All numbers in links matrix assigned to 0 at initialize.
        }
    }
}

Graph::~Graph() {
    for (int i = 0; i < n; ++i)delete[] A[i];
    delete[] A;
}

bool Graph::isConnected(int u, int v) {
    return (A[u - 1][v - 1] ==
            1);          ///Return bool value if given v and u verticiles connected. This will be used for BFS and DFS.
}

void Graph::addEdge(int u, int v) {     ///Graph structure will build via this function.
    A[u - 1][v - 1] = 1;
    A[v - 1][u - 1] = 1;
}


class State {
    int size;
    char **graphArray;

public:
    State(int u, int v);

    void setSourcePosition(int u, int v);

    void setMinerPosition(int u, int v);

    ~State();

    int getSize() const;

    void setSize(int size);

    void setGraph(char **initialArray, int u, int v);

    char **getGraphArray() const;

    void setGraphArray(char **graphArray);

};

State::State(int u, int v) {
    int i, j;
    graphArray = new char *[u];
    for (i = 0; i < u; ++i) {
        graphArray[i] = new char[v];
    }
    for (i = 0; i < u; ++i) {
        for (j = 0; j < v; ++j) {
            graphArray[i][j] = '.';
        }
    }
}


State::~State() {
    for (int i = 0; i < size; ++i)delete[] graphArray[i];
    delete[] graphArray;
}

void State::setSourcePosition(int u, int v) {
    graphArray[u][v] = 's';
}

void State::setMinerPosition(int u, int v) {
    graphArray[u][v] = 'm';
}

void State::setGraph(char **initialArray, int u, int v) {
    for (int i = 0; i < u; ++i) {
        for (int j = 0; j < v; ++j) {
            graphArray[i][j] = initialArray[i][j];
        }
    }
}

int State::getSize() const {
    return size;
}

void State::setSize(int size) {
    State::size = size;
}

char **State::getGraphArray() const {
    return graphArray;
}

void State::setGraphArray(char **graphArray) {
    State::graphArray = graphArray;
}

char **generateChild(State &parent, int u, int v, int *uValues, int *vValues) {
    char **currentArray = NULL;
    matrixCopy(parent.getGraphArray(), currentArray, u, v);
    for (int i = 0; i < u; ++i) {
        for (int j = 0; j < v; ++j) {
            if (parent.getGraphArray()[i][j] != 's' && uValues[i] != 0 && vValues[j] != 0) {
                currentArray[i][j] = 'm';

            }
        }
    }
}

void matrixCopy(char **parentArray, char **chidArray, int u, int v) {
    for (int i = 0; i < u; ++i) {
        for (int j = 0; j < v; ++j) {
            chidArray[i][j] = parentArray[i][j];
        }
    }
}

class properties{
    char **stateArray;
    int *xValues;
    int *yValues;
    int u;
    int v;
public:
    properties(char **stateArray, int *xValues, int *yValues, int u, int v) : stateArray(stateArray), xValues(xValues),
                                                                              yValues(yValues), u(u), v(v) {}

    char **getStateArray() const {
        return stateArray;
    }

    void setStateArray(char **stateArray) {
        properties::stateArray = stateArray;
    }

    int *getXValues() const {
        return xValues;
    }

    void setXValues(int *xValues) {
        properties::xValues = xValues;
    }

    int *getYValues() const {
        return yValues;
    }

    void setYValues(int *yValues) {
        properties::yValues = yValues;
    }

    int getU() const {
        return u;
    }

    void setU(int u) {
        properties::u = u;
    }

    int getV() const {
        return v;
    }

    void setV(int v) {
        properties::v = v;
    }
};
properties& readFromFile(const string file_name);
void printFile(properties prop);

int main() {
    properties *prop;
    *prop = readFromFile("input_0.txt");

    printFile(*prop);

}

properties& readFromFile(const string file_name) {
    char **stateArray;
    int *xValues;
    int *yValues;
    ifstream file(file_name);
    int u, v = 0;
    string line;
    int count = 0;

    while (getline(file, line)) {
        istringstream isStream(line);
        if (count == 0) {
            isStream >> u;
            isStream >> v;
            xValues = new int[u];
            yValues = new int[v];
            stateArray = new char*[u];
            for (int i = 0; i < u ; ++i) {
                stateArray[i] = new char[v];
            }
        }else if (count == 1) {
            for (int i = 0; i < u; ++i) {
                isStream >> xValues[i];
            }
        } else {
            string temp;
            isStream >> yValues[count - 2];
            int tempCount = 0;
            getline(file,temp,'\t');
            for (int i = 0; i < v; ++i) {
                getline(file,temp,'\t');
                stateArray[count-2][i] = temp.at(0);
            }
        }
        count++;
    }
    properties *prop = new properties(stateArray,xValues,yValues,u,v);
    return *prop;
}

void printFile(properties prop) {

    for (int i = 0; i < prop.getU(); ++i) {
        for (int j = 0; j < prop.getV(); ++j) {
            cout << prop.getStateArray()[i][j];
        }
        cout << endl;
    }
    cout << endl;
    for (int k = 0; k < prop.getU(); ++k) {
        cout << prop.getXValues();
    }
    cout << endl;

    for (int l = 0; l < prop.getV(); ++l) {
        cout << prop.getYValues();
    }

}
