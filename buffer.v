module buffer(
   input clk, 
   input reset, 
   input write, 
   input [2:0] Addr, 
   input [15:0] wData, 
   output [15:0] rData);

   reg [15:0] 	 regfile [0:7];

   assign rdDataA = regfile[rdAddrA];
   assign rdDataB = regfile[rdAddrB];

   integer 	 i;
   always @(posedge clk) begin
      if (reset) begin
	 for (i = 0; i < 8; i = i + 1) begin
	    regfile[i] <= 0;
	 end
      end else begin
	 if (write) regfile[wrAddr] <= wrData;
      end // else: !if(reset)
   end
endmodule