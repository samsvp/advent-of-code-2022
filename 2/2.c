#include "../utils.h"
#include "../sv_string.h"

enum moves {ROCK, PAPER, SCISSORS, NO_MOVE};
enum movesp2 {LOSE, DRAW, WIN, NO_MOVE_P2};

typedef struct {
    enum moves opp_choice;
    enum moves ply_choice; 
} RPSMatch;


enum moves str_to_move(String move_str) 
{
    char c = str_at(&move_str, 0);
    if (c == 'A' || c == 'X') return ROCK;
    if (c == 'B' || c == 'Y') return PAPER;
    if (c == 'C' || c == 'Z') return SCISSORS;
    return NO_MOVE;
}


const char* get_move_name(enum moves move)
{
    if (move == ROCK) return "ROCK";
    if (move == PAPER) return "PAPER";
    if (move == SCISSORS) return "SCISSORS";
    return "NO MOVE";
}


void print_match(RPSMatch match)
{
    printf("opp: %s, ply: %s\n", 
           get_move_name(match.opp_choice), 
           get_move_name(match.ply_choice));
}


int win_score(RPSMatch match)
{
    enum moves o = match.opp_choice;
    enum moves p = match.ply_choice;
    int win_score = 6;
    int draw_score = 3;
    int lose_score = 0;
    if (p == ROCK) {
        switch (o) 
        {
            case ROCK: return draw_score;
            case PAPER: return lose_score;
            case SCISSORS: return win_score;
            default: return 0;
        }
    }
    if (p == PAPER) {
        switch (o) 
        {
            case ROCK: return win_score;
            case PAPER: return draw_score;
            case SCISSORS: return lose_score;
            default: return 0;
        }
    }
    if (p == SCISSORS) {
        switch (o) 
        {
            case ROCK: return lose_score;
            case PAPER: return win_score;
            case SCISSORS: return draw_score;
            default: return 0;
        }
    }
    return 0;
}


int choice_score(RPSMatch match)
{
    switch (match.ply_choice) 
    {
        case ROCK: return 1;
        case PAPER: return 2;
        case SCISSORS: return 3;
        default: return 0;
    }
}



int score_match(RPSMatch match)
{
    return choice_score(match) + win_score(match);
}


void p2_match_to_p1(RPSMatch* match)
{
    enum moves o = match->opp_choice;
    enum moves p = match->ply_choice;
    if (p == LOSE) {
        switch (o) 
        {
            case ROCK: match->ply_choice = SCISSORS; return;
            case PAPER: match->ply_choice = ROCK; return;
            case SCISSORS: match->ply_choice = PAPER; return;
            default: return;
        } 
    }
    if (p == DRAW) {
        match->ply_choice = o;
        return;
    }
    if (p == WIN) {
        switch (o) 
        {
            case ROCK: match->ply_choice = PAPER; return;
            case PAPER: match->ply_choice = SCISSORS; return;
            case SCISSORS: match->ply_choice = ROCK; return;
            default: return;
        }
    }
}


void p1(Vector vs)
{
    long total_score = 0;
    for (int i=0; i<vs.length; i++) {
        String* s = (String*)vec_elem_loc(&vs, i);
        if (s->size == 0) { 
            continue;
        }

        Vector vs_rps = str_split(s, " ");
        String* vs_rps_array = ((String*)vs_rps.data);
        RPSMatch rps_match = {
            .opp_choice = str_to_move(vs_rps_array[0]),
            .ply_choice = str_to_move(vs_rps_array[1])
        };
        total_score += score_match(rps_match);
    } 
    printf("score: %ld\n", total_score);
}

void p2(Vector vs)
{
    long total_score = 0;
    for (int i=0; i<vs.length; i++) {
        String* s = (String*)vec_elem_loc(&vs, i);
        if (s->size == 0) { 
            continue;
        }

        Vector vs_rps = str_split(s, " ");
        String* vs_rps_array = ((String*)vs_rps.data);
        RPSMatch rps_match = {
            .opp_choice = str_to_move(vs_rps_array[0]),
            .ply_choice = str_to_move(vs_rps_array[1])
        };
        p2_match_to_p1(&rps_match);
        total_score += score_match(rps_match);
    } 
    printf("score 2: %ld\n", total_score);
}

int main(void)
{
    char* data = read_file("2.txt");
    String s = str_init(data);
    Vector vs = str_split(&s, "\n");
    p1(vs);
    p2(vs);
    return 0;
}
