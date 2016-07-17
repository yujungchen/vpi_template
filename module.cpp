#include  <vpi_user.h>
#include "module.h"


Vec3 VireportTran(Vec3 Vtx) {
  Vec3 Proj_Vtx;
  Proj_Vtx.x = 0.0f;
  Proj_Vtx.y = 0.0f;
  Proj_Vtx.z = 0.0f;
  float HalfWidth = (float)WIDTH / 2.0f;
  float HalfHeight = (float)HEIGHT / 2.0f;

  Proj_Vtx.x = Vtx.x * HalfWidth + HalfWidth;
  Proj_Vtx.y = Vtx.y * HalfHeight + HalfHeight;

  return Proj_Vtx;
}

void ComputeCoefficient(float *x10, float *x20, float *y10, float *y20, float *det) {

  Vec3 Vtx[3];
  Vec3 Proj_Vtx[3];
  Vtx[0].x = Buffer[0];
  Vtx[0].y = Buffer[1];
  Vtx[0].z = Buffer[2];

  Vtx[1].x = Buffer[3];
  Vtx[1].y = Buffer[4];
  Vtx[1].z = Buffer[5];

  Vtx[2].x = Buffer[6];
  Vtx[2].y = Buffer[7];
  Vtx[2].z = Buffer[8];

  Proj_Vtx[0] = VireportTran(Vtx[0]);
  Proj_Vtx[1] = VireportTran(Vtx[1]);
  Proj_Vtx[2] = VireportTran(Vtx[2]);

  *x10 = Proj_Vtx[1].x - Proj_Vtx[0].x;
  *x20 = Proj_Vtx[2].x - Proj_Vtx[0].x;

  *y10 = Proj_Vtx[1].y - Proj_Vtx[0].y;
  *y20 = Proj_Vtx[2].y - Proj_Vtx[0].y;

  *det = (*x10) * (*y20) - (*x20) * (*y10);
}




static int module_compiletf(char* user_data) {
  return 0;
}

static int module_calltf(char* user_data) {

  int m_Address, m_DataIn, m_State, m_Counter, m_Start, m_Request;
  vpiHandle systfref;
  vpiHandle args_iter;

  vpiHandle h_Address, h_DataIn, h_State, h_Counter, h_Start, h_Request;
  struct t_vpi_value val_Address, val_DataIn, val_State, val_Counter, val_Start, val_Request;

  // Parse Verilog Input  
  systfref = vpi_handle(vpiSysTfCall, NULL);
  args_iter = vpi_iterate(vpiArgument, systfref);

  // Address
  h_Address = vpi_scan(args_iter);
  val_Address.format = vpiIntVal;
  vpi_get_value(h_Address, &val_Address);
  m_Address = val_Address.value.integer;

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

  // Start Signal
  h_Start = vpi_scan(args_iter);
  val_Start.format = vpiIntVal;
  vpi_get_value(h_Start, &val_Start);
  m_Start = val_Start.value.integer;

  // Request Signal
  h_Request = vpi_scan(args_iter);
  val_Request.format = vpiIntVal;
  vpi_get_value(h_Request, &val_Request);
  m_Request = val_Request.value.integer;
  // Parse Verilog Input

  //vpi_printf("\nCall module done. %d\n", argval_1.value.integer);

  float *InputCast;

  switch(m_State) {
    case IDLE:{

      if(m_Start == 1) {
        val_State.value.integer = 1;
      }
      else {
        val_State.value.integer = 0;
      }

      break;
    }
    case COLLECT:{
        int CurrentValue = m_Counter;

        
        if(CurrentValue == 0 && val_Start.value.integer == 1) { 
          val_Start.value.integer = 0;
          val_Request.value.integer = 1;
          break;
        }

        InputCast = (float*)(&m_DataIn);
        vpi_printf("Address %d Data In: %f\n", CurrentValue, *InputCast);
        Buffer[CurrentValue] = *InputCast;

        if(CurrentValue == READ_DATA_CYCLE) {
          val_State.value.integer = 2;
          val_Request.value.integer = 0;
          val_Counter.value.integer = 0;
        }
        else {          
          CurrentValue = CurrentValue + 1;
          val_Counter.value.integer = CurrentValue;
          val_Address.value.integer = CurrentValue;
        }
      
      break;
    }
    case COMPUTE:{
      int CurrentValue = m_Counter;
        
      if(CurrentValue == 0)
        vpi_printf("\nCompute Barycentric Phase.\n");  


      if(CurrentValue == COMPUTE_CYCLE){
        val_State.value.integer = 0;
        val_Request.value.integer = 0;
        val_Counter.value.integer = 0;
      }

      vpi_printf("Address %d Data In: %f\n", CurrentValue, Buffer[CurrentValue]);

      CurrentValue = CurrentValue + 1;
      val_Counter.value.integer = CurrentValue;



      break;
    }
    default:{
      vpi_printf("What the hell!\n");
      break;
    }

  } 


  vpi_put_value(h_Address, &val_Address, NULL, vpiNoDelay);
  vpi_put_value(h_DataIn, &val_DataIn, NULL, vpiNoDelay);
  vpi_put_value(h_State, &val_State, NULL, vpiNoDelay);
  vpi_put_value(h_Counter, &val_Counter, NULL, vpiNoDelay);
  vpi_put_value(h_Start, &val_Start, NULL, vpiNoDelay);
  vpi_put_value(h_Request, &val_Request, NULL, vpiNoDelay);
  
  vpi_free_object(args_iter);

  return 0;
}

void module_register() {
  
  s_vpi_systf_data tf_data;
  
  tf_data.type = vpiSysTask;
  tf_data.tfname = "$CallModule";
  tf_data.calltf = module_calltf;
  tf_data.compiletf = module_compiletf;
  tf_data.sizetf = 0;
  tf_data.user_data = 0;
  vpi_register_systf(&tf_data);

  Buffer = (float*)malloc(BUFFER_SIZE * sizeof(float));
  for(int idx = 0 ; idx < BUFFER_SIZE ; idx ++){
    Buffer[idx] = 0.0f;
  }

}