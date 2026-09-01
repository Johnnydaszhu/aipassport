// Independent-block IMA ADPCM, low nibble first. Adapted from the MIT-licensed
// zhaohuaxiaoy/folo-ai-passport-voice implementation used on this hardware.
#include "adpcm.h"

static const int16_t step_table[89] = {
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
    19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
    50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
    130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
    337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
    876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
    2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
    5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
    15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767,
};

static const int8_t index_delta[16] = {
    -1, -1, -1, -1, 2, 4, 6, 8,
    -1, -1, -1, -1, 2, 4, 6, 8,
};

static int clamp_index(int value)
{
    if (value < 0) return 0;
    if (value > 88) return 88;
    return value;
}

static int16_t clamp_sample(int32_t value)
{
    if (value > 32767) return 32767;
    if (value < -32768) return -32768;
    return (int16_t)value;
}

void adpcm_state_reset(adpcm_state_t *state)
{
    if (!state) return;
    state->predictor = 0;
    state->index = 0;
}

static uint8_t encode_sample(adpcm_state_t *state, int16_t sample)
{
    int step = step_table[state->index];
    int32_t difference = (int32_t)sample - state->predictor;
    uint8_t code = 0;
    if (difference < 0) {
        code = 8;
        difference = -difference;
    }

    int32_t threshold = step;
    if (difference >= threshold) { code |= 4; difference -= threshold; }
    threshold >>= 1;
    if (difference >= threshold) { code |= 2; difference -= threshold; }
    threshold >>= 1;
    if (difference >= threshold) code |= 1;

    int32_t reconstructed = step >> 3;
    if (code & 4) reconstructed += step;
    if (code & 2) reconstructed += step >> 1;
    if (code & 1) reconstructed += step >> 2;
    state->predictor = clamp_sample(
        (int32_t)state->predictor + ((code & 8) ? -reconstructed : reconstructed)
    );
    state->index = (uint8_t)clamp_index((int)state->index + index_delta[code]);
    return code;
}

static int16_t decode_sample(adpcm_state_t *state, uint8_t code)
{
    int step = step_table[state->index];
    int32_t difference = step >> 3;
    if (code & 4) difference += step;
    if (code & 2) difference += step >> 1;
    if (code & 1) difference += step >> 2;
    state->predictor = clamp_sample(
        (int32_t)state->predictor + ((code & 8) ? -difference : difference)
    );
    state->index = (uint8_t)clamp_index((int)state->index + index_delta[code]);
    return state->predictor;
}

size_t adpcm_encode_block(
    adpcm_state_t *state,
    const int16_t *pcm,
    size_t samples,
    uint8_t *output,
    size_t output_capacity
)
{
    if (!state || !pcm || !output || samples == 0) return 0;
    size_t needed = ADPCM_HEADER_BYTES + samples / 2;
    if (output_capacity < needed) return 0;

    state->predictor = pcm[0];
    output[0] = (uint8_t)((uint16_t)pcm[0] & 0xFF);
    output[1] = (uint8_t)(((uint16_t)pcm[0] >> 8) & 0xFF);
    output[2] = state->index;
    output[3] = 0;

    size_t write_index = ADPCM_HEADER_BYTES;
    for (size_t sample_index = 1; sample_index < samples; sample_index++) {
        uint8_t code = encode_sample(state, pcm[sample_index]);
        if ((sample_index & 1U) == 1U) {
            output[write_index] = code & 0x0F;
            if (sample_index + 1 == samples) write_index++;
        } else {
            output[write_index] |= (uint8_t)(code << 4);
            write_index++;
        }
    }
    return needed;
}

size_t adpcm_decode_block(
    const uint8_t *input,
    size_t input_length,
    size_t samples,
    int16_t *pcm,
    size_t pcm_capacity
)
{
    if (!input || !pcm || samples == 0) return 0;
    size_t needed = ADPCM_HEADER_BYTES + samples / 2;
    if (input_length < needed || pcm_capacity < samples) return 0;

    adpcm_state_t state = {
        .predictor = (int16_t)(uint16_t)((uint16_t)input[0] | ((uint16_t)input[1] << 8)),
        .index = (uint8_t)clamp_index(input[2]),
    };
    pcm[0] = state.predictor;
    size_t read_index = ADPCM_HEADER_BYTES;
    for (size_t sample_index = 1; sample_index < samples; sample_index++) {
        uint8_t code;
        if ((sample_index & 1U) == 1U) {
            code = input[read_index] & 0x0F;
        } else {
            code = (input[read_index] >> 4) & 0x0F;
            read_index++;
        }
        pcm[sample_index] = decode_sample(&state, code);
    }
    return samples;
}
