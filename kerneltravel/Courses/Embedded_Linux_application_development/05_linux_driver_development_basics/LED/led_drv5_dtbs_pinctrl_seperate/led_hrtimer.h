#include <linux/timer.h>

#include "led_operations.h"

#define MIN_INTERVAL 100
#define TIMES_INTERVAL(n) (ms_to_ktime(MIN_INTERVAL * n))


struct led_hrtimer {
	struct hrtimer on_timer;
	struct hrtimer off_timer;
	struct led_operations* p_led_opr;

	int led_counter;
	int led_step;
};

static enum hrtimer_restart led_off_timer_cb(struct hrtimer *hrtimer)
{
	struct led_hrtimer *led = container_of(hrtimer, struct led_hrtimer, off_timer);
	enum hrtimer_restart ret = HRTIMER_NORESTART;

	if (led->p_led_opr)
		led->p_led_opr->set_value(0);

	if (led->led_counter == 1 || led->led_counter == 10)
		led->led_step *= -1;

	led->led_counter += led->led_step;

	hrtimer_start(&led->on_timer, TIMES_INTERVAL(led->led_counter), HRTIMER_MODE_REL);
	return ret;
}

static enum hrtimer_restart led_on_timer_cb(struct hrtimer *hrtimer)
{
	struct led_hrtimer *led = container_of(hrtimer, struct led_hrtimer, on_timer);
	enum hrtimer_restart ret = HRTIMER_NORESTART;

	if (led->p_led_opr)
		led->p_led_opr->set_value(1);

	hrtimer_start(&led->off_timer, ms_to_ktime(100), HRTIMER_MODE_REL);
	return ret;
}

void led_hrtimer_init(struct led_hrtimer *led, struct led_operations *p_led_opr)
{
	led->p_led_opr = p_led_opr;
	led->led_counter = 1;
	led->led_step = -1;

	hrtimer_init(&led->on_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	led->on_timer.function = led_on_timer_cb;

	hrtimer_init(&led->off_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	led->off_timer.function = led_off_timer_cb;
}

void led_hrtimer_start(struct led_hrtimer *led)
{
	if (hrtimer_is_queued(&led->on_timer))
		return;

	hrtimer_start_range_ns(&led->on_timer, TIMES_INTERVAL(30), 0, HRTIMER_MODE_REL);
}

void led_hrtimer_stop(struct led_hrtimer *led)
{
	if (hrtimer_active(&led->on_timer))
		hrtimer_cancel(&led->on_timer);

	if (hrtimer_active(&led->off_timer))
		hrtimer_cancel(&led->off_timer);
}

