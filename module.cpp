#include  <vpi_user.h>
#include "module.h"

#define READ_DATA_CYCLE 8
#define COMPUTE_CYCLE 16
#define WRITE_DATA_CYCLE 8

#define WIDTH 512
#define HEIGHT 512

#define BUFFER_SIZE 16

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

void ComputeCoefficient(float *_x10, float *_x20, float *_y10, float *_y20, float *_det) {

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

  
  ProjBuffer[0] = Proj_Vtx[0].x;
  ProjBuffer[1] = Proj_Vtx[0].y;
  ProjBuffer[2] = Proj_Vtx[0].z;
  
  ProjBuffer[3] = Proj_Vtx[1].x;
  ProjBuffer[4] = Proj_Vtx[1].y;
  ProjBuffer[5] = Proj_Vtx[1].z;
  
  ProjBuffer[6] = Proj_Vtx[2].x;
  ProjBuffer[7] = Proj_Vtx[2].y;
  ProjBuffer[8] = Proj_Vtx[2].z;
  
  *_x10 = Proj_Vtx[1].x - Proj_Vtx[0].x;
  *_x20 = Proj_Vtx[2].x - Proj_Vtx[0].x;

  *_y10 = Proj_Vtx[1].y - Proj_Vtx[0].y;
  *_y20 = Proj_Vtx[2].y - Proj_Vtx[0].y;

  *_det = (*_x10) * (*_y20) - (*_x20) * (*_y10);
  //vpi_printf("%f %f %f %f %f\n", *x10, *x20, *y10, *y20, *det);
}




static int module_compiletf(char* user_data) {
  return 0;
}

static int module_calltf(char* user_data) {

  int m_Address, m_DataIn, m_State, m_Counter, m_Start, m_VtxBuf_En, m_WriteProjEn, m_DataOut, m_Write, m_TilingStart;
  vpiHandle systfref;
  vpiHandle args_iter;

  vpiHandle h_Address, h_DataIn, h_State, h_Counter, h_Start, h_VtxBuf_En, h_WriteProjEn, h_DataOut, h_Write, h_TilingStart;
  struct t_vpi_value val_Address, val_DataIn, val_State, val_Counter, val_Start, val_VtxBuf_En, val_WriteProjEn, val_DataOut, val_Write, val_TilingStart;

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

  // Vtx Buffer Signal
  h_VtxBuf_En = vpi_scan(args_iter);
  val_VtxBuf_En.format = vpiIntVal;
  vpi_get_value(h_VtxBuf_En, &val_VtxBuf_En);
  m_VtxBuf_En = val_VtxBuf_En.value.integer;

  // Proj Vtx Enable
  h_WriteProjEn = vpi_scan(args_iter);
  val_WriteProjEn.format = vpiIntVal;
  vpi_get_value(h_WriteProjEn, &val_WriteProjEn);
  m_WriteProjEn = val_WriteProjEn.value.integer;

  // DataOut
  h_DataOut = vpi_scan(args_iter);
  val_DataOut.format = vpiIntVal;
  vpi_get_value(h_DataOut, &val_DataOut);
  m_DataOut = val_DataOut.value.integer;

  // Write Signal
  h_Write = vpi_scan(args_iter);
  val_Write.format = vpiIntVal;
  vpi_get_value(h_Write, &val_Write);
  m_Write = val_Write.value.integer;

  // Tiling Start Signal
  h_TilingStart = vpi_scan(args_iter);
  val_TilingStart.format = vpiIntVal;
  vpi_get_value(h_TilingStart, &val_TilingStart);
  m_TilingStart = val_TilingStart.value.integer;
  
  // Parse Verilog Input

  //vpi_printf("\nCall module done. %d\n", argval_1.value.integer);
  float *InputCast;
  
  switch(m_State) {
    case IDLE:{

      if(m_Start == 1) {
        val_State.value.integer = COLLECT;
      }
      else {
        val_State.value.integer = 0;
      }
      val_Counter.value.integer = 0;
      val_DataOut.value.integer = 0;
      val_Address.value.integer = 0;
      val_TilingStart.value.integer = 0;
      break;
    }
    case COLLECT:{
        int CurrentValue = m_Counter;

        
        if(CurrentValue == 0 && val_Start.value.integer == 1) { 
          val_Start.value.integer = 0;
          val_VtxBuf_En.value.integer = 1;
          break;
        }

        InputCast = (float*)(&m_DataIn);
        //vpi_printf("Address %d Data In: %f\n", CurrentValue, *InputCast);
        Buffer[CurrentValue] = *InputCast;

        if(CurrentValue == READ_DATA_CYCLE) {
          val_State.value.integer = COMPUTE;
          val_VtxBuf_En.value.integer = 0;
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


      if(CurrentValue == COMPUTE_CYCLE - 1){
        val_State.value.integer = WRITE_OUT;
        val_WriteProjEn.value.integer = 1;
        val_Write.value.integer = 1;
        val_Counter.value.integer = 0;
        val_Address.value.integer = 0;
        CurrentValue = 0;

        int *FormatCast;
        FormatCast = (int*)(&ProjBuffer[0]);
        
        //vpi_printf("Address %d Data In: %f %d\n", CurrentValue, Buffer[CurrentValue], *FormatCast);

        ComputeCoefficient(&x10, &x20, &y10, &y20, &det);
        ProjBuffer[9] = x10;
        ProjBuffer[10] = x20;
        ProjBuffer[11] = y10;
        ProjBuffer[12] = y20;
        ProjBuffer[13] = det;
        //for(int idx = 0 ; idx < BUFFER_SIZE ; idx++){
          //FormatCast = (int*)(&ProjBuffer[idx]);
          //vpi_printf("Address %d Data In: %f %d\n", idx, ProjBuffer[idx], *FormatCast);
        //}

        val_DataOut.value.integer = *FormatCast;
        break;
      }

      CurrentValue = CurrentValue + 1;
      val_Counter.value.integer = CurrentValue;

      break;
    }
    case WRITE_OUT:{

      int CurrentValue = m_Counter;
      CurrentValue = CurrentValue + 1;

      int *FormatCast;
      FormatCast = (int*)(&ProjBuffer[CurrentValue]);
      val_DataOut.value.integer = *FormatCast;
      //vpi_printf("Address %d Data In: %f %d\n", CurrentValue, Buffer[CurrentValue], *FormatCast);

      val_Counter.value.integer = CurrentValue;
      val_Address.value.integer = CurrentValue;

      if(CurrentValue == 16) {
        val_State.value.integer = IDLE;
        val_VtxBuf_En.value.integer = 0;
        val_WriteProjEn.value.integer = 0;

        val_Counter.value.integer = 0;
        val_DataOut.value.integer = 0;
        val_Address.value.integer = 0;
        val_Write.value.integer = 0;
        val_TilingStart.value.integer = 1;
        //vpi_printf("%f %f %f %f %f\n", x10, x20, y10, y20, det);
        break;
      }


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
  vpi_put_value(h_VtxBuf_En, &val_VtxBuf_En, NULL, vpiNoDelay);
  vpi_put_value(h_WriteProjEn, &val_WriteProjEn, NULL, vpiNoDelay); //vpiInertialDelay
  vpi_put_value(h_DataOut, &val_DataOut, NULL, vpiNoDelay);
  vpi_put_value(h_Write, &val_Write, NULL, vpiNoDelay);
  vpi_put_value(h_TilingStart, &val_TilingStart, NULL, vpiNoDelay);


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
  ProjBuffer = (float*)malloc(BUFFER_SIZE * sizeof(float));
  for(int idx = 0 ; idx < BUFFER_SIZE ; idx ++){
    Buffer[idx] = 0.0f;
    ProjBuffer[idx] = 0.0f;
  }

}