#include "pintos_thread.h"

struct station {
	// FILL ME IN
	struct lock lo;
	struct condition trainWait;
	struct condition paWait;
	unsigned int seatNum;
};

void
station_init(struct station *station)
{
	// FILL ME IN
	lock_init(&station->lo);
	cond_init(&station->paWait);
	cond_init(&station->trainWait);
	station->seatNum = 0;
}

void
station_load_train(struct station *station, int count)
{
	// FILL ME IN
	lock_acquire(&station->lo);
	station->seatNum = count;
	cond_broadcast(&station->paWait, &station->lo);
	while (station->seatNum != 0)
	{
		cond_wait(&station->trainWait, &station->lo);
	}
	lock_release(&station->lo);
}

void
station_wait_for_train(struct station *station)
{
	// FILL ME IN
	lock_acquire(&station->lo);
	while (station->seatNum==0)
	{
		cond_wait(&station->paWait, &station->lo);
	}
	lock_release(&station->lo);
}

void
station_on_board(struct station *station)
{
	// FILL ME IN
	station->seatNum--;
	if (station->seatNum == 0)
	{
		cond_signal(&station->trainWait, &station->lo);
	}
}
