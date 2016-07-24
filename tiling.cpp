#include  <vpi_user.h>
#include "tiling.h"

#define READ_DATA_CYCLE 16
#define COMPUTE_CYCLE 16
#define WRITE_DATA_CYCLE 8

#define BUFFER_SIZE 16

float *ProjVtxBuffer;
float _x10;
float _x20;
float _y10;
float _y20;
float _det;
Vec3 ProjVtx[3];

float Max_X;
float Max_Y;
float Min_X;
float Min_Y;

int StartTileX;
int StartTileY;
int EndTileX;
int EndTileY;

int Tid_X;
int Tid_Y;

typedef enum {
  IDLE = 0, 
  COLLECT = 1,
  COMPUTE_BOUND = 2,
  COMPUTE_TID = 3
} TilingState;


static int tile_module_compiletf(char* user_data) {
  return 0;
}

static int tile_module_calltf(char* user_data) {
 	
 	  int m_TilingStart, m_Address, m_DataOut, m_DataIn, m_State, m_Counter, m_TileProjEn, m_Write, m_Sel;
 	  vpiHandle systfref;
 	  vpiHandle args_iter;

  	vpiHandle h_TilingStart, h_Address, h_DataOut, h_DataIn, h_State, h_Counter, h_TileProjEn, h_Write, h_Sel;
  	struct t_vpi_value val_TilingStart, val_Address, val_DataOut, val_DataIn, val_State, val_Counter, val_TileProjEn, val_Write, val_Sel;

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

    // Sel Signal
    h_Sel = vpi_scan(args_iter);
    val_Sel.format = vpiIntVal;
    vpi_get_value(h_Sel, &val_Sel);
    m_Sel = val_Sel.value.integer;

  	switch(m_State) {
		  case IDLE:{
        if(m_TilingStart == 1) {
          val_State.value.integer = COLLECT;
          //val_Sel.value.integer = 1;
          Tid_X = 0;
          Tid_Y = 0;
        }
        else {
          val_State.value.integer = 0;
          val_Sel.value.integer = 0;
        }
        
        val_DataOut.value.integer = 0;
        val_Address.value.integer = 0;
        val_Counter.value.integer = 0;

        break;
		  }
      case COLLECT:{

        
        int CurrentValue = m_Counter;

        if(CurrentValue == 0 && val_TilingStart.value.integer == 1) { 
          val_TilingStart.value.integer = 0;
          val_TileProjEn.value.integer = 1;
          val_Sel.value.integer = 1;
          break;
        }
        
        val_TileProjEn.value.integer = 1;

        float *FormatCast;
        FormatCast = (float*)(&m_DataIn);
        //vpi_printf("Address %d Data In: %f\n", CurrentValue, *FormatCast);
        ProjVtxBuffer[CurrentValue] = *FormatCast;
        CurrentValue = CurrentValue + 1;

        if(CurrentValue < READ_DATA_CYCLE) {  
          val_Counter.value.integer = CurrentValue;
          val_Address.value.integer = CurrentValue;
        }
        else {          
          val_State.value.integer = COMPUTE_BOUND;
          val_TileProjEn.value.integer = 0;
          val_Counter.value.integer = 0;
          val_Address.value.integer = 0;
        }
        //val_State.value.integer = COMPUTE_COEF;
        break;
		  }
		  case COMPUTE_BOUND:{
        for(int idx = 0 ; idx < BUFFER_SIZE ; idx++){
          vpi_printf("Address %d Data In: %f\n", idx, ProjVtxBuffer[idx]);
        }

        ProjVtx[0].x = ProjVtxBuffer[0];
        ProjVtx[0].y = ProjVtxBuffer[1];
        ProjVtx[0].z = ProjVtxBuffer[2];
        
        ProjVtx[1].x = ProjVtxBuffer[3];
        ProjVtx[1].y = ProjVtxBuffer[4];
        ProjVtx[1].z = ProjVtxBuffer[5];

        ProjVtx[2].x = ProjVtxBuffer[6];
        ProjVtx[2].y = ProjVtxBuffer[7];
        ProjVtx[2].z = ProjVtxBuffer[8];

        _x10 = ProjVtxBuffer[9];
        _x20 = ProjVtxBuffer[10];
        _y10 = ProjVtxBuffer[11];
        _y20 = ProjVtxBuffer[12];
        _det = ProjVtxBuffer[13];
        
        // Compute Bound
        Max_X = (ProjVtx[0].x > ProjVtx[1].x) ? ProjVtx[0].x : ProjVtx[1].x;
        Max_X = (Max_X > ProjVtx[2].x) ? Max_X : ProjVtx[2].x;

        Max_Y = (ProjVtx[0].y > ProjVtx[1].y) ? ProjVtx[0].y : ProjVtx[1].y;
        Max_Y = (Max_Y > ProjVtx[2].y) ? Max_Y : ProjVtx[2].y;

        Min_X = (ProjVtx[0].x <= ProjVtx[1].x) ? ProjVtx[0].x : ProjVtx[1].x;
        Min_X = (Min_X <= ProjVtx[2].x) ? Min_X : ProjVtx[2].x;

        Min_Y = (ProjVtx[0].y <= ProjVtx[1].y) ? ProjVtx[0].y : ProjVtx[1].y;
        Min_Y = (Min_Y <= ProjVtx[2].y) ? Min_Y : ProjVtx[2].y;
        vpi_printf("(%f %f) (%f %f)\n", Max_X, Max_Y, Min_X, Min_Y);

        // Compute Tile NumBound
        StartTileX = (int)(Min_X / (float)TILE_SIZE);
        StartTileY = (int)(Min_Y / (float)TILE_SIZE);
        EndTileX = (int)(Max_X / (float)TILE_SIZE) + 1;
        EndTileY = (int)(Max_Y / (float)TILE_SIZE) + 1;
        vpi_printf("(%d %d) (%d %d)\n", StartTileX, EndTileX, StartTileY, EndTileY);

        Tid_X = StartTileX;
        Tid_Y = StartTileY;

        val_State.value.integer = COMPUTE_TID;
        break;
		  }
		  case COMPUTE_TID:{


        if(Tid_X < EndTileX && Tid_Y < EndTileY){
          val_State.value.integer = COMPUTE_TID;
          vpi_printf("(%d %d)\n", Tid_X, Tid_Y);
          
          Tid_X++;
          if(Tid_X == EndTileX){
            Tid_X = StartTileX;
            Tid_Y++;
          }
        }
        else{
          val_State.value.integer = IDLE;
        }
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
    vpi_put_value(h_Sel, &val_Sel, NULL, vpiNoDelay);
    
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

  for(int idx = 0 ; idx < BUFFER_SIZE ; idx++){
    ProjVtxBuffer[idx] = 0.0f;
  }
}