// IMA ADPCM block codec adapted from zhaohuaxiaoy/folo-ai-passport-voice (MIT).
#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ADPCM_HEADER_BYTES 4
#define ADPCM_BLOCK_SAMPLES 1600
#define ADPCM_BLOCK_BYTES (ADPCM_HEADER_BYTES + (ADPCM_BLOCK_SAMPLES / 2))

typedef struct {
    int16_t predictor;
    uint8_t index;
} adpcm_state_t;

void adpcm_state_reset(adpcm_state_t *state);
size_t adpcm_encode_block(
    adpcm_state_t *state,
    const int16_t *pcm,
    size_t samples,
    uint8_t *output,
    size_t output_capacity
);
size_t adpcm_decode_block(
    const uint8_t *input,
    size_t input_length,
    size_t samples,
    int16_t *pcm,
    size_t pcm_capacity
);

#ifdef __cplusplus
}
#endif
