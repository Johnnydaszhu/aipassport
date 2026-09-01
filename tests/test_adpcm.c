#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "adpcm.h"

static void assert_round_trip(size_t sample_count)
{
    int16_t source[ADPCM_BLOCK_SAMPLES];
    int16_t decoded[ADPCM_BLOCK_SAMPLES];
    uint8_t encoded[ADPCM_BLOCK_BYTES];
    for (size_t i = 0; i < sample_count; i++) {
        source[i] = (int16_t)(((int)(i % 200) - 100) * 180);
    }

    adpcm_state_t state;
    adpcm_state_reset(&state);
    size_t encoded_bytes = ADPCM_HEADER_BYTES + sample_count / 2;
    assert(adpcm_encode_block(
        &state, source, sample_count, encoded, sizeof(encoded)
    ) == encoded_bytes);
    assert(adpcm_decode_block(
        encoded, encoded_bytes, sample_count, decoded, ADPCM_BLOCK_SAMPLES
    ) == sample_count);
    assert(decoded[0] == source[0]);

    long long absolute_error = 0;
    for (size_t i = 0; i < sample_count; i++) {
        absolute_error += llabs((long long)source[i] - decoded[i]);
    }
    assert(absolute_error / (long long)sample_count < 1500);
}

int main(void)
{
    assert_round_trip(ADPCM_BLOCK_SAMPLES);
    assert_round_trip(800);
    return 0;
}
