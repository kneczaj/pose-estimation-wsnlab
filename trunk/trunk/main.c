// Main oscillator frequency - needed for delay.h
#define F_CPU 7372800

#include "nrk_cfg.h"

#include <nrk.h>
#include <hal.h>
#include <util/delay.h>

int main ()
{
  nrk_setup_ports();

  while(1)
  {
	nrk_led_toggle(RED_LED);
	nrk_led_toggle(GREEN_LED);
	nrk_led_toggle(YELLOW_LED);
	_delay_ms(500);
  }
  return 0;
}
