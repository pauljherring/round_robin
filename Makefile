CFLAGS=-Wall

.PHONEY: all clean
all: generate_schedule generate_berger

clean:
	rm -f generate_schedule generate_berger

generate_schedule: generate_schedule.c
	cc     generate_schedule.c   -g -o generate_schedule

generate_berger: generate_berger_tables.c
	cc     generate_berger_tables.c   -g -o generate_berger_tables