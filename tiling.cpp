#include  <vpi_user.h>
#include "tiling.h"

#define WIDTH 512
#define HEIGHT 512

#define BUFFER_SIZE 16


float *ProjVtxBuffer;

typedef enum {
  IDLE = 0, 
  COLLECT = 1,
  COMPUTE = 2,
  WRITE_OUT = 3
} TilingState;


static int tile_module_compiletf(char* user_data) {
  return 0;
}

static int tile_module_calltf(char* user_data) {
 	
 	int m_TilingStart, m_Address, m_DataOut, m_DataIn, m_State, m_Counter, m_TileProjEn, m_Write;
 	vpiHandle systfref;
 	vpiHandle args_iter;

  	vpiHandle h_TilingStart, h_Address, h_DataOut, h_DataIn, h_State, h_Counter, h_TileProjEn, h_Write;
  	struct t_vpi_value val_TilingStart, val_Address, val_DataOut, val_DataIn, val_State, val_Counter, val_TileProjEn, val_Write;

  	// Parse Verilog Input  
  	systfref = vpi_handle(vpiSysTfCall, NULL);
  	args_iter = vpi_iterate(vpiArgument, systfref);

  	// Tiling Start Signal
  	h_TilingStart = vpi_scan(args_iter);
  	val_TilingStart.format = vpiIntVal;
  	vpi_get_value(h_TilingStart, &val_TilingStart);
  	m_TilingStart = val_TilingStart.value.integer;

  	// Address
  	h_Address = vpi_scan(args_iter);
  	val_Address.format = vpiIntVal;
  	vpi_get_value(h_Address, &val_Address);
  	m_Address = val_Address.value.integer;

  	// DataOut
  	h_DataOut = vpi_scan(args_iter);
  	val_DataOut.format = vpiIntVal;
  	vpi_get_value(h_DataOut, &val_DataOut);
  	m_DataOut = val_DataOut.value.integer;

  	// DataIn
  	h_DataIn = vpi_scan(args_iter);
  	val_DataIn.format = vpiIntVal;
 	vpi_get_value(h_DataIn, &val_DataIn);
  	m_DataIn = val_DataIn.value.integer;
  
  	// State 
  	h_State = vpi_scan(args_iter);
  	val_State.format = vpiIntVal;
  	vpi_get_value(h_State, &val_State);
  	m_State = val_State.value.integer;

  	// State Counter
  	h_Counter = vpi_scan(args_iter);
  	val_Counter.format = vpiIntVal;
  	vpi_get_value(h_Counter, &val_Counter);
  	m_Counter = val_Counter.value.integer;

  	// Proj Vtx Enable
  	h_TileProjEn = vpi_scan(args_iter);
  	val_TileProjEn.format = vpiIntVal;
  	vpi_get_value(h_TileProjEn, &val_TileProjEn);
  	m_TileProjEn = val_TileProjEn.value.integer;

  	// Write Signal
  	h_Write = vpi_scan(args_iter);
  	val_Write.format = vpiIntVal;
  	vpi_get_value(h_Write, &val_Write);
  	m_Write = val_Write.value.integer;

  	switch(m_State) {
		case IDLE:{
			if(m_TilingStart == 1) {
        		val_State.value.integer = COLLECT;
      		}

			break;
		}
		case COLLECT:{
			val_State.value.integer = COMPUTE;
			break;
		}
		case COMPUTE:{
			val_State.value.integer = WRITE_OUT;
			break;
		}
		case WRITE_OUT:{
			val_State.value.integer = IDLE;
			break;
		}
		default:{
      		vpi_printf("What the hell!\n");
      		break;
    	}
    }


	vpi_put_value(h_TilingStart, &val_TilingStart, NULL, vpiNoDelay);
	vpi_put_value(h_Address, &val_Address, NULL, vpiNoDelay);
	vpi_put_value(h_DataOut, &val_DataOut, NULL, vpiNoDelay);
	vpi_put_value(h_DataIn, &val_DataIn, NULL, vpiNoDelay);
	vpi_put_value(h_State, &val_State, NULL, vpiNoDelay);
	vpi_put_value(h_Counter, &val_Counter, NULL, vpiNoDelay);
	vpi_put_value(h_TileProjEn, &val_TileProjEn, NULL, vpiNoDelay);
	vpi_put_value(h_Write, &val_Write, NULL, vpiNoDelay);
	


  	vpi_free_object(args_iter);
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

  ProjVtxBuffer = (float*)malloc(BUFFER_SIZE * sizeof(float));
}