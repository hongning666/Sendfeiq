#include "ackObj.h"

int main()
{
	ackObj_init();
	ackObj_create();
	ackObj_handle("1:100:T.T:PC:32:hello!nihao");
	ackObj_release();
	return 0;
}
