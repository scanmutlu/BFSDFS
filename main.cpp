#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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
    A[u ][v] = 1;
    A[v][u] = 1;
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
    return NULL;
}

void matrixCopy(char **parentArray, char **chidArray, int u, int v) {
    for (int i = 0; i < u; ++i) {
        for (int j = 0; j < v; ++j) {
            chidArray[i][j] = parentArray[i][j];
        }
    }
}

class Properties{
    char **stateArray;
    int *xValues;
    int *yValues;
    int u;
    int v;
public:
    Properties(int u, int v){
        this->u = u;
        this->v = v;
        this->stateArray = new char*[u];
        for (int i = 0; i < u ; ++i) {
            this->stateArray[i] = new char[v];
        }
        this->xValues = new int[u];
        this->yValues = new int[v];
    };
    Properties(char **stateArray, int *xValues, int *yValues, int u, int v) : stateArray(stateArray), xValues(xValues),
                                                                              yValues(yValues), u(u), v(v) {}

    char **getStateArray();

    void setStateArray(char **stateArray);

    int *getXValues();

    void setXValues(int *xValues);

    int *getYValues();

    void setYValues(int *yValues);

    int getU();

    void setU(int u);

    int getV();

    void setV(int v);
};

char **Properties::getStateArray() {
    return stateArray;
}

void Properties::setStateArray(char **stateArray) {
    Properties::stateArray = stateArray;
}

int *Properties::getXValues(){
    return xValues;
}

void Properties::setXValues(int *xValues) {
    Properties::xValues = xValues;
}

int *Properties::getYValues() {
    return yValues;
}

void Properties::setYValues(int *yValues) {
    Properties::yValues = yValues;
}

int Properties::getU() {
    return u;
}

void Properties::setU(int u) {
    Properties::u = u;
}

int Properties::getV() {
    return v;
}

void Properties::setV(int v) {
    Properties::v = v;
}

Properties readFromFile(const string file_name);
void printFile(Properties prop);
bool isEqualMatrix(char **A, char **B, int u, int v);
int findInStates(vector<Properties>stateArray, char **A);
bool controlGraphState(Properties prop);
void generateAllStates(vector<Properties>&stateArray, Properties parent, Graph &graph);

int main() {
    Properties prop = readFromFile("input_0.txt");
    vector<Properties> stateArray;
    int u = prop.getU();
    int v = prop.getV();
    stateArray.push_back(prop);
    Graph *graph = new Graph(u*u*v*v);
    generateAllStates(stateArray,prop,*graph);
    for (int i = 0; i < stateArray.size(); ++i) {
        printFile(stateArray.at(i));
    }
}

Properties readFromFile(const string file_name) {
    char **stateArray;
    int *xValues;
    int *yValues;
    ifstream file(file_name);
    int u = 0;
    int v = 0;
    string line;
    int count = 0;

    while (getline(file, line)) {
        istringstream isStream(line);
        if (count == 0) {
            isStream >> v;
            isStream >> u;
            yValues = new int[v];
            xValues = new int[u];
            stateArray = new char*[u];
            for (int i = 0; i < u ; ++i) {
                stateArray[i] = new char[v];
            }
        }else if (count == 1) {
            for (int i = 0; i < v; ++i) {
                isStream >> yValues[i];
            }
        } else {
            string temp;
            getline(isStream,temp,'\t');
            xValues[count-2] = stoi(temp);
            for (int i = 0; i < v; ++i) {
                getline(isStream,temp,'\t');
                stateArray[count-2][i] = temp.at(0);
            }
        }
        count++;
    }
    Properties *prop = new Properties(stateArray,xValues,yValues,u,v);
    return *prop;
}

void printFile(Properties prop) {

    for (int i = 0; i < prop.getU(); ++i) {
        for (int j = 0; j < prop.getV(); ++j) {
            cout << prop.getStateArray()[i][j];
        }
        cout << endl;
    }
    cout << endl;
    for (int k = 0; k < prop.getU(); ++k) {
        cout << prop.getXValues()[k];
    }
    cout << endl;

    for (int l = 0; l < prop.getV(); ++l) {
        cout << prop.getYValues()[l];
    }
    cout << endl;
    cout << endl;
}

bool isEqualMatrix(char **A, char **B, int u, int v){
    bool equal = true;
    for (int i = 0; i < u; ++i) {
        for (int j = 0; j < v; ++j) {
            if(A[i][j] != B[i][j]){
                equal = false;
                return equal;
            }
        }
    }
    return equal;
}

void generateAllStates(vector<Properties>&stateArray, Properties parent, Graph &graph){
    int u = parent.getU();
    int v = parent.getV();
    char **tempArray = new char*[u];
    for (int k = 0; k < u; ++k) {
        tempArray[k] = new char[v];
    }

    for (int i = 0; i <u ; ++i) {
        for (int j = 0; j < v ; ++j) {
            matrixCopy(parent.getStateArray(),tempArray,u,v);
            if(tempArray[i][j] == '.'){
                tempArray[i][j] = 'm';
                if(findInStates(stateArray,tempArray) == -1){
                    Properties tempProp(u,v);
                    tempProp.setStateArray(tempArray);
                    tempProp.setXValues(parent.getXValues());
                    tempProp.setYValues(parent.getYValues());
                    stateArray.push_back(tempProp);
                    graph.addEdge(findInStates(stateArray, parent.getStateArray()), findInStates(stateArray, tempArray));
                    if(controlGraphState(tempProp)){
                        cout<<stateArray.size()<<endl;
                        generateAllStates(stateArray,tempProp,graph);
                        cout<<stateArray.size()<<endl;
                    }
                }else{
                    graph.addEdge(findInStates(stateArray, parent.getStateArray()), findInStates(stateArray, tempArray));
                }
            }
        }
    }
}

int findInStates(vector<Properties>stateArray, char **A){
    for (int i = 0; i < stateArray.size(); ++i) {
        if(isEqualMatrix(stateArray[i].getStateArray(),A,stateArray[i].getU(),stateArray[i].getV())){
            return i;
        }
    }
    return -1;
}

bool controlGraphState(Properties prop){
    int u = prop.getU();
    int v = prop.getV();
    int *totalMinerX = new int[u];
    for (int k = 0; k < u ; ++k) {
        totalMinerX[k] = 0;
    }
    for (int l = 0; l < v; ++l) {
        totalMinerX[l] = 0;
    }
    int *totalMinerY = new int[v];
    for (int i = 0; i < u; ++i) {
        for (int j = 0; j < v; ++j) {
            if(prop.getStateArray()[i][j] == 'm'){
                totalMinerX[i]++;
                totalMinerY[i]++;
            }
        }
    }

    for (int m = 0; m < u ; ++m) {
        if(totalMinerX[m] > prop.getXValues()[m])
            return false;
    }
    for (int n = 0; n < v ; ++n) {
        if(totalMinerY[n] > prop.getYValues()[n])
            return false;
    }
    return true;
}

