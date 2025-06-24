CFLAGS=-Wall

generate_schedule: generate_schedule.c
	cc     generate_schedule.c   -g -o generate_schedule
