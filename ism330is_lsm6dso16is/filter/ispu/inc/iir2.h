#ifndef _IIR2_H_
#define _IIR2_H_

struct iir2 {
	float b[3];
	float a[3];
	float w[3];
};

void iir2_init(struct iir2 *f, const float b[3], const float a[3]);
float iir2_run(struct iir2 *f, float x);

#endif

