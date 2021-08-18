#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>

#define MAX_TEAMS 20
#define MAX_WEEKS 52

const char* header= "Date dd/mm/yyyy,Time HH:MM,Division,Home Team,Away Team,Venue,Pitch,Home Score,Away Score";

char division[PATH_MAX];
char starttime[PATH_MAX] = "20:00";
char teams[MAX_TEAMS][PATH_MAX];
char venues[MAX_TEAMS][PATH_MAX];
int lastteam = 0;
char weeks[MAX_WEEKS][PATH_MAX];
int lastweek = 0;

int round_robin_20[20][19] = {
    {   2,  -3,   4,  -5,   6,  -7,   8,  -9,  10, -11,  12, -13,  14, -15,  16, -17,  18, -19,  20},
    {  -1,   4,  -6,   8, -10,  12, -14,  16, -18,  20,  -3,   5,  -7,   9, -11,  13, -15,  17, -19},
    { -20,   1,   5,  -7,   9, -11,  13, -15,  17, -19,   2,  -4,   6,  -8,  10, -12,  14, -16,  18},
    {  19,  -2,  -1,   6,  -8,  10, -12,  14, -16,  18, -20,   3,  -5,   7,  -9,  11, -13,  15, -17},
    { -18,  20,  -3,   1,   7,  -9,  11, -13,  15, -17,  19,  -2,   4,  -6,   8, -10,  12, -14,  16},
    {  17, -19,   2,  -4,  -1,   8, -10,  12, -14,  16, -18,  20,  -3,   5,  -7,   9, -11,  13, -15},
    { -16,  18, -20,   3,  -5,   1,   9, -11,  13, -15,  17, -19,   2,  -4,   6,  -8,  10, -12,  14},
    {  15, -17,  19,  -2,   4,  -6,  -1,  10, -12,  14, -16,  18, -20,   3,  -5,   7,  -9,  11, -13},
    { -14,  16, -18,  20,  -3,   5,  -7,   1,  11, -13,  15, -17,  19,  -2,   4,  -6,   8, -10,  12},
    {  13, -15,  17, -19,   2,  -4,   6,  -8,  -1,  12, -14,  16, -18,  20,  -3,   5,  -7,   9, -11},
    { -12,  14, -16,  18, -20,   3,  -5,   7,  -9,   1,  13, -15,  17, -19,   2,  -4,   6,  -8,  10},
    {  11, -13,  15, -17,  19,  -2,   4,  -6,   8, -10,  -1,  14, -16,  18, -20,   3,  -5,   7,  -9},
    { -10,  12, -14,  16, -18,  20,  -3,   5,  -7,   9, -11,   1,  15, -17,  19,  -2,   4,  -6,   8},
    {   9, -11,  13, -15,  17, -19,   2,  -4,   6,  -8,  10, -12,  -1,  16, -18,  20,  -3,   5,  -7},
    {  -8,  10, -12,  14, -16,  18, -20,   3,  -5,   7,  -9,  11, -13,   1,  17, -19,   2,  -4,   6},
    {   7,  -9,  11, -13,  15, -17,  19,  -2,   4,  -6,   8, -10,  12, -14,  -1,  18, -20,   3,  -5},
    {  -6,   8, -10,  12, -14,  16, -18,  20,  -3,   5,  -7,   9, -11,  13, -15,   1,  19,  -2,   4},
    {   5,  -7,   9, -11,  13, -15,  17, -19,   2,  -4,   6,  -8,  10, -12,  14, -16,  -1,  20,  -3},
    {  -4,   6,  -8,  10, -12,  14, -16,  18, -20,   3,  -5,   7,  -9,  11, -13,  15, -17,   1,   2},
    {   3,  -5,   7,  -9,  11, -13,  15, -17,  19,  -2,   4,  -6,   8, -10,  12, -14,  16, -18,  -1},
};

int round_robin_18[18][17] = {
    {   2,  -3,   4,  -5,   6,  -7,   8,  -9,  10, -11,  12, -13,  14, -15,  16, -17,  18},
    {  -1,   4,  -6,   8, -10,  12, -14,  16, -18,   3,  -5,   7,  -9,  11, -13,  15, -17},
    { -18,   1,   5,  -7,   9, -11,  13, -15,  17,  -2,   4,  -6,   8, -10,  12, -14,  16},
    {  17,  -2,  -1,   6,  -8,  10, -12,  14, -16,  18,  -3,   5,  -7,   9, -11,  13, -15},
    { -16,  18,  -3,   1,   7,  -9,  11, -13,  15, -17,   2,  -4,   6,  -8,  10, -12,  14},
    {  15, -17,   2,  -4,  -1,   8, -10,  12, -14,  16, -18,   3,  -5,   7,  -9,  11, -13},
    { -14,  16, -18,   3,  -5,   1,   9, -11,  13, -15,  17,  -2,   4,  -6,   8, -10,  12},
    {  13, -15,  17,  -2,   4,  -6,  -1,  10, -12,  14, -16,  18,  -3,   5,  -7,   9, -11},
    { -12,  14, -16,  18,  -3,   5,  -7,   1,  11, -13,  15, -17,   2,  -4,   6,  -8,  10},
    {  11, -13,  15, -17,   2,  -4,   6,  -8,  -1,  12, -14,  16, -18,   3,  -5,   7,  -9},
    { -10,  12, -14,  16, -18,   3,  -5,   7,  -9,   1,  13, -15,  17,  -2,   4,  -6,   8},
    {   9, -11,  13, -15,  17,  -2,   4,  -6,   8, -10,  -1,  14, -16,  18,  -3,   5,  -7},
    {  -8,  10, -12,  14, -16,  18,  -3,   5,  -7,   9, -11,   1,  15, -17,   2,  -4,   6},
    {   7,  -9,  11, -13,  15, -17,   2,  -4,   6,  -8,  10, -12,  -1,  16, -18,   3,  -5},
    {  -6,   8, -10,  12, -14,  16, -18,   3,  -5,   7,  -9,  11, -13,   1,  17,  -2,   4},
    {   5,  -7,   9, -11,  13, -15,  17,  -2,   4,  -6,   8, -10,  12, -14,  -1,  18,  -3},
    {  -4,   6,  -8,  10, -12,  14, -16,  18,  -3,   5,  -7,   9, -11,  13, -15,   1,   2},
    {   3,  -5,   7,  -9,  11, -13,  15, -17,   2,  -4,   6,  -8,  10, -12,  14, -16,  -1},
};

int round_robin_16[16][15] = {
    {   2,  -3,   4,  -5,   6,  -7,   8,  -9,  10, -11,  12, -13,  14, -15,  16}, 
    {  -1,   4,  -6,   8, -10,  12, -14,  16,  -3,   5,  -7,   9, -11,  13, -15}, 
    { -16,   1,   5,  -7,   9, -11,  13, -15,   2,  -4,   6,  -8,  10, -12,  14}, 
    {  15,  -2,  -1,   6,  -8,  10, -12,  14, -16,   3,  -5,   7,  -9,  11, -13}, 
    { -14,  16,  -3,   1,   7,  -9,  11, -13,  15,  -2,   4,  -6,   8, -10,  12}, 
    {  13, -15,   2,  -4,  -1,   8, -10,  12, -14,  16,  -3,   5,  -7,   9, -11}, 
    { -12,  14, -16,   3,  -5,   1,   9, -11,  13, -15,   2,  -4,   6,  -8,  10}, 
    {  11, -13,  15,  -2,   4,  -6,  -1,  10, -12,  14, -16,   3,  -5,   7,  -9}, 
    { -10,  12, -14,  16,  -3,   5,  -7,   1,  11, -13,  15,  -2,   4,  -6,   8}, 
    {   9, -11,  13, -15,   2,  -4,   6,  -8, - 1,  12, -14,  16,  -3,   5,  -7}, 
    {  -8,  10, -12,  14, -16,   3,  -5,   7,  -9,   1,  13, -15,   2,  -4,   6}, 
    {   7,  -9,  11, -13,  15,  -2,   4,  -6,   8, -10,  -1,  14, -16,   3,  -5}, 
    {  -6,   8, -10,  12, -14,  16,  -3,   5,  -7,   9, -11,   1,  15,  -2,   4}, 
    {   5,  -7,   9, -11,  13, -15,   2,  -4,   6,  -8,  10, -12,  -1,  16,  -3}, 
    {  -4,   6,  -8,  10, -12,  14, -16,   3,  -5,   7,  -9,  11, -13,   1,   2}, 
    {   3,  -5,   7,  -9,  11, -13,  15,  -2,   4,  -6,   8, -10,  12, -14,  -1}, 
};

int round_robin_14[14][13] = {
    {   2,  -3,   4,  -5,   6,  -7,   8,  -9,  10, -11,  12, -13,  14},
    {  -1,   4,  -6,   8, -10,  12, -14,   3,  -5,   7,  -9,  11, -13},
    { -14,   1,   5,  -7,   9, -11,  13,  -2,   4,  -6,   8, -10,  12},
    {  13,  -2,  -1,   6,  -8,  10, -12,  14,  -3,   5,  -7,   9, -11},
    { -12,  14,  -3,   1,   7,  -9,  11, -13,   2,  -4,   6,  -8,  10},
    {  11, -13,   2,  -4,  -1,   8, -10,  12, -14,   3,  -5,   7,  -9},
    { -10,  12, -14,   3,  -5,   1,   9, -11,  13,  -2,   4,  -6,   8},
    {   9, -11,  13,  -2,   4,  -6,  -1,  10, -12,  14,  -3,   5,  -7},
    {  -8,  10, -12,  14,  -3,   5,  -7,   1,  11, -13,   2,  -4,   6},
    {   7,  -9,  11, -13,   2,  -4,   6,  -8,  -1,  12, -14,   3,  -5},
    {  -6,   8, -10,  12, -14,   3,  -5,   7,  -9,   1,  13,  -2,   4},
    {   5,  -7,   9, -11,  13,  -2,   4,  -6,   8, -10,  -1,  14,  -3},
    {  -4,   6,  -8,  10, -12,  14,  -3,   5,  -7,   9, -11,   1,   2},
    {   3,  -5,   7,  -9,  11, -13,   2,  -4,   6,  -8,  10, -12,  -1},
};

int round_robin_12[12][11] = {
    {   2,  -3,   4,  -5,   6,  -7,   8,  -9,  10, -11,  12},
    {  -1,   4,  -6,   8, -10,  12,  -3,   5,  -7,   9, -11},
    { -12,   1,   5,  -7,   9, -11,   2,  -4,   6,  -8,  10},
    {  11,  -2,  -1,   6,  -8,  10, -12,   3,  -5,   7,  -9},
    { -10,  12,  -3,   1,   7,  -9,  11,  -2,   4,  -6,   8},
    {   9, -11,   2,  -4,  -1,   8, -10,  12,  -3,   5,  -7},
    {  -8,  10, -12,   3,  -5,   1,   9, -11,   2,  -4,   6},
    {   7,  -9,  11,  -2,   4,  -6,  -1,  10, -12,   3,  -5},
    {  -6,   8, -10,  12,  -3,   5,  -7,   1,  11,  -2,   4},
    {   5,  -7,   9, -11,   2,  -4,   6,  -8,  -1,  12,  -3},
    {  -4,   6,  -8,  10, -12,   3,  -5,   7,  -9,   1,   2},
    {   3,  -5,   7,  -9,  11,  -2,   4,  -6,   8, -10,  -1},
};

int round_robin_10[10][9] = {
    {   2,  -3,   4,  -5,   6,  -7,   8,  -9,  10},
    {  -1,   4,  -6,   8, -10,   3,  -5,   7,  -9},
    { -10,   1,   5,  -7,   9,  -2,   4,  -6,   8},
    {   9,  -2,  -1,   6,  -8,  10,  -3,   5,  -7},
    {  -8,  10,  -3,   1,   7,  -9,   2,  -4,   6},
    {   7,  -9,   2,  -4,  -1,   8, -10,   3,  -5},
    {  -6,   8, -10,   3,  -5,   1,   9,  -2,   4},
    {   5,  -7,   9,  -2,   4,  -6,  -1,  10,  -3},
    {  -4,   6,  -8,  10,  -3,   5,  -7,   1,   2},
    {   3,  -5,   7,  -9,   2,  -4,   6,  -8,  -1},
};

struct round_robins{
    int max_teams;
    int** robins;
} round_robins[] = {
    {10, (int**)round_robin_10},
    {12, (int**)round_robin_12},
    {14, (int**)round_robin_14},
    {16, (int**)round_robin_16},
    {18, (int**)round_robin_18},
    {20, (int**)round_robin_20},
    {0}
};

int main(int argc, char** argv){
    FILE* fp;
    char buf[PATH_MAX];
    int i, weekno, matchcount;

    if (argc < 2){
        fprintf(stderr, "generate_schedule <file>\n");
        exit(EXIT_FAILURE);
    }

    if (!(fp = fopen(argv[1], "r"))){
        fprintf(stderr, "Cannot open/find %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    while (fgets(buf, sizeof buf, fp)){
        int teamnum;
        char left[PATH_MAX], right[PATH_MAX];
        sscanf(buf, "%[^:]:%[^\n]", left, right);
        for (char* p=left; *p; ++p) *p = tolower(*p);
        if (!strcasecmp(left, "division")){
            strncpy(division, right, sizeof division);
        }else if (!strcasecmp(left, "time")){
            strncpy(starttime, right, sizeof starttime);
        }else if ( (teamnum = atoi(left)) >0 && (teamnum <= MAX_TEAMS) ){
            char team[PATH_MAX], venue[PATH_MAX] = {0};
            // printf("Trying [%s]\n", )
            if (sscanf(right, "%[^|]|%[^\n]", team, venue) > 0){
                if (teams[teamnum-1][0]){
                    fprintf(stderr, "Duplicate team number %d: %s/%s\n", teamnum, teams[teamnum-1], team);
                    exit(EXIT_FAILURE);
                }
                strncpy(&teams[teamnum-1][0], team, sizeof teams[0]);
                strncpy(&venues[teamnum-1][0], *venue?venue:team, sizeof venues[0]);
                if (teamnum > lastteam){
                    lastteam = teamnum;
                }
            }
        }else if (sscanf(left, "week%d", &weekno)){
            if (weekno > 0 && weekno <= MAX_WEEKS){
                strncpy(weeks[weekno -1], right, sizeof weeks[0]);
                if (weekno > lastweek){
                    lastweek = weekno;
                }
            }
        }else{
            printf("Ignoring line: %s", buf);
        }
    }

    printf("Division - %s\n", division);
    printf("Start time - %s\n", starttime);
    printf("%d team: \n", lastteam);
    for(i=0; i < lastteam; ++i){
        printf("%d - %s(%s)\n", i+1, teams[i], venues[i]);
    }

    int success = 0;
    for(i=0; round_robins[i].robins; i++){
        if ((lastteam == round_robins[i].max_teams) || (lastteam == round_robins[i].max_teams-1)){
            printf("lastteam:%d round_robins[i].max_teams:%d\n", lastteam, round_robins[i].max_teams);
            while (lastteam < round_robins[i].max_teams){
                lastteam++;
                strncpy(&teams[lastteam-1][0], "Bye", sizeof teams[0]);
                strncpy(&venues[lastteam-1][0], "Bye", sizeof venues[0]);
            }
            success = 1;
            break;
        }else{
            printf("%d failed againt %d\n", lastteam, round_robins[i].max_teams);
        }
    }
    if (!success){
        fprintf(stderr, "Can't find a matrix for %d teams (only have for 10,12,14,16,18,20)\n", lastteam);
        fprintf(stderr, "round_robins[0].robins = %p\n", round_robins[0].robins);
        exit(EXIT_FAILURE);
    }
    printf("Using matrix for %d teams\n", lastteam);
    matchcount = lastteam-1;

    for(int league_week = 0; league_week < lastweek; league_week++){
        int robin = league_week / matchcount;
        for(int team = 0; team < lastteam; ++team){
            int team1, team2;
            // int home, away;

            team1 = team+1;
            switch (lastteam){
                case 10:
                    team2 = round_robin_10[team][league_week % matchcount];
                    break;
                case 12:
                    team2 = round_robin_12[team][league_week % matchcount];
                    break;
                case 14:
                    team2 = round_robin_14[team][league_week % matchcount];
                    break;
                case 16:
                    team2 = round_robin_16[team][league_week % matchcount];
                    break;
                case 18:
                    team2 = round_robin_18[team][league_week % matchcount];
                    break;
                case 20:
                    team2 = round_robin_20[team][league_week % matchcount];
                    break;
                default:
                    fprintf(stderr, "Unknown lastteam: %d\n", lastteam);
                    exit(EXIT_FAILURE);
            }

            if (robin%2 == 1){
                team2 *= -1; // swap venue
            }

            // printf("Team1: %d, team2: %d\n", team1, team2);
            if (team2 > 0){
                printf("%d, [%s] %s(%d) v %s(%d) @ %s\n", league_week+1, weeks[league_week], teams[team1-1], team1, teams[team2-1], team2, venues[team1-1]);
            }

            // if(team1 < team2){
            //     home = team2 -1;
            //     away = -team1 -1;
            // }else{
            //     home = team1 -1;
            //     away = -team2 -1;
            // }

//             if (robin%2 == 1){
//                 int temp = home;
//                 home = away;
//                 away = temp;
//             }
//             if (!strcasecmp(teams[home], "bye")){
//                 int temp = home;
//                 home = away;
//                 away = temp;
//             }

// // const char* header= "Date dd/mm/yyyy,Time HH:MM,Division,Home Team,Away Team,Venue,Pitch,Home Score,Away Score";
//             printf("%s,%s,%s,%s,%s,%s,-,-,-\n", weeks[league_week], starttime, division, teams[home], teams[away], venues[home]);
//             printf("%d, [%s] %s(%d) v %s(%d) @ %s\n", league_week+1, weeks[league_week], teams[home], home+1, teams[away], away+1, venues[home]);
            // team = lastteam; // TODO: remove
        }
    }
}

/** Format for input file

Division: <name of division>
Time: <time for matches, default 20:00>
1: <first team name>{|<venue name>}
2: <second team name>{|<venue name>}
3: <...>
...

*/
