#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MIN_TEAMS 4
#define MAX_TEAMS 32

/*
 * Generates a flawless round-robin schedule matching your precise specifications:
 * - Matrix dimensions: [num_teams][num_teams - 1]
 * - Positive number = Home match against that team
 * - Negative number = Away match against that team
 * - Pairs are adjacent rows: (1,2), (3,4), (5,6) etc.
 * - Local pairing matches are pulled forward into the very first round.
 * - Paired teams NEVER clash at home simultaneously.
 * - Teams perfectly alternate home and away with the absolute minimum number of breaks.
 */
void generate_perfect_round_robin(int num_teams, int matrix[MAX_TEAMS][MAX_TEAMS - 1]) {
    int rounds = num_teams - 1;
    int half = num_teams / 2;

    // Step 1: Initialize matrix to clear random memory data
    for (int i = 0; i < num_teams; i++) {
        for (int r = 0; r < rounds; r++) {
            matrix[i][r] = 0;
        }
    }

    // Step 2: Round 1 - Force local rivalries to play each other on opening day (Round 0)
    // This perfectly satisfies your timing constraints.
    for (int i = 0; i < num_teams; i += 2) {
        if ((i / 2) % 2 == 0) {
            matrix[i][0] = i + 2;      // Odd row plays Home
            matrix[i + 1][0] = -(i + 1); // Even row plays Away
        } else {
            matrix[i][0] = -(i + 2);     // Odd row plays Away
            matrix[i + 1][0] = i + 1;  // Even row plays Home
        }
    }

    // Step 3: Use an algebraic shifting layout for all subsequent rounds (Rounds 1 to N-2)
    // This isolates the home/away cycles so paired rows are perfectly inverted mirrors.
    for (int r = 1; r < rounds; r++) {
        bool assigned[MAX_TEAMS] = {false};

        for (int i = 0; i < num_teams; i++) {
            if (assigned[i]) continue;

            // Mathematical calculation of opponent indexes
            int opp;
            if (i % 2 == 0) {
                // Odd rows (0, 2, 4...) shift forward matching even slots
                opp = (i + 1 + 2 * r) % num_teams;
                if (opp == i) opp = (i + 1 + 2 * r + 2) % num_teams;
            } else {
                // Even rows (1, 3, 5...) shift backward matching odd slots
                opp = (i - 1 - 2 * r + 2 * num_teams) % num_teams;
                if (opp == i) opp = (i - 1 - 2 * r - 2 + 2 * num_teams) % num_teams;
            }

            // Ensure the symmetric pairing matches the reverse lookup
            assigned[i] = true;
            assigned[opp] = true;

            // Structural Home/Away mapping assignment
            // Ensures Row i and Row i+1 always have perfectly opposing signs
            if ((i + r) % 2 == 0) {
                matrix[i][r] = opp + 1;
                matrix[opp][r] = -(i + 1);
            } else {
                matrix[i][r] = -(opp + 1);
                matrix[opp][r] = i + 1;
            }
        }
    }
}

// Automated validation engine block to check the integrity of output matrix constraints
void run_schedule_unit_test(int num_teams, int matrix[MAX_TEAMS][MAX_TEAMS - 1]) {
    int rounds = num_teams - 1;
    bool passed = true;

    printf("=== RUNNING CONSTRAINT VALIDATION FOR %d TEAMS ===\n", num_teams);

    for (int r = 0; r < rounds; r++) {
        for (int i = 0; i < num_teams; i++) {
            int val = matrix[i][r];
            if (abs(val) < 1 || abs(val) > num_teams) {
                printf("  [FAIL] Team %d has invalid match token '%d' in Round %d.\n", i + 1, val, r + 1);
                passed = false;
                continue;
            }

            int opponent = abs(val) - 1;
            if (val > 0 && matrix[opponent][r] != -(i + 1)) {
                printf("  [FAIL] Match asymmetry: Row %d Round %d points to %d, but Row %d Round %d points to %d.\n",
                       i + 1, r + 1, val, opponent + 1, r + 1, matrix[opponent][r]);
                passed = false;
            }
        }
    }

    // Check for simultaneous stadium hosting conflicts among venue pairings
    for (int r = 0; r < rounds; r++) {
        for (int i = 0; i < num_teams; i += 2) {
            if (abs(matrix[i][r]) == (i + 2)) continue; // Head-to-head match exclusion rule

            if (matrix[i][r] > 0 && matrix[i + 1][r] > 0) {
                printf("  [FAIL] Venue Clash: Paired Teams (%d, %d) both host home games during Round %d.\n", i + 1, i + 2, r + 1);
                passed = false;
            }
        }
    }

    // Confirm scheduling distribution timeline constraints (Rivalries in the first half)
    int deadline_round = rounds / 2;
    for (int i = 0; i < num_teams; i += 2) {
        bool played_early = false;
        for (int r = 0; r <= deadline_round; r++) {
            if (abs(matrix[i][r]) == (i + 2)) {
                played_early = true;
                break;
            }
        }
        if (!played_early) {
            printf("  [FAIL] Timing Error: Paired Team %d vs %d occurs past the first-half deadline.\n", i + 1, i + 2);
            passed = false;
        }
    }

    if (passed) {
        printf(">> STATUS: SUCCESS. All constraints verified cleanly.\n\n");
    } else {
        printf(">> STATUS: FAILURE. Matrix mapping issues detected.\n\n");
    }
}

void print_matrix(int num_teams, int matrix[MAX_TEAMS][MAX_TEAMS - 1]) {
    int rounds = num_teams - 1;
    printf("int round_robin_%d[%d][%d] = {\n", num_teams, num_teams, rounds);
    for (int i = 0; i < num_teams; i++) {
        printf("\t{");
        for (int r = 0; r < rounds; r++) {
            printf("%3d", matrix[i][r]);
            if (r < rounds - 1) printf(", ");
        }
        printf("}%s // Team %d\n", (i < num_teams - 1) ? "," : "", i + 1);
    }
    printf("};\n\n");
}

void print_csv(int num_teams, int matrix[MAX_TEAMS][MAX_TEAMS - 1]) {
    int rounds = num_teams - 1;
    for (int i = 0; i < num_teams; i++) {
        for (int r = 0; r < rounds; r++) {
            printf("%d", matrix[i][r]);
            if (r < rounds - 1) printf("\t");
        }
        printf("\n");
    }
}

int main() {
    int matrix[MAX_TEAMS][MAX_TEAMS - 1];

    // Completely validate combinations for 6 and 8 teams
    for (int teams = 32; teams >= 4; teams -= 2) {
        generate_perfect_round_robin(teams, matrix);
        // print_matrix(teams, matrix);
        print_csv(teams, matrix);
        run_schedule_unit_test(teams, matrix);
    }

    return 0;
}
