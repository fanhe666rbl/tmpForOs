#include "pintos_thread.h"

struct station {
	// FILL ME IN
	struct lock lo;
	struct condition trainWait;
	struct condition paWait;
	unsigned int seatNum;
	unsigned int paNum;
	unsigned int upLoading;
};

void
station_init(struct station *station)
{
	// FILL ME IN
	lock_init(&station->lo);
	cond_init(&station->paWait);
	cond_init(&station->trainWait);
	station->seatNum = 0;
	station->paNum = 0;
	station->upLoading = 0;
}

void
station_load_train(struct station *station, int count)
{
	// FILL ME IN
	lock_acquire(&station->lo);
	station->seatNum = count;
	station->upLoading = 0;
	cond_broadcast(&station->paWait, &station->lo);
	// printf("??? %d %d %d\n", station->seatNum, station->paNum, station->upLoading);
	while (station->seatNum > 0 && (station->paNum > 0 || station->upLoading > 0))
	{
		cond_wait(&station->trainWait, &station->lo);
	}
	station->upLoading = 0;
	station->seatNum = 0;
	lock_release(&station->lo);
}

void
station_wait_for_train(struct station *station)
{
	// FILL ME IN
	lock_acquire(&station->lo);
	station->paNum++;
	// printf("=== %d %d %d\n", station->seatNum, station->paNum, station->upLoading);
	while (station->seatNum <= 0 || station->upLoading >= station->seatNum)
	{
		cond_wait(&station->paWait, &station->lo);
	}
	station->upLoading++;
	station->paNum--;
	lock_release(&station->lo);
}

void
station_on_board(struct station *station)
{
	// FILL ME IN
	lock_acquire(&station->lo);
	station->seatNum--;
	station->upLoading--;
	// printf("::: %d %d %d\n", station->seatNum, station->paNum, station->upLoading);
	
	cond_signal(&station->trainWait, &station->lo);
	
	lock_release(&station->lo);
}
