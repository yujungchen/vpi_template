#include <stdio.h>
#include <stdlib.h>

#define READ_DATA_CYCLE 8
#define COMPUTE_CYCLE 16
#define WRITE_DATA_CYCLE 8

#define WIDTH 512
#define HEIGHT 512

#define BUFFER_SIZE 16

extern float *Buffer;
extern float *ProjBuffer;
extern float x10, x20, y10, y20, det;

typedef enum {
  IDLE = 0, 
  COLLECT = 1,
  COMPUTE = 2,
  WRITE_OUT = 3
} BarycentricState;

typedef struct _Vec3{
  float x;
  float y;
  float z;
} Vec3;

static int module_compiletf(char* user_data);
static int module_calltf(char* user_data);
void module_register();
Vec3 VireportTran(Vec3 Vtx);
void ComputeCoefficient(float *_x10, float *_x20, float *_y10, float *_y20, float *_det);