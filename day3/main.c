#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *INPUT_PATH = "input.txt";

enum Cursor { OPERATION, BR_OPEN, ARG1, COMMA, ARG2, BR_CLOSE };

typedef struct {
  char chars[64];
  int parsedChars;
  enum Cursor cursor;
  char arg1[32];
  int arg1len;
  char arg2[32];
  int arg2len;
} State;

void stateInit(State *state) {
  state->parsedChars = 0;
  state->cursor = OPERATION;
  state->arg1len = 0;
  state->arg2len = 0;
  memset(state->chars, 0, sizeof(state->chars));
  memset(state->arg1, 0, sizeof(state->arg1));
  memset(state->arg2, 0, sizeof(state->arg2));
}

int stateEval(State *state) {
  long arg1, arg2;

  char arg1chars[state->arg1len + 1];
  strncpy(arg1chars, state->arg1, state->arg1len);
  arg1chars[state->arg1len] = '\0';
  arg1 = strtol(arg1chars, NULL, 10);

  char arg2chars[state->arg2len + 1];
  strncpy(arg2chars, state->arg2, state->arg2len);
  arg2chars[state->arg2len] = '\0';
  arg2 = strtol(arg2chars, NULL, 10);

  return arg1 * arg2;
}

void stateParseChar(State *state, char c) {
  state->chars[state->parsedChars] = c;
  state->parsedChars++;
}

void stateNext(State *state) {
  switch (state->cursor) {
  case OPERATION:
    state->cursor = BR_OPEN;
    break;
  case BR_OPEN:
    state->cursor = ARG1;
    break;
  case ARG1:
    state->cursor = COMMA;
    break;
  case COMMA:
    state->cursor = ARG2;
    break;
  case ARG2:
    state->cursor = BR_CLOSE;
    break;
  case BR_CLOSE:
    break;
  }
}

bool validateNum(char c) {
  char percent = '%';
  char *endptr = &percent;
  strtol(&c, &endptr, 10);

  if (*endptr == c) {
    return false;
  }

  return true;
}

bool stateProcessCharBrOpen(State *state, char c) {
  if (!validateNum(c)) {
    return false;
  }

  state->arg1[state->arg1len] = c;
  state->arg1len++;

  stateNext(state);

  return true;
}

bool stateProcessCharComma(State *state, char c) {
  if (!validateNum(c)) {
    return false;
  }

  state->arg2[state->arg2len] = c;
  state->arg2len++;

  stateNext(state);

  return true;
}

bool stateProcessChaArg2(State *state, char c) {
  if (state->arg2len > 0 && c == ')') {
    stateNext(state);
    return true;
  }

  if (!validateNum(c)) {
    return false;
  }

  state->arg2[state->arg2len] = c;
  state->arg2len++;

  return true;
}

bool stateProcessCharArg1(State *state, char c) {
  if (state->arg1len > 0 && c == ',') {
    stateNext(state);
    return true;
  }

  if (!validateNum(c)) {
    return false;
  }

  state->arg1[state->arg1len] = c;
  state->arg1len++;

  return true;
}

bool stateProcessCharOperation(State *state, char c) {
  switch (state->parsedChars) {
  case 0:
    if (c != 'm') {
      return false;
    }
    break;
  case 1:
    if (c != 'u') {
      return false;
    }
    break;
  case 2:
    if (c != 'l') {
      return false;
    }
    break;
  case 3:
    if (c != '(') {
      return false;
    }

    stateNext(state);
    break;
  default:
    perror("invalid state");
    exit(EXIT_FAILURE);
  }

  return true;
}

bool stateIsFinished(State *state) { return state->cursor == BR_CLOSE; }

bool stateProcessChar(State *state, char c) {

  if (state->cursor == BR_CLOSE) {
    return true;
  }

  switch (state->cursor) {
  case OPERATION:
    if (!stateProcessCharOperation(state, c)) {
      return false;
    }
    break;
  case BR_OPEN:
    if (!stateProcessCharBrOpen(state, c)) {
      return false;
    }
    break;
  case ARG1:
    if (!stateProcessCharArg1(state, c)) {
      return false;
    }
    break;
  case COMMA:
    if (!stateProcessCharComma(state, c)) {
      return false;
    }
    break;
  case ARG2:
    if (!stateProcessChaArg2(state, c)) {
      return false;
    }
    break;
  default:
    perror("invalid state");
    exit(EXIT_FAILURE);
  }

  stateParseChar(state, c);

  return true;
}

int part1() {
  FILE *fp = fopen(INPUT_PATH, "r");
  if (!fp) {
    perror("Error open the file");
    exit(EXIT_FAILURE);
  }

  char buffer[256];
  size_t bytes_read;
  int res = 0;
  State state;
  State *state_p = &state;
  stateInit(state_p);
  while ((bytes_read = fread(&buffer, 1, sizeof(buffer), fp)) > 0) {
    for (int i = 0; i < bytes_read; i++) {
      if (stateIsFinished(state_p)) {
        res += stateEval(state_p);

        printf("successfully parsed state: %s\n", state_p->chars);

        State state;
        state_p = &state;
        stateInit(state_p);
      }

      char ch = buffer[i];
      if (!stateProcessChar(state_p, ch) && state_p->parsedChars > 0) {
        printf("parsing failed for state: %s on char: %c\n", state_p->chars,
               ch);

        State state;
        state_p = &state;
        stateInit(state_p);
        continue;
      };
    }
  }

  if (ferror(fp)) {
    perror("Error reading the file");
  } else if (feof(fp)) {
    printf("\nEnd of file reached.\n");
  }

  fclose(fp);
  return res;
}

int main() {
  int resp1 = part1();
  printf("result p1: %d\n", resp1);
  return EXIT_SUCCESS;
}
