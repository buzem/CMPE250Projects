/*
Student Name:Berkay Özerbay
Student Number:2016400270
Project Number:2
Operating System:MAC OS-Virtualbox(Linux)
Compile Status:
Program Status:
Notes: Anything you want to say about your code that will be helpful in the grading process.*/
#include <iostream>
#include <algorithm>
#include <fstream>
#include <queue>
#include <iomanip>

using namespace std;

int N;
int M;
int tasknum;
class Task{
public:
    int firstid;
    int whichcpu;
    int whichout;
    double arvl;
    double cwork;
    double owork;
    double cwait;
    double cproc;
    double owait;
    double oproc;
    int stat;
    double Evtime;

};
struct CompareEvtime {
    bool operator()(Task const &t1, Task const &t2) {
        bool result= t1.Evtime > t2.Evtime;
        if(t1.Evtime==t2.Evtime&&t2.stat <t1.stat)
            result=t2.Evtime>t1.Evtime;
        return result;
    };
};


struct CompareCwork {
    bool operator()(Task const & t1, Task const & t2) {

        return t1.cwork > t2.cwork;
    }
};


class CPU{
public:

    double freq;
    double active;
    bool isfull;
    bool ReturnIsFull(){
            return isfull;
    };
    void MakeFull(){
        isfull=true;
    };
    void MakeEmpty(){
        isfull=false;
    };

};
class OUTPUT{
public:
    double quant;
    double active;
    bool isfull;
    bool ReturnIsFull(){
        return isfull;
    };
    void MakeFull(){
        isfull=true;
    };
    void MakeEmpty(){
        isfull=false;
    };
};
double WaitTimeTotal;
double AverageWTime;
double LongestWTime=0;
double TotalTimeSpent;
double AverageSTime;
int MostActiveCpu;
int MostActiveOut;
int main (int argc, char* argv[]) {

    if (argc != 3) {
        cout << "Run the code with the following command: ./project1 [input_file] [output_file]" << endl;
        return 1;
    };

    cout << "input file: " << argv[1] << endl;
    cout << "output file: " << argv[2] << endl;


    ifstream infile;
    infile.open(argv[1]);




    infile >> N;
    CPU cpu[N];

    //input for cpu
    for (int i = 0; i < N; i++) {
        infile >> cpu[i].freq;
        cpu[i].active=0;
        cpu[i].isfull=0;
    };

    infile >> M;
    OUTPUT out[M];
    for (int j = 0; j < M; j++) {
        infile >> out[j].quant;
        out[j].active=0.0;
        out[j].isfull=false;
    };
    Task b;
    infile >> tasknum;

    priority_queue<Task, vector<Task>, CompareCwork> FirstQ;
    priority_queue<Task, vector<Task>, CompareEvtime> EventQ;
    queue<Task> SecQ;
    //input for task
    for (int k = 0; k < tasknum; k++) {
        b.firstid = k ;
        infile >> b.arvl;
        b.Evtime = b.arvl;
        infile >> b.cwork;
        infile >> b.owork;
        b.cproc=0.0;
        b.cwait=0.0;
        b.oproc=0.0;
        b.owait=0.0;
        b.stat=-1;
        EventQ.push(b);


    }

    double temp1;
    Task CurEvent;
    double temp;
    int tempcpunum;
    int tempoutnum;
    double Fmax = 0;
    double Smax = 0;
    bool allfullc ;
    bool allfullo ;
    //Input for Eventqueue
    while (EventQ.size() > 0) {

        allfullc = true;
        allfullo = true;
        CurEvent = EventQ.top();
        EventQ.pop();
//the case for incomming tasks
        if (CurEvent.stat == -1) {

            for (int i = 0; i < N; i++) {
                if (cpu[i].isfull== false) {
                    cpu[i].active += ((CurEvent.cwork) / (cpu[i].freq));
                    CurEvent.Evtime += ((CurEvent.cwork) / (cpu[i].freq));
                    CurEvent.whichcpu = i;
                    allfullc = false;
                    CurEvent.stat = 1;
                    EventQ.push(CurEvent);
                    tempcpunum=i;
                    cpu[i].MakeFull();

                    break;
                };
            };

            cpu[tempcpunum].isfull=true;

            if (allfullc==true) {//if there is no empty cpu,push to firstpqueue
                CurEvent.stat = 0;
                FirstQ.push(CurEvent);
                if (Fmax < FirstQ.size())
                    Fmax = FirstQ.size();
            };


        }

        else if (CurEvent.stat == 1) {//exit from Cpu


            if (FirstQ.size() > 0)
                temp = CurEvent.Evtime - FirstQ.top().arvl;//for wait if FirstQ
            for (int i = 0; i < M; i++) {
                if (out[i].isfull==false) {
                    if (out[i].quant >= CurEvent.owork) {//outputwork<quant Bitecek
                        CurEvent.stat = 4;
                        CurEvent.Evtime += CurEvent.owork;
                        CurEvent.oproc += CurEvent.owork;
                        out[i].active += CurEvent.owork;
                        CurEvent.owork = 0;
                        out[i].MakeFull();

                    }
                    else {//outputwork>quant,second queueya gidebilir
                        CurEvent.stat = 3;
                        out[i].active += out[i].quant;
                        CurEvent.Evtime += out[i].quant;
                        CurEvent.oproc += out[i].quant;
                        CurEvent.owork -= out[i].quant;
                        out[i].MakeFull();
                    }
                    CurEvent.whichout = i;
                    allfullo = false;
                    EventQ.push(CurEvent);
                    break;
                };

            };
            if (allfullo == true) {//Output devicelar dolu second queuya gitti
                CurEvent.stat = 2;
                SecQ.push(CurEvent);
                if (Smax < SecQ.size()){
                    Smax = SecQ.size();
                };

            };


            if (FirstQ.size() > 0 ) {//Firstquedan task alma
                tempcpunum = CurEvent.whichcpu;
                CurEvent = FirstQ.top();
                FirstQ.pop();
                CurEvent.whichcpu = tempcpunum;
                CurEvent.cwait = temp;
                cpu[tempcpunum].active += ((CurEvent.cwork)/(cpu[tempcpunum].freq));
                CurEvent.Evtime = CurEvent.arvl + CurEvent.cwait + ((CurEvent.cwork)/(cpu[tempcpunum].freq));
                CurEvent.stat = 1;
                EventQ.push(CurEvent);//Eventqueueya attı,cpudan çıkacak vakti geldiğinde

            }
            else
                cpu[CurEvent.whichcpu].MakeEmpty();//First Queue boş


        }
    else if(CurEvent.stat==3){//Output devicedan çıktı Sec Queuya girebilir


            tempoutnum=CurEvent.whichout;
            if(SecQ.size()>0) {
            temp=CurEvent.Evtime-SecQ.front().Evtime;//Wait in Sec
            SecQ.push(CurEvent);
            CurEvent=SecQ.front();
            SecQ.pop();
            CurEvent.owait+=temp;
            CurEvent.Evtime+=temp;
            if (out[tempoutnum].quant >= CurEvent.owork) {//SecQnun ilki girdi
                CurEvent.stat = 4;
                temp1=CurEvent.owork;
                CurEvent.Evtime+=temp1;
                out[tempoutnum].active+=temp1;
                CurEvent.oproc+=temp1;
                CurEvent.owork=0;
                out[tempoutnum].MakeFull();

            }
            else {
                CurEvent.stat = 3;
                out[tempoutnum].active += out[tempoutnum].quant;
                CurEvent.Evtime+=out[tempoutnum].quant;
                CurEvent.oproc+=out[tempoutnum].quant;
                CurEvent.owork-=out[tempoutnum].quant;
                out[tempoutnum].MakeFull();
            }
            CurEvent.whichout=tempoutnum;
            EventQ.push(CurEvent);

        }
        else{//SecQ boşsa devam
            if (out[tempoutnum].quant >= CurEvent.owork) {

                CurEvent.stat = 4;
                temp1=CurEvent.owork;
                CurEvent.Evtime+=temp1;
                out[tempoutnum].active+=CurEvent.owork;
                CurEvent.oproc+=CurEvent.owork;
                CurEvent.owork=0;
                out[tempoutnum].MakeFull();

            }

            else {
                CurEvent.stat = 3;
                out[tempoutnum].active += out[tempoutnum].quant;
                CurEvent.Evtime+=out[tempoutnum].quant;
                CurEvent.oproc+=out[tempoutnum].quant;
                CurEvent.owork-=out[tempoutnum].quant;
                out[tempoutnum].MakeFull();
            }
            CurEvent.whichout=tempoutnum;
            EventQ.push(CurEvent);



            }


    }
    else if(CurEvent.stat==4){//Tamamen bitti

        WaitTimeTotal+=(CurEvent.cwait+CurEvent.owait);
        TotalTimeSpent+=CurEvent.Evtime-CurEvent.arvl;
        if(LongestWTime<(CurEvent.cwait+CurEvent.owait))
            LongestWTime=(CurEvent.cwait+CurEvent.owait);

        tempoutnum=CurEvent.whichout;

        if(SecQ.size()>0) {
            temp=CurEvent.Evtime-SecQ.front().Evtime;//Wait in Sec
            CurEvent=SecQ.front();//Secquedan yeni elemanı output device a atıyor
            SecQ.pop();
            CurEvent.owait+=temp;
            CurEvent.Evtime+=temp;
            if (out[tempoutnum].quant >= CurEvent.owork) {

                CurEvent.stat = 4;
                temp1=CurEvent.owork;
                CurEvent.Evtime+=temp1;
                out[tempoutnum].active+=CurEvent.owork;
                CurEvent.oproc+=CurEvent.owork;
                CurEvent.owork=0;
                out[tempoutnum].MakeFull();

            }

            else {
                CurEvent.stat = 3;
                out[tempoutnum].active += out[tempoutnum].quant;
                CurEvent.Evtime+=out[tempoutnum].quant;
                CurEvent.oproc+=out[tempoutnum].quant;
                CurEvent.owork-=out[tempoutnum].quant;
                out[tempoutnum].MakeFull();
            }
            CurEvent.whichout=tempoutnum;
            EventQ.push(CurEvent);

        }
          else{
            out[CurEvent.whichout].MakeEmpty();

            }

        }
    }
//cout<<Fmax<<endl;
//cout<<Smax<<endl;
    int tempmax=0;
    int tempid;
    for(int j=0;j<N;j++){
        if(cpu[j].active>tempmax){
            tempmax=cpu[j].active;
            tempid=j+1;
        }
    }
    MostActiveCpu=tempid;
 //   cout<<MostActiveCpu<<endl;
    tempmax=0;
    for(int k=0;k<M;k++){
        if(out[k].active>tempmax){
            tempmax=out[k].active;
            tempid=k+1;
        }
    }
    MostActiveOut=tempid;
  //  cout<<MostActiveOut<<endl;
    AverageWTime=(WaitTimeTotal/tasknum);
   // cout<<AverageWTime<<endl;
  //  cout<<LongestWTime<<endl;
    AverageSTime=(TotalTimeSpent/tasknum);
 //   cout<<AverageSTime<<endl;

    ofstream myfile;
    myfile.open(argv[2]);
    myfile<<Fmax<<endl;
    myfile<<Smax<<endl;
    myfile<<fixed<<setprecision(6);
    myfile<<MostActiveCpu<<endl;
    myfile<<MostActiveOut<<endl;
    myfile<<AverageWTime<<endl;
    myfile<<LongestWTime<<endl;
    myfile<<AverageSTime<<endl;

    myfile.close();


}





