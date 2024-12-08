#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *INPUT_PATH = "input.txt";

enum CursorMul { MUL_NAME, MUL_BR_OPEN, ARG1, COMMA, ARG2, MUL_BR_CLOSE };
enum Cursor { NAME, BR_OPEN, BR_CLOSE };

typedef struct {
  char chars[64];
  int parsedChars;
  bool isDo;
  enum Cursor cursor;
} State;

void stateInit(State *state) {
  state->parsedChars = 0;
  state->cursor = NAME;
  state->isDo = false;
  memset(state->chars, 0, sizeof(state->chars));
};

bool stateIsFinished(State *state) { return state->cursor == BR_CLOSE; }

bool stateEval(State *state) { return state->isDo; }

void stateParseChar(State *state, char c) {
  state->chars[state->parsedChars] = c;
  state->parsedChars++;
}

void stateNext(State *state) {
  switch (state->cursor) {
    case NAME:
      state->cursor = BR_OPEN;
      break;
    case BR_OPEN:
      state->cursor = BR_CLOSE;
      break;
    case BR_CLOSE:
      break;
  }
}

bool stateProcessCharName(State *state, char c) {
  switch (state->parsedChars) {
    case 0:
      if (c != 'd') {
        return false;
      }
      break;
    case 1:
      if (c != 'o') {
        return false;
      }
      break;
    case 2:
      if (c == '(') {
        state->isDo = true;
        stateNext(state);
        return true;
      }

      if (c != 'n') {
        return false;
      }
      break;
    case 3:
      if (c != '\'') {
        return false;
      }
      break;
    case 4:
      if (c != 't') {
        return false;
      }
      break;
    case 5:
      if (c != '(') {
        return false;
      }

      stateNext(state);
      return true;
    default:
      perror("invalid state");
      exit(EXIT_FAILURE);
  }

  return true;
}

bool stateProcessBrOpen(State *state, char c) {
  if (c != ')') {
    return false;
  }

  stateNext(state);
  return true;
}

bool stateProcessChar(State *state, char c) {
  if (state->cursor == BR_CLOSE) {
    return true;
  }

  switch (state->cursor) {
    case NAME:
      if (!stateProcessCharName(state, c)) {
        return false;
      }
      break;
    case BR_OPEN:
      if (!stateProcessBrOpen(state, c)) {
        return false;
      }
      break;
    case BR_CLOSE:
      break;
    default:
      perror("invalid state");
      exit(EXIT_FAILURE);
  }

  stateParseChar(state, c);

  return true;
}

typedef struct {
  char chars[64];
  int parsedChars;
  enum CursorMul cursor;
  char arg1[32];
  int arg1len;
  char arg2[32];
  int arg2len;
} StateOperation;

void stateOperationInit(StateOperation *state) {
  state->parsedChars = 0;
  state->cursor = MUL_NAME;
  state->arg1len = 0;
  state->arg2len = 0;
  memset(state->chars, 0, sizeof(state->chars));
  memset(state->arg1, 0, sizeof(state->arg1));
  memset(state->arg2, 0, sizeof(state->arg2));
}

int stateOperationEval(StateOperation *state) {
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

void stateOperationParseChar(StateOperation *state, char c) {
  state->chars[state->parsedChars] = c;
  state->parsedChars++;
}

void stateOperationNext(StateOperation *state) {
  switch (state->cursor) {
    case MUL_NAME:
      state->cursor = MUL_BR_OPEN;
      break;
    case MUL_BR_OPEN:
      state->cursor = ARG1;
      break;
    case ARG1:
      state->cursor = COMMA;
      break;
    case COMMA:
      state->cursor = ARG2;
      break;
    case ARG2:
      state->cursor = MUL_BR_CLOSE;
      break;
    case MUL_BR_CLOSE:
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

bool stateOperationProcessCharBrOpen(StateOperation *state, char c) {
  if (!validateNum(c)) {
    return false;
  }

  state->arg1[state->arg1len] = c;
  state->arg1len++;

  stateOperationNext(state);

  return true;
}

bool stateOperationProcessCharComma(StateOperation *state, char c) {
  if (!validateNum(c)) {
    return false;
  }

  state->arg2[state->arg2len] = c;
  state->arg2len++;

  stateOperationNext(state);

  return true;
}

bool stateOperationProcessChaArg2(StateOperation *state, char c) {
  if (state->arg2len > 0 && c == ')') {
    stateOperationNext(state);
    return true;
  }

  if (!validateNum(c)) {
    return false;
  }

  state->arg2[state->arg2len] = c;
  state->arg2len++;

  return true;
}

bool stateOperationProcessCharArg1(StateOperation *state, char c) {
  if (state->arg1len > 0 && c == ',') {
    stateOperationNext(state);
    return true;
  }

  if (!validateNum(c)) {
    return false;
  }

  state->arg1[state->arg1len] = c;
  state->arg1len++;

  return true;
}

bool stateOperationProcessCharMulName(StateOperation *state, char c) {
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

      stateOperationNext(state);
      break;
    default:
      perror("invalid state");
      exit(EXIT_FAILURE);
  }

  return true;
}

bool stateOperationIsFinished(StateOperation *state) {
  return state->cursor == MUL_BR_CLOSE;
}

bool stateOperationProcessChar(StateOperation *state, char c) {
  if (state->cursor == MUL_BR_CLOSE) {
    return true;
  }

  switch (state->cursor) {
    case MUL_NAME:
      if (!stateOperationProcessCharMulName(state, c)) {
        return false;
      }
      break;
    case MUL_BR_OPEN:
      if (!stateOperationProcessCharBrOpen(state, c)) {
        return false;
      }
      break;
    case ARG1:
      if (!stateOperationProcessCharArg1(state, c)) {
        return false;
      }
      break;
    case COMMA:
      if (!stateOperationProcessCharComma(state, c)) {
        return false;
      }
      break;
    case ARG2:
      if (!stateOperationProcessChaArg2(state, c)) {
        return false;
      }
      break;
    default:
      perror("invalid state");
      exit(EXIT_FAILURE);
  }

  stateOperationParseChar(state, c);

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
  StateOperation state;
  StateOperation *state_p = &state;
  stateOperationInit(state_p);
  while ((bytes_read = fread(&buffer, 1, sizeof(buffer), fp)) > 0) {
    for (int i = 0; i < bytes_read; i++) {
      if (stateOperationIsFinished(state_p)) {
        res += stateOperationEval(state_p);

        printf("successfully parsed state: %s\n", state_p->chars);

        StateOperation state;
        state_p = &state;
        stateOperationInit(state_p);
      }

      char ch = buffer[i];
      if (!stateOperationProcessChar(state_p, ch) && state_p->parsedChars > 0) {
        printf("parsing failed for state: %s on char: %c\n", state_p->chars,
               ch);

        StateOperation state;
        state_p = &state;
        stateOperationInit(state_p);
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

int part2() {
  FILE *fp = fopen(INPUT_PATH, "r");
  if (!fp) {
    perror("Error open the file");
    exit(EXIT_FAILURE);
  }

  char buffer[256];
  size_t bytes_read;
  int res = 0;
  bool parsingEnabled = true;

  State state;
  State *state_p = &state;
  stateInit(state_p);

  StateOperation state_operation;
  StateOperation *state_operation_p = &state_operation;
  stateOperationInit(state_operation_p);
  while ((bytes_read = fread(&buffer, 1, sizeof(buffer), fp)) > 0) {
    for (int i = 0; i < bytes_read; i++) {
      if (stateOperationIsFinished(state_operation_p)) {
        res += stateOperationEval(state_operation_p);

        printf("successfully parsed operation state: %s\n",
               state_operation_p->chars);

        StateOperation state_operation;
        state_operation_p = &state_operation;
        stateOperationInit(state_operation_p);
      }

      if (stateIsFinished(state_p)) {
        parsingEnabled = stateEval(state_p);

        printf("successfully parsed state: %s\n", state_p->chars);

        State state;
        state_p = &state;
        stateInit(state_p);
      }

      char ch = buffer[i];

      if (!stateProcessChar(state_p, ch)) {
        printf("parsing failed for state: %s on char: %c\n", state_p->chars,
               ch);

        State state;
        state_p = &state;
        stateInit(state_p);
      }

      if (!parsingEnabled || state_p->parsedChars > 0) {
        continue;
      }

      if (!stateOperationProcessChar(state_operation_p, ch)) {
        printf("parsing failed for operation state: %s on char: %c\n",
               state_operation_p->chars, ch);

        StateOperation state_operation;
        state_operation_p = &state_operation;
        stateOperationInit(state_operation_p);
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
  int res1 = part1();
  printf("result p1: %d\n", res1);

  int res2 = part2();
  printf("result p2: %d\n", res2);

  return EXIT_SUCCESS;
}
