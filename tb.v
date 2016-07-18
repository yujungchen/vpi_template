`timescale 1ns/1ns
`define CYCLE 10

module TopModule();
	
	/*
	0 for VtxBuffer (RTL)
	1 for Barycentric unit (C++)
	2 for ProjVtxBuffer (RTL)
	3 for CoefficientBuffer (RTL)
	4 for Tiling Unit (C++)
	5 for TileIndexBuffer (RTL)
	6 for Interpolation Unit (RTL)
	*/


	// System Signal
	reg m_rst;
	reg m_clk;
	reg m_Arbiter;
	reg [3:0] m_Address;
	reg [31:0] m_WriteData;
	reg [31:0] m_ReadData;
	reg m_Enable;
	
	wire [7:0] m_BusArbiterVec;

	// Barycentric Unit
	reg [3:0] m_BaryAddress;	
	reg [2:0] m_BaryMonitor;
	reg [7:0] m_BaryCounter;
	reg [31:0] m_BaryWriteData;
	reg m_BaryStart;
	reg m_BaryVtxBuf_En;
	reg m_BaryProjVtxBuf_En;
	reg m_BaryWrite;
	reg m_BarySel;

	// Pseudo Bus Master Assignment
	reg [3:0] m_PseudoAddress;
	reg [31:0] m_PseudoData;
	reg m_PseudoWrite;
	reg m_PseudoVtxBufSel;

	
	// InputBuffer
	wire [31:0] m_Buf_rData;
	reg m_Buf_En;
	reg m_Buf_Write;
	wire [1:0] m_Buf_Vec;

	// ProjVtxBuffer
	wire [31:0] m_ProjBuf_rData;
	reg m_ProjBuf_En;
	reg m_ProjBuf_Write;
	wire [1:0] m_ProjBuf_Vec;
	


	assign m_BusArbiterVec = {m_PseudoVtxBufSel, m_BarySel, 6'b0};

	vtxbuffer InputBuffer(.clk(m_clk), .reset(m_rst), .write(m_Buf_Write), .en(m_Buf_En), .Addr(m_Address), .wData(m_WriteData), .rData(m_Buf_rData));
	vtxbuffer ProjVtxBuffer(.clk(m_clk), .reset(m_rst), .write(m_ProjBuf_Write), .en(m_ProjBuf_En), .Addr(m_Address), .wData(m_WriteData), .rData(m_ProjBuf_rData));

	// Create Clock
	always begin 
		#(`CYCLE/2) m_clk <= ~m_clk; 
	end

	// Mux for Input Buffer
	assign m_Buf_Vec = {m_PseudoVtxBufSel, m_BaryVtxBuf_En};
	always @ (*) begin
		case (m_Buf_Vec)
			2'b10 : begin
				m_Buf_En = m_PseudoVtxBufSel;
				m_Buf_Write = m_PseudoWrite;
			end
			2'b01 : begin
				m_Buf_En = m_BaryVtxBuf_En;
				m_Buf_Write = m_BaryWrite;
			end
			default : begin
				m_Buf_En = 1'b0;
				m_Buf_Write = 1'b0;
			end
		endcase
	end
	
	// Mux for ProjVtx Buffer
	assign m_ProjBuf_Vec = {m_BaryProjVtxBuf_En, 1'b0};
	always @ (*) begin
		case (m_ProjBuf_Vec)
			2'b10 : begin
				m_ProjBuf_En = m_BaryProjVtxBuf_En;
				m_ProjBuf_Write = m_BaryWrite;
			end
			2'b01 : begin
				m_ProjBuf_En = 1'b0;
				m_ProjBuf_Write = 1'b0;
			end
			default : begin
				m_ProjBuf_En = 1'b0;
				m_ProjBuf_Write = 1'b0;
			end
		endcase
	end
	
	// Assign arbitration
	always @ (*) begin
		case (m_BusArbiterVec)
			8'b10000000 : begin 
				m_Address = m_PseudoAddress;
				m_WriteData = m_PseudoData;
			end
			8'b01000000 : begin
				m_Address = m_BaryAddress;
				m_ReadData = m_Buf_rData;
				m_WriteData = m_BaryWriteData;
			end
			default : begin
				m_Address = 4'b0;
				m_ReadData = 32'b0;
			end
		endcase
	end

	// Trigger function
	always @ (posedge m_clk) begin
		if(!m_rst) begin
			//$display("%dns", $time);
			$CallModule(m_BaryAddress, m_ReadData, m_BaryMonitor, m_BaryCounter, m_BaryStart, m_BaryVtxBuf_En, m_BaryProjVtxBuf_En, m_BaryWriteData, m_BaryWrite);
		end
	end

	// Fire
	initial begin
		$dumpfile("signal.vcd");
		$dumpvars(0, TopModule);

		m_clk = 1'b0;
		m_rst = 1'b1;
		m_BaryVtxBuf_En = 1'b0;
		m_BaryMonitor = 2'b0;
		m_BaryCounter = 7'b0;
		m_BaryStart = 1'b0;
		m_BaryProjVtxBuf_En = 1'b0;
		m_BaryWriteData = 32'b0;
		m_BaryWrite = 1'b0;
		m_BarySel = 1'b0;


		m_PseudoVtxBufSel = 1'b0;
		m_PseudoWrite = 1'b0;
		$Test_Connection;

		#(`CYCLE)	m_rst = 1'b0;

		// Write Data into Vtx Buffer
		#15 		m_Enable = 1'b1;
					m_PseudoWrite = 1'b1;
					m_PseudoVtxBufSel = 1'b1;
					//Vtx1
					m_PseudoAddress = 4'd0;
					m_PseudoData = 32'h3f000000;
		#(`CYCLE)	m_PseudoAddress = 4'd1;
					m_PseudoData = 32'hbf333333;
		#(`CYCLE)	m_PseudoAddress = 4'd2;
					m_PseudoData = 32'h00000000;
					//Vtx2
		#(`CYCLE)	m_PseudoAddress = 4'd3;
					m_PseudoData = 32'h3f000000;
		#(`CYCLE)	m_PseudoAddress = 4'd4;
					m_PseudoData = 32'h3f666666;
		#(`CYCLE)	m_PseudoAddress = 4'd5;
					m_PseudoData = 32'h00000000;
					//Vtx3
		#(`CYCLE)	m_PseudoAddress = 4'd6;
					m_PseudoData = 32'hbf333333;
		#(`CYCLE)	m_PseudoAddress = 4'd7;
					m_PseudoData = 32'hbf000000;
		#(`CYCLE)	m_PseudoAddress = 4'd8;
					m_PseudoData = 32'h00000000;
		#(`CYCLE)	m_PseudoWrite = 1'b0;
					
		#(`CYCLE)	m_PseudoVtxBufSel = 1'b0;
		// Start Compuitng Barycentrci Coefficient
		#(`CYCLE)	m_BaryStart = 1'b1;
					m_BarySel = 1'b1;
						

		

		#1000 $finish;
	end

endmodule