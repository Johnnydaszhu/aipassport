#include "passport_protocol.h"

#include <string.h>

static bool parse_progress(const uint8_t *text, size_t len, uint8_t *value)
{
    if (!text || !value || len == 0 || len > 3) return false;
    unsigned parsed = 0;
    for (size_t i = 0; i < len; i++) {
        if (text[i] < '0' || text[i] > '9') return false;
        parsed = parsed * 10U + (unsigned)(text[i] - '0');
    }
    if (parsed > 100U) return false;
    *value = (uint8_t)parsed;
    return true;
}

static bool parse_task_count(const uint8_t *text, size_t len, uint8_t *value)
{
    if (!text || !value || len != 1 || text[0] < '0' || text[0] > '0' + PASSPORT_BOARD_TASK_COUNT) {
        return false;
    }
    *value = (uint8_t)(text[0] - '0');
    return true;
}

static bool copy_text(char *dst, const uint8_t *src, size_t len)
{
    if (!dst || !src || len > PASSPORT_BOARD_TEXT_CAP) return false;
    memcpy(dst, src, len);
    dst[len] = '\0';
    return true;
}

static bool copy_dialogue_text(char *dst, const uint8_t *src, size_t len)
{
    if (!dst || !src || len > PASSPORT_DIALOGUE_TEXT_CAP) return false;
    memcpy(dst, src, len);
    dst[len] = '\0';
    return true;
}

static bool parse_dialogue_status(
    const uint8_t *text,
    size_t len,
    passport_dialogue_status_t *status
)
{
    if (!text || !status || len != 1) return false;
    switch (text[0]) {
    case 'I': *status = PASSPORT_DIALOGUE_IDLE; break;
    case 'T': *status = PASSPORT_DIALOGUE_TRANSCRIBING; break;
    case 'H': *status = PASSPORT_DIALOGUE_THINKING; break;
    case 'A': *status = PASSPORT_DIALOGUE_ANSWERED; break;
    case 'E': *status = PASSPORT_DIALOGUE_ERROR; break;
    default: return false;
    }
    return true;
}

bool passport_board_apply(passport_board_t *board, const uint8_t *payload, size_t len)
{
    if (!board || !payload || len == 0 || len > PASSPORT_BOARD_PAYLOAD_CAP) return false;

    passport_board_t next = *board;
    bool changed = false;
    size_t start = 0;

    while (start < len) {
        size_t end = start;
        while (end < len && payload[end] != '\n') end++;
        size_t line_len = end - start;
        if (line_len > 0) {
            if (line_len < 2 || payload[start + 1] != ':') return false;
            const uint8_t *value = payload + start + 2;
            size_t value_len = line_len - 2;
            switch (payload[start]) {
            case 'P':
                if (!parse_progress(value, value_len, &next.progress)) return false;
                changed = true;
                break;
            case 'Y':
                if (!copy_text(next.year_goal, value, value_len)) return false;
                changed = true;
                break;
            case 'M':
                if (!copy_text(next.month_goal, value, value_len)) return false;
                changed = true;
                break;
            case 'W':
            case 'G': // Legacy iPhone build: G was the weekly goal.
                if (!copy_text(next.week_goal, value, value_len)) return false;
                changed = true;
                break;
            case 'C': {
                uint8_t count;
                if (!parse_task_count(value, value_len, &count)) return false;
                next.task_count = count;
                for (uint8_t i = count; i < PASSPORT_BOARD_TASK_COUNT; i++) {
                    next.tasks[i].text[0] = '\0';
                    next.tasks[i].status = PASSPORT_TASK_TODO;
                }
                changed = true;
                break;
            }
            case 'U':
                if (!copy_text(next.user_name, value, value_len)) return false;
                changed = true;
                break;
            case 'Q':
                if (!copy_dialogue_text(next.dialogue_user, value, value_len)) return false;
                changed = true;
                break;
            case 'A':
                if (!copy_dialogue_text(next.dialogue_answer, value, value_len)) return false;
                changed = true;
                break;
            case 'S':
                if (!parse_dialogue_status(value, value_len, &next.dialogue_status)) return false;
                changed = true;
                break;
            case 'N': // Legacy iPhone build: N was the single current task.
                if (!copy_text(next.tasks[0].text, value, value_len)) return false;
                next.tasks[0].status = PASSPORT_TASK_TODO;
                if (next.task_count == 0) next.task_count = 1;
                changed = true;
                break;
            default:
                if (payload[start] < '0' || payload[start] >= '0' + PASSPORT_BOARD_TASK_COUNT) {
                    return false;
                }
                if (value_len < 1) return false;
                uint8_t task_index = (uint8_t)(payload[start] - '0');
                switch (value[0]) {
                case 'O': next.tasks[task_index].status = PASSPORT_TASK_TODO; break;
                case 'B': next.tasks[task_index].status = PASSPORT_TASK_BLOCKED; break;
                case 'D': next.tasks[task_index].status = PASSPORT_TASK_DONE; break;
                default: return false;
                }
                if (!copy_text(next.tasks[task_index].text, value + 1, value_len - 1)) return false;
                if (next.task_count <= task_index) next.task_count = task_index + 1;
                changed = true;
                break;
            }
        }
        start = end + 1;
    }

    if (!changed) return false;
    *board = next;
    return true;
}
