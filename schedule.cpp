#include"schedule.h"
int Schedule::enq(const jobinfo &jo)
{

	if (readydeq==NULL)
	{
		readydeq = (waitque *)malloc(sizeof(waitque));
		if (!readydeq)
		{
			return -1;
		}
		readydeq->job = jo;
		readydeq->next = NULL;
	}
	else
	{
		waitque *wait_temp = readydeq;
		for (; wait_temp->next != NULL; wait_temp = wait_temp->next);
		waitque *s = (waitque *)malloc(sizeof(waitque));
		if (!s)
		{
			return -1;
		}
		s->job = jo;
		s->next = NULL;
		wait_temp->next = s;
	}
	return 0;
}
bool Schedule::deq(int jobid)
{
	if (readydeq == NULL)//就绪队列为空
	{
		std::cout << "there is no "<<jobid << std::endl;
		return false;
	}
	if (readydeq->job.jid == jobid)//当开头结点是要删除结点时
	{
		if (readydeq->next == NULL)//在只有一个情况下
		{
			free(readydeq);
			readydeq = NULL;
		}
		else//在有多个情况下
		{
			waitque *temp = readydeq;
			readydeq = readydeq->next;
			free(temp);
		}
		return true;
	}
	waitque *p_temp = readydeq;
	for (; p_temp->next && p_temp->next->job.jid != jobid; p_temp = p_temp->next);
	if (p_temp->next == NULL)
	{
		std::cout << "can not find jobid!" << std::endl;
		return false;
	}
	else
	{
		waitque *q_temp = p_temp->next;
		p_temp->next = q_temp->next;
		free(q_temp);
		return true;
	}
}
waitque *Schedule::findMaxPriority()
{
	waitque *p_temp = readydeq, *p_return = readydeq;
	if (p_temp == NULL)
	{
		std::cout << "there is no job!" << std::endl;
		return NULL;
	}
	int maxnum = readydeq->job.curpri;
	for (; p_temp; p_temp = p_temp->next)
	{
		if (p_temp->job.curpri > maxnum)
		{
			maxnum = p_temp->job.curpri;
			p_return = p_temp;
		}
	}
	return p_return;
}
bool Schedule::findOperatingJobIsOver()
{
	if (operatingjob.run_time>=operatingjob.need_time)
		return true;
	else
		return false;
}
void Schedule::ChangeJobState()//就绪队列的进程改变
{
	if (readydeq)
	{
		waitque *p_temp = readydeq;
		for (; p_temp; p_temp = p_temp->next)
		{
			p_temp->job.wait_time += TIMEINCREASE;
			if (p_temp->job.wait_time % 100 == 0)//每过100s则优先级加一
			{
				p_temp->job.curpri += 1;
			}
		}
	}
}
void Schedule::processingSchedule()
{
	if (readydeq == NULL)
	{
		std::cout << "wait queue is null, can not schedule! please firstly add job to queue by enq function!" << std::endl;
		return;
	}
	waitque *p_maxpri = findMaxPriority();
	operatingjob = p_maxpri->job;//开始运行优先级最高的进程
	deq(p_maxpri->job.jid);//把运行的进程从就绪队列删除
	while(readydeq)
	{
		if (findOperatingJobIsOver()|| operatingjob.run_time >= OPERATE_TIME)//进程运行完毕或者时间片到，则把进程从就绪队列中删除
		{
			std::cout << "process: " << operatingjob.jid << " has finished a time circle!";;
			if (operatingjob.run_time >= OPERATE_TIME)//时间片到，把进程放入就绪队列，并更改相关信息
			{
				operatingjob.curpri = 0;//把正在运行的进程优先级设为0
				operatingjob.wait_time = 0;
				operatingjob.need_time=operatingjob.need_time-operatingjob.run_time;
				operatingjob.run_time = 0;
				std::cout << " it will wait next time circle!" <<" it has "<<operatingjob.need_time <<" rest!"<< std::endl;
				std::cout << "*************************************************" << std::endl;
				enq(operatingjob);
			}
			else
			{
				std::cout << " it has finished!" << std::endl;
				std::cout << "*************************************************" << std::endl;
			}
			p_maxpri = findMaxPriority();
			operatingjob = p_maxpri->job;
			deq(p_maxpri->job.jid);//从就绪队列中删除
		}
		Sleep(TIMEINCREASE);
		operatingjob.run_time += TIMEINCREASE;
		ChangeJobState();
		std::cout << "operating process: " << operatingjob.jid <<" up to"<< operatingjob.run_time 
		 << std::endl;
	}
}
