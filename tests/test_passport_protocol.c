#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "passport_protocol.h"

int main(void)
{
    passport_board_t board = { 0 };
    const char full[] =
        "P:42\nY:Build a durable product\nM:Ship the companion\nW:Finish this week's prototype\n"
        "C:3\n0:OShip the BLE bridge\n1:BVerify ASR\n2:DDefine the protocol\nL:en\n";
    assert(passport_board_apply(&board, (const uint8_t *)full, strlen(full)));
    assert(board.language == PASSPORT_LANGUAGE_EN);
    assert(board.progress == 42);
    assert(strcmp(board.year_goal, "Build a durable product") == 0);
    assert(strcmp(board.month_goal, "Ship the companion") == 0);
    assert(strcmp(board.week_goal, "Finish this week's prototype") == 0);
    assert(board.task_count == 3);
    assert(board.tasks[0].status == PASSPORT_TASK_TODO);
    assert(strcmp(board.tasks[0].text, "Ship the BLE bridge") == 0);
    assert(board.tasks[1].status == PASSPORT_TASK_BLOCKED);
    assert(board.tasks[2].status == PASSPORT_TASK_DONE);

    const char *languages[] = { "zh-Hans", "zh-Hant", "en", "es", "fr", "de", "ko", "ja" };
    for (size_t i = 0; i < sizeof(languages) / sizeof(languages[0]); i++) {
        char payload[16];
        int length = snprintf(payload, sizeof(payload), "L:%s\n", languages[i]);
        assert(length > 0);
        assert(passport_board_apply(&board, (const uint8_t *)payload, (size_t)length));
        assert(board.language == (passport_language_t)i);
    }

    passport_board_t before_invalid_language = board;
    const char invalid_language[] = "L:unknown\n";
    assert(!passport_board_apply(
        &board,
        (const uint8_t *)invalid_language,
        strlen(invalid_language)
    ));
    assert(memcmp(&board, &before_invalid_language, sizeof(board)) == 0);

    const char dialogue[] =
        "U:星火\nQ:我接下来应该先做什么？\nS:H\n"
        "A:先完成蓝牙联动，再做真机回归。\nS:A\n";
    assert(passport_board_apply(&board, (const uint8_t *)dialogue, strlen(dialogue)));
    assert(strcmp(board.user_name, "星火") == 0);
    assert(strcmp(board.dialogue_user, "我接下来应该先做什么？") == 0);
    assert(strcmp(board.dialogue_answer, "先完成蓝牙联动，再做真机回归。") == 0);
    assert(board.dialogue_status == PASSPORT_DIALOGUE_ANSWERED);

    const char utf8[] = "P:88\nY:建立长期系统\nM:完成硬件联动\nW:跑通真机语音\n0:O完成蓝牙联动\n";
    assert(passport_board_apply(&board, (const uint8_t *)utf8, strlen(utf8)));
    assert(board.progress == 88);
    assert(strcmp(board.year_goal, "建立长期系统") == 0);
    assert(strcmp(board.month_goal, "完成硬件联动") == 0);
    assert(strcmp(board.week_goal, "跑通真机语音") == 0);
    assert(strcmp(board.tasks[0].text, "完成蓝牙联动") == 0);

    passport_board_t before = board;
    const char invalid[] = "P:101\n";
    assert(!passport_board_apply(&board, (const uint8_t *)invalid, strlen(invalid)));
    assert(memcmp(&board, &before, sizeof(board)) == 0);

    const char invalid_dialogue_status[] = "S:X\n";
    assert(!passport_board_apply(
        &board,
        (const uint8_t *)invalid_dialogue_status,
        strlen(invalid_dialogue_status)
    ));
    assert(memcmp(&board, &before, sizeof(board)) == 0);

    const char partial[] = "C:1\n0:D下一步\n";
    assert(passport_board_apply(&board, (const uint8_t *)partial, strlen(partial)));
    assert(board.progress == 88);
    assert(board.task_count == 1);
    assert(board.tasks[0].status == PASSPORT_TASK_DONE);
    assert(strcmp(board.tasks[0].text, "下一步") == 0);
    assert(strcmp(board.week_goal, "跑通真机语音") == 0);

    const char legacy[] = "N:旧版当前任务\nG:旧版周目标\n";
    assert(passport_board_apply(&board, (const uint8_t *)legacy, strlen(legacy)));
    assert(strcmp(board.tasks[0].text, "旧版当前任务") == 0);
    assert(strcmp(board.week_goal, "旧版周目标") == 0);
    return 0;
}
