#include  <vpi_user.h>
#include "tiling.h"


static int tile_module_compiletf(char* user_data) {
  return 0;
}

static int tile_module_calltf(char* user_data) {
  vpi_printf("\n\nTiling\n");
  return 0;
}

void tile_module_register() {
  s_vpi_systf_data tf_data;
  
  tf_data.type = vpiSysTask;
  tf_data.tfname = "$Tiling";
  tf_data.calltf = tile_module_calltf;
  tf_data.compiletf = tile_module_compiletf;
  tf_data.sizetf = 0;
  tf_data.user_data = 0;
  vpi_register_systf(&tf_data);
}