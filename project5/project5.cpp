/*
Student Name:Berkay Özerbay
Student Number:2016400270
Project Number:5
Operating System:MAC OS-Virtual Box(Ubuntu)
Compile Status:OK
Program Status:Testcase 3 error
Notes: Anything you want to say about your code that will be helpful in the grading process.

*/
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstdio>
#include <vector>
#include <queue>
using namespace std;



struct Node{

    int f=0;
    int id;
    int real=0;
    int heuro=0;
    int visited=0;
};
struct CompareFValue {
    bool operator()(const Node *t1, const Node *t2) {
        return t1->f > t2->f;
    };
};

struct GRAPH {

    int size;
    int **adj;

    GRAPH(int n) {
        this->size = n;
        adj = new int *[n];
        for (int i = 0; i < n; i++) {
            adj[i] = new int[n];
            for (int j = 0; j < n; j++) {
                adj[i][j] = 0;
            }
        }

    }
};


int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "Run the code with the following command: ./project1 [input_file] [output_file]" << endl;
        return 1;
    }

    cout << "input file: " << argv[1] << endl;
    cout << "output file: " << argv[2] << endl;

    ifstream infile(argv[1]);
    int k, l, dist, source, target;
    int N;
    infile >> N;
    int E;
    infile >> E;
    GRAPH graph(N);
    Node *nodes = new Node[N];

    for (int i = 0; i < N; i++) {
        infile >> nodes[i].heuro;
        nodes[i].id = i;
        nodes[i].visited = 0;
        nodes[i].real = 20000000;
        nodes[i].f = nodes[i].real + nodes[i].heuro;

    }
    for (int i = 0; i < E; i++) {
        infile >> k;
        infile >> l;
        infile >> dist;
        graph.adj[k][l] = dist;
        graph.adj[l][k] = dist;

    }

    infile >> source >> target;



    int Curid;

    priority_queue<Node *, vector<Node *>, CompareFValue> pq;
    nodes[source].real = 0;
    nodes[source].f = nodes[source].heuro;
    Node*T = &nodes[source];
    pq.push(T);




    int c=0;
    int m=0;

    while (pq.size() > 0) {
        if (pq.top()->visited == 2) {
            pq.pop();
        } else {


            T = pq.top();
            pq.pop();
            T->visited = 2;
            Curid = T->id;


            for (int i = 0; i < N; i++) {
                if (graph.adj[Curid][i] != 0) {
                    if (nodes[i].visited == 0) {
                        nodes[i].real = nodes[Curid].real + graph.adj[Curid][i];
                        nodes[i].f = nodes[i].real + nodes[i].heuro;
                        nodes[i].visited = 1;
                        pq.push(&nodes[i]);


                    } else {
                        if (nodes[i].real > nodes[Curid].real + graph.adj[Curid][i]) {
                            nodes[i].real = nodes[Curid].real + graph.adj[Curid][i];
                            nodes[i].f = nodes[i].real + nodes[i].heuro;
                            nodes[i].visited = 1;
                            pq.push(&nodes[i]);



                        }
                    }



                }

            }
        }
    }
    cout << nodes[target].real;
    ofstream myfile;
    myfile.open(argv[2]);
    myfile<<nodes[target].real<<endl;



}

