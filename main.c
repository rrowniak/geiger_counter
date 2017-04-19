/*
 * main.c
 *
 * Created: 2/11/2017 12:23:11 PM
 * Author : rr
 */ 

#define F_CPU (1000000)

#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "lcd.h"

void enable_cnt_int();
void enable_timer();
void reset();
void select();
void print();

#define OVERFLOW_MAX 49200

#define DOWNCOUNTER_MAX 30

volatile uint16_t counter = 0;
volatile uint8_t down_counter = DOWNCOUNTER_MAX;
volatile uint16_t cps = 0;
volatile uint16_t cps_counter = 0;
volatile uint16_t uSvh = 0;
volatile uint8_t overflow = 0;

char line0[17] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'u', 'S', 'v', '/', 'h', ' ', '!', '!', '!', '\0'};
char line1[17] = {'I', 'n', 't', '=', '3', '0', ' ', ' ', 'c', 'p', 's', '=', '9', '9', '9', '9', '\0'};

int main(void)
{
	uint8_t cnt = 0;
	uint8_t redraw = 0;
	
	cli();
	
	// activate pull up resistors
	PORTD = _BV(PIND2) | _BV(PIND1);
	
	enable_cnt_int();
	enable_timer();
	sei();
	
	lcd_init();
	lcd_on();
	lcd_disable_cursor();
	lcd_puts("Initializing...");
	
	for (;;) {
		
		if (!(PIND & _BV(PIND1))) {
			redraw = 1;
			reset();
		}
		
		if (cnt == 20 || redraw) {
			print();
			redraw = 0;
		}
		
		_delay_ms(50);
		
		cnt++;
		if (cnt > 20) {
			cnt = 0;
		}
	}
}

void enable_cnt_int()
{
	// enable interrupt 0 on pin D2
	PCMSK |= _BV(PIND2);
	// interrupt 0 on failing edge
	MCUCR = _BV(ISC01);
	// enable interrupt 0
	GIMSK |= _BV(INT0);
}

ISR(INT0_vect)
{ 
	counter++;
	cps_counter++;
}

void enable_timer()
{
	// prescaler 1024 and CTC
	TCCR1B = _BV(CS12) | _BV(CS10) | _BV(WGM12);
	OCR1A = 977;
	// enable interrupts for that timer
	TIMSK = _BV(OCIE1A);
}

ISR(TIMER1_COMPA_vect)
{	
	down_counter--;
	
	cps = cps_counter;
	cps_counter = 0;
	
	if (down_counter > DOWNCOUNTER_MAX) {
		down_counter = DOWNCOUNTER_MAX;
		if (counter >= OVERFLOW_MAX) {
			counter = OVERFLOW_MAX;
			overflow = 1;
		} else {
			overflow = 0;
		}
		uSvh = (counter / 100) * 23 + counter;
		counter = 0;
	}
}

void reset()
{
	counter = 0;
	down_counter = DOWNCOUNTER_MAX;
}

void put_number(const char* num, char* dst, uint8_t size)
{
	char* it = dst;
	while (*num != '\0') {
		*it++ = *num++;
	}
	
	while(it < dst + size) {
		*it++ = ' ';
	}
}

uint8_t short_len(const char* str, uint8_t max_size)
{
	uint8_t ret = 0;
	while (*str++ != '\0' && ret < max_size) {
		++ret;
	}
	return ret;
}

void put_fix_pos_num(const char* num, char* dst, uint8_t size, uint8_t decimal_src, uint8_t decimal_dst)
{
	// copy num to dst
	uint8_t dec_diff = decimal_src - decimal_dst;
	const char* nit = num + short_len(num, size) - dec_diff - 1;
	char* rit = dst + size;
	
	// copy decimal
	while (nit >= num && dec_diff) {
		*rit-- = *nit--;
		--dec_diff;
	}
	
	// put decimal point
	*rit-- = '.';
	
	// copy the rest
	while (nit >= num) {
		*rit-- = *nit--;
	}
	
	// anything copied?
	if (*(rit + 1) == '.') {
		*rit-- = '0';
	}
	
	// fill leading space with spaces
	while (rit >= dst) {
		*rit-- = ' ';
	}
}

void print()
{
	uint16_t tmp;
	static char num[6];
	
	// value
	cli();
	tmp = uSvh;
	sei();

	utoa(tmp, num, 10);
	put_fix_pos_num(num, line0, 5, 2, 1);
	
	// level
	if (overflow) {
		strncpy(line0 + 13, "!OL", 3);
	} else {
		if (tmp < 500) {
			strncpy(line0 + 13, "low", 3);
		} else if (tmp < 5000) {
			strncpy(line0 + 13, "__!", 3);
		} else if (tmp < 50000) {
			strncpy(line0 + 13, "_!!", 3);
		}
	}
	
	// down-counter
	utoa(down_counter, num, 10);
	put_number(num, line1 + 4, 4);
	
	// cps
	cli();
	tmp = cps;
	sei();
	utoa(tmp, num, 10);
	put_number(num, line1 + 12, 4);
	
	//lcd_clear();
	lcd_set_cursor(0, 0);
	lcd_puts(line0);
	
	lcd_set_cursor(0, 1);
	lcd_puts(line1);
}