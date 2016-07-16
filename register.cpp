#include  <vpi_user.h>
#include "module.h"


float *Buffer;


static int fn_compiletf(char* user_data) {
  return 0;
}

static int fn_calltf(char* user_data) {
  vpi_printf("\n\nConnection Established\n");
  return 0;
}

void fn_register() {
  s_vpi_systf_data tf_data;
  
  tf_data.type = vpiSysTask;
  tf_data.tfname = "$Test_Connection";
  tf_data.calltf = fn_calltf;
  tf_data.compiletf = fn_compiletf;
  tf_data.sizetf = 0;
  tf_data.user_data = 0;
  vpi_register_systf(&tf_data);
}

void (*vlog_startup_routines[])() = {
  fn_register,
  module_register,
  0
};