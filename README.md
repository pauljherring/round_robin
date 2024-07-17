# Round Robin

Generate 'balanced' round-robin schedules for anything from 10 to 20 teams.

## What's a round robin?

Given a set of `X` teams, a round robin is when each team plays every other team once, and once only.

So, on the first fixture date, there will be `X÷2` (rounded down if an odd number of teams - one team won't play that date) games.

Presuming one fixture a week, this will take `X-1` weeks (teams don't 'play themselves')

Some tournaments have each team play every other team twice - `Team A` v `Team B` (where they play at `Team A`'s venue) then `Team B` v `Team A` (where they play at `B`'s venue)

This is normally achieved by having two round robins, one after the other, with the same teams playing each other each fixture, with the venue swapped.

## Compiling

`make`

This produces `generate_schedule`

## Use

`generate_schedule` can produce output in two formats:

- "Human" form - produces a long-form output, names the teams in full, with fixture numbers, dates, home and away team and venue:

```text
$ ./generate_schedule 2021_2022_20_once_final.txt `
Division - 20-league
Start time - 20:00
20 team:
1 - Birtley A[BRTA](Birtley A)
2 - Birtley B[BRTB](Birtley B)
3 - Top Bay Horse[TBH](Top Bay Horse)
[...]
Using matrix for 20 teams
1, [02/09/2021] Birtley A(1) v Birtley B(2) @ Birtley A
1, [02/09/2021] Travellers Rest(4) v Brandling A(19) @ Travellers Rest
1, [02/09/2021] Saltwell Club(6) v Mallard A(17) @ Saltwell Club
1, [02/09/2021] Sky Lounge(8) v Pelaw Inn A(15) @ Sky Lounge
[...]
2, [09/09/2021] Birtley B(2) v Travellers Rest(4) @ Birtley B
2, [09/09/2021] Top Bay Horse(3) v Birtley A(1) @ Top Bay Horse
2, [09/09/2021] Corporation Club(5) v Brandling B(20) @ Corporation Club
[...]
19, [03/02/2022] Pelaw Inn A(15) v Saltwell Club(6) @ Pelaw Inn A
19, [03/02/2022] Mallard A(17) v Travellers Rest(4) @ Mallard A
19, [03/02/2022] Brandling A(19) v Birtley B(2) @ Brandling A
```

- "League Republic" form - suitable for creating a `.csv` file that can be uploaded to [League Republic](https://www.leaguerepublic.com) if you use that for admin of a league, and don't want to rely on their own scheduling algorithms (the main reason why this program even exists.)

```text
$ ./generate_schedule 2021_2022_20_once_final.txt -l | tee output.csv
Date dd/mm/yyyy,Time HH:MM,Division,Home Team,Away Team,Venue,Pitch,Home Score,Away Score
02/09/2021,20:00,,BRTA,BRTB,,,,
02/09/2021,20:00,,TR,BRNA,,,,
02/09/2021,20:00,,STL,MALA,,,,
02/09/2021,20:00,,SKY,PEL,,,,
[...]
09/09/2021,20:00,,BRTB,TR,,,,
09/09/2021,20:00,,TBH,BRTA,,,,
09/09/2021,20:00,,COR,BRNB,,,,
[...]
03/02/2022,20:00,,PEL,STL,,,,
03/02/2022,20:00,,MALA,TR,,,,
03/02/2022,20:00,,BRNA,BRTB,,,,
```

### Options

```bash
  -m, --human           output in human-readable format
  -l, --league_republic output in csv format for LR
  -h, --help            this help
```

# Input file format

```text
# Header

# Description of the division. Not currently used in anything but the human readable output
Division: <name of division>

# When matches would normally start. Used in the League Republic output to populate that
Time: <time for matches, default 20:00>

# List of teams
# The number on the left, before the colon, must be unique in this file, but
# numbers need not be sequential. It's suggested that you start at one and go from
# there
# <nth team name> is a human readable name, which is output in that particular mode
# <venue name> optional; the name of the venue if vastly different from the team name
# <short code> is the `Short Code` for the team in League Republic and MUST be present before upload
1: <first team name>{|<venue name>}{%<short code>}
2: <second team name>{|<venue name>}{%<short code>}
3: <...>
...

# When fixtures happen. Lines must start with `week` (case insensitive) followed by a
# monotonically incrementing number.
#
# If you wish two sets of fixtures on the same date, use that same date on two consecutive
# 'weeks'
#
# Hopefully obviously, if you want calendar weeks off, simply omit them and use WeekN+1
# for the next calendar match. For (real) example, 21/22 Christmas with our matches on Thursdays:
# Week14:09/12/2021
# Week15:16/12/2021
#  <xmas/new year here>
# Week16:06/01/2022 # NOT week 18.
# Week17:20/01/2022
#
# Not monotonically incrementing the Week# number will result in fixture sets for those missing
# being omitted.
Week1:dd/mm/yyy
Week2:dd/mm/yyy
Week3:dd/mm/yyy
...
Week19:dd/mm/yyy
```

Here `<short code>` is only *mandatory* if using this to produce a csv for League Republic, and *MUST* be the `Short Code` for the team concerned:

!['Short Code' on LR](doc/short_code.png?raw=true "Short Code")

An example input file can be found at [gdpl/2021_2022_20_once.txt](gdpl/2021_2022_20_once.txt), a snippet of which is presented here:

```text
Division:20-league
# Time: <time for matches, default 20:00>
1:Birtley A%BRTA
2:Birtley B%BRTB
3:Top Bay Horse%TBH
<snip>


Week1:02/09/2021
Week2:09/09/2021
<snip>
Week18:27/01/2022
Week19:03/02/2022
```
