`timescale 1ns/1ns
`define CYCLE 10

module TopModule();

	reg [7:0] m_Address;
	reg [31:0] m_Data;
	
	reg [1:0] m_BarycentricMonitor;
	reg [7:0] m_InternalCounter;
	reg m_StartBaryCentric;

	reg m_rst;
	reg m_clk;
	reg m_Request;

	wire [7:0] w_Address;
	wire [31:0] w_DataIn;
	wire w_Request;

	assign w_Address = m_Address;
	assign w_DataIn = m_Data;
	assign w_Request = m_Request;


	// Test Buffer
	reg buf_en;
	reg buf_w;
	reg [3:0] buf_Addr;
	reg [31:0] buf_wData;
	wire [31:0] buf_rData;
	buffer Exp(.clk(m_clk), .reset(m_rst), .write(buf_w), .en(buf_en), .Addr(buf_Addr), .wData(buf_wData), .rData(buf_rData));

	// Create Clock
	always begin 
		#(`CYCLE/2) m_clk <= ~m_clk; 
	end

	// Trigger function
	always @ (posedge m_clk) begin
		if(!m_rst) begin
			//$display("%dns", $time);
			$CallModule(m_Address, w_DataIn, m_BarycentricMonitor, m_InternalCounter, m_StartBaryCentric, m_Request);
		end
	end

	// Fire
	initial begin
		$dumpfile("signal.vcd");
		$dumpvars(0, TopModule);

		m_clk = 1'b0;
		m_rst = 1'b1;
		m_Request = 1'b0;
		m_BarycentricMonitor = 2'b0;
		m_InternalCounter = 7'b0;
		m_StartBaryCentric = 1'b0;

		$Test_Connection;
		#10 m_Address = 8'd100;
			m_Data = 32'hbf333333;
			m_rst = 1'b0;
		#10 m_StartBaryCentric = 1'b1;

		#15 buf_en = 1'b1;
			buf_w = 1'b1;
			buf_Addr = 4'd0;
			buf_wData = 32'h3f000000;
		#10 buf_Addr = 4'd1;
			buf_wData = 32'h3f333333;
		#10 buf_Addr = 4'd2;
			buf_wData = 32'hbf000000;
		#10 buf_w = 1'b0;
			buf_Addr = 4'd0;
		#10 buf_Addr = 4'd1;
		#10 buf_Addr = 4'd2;



		#1000 $finish;
	end

endmodule