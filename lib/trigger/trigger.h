#ifndef TRIGGER_H_
#define TRIGGER_H_

#ifdef __cplusplus
 extern "C" {
#endif


#define TRIGGER_RECSTA_THRESHOLD  (1.75f)
#define TRIGGER_RECSTA_NSTA       64
#define TRIGGER_RECSTA_NLTA       128


typedef struct {
    float csta;
    float clta;
    float sta;
    float lta;
} recsta_t;


float recstalta(float a, recsta_t *rsta);

void recstalta_init(recsta_t *rsta, int nsta, int nlta);

int stalta(const float *data, float *charfct, int ndat, int nsta, int nlta);


#ifdef __cplusplus
}
#endif

#endif  /* TRIGGER_H_ */