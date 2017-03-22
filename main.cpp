#include"schedule.h"
int main()
{
	Schedule sc;
	jobinfo jo1(1, 1, 1, 0, 0, 40);
	jobinfo jo2(2, 3, 3, 0, 0, 80);
	jobinfo jo3(3, 2, 2, 0, 0, 120);
	sc.enq(jo2);
	sc.enq(jo1);
	sc.enq(jo3);
	sc.processingSchedule();
	return 0;
}
