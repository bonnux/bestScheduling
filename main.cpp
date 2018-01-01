#include <iostream>
#include <random>
#include <iomanip>
#define MAXSIZE 100           //最大任务数
using namespace std;

int N,K;                    //N：任务数，K:机器数
int taskTime[MAXSIZE];      //任务对应的时间
int bestTime=65536;         //最优解：即完成全部任务最短时间
int scheduleNow[MAXSIZE];   //当前最优调度方案
int bestSchedule[MAXSIZE];   //最优解的调度方案

//界面显示并接收N、K的值
void Display()
{
    cout<<"任务数: ";
    cin>>N;
    cout<<"机器数: ";
    cin>>K;
}

//随机产生完成任务i需要的时间
void setTaskTime(int taskTime[],int N)
{
    srand(unsigned(time(NULL)));
    for(int i=0;i<N;i++)
        taskTime[i]=int((rand()%20)+3);   //时间范围：[3,23)之间的整数
}

//打印原始任务时间对应表
void printTaskTime(int taskTime[],int N)
{
    cout<<"\n各任务完成所需时间为："<<endl;
    for(int i=0;i<N;i++)
        cout<<"任务"<<i+1<<" : "<<setw(2)<<taskTime[i]<<endl;
}

//搜索到叶节点时，计算叶节点可行解完成任务的时间
int ScheduleTime()
{
    int t[MAXSIZE]={0};    //记录每个机器对应工作的总时间,机器下标
    //从1开始算，因为scheduleNow[i]=0时表示未分配
    for(int i=0;i<N;i++)
    {
        t[scheduleNow[i]]+=taskTime[i]; //将第i个任务时间加到第‘scheduleNow[i]’个机器中去
    }

    int max=t[1];
    for(int i=1;i<=K;i++)
    {
        if(t[i]>max)
            max=t[i]; //并行工作的所有机器中工作时间最长的那个时间就是叶子节点可行解时间
    }
    return max;
}

//回溯法
void BackTrack(int deep)
{
    if (deep==N)
    {
        int temp=ScheduleTime();    //临时存放叶节点的可行解的值
        if(temp<bestTime)           //可行解与当前最优解进行比较
        {
            bestTime=temp;
            for(int i=0;i<N;i++)
            {
                bestSchedule[i]=scheduleNow[i];
            }
        }
        return;
    }

    for(int i=1;i<=K;i++)
    {
        scheduleNow[deep]=i;         //将任务deep分配给当前机器
        if(ScheduleTime()<bestTime)  //剪枝，约束条件
            BackTrack(deep+1);
        scheduleNow[deep]=0;         //回溯，不将任务分配给当前机器，i=0表示为分配
    }
}

//打印最终可行解结果
void printSchedulePlan()
{
    cout<<endl<<"各台机器完成的任务： \n";
    for(int i=1;i<=K;i++)
    {
        bool hasTask=false;    //hasTask用于记录机器是否有分配，若一个任务都没分配，则显示“未分配任务”
        cout<<"机器"<<i<<"： ";
        for(int j=0;j<N;j++)
        {
            if(i==bestSchedule[j])
            {
                cout<<"任务"<<j+1<<" ";
                hasTask=true;
            }
        }
        if(hasTask==false)cout<<"未分配任务！";
        cout<<endl;
    }
}

int main()
{
    Display();                   //界面显示并接收N、K
    setTaskTime(taskTime,N);     //随机产生完成任务i需要的时间
    printTaskTime(taskTime,N);   //打印原始任务时间对应表
    BackTrack(0);                //排列树回溯法（含剪枝）
    printSchedulePlan();         //打印最终可行解结果
}