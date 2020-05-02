#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TRANSITIONS 255

struct Transition {
    char *value;
    struct State *nextState;
};

struct State {
    bool isEnd;
    struct Transition *transitions;
    int transitionsCount;
};

struct State e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10;
char* detects_element = "numerical constant";


void initStates();
void initAState(struct State* state, bool isEnd);

void addTransition(struct State *fromState, char *value, struct State *toState);

bool isStringIn(char *input, struct State *state);

int main() {
    initStates();

    while(true) {
        char *input;
        scanf("%s", input);

        bool result = isStringIn(input,&e0);
        if(result)
            printf("YES! %s is a %s\n", input, detects_element);
        else
            printf("NO! %s is not a %s\n", input, detects_element);
    }

}


void initStates() {
    initAState(&e0, false);
    initAState(&e1, false);
    initAState(&e2, true);  // e2 is final state
    initAState(&e3, false);
    initAState(&e4, true);  // e4 is final state
    initAState(&e5, false);
    initAState(&e6, false);
    initAState(&e7, true);  // e7 is final state
    initAState(&e8, true); // e8 is final state
    initAState(&e9, false);
    initAState(&e10, true);// e10 is final state

    // ------- Numerical Constants -------

    char* n_transition = "0123456789";
    char* s_transition = "+-";
    char* E_transition = "E";
    char* dot_transition = ".";

    addTransition(&e0, s_transition, &e1);
    addTransition(&e0, n_transition, &e2);

    addTransition(&e1, n_transition, &e2);

    addTransition(&e2, n_transition, &e2);
    addTransition(&e2, dot_transition, &e3);
    addTransition(&e2, E_transition, &e5);

    addTransition(&e3, n_transition, &e4);

    addTransition(&e4, n_transition, &e4);
    addTransition(&e4, E_transition, &e5);

    addTransition(&e5, s_transition, &e6);
    addTransition(&e5, n_transition, &e7);

    addTransition(&e6, n_transition, &e7);

    addTransition(&e7, n_transition, &e7);


    // ------- Identifiers -------

    // ------- Strings -------

}

void initAState(struct State* state, bool isEnd) {
    struct State aux = {.isEnd = isEnd, .transitionsCount = 0, .transitions = malloc(sizeof(struct Transition) * MAX_TRANSITIONS)};
    *state = aux;
}

void addTransition(struct State *fromState, char *value, struct State *toState) {
    struct Transition new_transition = {.value = value, .nextState = toState};

    int i = fromState->transitionsCount;
    fromState->transitions[i] = new_transition;
    fromState->transitionsCount++;

}

bool isStringIn(char *input, struct State *state) {
    int input_index = 0;
    char c = input[input_index];
    struct State* current_state = state;

    while(c != '\0') {

        int i;
        bool transitioned = false;
        for(i = 0; i < current_state->transitionsCount; i++) {
            if (strchr(current_state->transitions[i].value, c) != NULL) {
                //character recognized
                current_state = current_state->transitions[i].nextState;
                transitioned = true;
                break;
            }
        }

        if(!transitioned) {
            // string not recognized
            return false;
        }

        c = input[++input_index];
    }

    return current_state->isEnd;
}
