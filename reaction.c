#include "pintos_thread.h"

// Forward declaration. This function is implemented in reaction-runner.c,
// but you needn't care what it does. Just be sure it's called when
// appropriate within reaction_o()/reaction_h().
void make_water();

struct reaction {
	// FILL ME IN
	struct lock lo;
	struct condition o;
	struct condition h;
	unsigned int h_needed;
	unsigned int h_unused;
};

void
reaction_init(struct reaction *reaction)
{
	// FILL ME IN
	lock_init(&reaction->lo);
	cond_init(&reaction->h);
	cond_init(&reaction->o);
	reaction->h_needed = 0;
	reaction->h_unused = 0;
}

void
reaction_h(struct reaction *reaction)
{
	// FILL ME IN
	lock_acquire(&reaction->lo);
	reaction->h_unused++;
	cond_signal(&reaction->o, &reaction->lo);
	while (reaction->h_needed == 0)
	{
		cond_wait(&reaction->h, &reaction->lo);
	}
	reaction->h_needed--;
	lock_release(&reaction->lo);
}

void
reaction_o(struct reaction *reaction)
{
	// FILL ME IN
	lock_acquire(&reaction->lo);
	// cond_signal(&reaction->h, &reaction->lo);
	while (reaction->h_unused<2)
	{
		cond_wait(&reaction->o, &reaction->lo);
	}
	reaction->h_needed+=2;
	cond_broadcast(&reaction->h, &reaction->lo);
	make_water();
	reaction->h_unused-=2;
	
	lock_release(&reaction->lo);
}
