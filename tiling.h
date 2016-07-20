#include <stdio.h>
#include <stdlib.h>


#define WIDTH 512
#define HEIGHT 512

#define BUFFER_SIZE 16


typedef enum {
  IDLE = 0, 
  COLLECT = 1,
  COMPUTE = 2,
  WRITE_OUT = 3
} TilingState;


static int tile_module_compiletf(char* user_data);
static int tile_module_calltf(char* user_data);
void tile_module_register();