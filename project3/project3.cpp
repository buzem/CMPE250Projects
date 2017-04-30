/*
Student Name:Berkay Özerbay
Student Number:2016400270
Project Number:3
Operating System:MAC OS- Virtual Box(Linux)
Compile Status:OK
Program Status:OK-working
Notes: Anything you want to say about your code that will be helpful in the grading process.

*/
#include <iostream>
#include <algorithm>
#include <fstream>
#include <queue>
#include <iomanip>

using namespace std;



struct Node{

    double time;
    int ID;
    vector<double*> PrereqTimes;//The times of vertices which are prerequisite of this vertice
    int indegree;


};

int main (int argc, char* argv[]) {
    int V, E;
    double mintime=0;

    if (argc != 3) {
        cout << "Run the code with the following command: ./project1 [input_file] [output_file]" << endl;
        return 1;
    }

    cout << "input file: " << argv[1] << endl;
    cout << "output file: " << argv[2] << endl;


    ifstream infile;
    infile.open(argv[1]);

    infile >> V;
    infile>>E;
    vector<vector<int>> graph(V);
    Node nodes[V];

    for(int i=0; i<V;i++){//reading times of vertices
        infile>>nodes[i].time;
        nodes[i].ID=i;
    }




    for(int i=0; i<V; i++){
        nodes[i].indegree=0;


    }

    int from, to;


    double* tempPtr=NULL;
    for(int i=0; i<E; i++) {//reading the edges from file
        infile >> from >> to;
        nodes[to].indegree++;
        tempPtr=&nodes[from].time;
        nodes[to].PrereqTimes.push_back(tempPtr);
        graph[from].push_back(to);
        tempPtr=new double;
    }
    infile.close();

    queue<int> zero_list;

    for(int i=0; i<V; i++){//push source nodes to zero_list
        if(nodes[i].indegree==0){
            zero_list.push(i);
        }
    }


    Node* CurNode1;
    Node* CurNode2;
    int CurID1;
    int cnt = 0;
    double maxTimePreq=0;
    while(!zero_list.empty()){
        CurID1 = zero_list.front();
        CurNode1=&nodes[CurID1];
        zero_list.pop();
        cnt++;
        for(int i=0; i<graph[CurID1].size(); i++){
            CurNode2=&nodes[graph[CurID1][i]];
            CurNode2->indegree--;
            if(CurNode2->indegree==0){
//if indegree of one of nodes ,which Curnode1 points to, becomes zero
                for(int j=0;j<CurNode2->PrereqTimes.size();++j){
                    if(maxTimePreq<*CurNode2->PrereqTimes[j]){
                        maxTimePreq=*CurNode2->PrereqTimes[j];
                        //Finding max time of prerequisite of CurNode2
                    }//in order to add it to its time

                }
                CurNode2->time+=maxTimePreq;
                //update the current time for completing the Curnode2
                maxTimePreq=0;
                zero_list.push(CurNode2->ID);

            }


        }

    }
    if(cnt<V){//if it is not a DAG
        mintime=-1;
    }
    else
    {//finding mintime
        for (int k = 0; k < V; k++) {
            if (graph[k].size() == 0) {
                if (nodes[k].time > mintime)
                    mintime = nodes[k].time;
            }
        }



    }
    ofstream myfile;
    myfile.open(argv[2]);
    myfile<<fixed<<setprecision(6);
    myfile<<mintime<<endl;




}