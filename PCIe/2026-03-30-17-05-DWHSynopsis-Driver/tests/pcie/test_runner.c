#include <stdio.h>

int test_flit(void);
int test_pipe_phy(void);
int test_spcie(void);
int test_ltssm_eq(void);
int test_aspm(void);

int main(void)
{
    int pass=0, fail=0; int rc;
    rc = test_flit();     if (rc==0) { pass++; } else { fail++; }
    rc = test_pipe_phy(); if (rc==0) { pass++; } else { fail++; }
    rc = test_spcie();    if (rc==0) { pass++; } else { fail++; }
    rc = test_ltssm_eq(); if (rc==0) { pass++; } else { fail++; }
    rc = test_aspm();     if (rc==0) { pass++; } else { fail++; }

    printf("TEST SUMMARY: pass=%d fail=%d\n", pass, fail);
    return (fail==0)?0:1;
}
