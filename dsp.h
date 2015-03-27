// dsp.h
// Copied over from soundcard-demo.c ---  Miraj Patel

// Sample sound card code for use with Cygwin etc.
// Stuart Kirtman, 2013
//
// For additional information, see:
// http://linuxdevcenter.com/pub/a/linux/2007/08/02/an-introduction-to-linux-audio.html
// or print version at:
// http://linuxdevcenter.com/lpt/a/7099

#ifndef  DSP_H_GUARD
#define  DSP_H_GUARD

void dsp_open(char *dsp_name, int mode, int samp_fmt, int n_chan, int samp_rate);

int dsp_fd;		// File descr for sound card; used by several functions

//void dsp_write(void *data, size_t samp_size, size_t n_samps);

void dsp_sync(void);

void dsp_close(void);

#endif

#include <fcntl.h>
#include <sys/soundcard.h>



