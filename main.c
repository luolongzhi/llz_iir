/*
  llzlab - luolongzhi algorithm lab 
  Copyright (C) 2013 luolongzhi 罗龙智 (Chengdu, China)

  This program is part of llzlab, all copyrights are reserved by luolongzhi. 

  filename: main.c 
  time    : 2012/07/08 18:33 
  author  : luolongzhi ( luolongzhi@gmail.com )
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include "llz_wavfmt.h"
#include "llz_parseopt.h"
#include "llz_iir.h"

#if 1 
#define NA  1 
#define NB  2 
#else 
#define NA  2 
#define NB  2 
#endif

int main(int argc, char *argv[])
{
    int ret;
    int frame_index = 0;

	FILE  * destfile;
	FILE  * sourcefile;
	llz_wavfmt_t fmt;

	int i;
    int is_last = 0;
    int read_len = 0;
    int write_total_size= 0;

    uintptr_t  h_flt;
    double a[NA+1];
    double b[NB+1];

	short wavsamples_in[FRAME_SIZE_MAX];
	short wavsamples_out[FRAME_SIZE_MAX];
	double buf_in[FRAME_SIZE_MAX];
    double buf_out[FRAME_SIZE_MAX];


    ret = llz_parseopt(argc, argv);
    if(ret) return -1;

    if ((destfile = fopen(opt_outputfile, "w+b")) == NULL) {
		printf("output file can not be opened\n");
		return 0; 
	}                         

	if ((sourcefile = fopen(opt_inputfile, "rb")) == NULL) {
		printf("input file can not be opened;\n");
		return 0; 
    }

    fmt = llz_wavfmt_readheader(sourcefile);
    fseek(sourcefile,44,0);
    fseek(destfile  ,  0, SEEK_SET);
    llz_wavfmt_writeheader(fmt, destfile);

    /*init filter*/
#if 1 
    b[0] = 1.0;
    b[1] = -0.97;
    a[0] = 1.;
#else
    b[0] = 0.1311;
    b[1] = -0.2622;
    b[2] = 0.1311;
    a[0] = 1.;
    a[1] = 0.7478;
    a[2] = 0.2722;
#endif

    h_flt = llz_iir_filter_init(NA, a, NB, b);

    while(1)
    {
        if(is_last)
            break;

        memset(wavsamples_in, 0, 2*opt_framelen);
        read_len = fread(wavsamples_in, 2, opt_framelen, sourcefile);
        if(read_len < opt_framelen)
            is_last = 1;
       
		for(i = 0 ; i < read_len; i++) {
			buf_in[i] = (double)wavsamples_in[i]/32768;
			buf_out[i] = 0;
		}
		
		llz_iir_filter(h_flt, buf_in, buf_out, read_len);

		for(i = 0 ; i < opt_framelen; i++) {
			double temp;
			temp = buf_out[i] * 32768;

			if (temp >= 32767)
				temp = 32767;
			if (temp < -32768)
				temp = -32768;

			wavsamples_out[i] = temp;
		}

        fwrite(wavsamples_out, 2, opt_framelen, destfile);

        write_total_size += 2 * opt_framelen;

        frame_index++;
        printf("the frame = [%d]\r", frame_index);
    }

    ret = llz_iir_filter_flush(h_flt, buf_out);
    for(i = 0 ; i < ret; i++) {
        double temp;
        temp = buf_out[i] * 32768;

        if (temp >= 32767)
            temp = 32767;
        if (temp < -32768)
            temp = -32768;

        wavsamples_out[i] = temp;
    }

    fwrite(wavsamples_out, 2, ret, destfile);

    write_total_size += 2*ret;

    frame_index++;
    printf("the frame = [%d]\n", frame_index);

    fmt.data_size=write_total_size/fmt.block_align;
    fseek(destfile, 0, SEEK_SET);
    llz_wavfmt_writeheader(fmt,destfile);

	llz_iir_filter_uninit(h_flt);
    fclose(sourcefile);
    fclose(destfile);

    return 0;
}
