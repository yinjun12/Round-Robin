#pragma once
#include<iostream>
#include<Windows.h>
#include<time.h>
const int OPERATE_TIME = 100;
const int TIMEINCREASE = 20;
struct jobinfo
{
	
	int jid;
	int defpri;
	int curpri;
	int wait_time;
	int run_time;
	int need_time;
	char create_time[16];
	struct jobinfo(int j = 0, int d = 0, int c = 0, int w = 0,
		int ru = 0,int ne=0) :jid(j), defpri(d), curpri(c), wait_time(w)
		, run_time(ru),need_time(ne)
	{
		time_t t = time(0);
		strftime(create_time, sizeof(create_time), "%H:%M:%S", localtime(&t));
	}
};
struct waitque//就绪队列结点
{
	jobinfo job;
	struct waitque *next;
};
class Schedule
{
private:
	jobinfo operatingjob;
	waitque *readydeq;
public:
	Schedule()
	{
		readydeq = NULL;
	}
	int enq(const jobinfo &job);//作业进入就绪队列
	bool deq( int jobid);//作业出队并打印出队信息
	waitque *findMaxPriority();//find max priority and return its pointer
	bool findOperatingJobIsOver();//查看当前运行的进程是否结束
	void ChangeJobState();
	void processingSchedule();//schedule process start
};
