#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 10
extern int *Buffer;

static int module_compiletf(char* user_data);
static int module_calltf(char* user_data);
void module_register();