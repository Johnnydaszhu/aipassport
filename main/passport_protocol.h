#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PASSPORT_BOARD_TEXT_CAP 96
#define PASSPORT_DIALOGUE_TEXT_CAP 192
#define PASSPORT_BOARD_PAYLOAD_CAP 240
#define PASSPORT_BOARD_TASK_COUNT 5

typedef enum {
    PASSPORT_TASK_TODO = 0,
    PASSPORT_TASK_BLOCKED,
    PASSPORT_TASK_DONE,
} passport_task_status_t;

typedef struct {
    passport_task_status_t status;
    char text[PASSPORT_BOARD_TEXT_CAP + 1];
} passport_task_t;

typedef enum {
    PASSPORT_DIALOGUE_IDLE = 0,
    PASSPORT_DIALOGUE_TRANSCRIBING,
    PASSPORT_DIALOGUE_THINKING,
    PASSPORT_DIALOGUE_ANSWERED,
    PASSPORT_DIALOGUE_ERROR,
} passport_dialogue_status_t;

typedef struct {
    uint8_t progress;
    char year_goal[PASSPORT_BOARD_TEXT_CAP + 1];
    char month_goal[PASSPORT_BOARD_TEXT_CAP + 1];
    char week_goal[PASSPORT_BOARD_TEXT_CAP + 1];
    uint8_t task_count;
    passport_task_t tasks[PASSPORT_BOARD_TASK_COUNT];
    passport_dialogue_status_t dialogue_status;
    char user_name[PASSPORT_BOARD_TEXT_CAP + 1];
    char dialogue_user[PASSPORT_DIALOGUE_TEXT_CAP + 1];
    char dialogue_answer[PASSPORT_DIALOGUE_TEXT_CAP + 1];
} passport_board_t;

// Apply one UTF-8 board update sent by TheGreatMe. The compact wire format is:
//   P:<0-100>\nY:<year goal>\nM:<month goal>\nW:<week goal>\n
//   C:<0-5>\n0:<O|B|D><task text>\n ... 4:<O|B|D><task text>\n
//   U:<user name>\nQ:<latest user text>\nA:<latest HQ answer>\nS:<I|T|H|A|E>\n
// Fields may be omitted for partial updates. Invalid input leaves board unchanged.
bool passport_board_apply(passport_board_t *board, const uint8_t *payload, size_t len);

#ifdef __cplusplus
}
#endif
