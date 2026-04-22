#include <stdio.h>
#include <stdint.h>
#include "tms320c6452_dsp_write_output_out_data01.h"

static void pr(const char *tag, int rc)
{
    printf("%s rc=%d\n", tag, rc);
}

int main(void)
{
    tms320c6452_dsp_out_ctx_t ctx;
    uint32_t v = 0u;
    int rc;

    rc = tms320c6452_dsp_write_output_out_data01_init(&ctx, TMS320C6452_DSP_GPIO_BASE_DEFAULT);
    pr("init", rc);
    if (rc != 0) { return rc; }

    rc = tms320c6452_dsp_write_output_out_data01_write_all(&ctx, 0x00000000u);
    pr("write_all_zero", rc);

    rc = tms320c6452_dsp_write_output_out_data01_update_mask(&ctx, 0x0000000Fu, 0x0000000Au); /* pins 0..3 = 1010 */
    pr("update_mask", rc);

    rc = tms320c6452_dsp_write_output_out_data01_read_all(&ctx, &v);
    pr("read_all", rc);
    printf("OUT_DATA01=0x%08X (expect 0x0000000A)\n", v);

    rc = tms320c6452_dsp_write_output_out_data01_write_pin(&ctx, 3u, 1u);
    pr("write_pin3_high", rc);

    rc = tms320c6452_dsp_write_output_out_data01_read_pin(&ctx, 3u, &v);
    pr("read_pin3", rc);
    printf("pin3=%u (expect 1)\n", v);

    rc = tms320c6452_dsp_write_output_out_data01_write_pin(&ctx, 3u, 0u);
    pr("write_pin3_low", rc);

    rc = tms320c6452_dsp_write_output_out_data01_read_pin(&ctx, 3u, &v);
    pr("read_pin3_again", rc);
    printf("pin3=%u (expect 0)\n", v);

    rc = tms320c6452_dsp_write_output_out_data01_deinit(&ctx);
    pr("deinit", rc);

    return 0;
}
