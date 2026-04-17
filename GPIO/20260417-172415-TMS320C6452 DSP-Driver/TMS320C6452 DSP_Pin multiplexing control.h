#ifndef TMS320C6452_DSP_PIN_MULTIPLEXING_CONTROL_H
#define TMS320C6452_DSP_PIN_MULTIPLEXING_CONTROL_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TMS320C6452_DSP_PINMUX_STATUS_UNKNOWN 1

#define TMS320C6452_DSP_PINMUX_OK             (0)
#define TMS320C6452_DSP_PINMUX_ERR_UNKNOWN   (-2)

typedef struct {
    uint32_t pin_id;
    uint32_t function;
} TMS320C6452_DSP_pinmux_request_t;

int  TMS320C6452_DSP_Pin_multiplexing_control_init(void);
bool TMS320C6452_DSP_Pin_multiplexing_control_is_ready(void);
int  TMS320C6452_DSP_Pin_multiplexing_control_apply(const TMS320C6452_DSP_pinmux_request_t *req);
int  TMS320C6452_DSP_Pin_multiplexing_control_readback(uint32_t pin_id, uint32_t *out_function);

#ifdef __cplusplus
}
#endif

#endif /* TMS320C6452_DSP_PIN_MULTIPLEXING_CONTROL_H */
