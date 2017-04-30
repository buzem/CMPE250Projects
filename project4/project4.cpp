/*
Student Name:Berkay Özerbay
Student Number:2016400270
Project Number:4
Operating System:Virtual Box(Linux)-Mac 0S
Compile Status:Succesful
Program Status:
Notes: Anything you want to say about your code that will be helpful in the grading process.

*/
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>

using namespace std;
struct Edge {
    int n1;
    int n2;
    int w;
    Edge(int n1_, int n2_, int w_) {//constructor
        n1 = n1_;
        n2 = n2_;
        w = w_;
    }
};
vector<Edge> edges;

class DisjointSet{
public:
    int SIZE;
    int* arr;
    DisjointSet(int size){
        SIZE = size;
        arr = new int[SIZE];
        // Make each node a root.
        for (int i = 0; i < SIZE; ++i) {
            arr[i] = -1;
        }
    }
    int find(int set){
        if(arr[set] < 0){
            // Means this is a root
            return set;
        }else{
            // Call find with its root.
            return find(arr[set]);
        }
    }
    void Union(int set1,int set2){
        set1 = find(set1);
        set2 = find(set2);
        if(arr[set1] > arr[set2]){
            // First one is less deep, so make its root second.
            //If one of them is the vertex which we search, make it root
            arr[set1] = set2;
        }
        else{
            // They are equal in depth. Set first one as second one's root. (Arbitrarily chosen)
            arr[set2] = set1;
        }
    }
};


bool operator<(const Edge& e1, const Edge& e2) {
    return e1.w > e2.w;//for max to min sort
}


int main (int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Run the code with the following command: ./project1 [input_file] [output_file]" << endl;
        return 1;
    }

    cout << "input file: " << argv[1] << endl;
    cout << "output file: " << argv[2] << endl;

    ifstream infile(argv[1]);

    int N;
    infile>>N;
    int D;
    infile>>D;
    int j,k,dist;
    for(int i=1;i<N;i++){
        infile>>j;
        infile>>k;
        infile>>dist;
        edges.push_back(Edge(j,k,dist));
    }
    int t;
    DisjointSet ds(N);
    for(int i=0;i<D;i++){
        infile>>t;
        ds.arr[t]=-INT32_MAX;
        //the arr value of the vertices which their should not be connected
        //should be -INTMAX,so they will be the root of the other vertices after union
    }

    sort(edges.begin(), edges.end());
    long long resmin=0;
    int c=0;
    int l=0;


    while (c < D-1) {// process until finding all the min distances between the given vertices
        int root1 = ds.find(edges[l].n1);
        int root2 = ds.find(edges[l].n2);
        if (root1 != root2 ){
            if(ds.arr[root1]==-INT32_MAX && ds.arr[root2]==-INT32_MAX) {// if the roots of both tree is the given vertice

                resmin+=edges[l].w; //we find the min distance so add to it the result
                c++;

            }
            else{
            ds.Union(root1,root2);//else make one of them the root of other
            }
            ;

        }
        l++;
    }

   // cout<<resmin<<endl;
    ofstream myfile;
    myfile.open(argv[2]);
    myfile<<resmin<<endl;

    return 0;

}