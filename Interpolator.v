module Interpolator(
   input enable,
   input [15:0] Alpha,
   input [15:0] Beta, 
   input [15:0] Gamma,
   output [7:0] R,
   output [7:0] G,
   output [7:0] B);
	
	wire [31:0] TempR, TempG, TempB;
	assign TempR = (Alpha * 8'd255) >> 16;
	assign TempG = (Beta * 8'd255) >> 16;
	assign TempB = (Gamma * 8'd255) >> 16;

	assign R = TempR[7:0];
	assign G = TempG[7:0];
	assign B = TempB[7:0];

endmodule