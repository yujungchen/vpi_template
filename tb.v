`timescale 1ns/1ns
`define CYCLE 10

module TopModule();

	// System Signal
	reg m_rst;
	reg m_clk;
	wire m_Arbiter;	// 0 for Buffer, 1 for Barycentric unit
	reg [3:0] m_Address;
	reg [31:0] m_WriteData;
	reg [31:0] m_ReadData;
	reg m_Enable;
	reg m_Write;

	// Barycentric Unit
	reg [3:0] m_BaryAddress;	
	reg [1:0] m_BaryMonitor;
	reg [7:0] m_BaryCounter;
	reg m_BaryStart;
	reg m_BaryRequest;

	// Pseudo Bus Master Assignment
	reg [3:0] m_PseudoAddress;
	reg [31:0] m_PseudoData;
	reg m_PseudoWrite;

	
	// Buffer
	wire [31:0] m_Buf_rData;

	assign m_Arbiter = m_BaryRequest;

	buffer Exp(.clk(m_clk), .reset(m_rst), .write(m_Write), .en(m_Enable), .Addr(m_Address), .wData(m_WriteData), .rData(m_Buf_rData));

	// Create Clock
	always begin 
		#(`CYCLE/2) m_clk <= ~m_clk; 
	end

	// Assign arbitration
	always @ (*) begin
		if(~m_Arbiter) begin
			m_Address = m_PseudoAddress;
			m_WriteData = m_PseudoData;
			m_Write = m_PseudoWrite;
		end
		else begin
			m_Address = m_BaryAddress;
			m_ReadData = m_Buf_rData;
		end
	end

	// Trigger function
	always @ (posedge m_clk) begin
		if(!m_rst) begin
			//$display("%dns", $time);
			$CallModule(m_BaryAddress, m_ReadData, m_BaryMonitor, m_BaryCounter, m_BaryStart, m_BaryRequest);
		end
	end

	// Fire
	initial begin
		$dumpfile("signal.vcd");
		$dumpvars(0, TopModule);

		m_clk = 1'b0;
		m_rst = 1'b1;
		m_BaryRequest = 1'b0;
		m_BaryMonitor = 2'b0;
		m_BaryCounter = 7'b0;
		m_BaryStart = 1'b0;
		
		$Test_Connection;

		#(`CYCLE)	m_rst = 1'b0;

		#15 		m_Enable = 1'b1;
					m_PseudoWrite = 1'b1;
					
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
		#(`CYCLE)	m_BaryStart = 1'b1;			
		

		#1000 $finish;
	end

endmodule