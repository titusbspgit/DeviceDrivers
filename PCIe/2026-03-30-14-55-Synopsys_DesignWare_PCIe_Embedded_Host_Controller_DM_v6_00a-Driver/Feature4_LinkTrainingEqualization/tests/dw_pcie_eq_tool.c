/* SPDX-License-Identifier: BSD-3-Clause */
/* User-space test app interacting with kernel sysfs/debugfs for EQ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

static int rd(const char *p, char *buf, size_t n){ FILE *f=fopen(p,"rb"); size_t r; if(!f)return -errno; r=fread(buf,1,n-1,f); fclose(f); if(!r)return -EIO; buf[r]='\0'; return 0; }
static int wr(const char *p, const char *v){ FILE *f=fopen(p,"wb"); size_t r; if(!f)return -errno; r=fwrite(v,1,strlen(v),f); fclose(f); return (r==strlen(v))?0:-EIO; }

int main(int argc, char **argv){
    if (argc < 2) { fprintf(stderr, "Usage: %s <BDF> [--get] [--fs=N --lf=N] [--pset idx pre cur post] [--pvec mask] [--recovery]\n", argv[0]); return 1; }
    const char *bdf = argv[1]; char base[256], path[512], buf[1024];
    snprintf(base,sizeof(base),"/sys/bus/pci/devices/%s/eq_dbg", bdf);

    for (int i=2;i<argc;i++) if (strcmp(argv[i],"--get")==0){
        snprintf(path,sizeof(path),"%s/gen3_related", base); if(rd(path,buf,sizeof(buf))==0) printf("gen3_related: %s",buf);
        snprintf(path,sizeof(path),"%s/local_fs_lf", base); if(rd(path,buf,sizeof(buf))==0) printf("local_fs_lf: %s",buf);
        snprintf(path,sizeof(path),"%s/coeff_legal_status", base); if(rd(path,buf,sizeof(buf))==0) printf("coeff_legal_status: %s",buf);
        /* dump presets */
        for (int p=0;p<=10;p++){ char v[32]; snprintf(path,sizeof(path),"%s/preset_index", base); snprintf(v,sizeof(v),"%d", p); (void)wr(path,v);
            snprintf(path,sizeof(path),"%s/preset_tuple", base); if(rd(path,buf,sizeof(buf))==0) printf("PSET[%d]: %s", p, buf);
        }
    }

    for (int i=2;i<argc;i++) if (strcmp(argv[i],"--recovery")==0){ snprintf(path,sizeof(path),"%s/force_recovery", base); (void)wr(path,"1"); printf("Requested Recovery; waiting in kernel...\n"); }

    for (int i=2;i<argc;i++) if (strncmp(argv[i],"--fs=",5)==0){
        const char *fs=&argv[i][5]; const char *lfv="0"; for (int j=2;j<argc;j++) if (strncmp(argv[j],"--lf=",5)==0) { lfv=&argv[j][5]; break; }
        snprintf(path,sizeof(path),"%s/local_fs_lf", base); char v[64]; snprintf(v,sizeof(v),"%s %s", fs, lfv); if(wr(path,v)==0) printf("Set FS/LF -> %s\n", v);
    }

    for (int i=2;i<argc;i++) if (strcmp(argv[i],"--pset")==0 && i+4<argc){
        snprintf(path,sizeof(path),"%s/preset_index", base); (void)wr(path, argv[i+1]);
        snprintf(path,sizeof(path),"%s/preset_tuple", base); char v[128]; snprintf(v,sizeof(v),"%s %s %s", argv[i+2], argv[i+3], argv[i+4]); if(wr(path,v)==0) printf("Programmed preset[%s] -> %s\n", argv[i+1], v);
    }

    for (int i=2;i<argc;i++) if (strcmp(argv[i],"--pvec")==0 && i+1<argc){ snprintf(path,sizeof(path),"%s/preset_vector", base); if(wr(path, argv[i+1])==0) printf("Set preset vector -> %s\n", argv[i+1]); }

    return 0; }
