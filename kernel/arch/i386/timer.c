#include <kernel/timer.h>

/* Ticks since system start */
long long int timer_ticks = 0;

void handle_timer(){
	timer_ticks++;
}
