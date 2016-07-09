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

	// Dump waveform
	initial begin
		$dumpfile("signal.vcd");
		$dumpvars;
	end

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
		#200 $finish;
	end

endmodule