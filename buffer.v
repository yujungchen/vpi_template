module buffer(
   input clk, 
   input reset, 
   input write, // 0 for read 1 for write
   input en,
   input [3:0] Addr, // 32 entry
   input [31:0] wData, 
   output [31:0] rData);

   reg [31:0] Buf [0:15];

   assign rData = (en && ~write) ? Buf[Addr] : 32'b0;

   always @ (posedge clk) begin
      if (reset) begin
         Buf[0] <= 32'b0;
         Buf[1] <= 32'b0;
         Buf[2] <= 32'b0;
         Buf[3] <= 32'b0;
         Buf[4] <= 32'b0;
         Buf[5] <= 32'b0;
         Buf[6] <= 32'b0;
         Buf[7] <= 32'b0;
         Buf[8] <= 32'b0;
         Buf[9] <= 32'b0;
         Buf[10] <= 32'b0;
         Buf[11] <= 32'b0;
         Buf[12] <= 32'b0;
         Buf[13] <= 32'b0;
         Buf[14] <= 32'b0;
         Buf[15] <= 32'b0;
      end
      else begin
         if(write && en) begin
            Buf[Addr] <= wData;
         end
         else begin
            Buf[0] <= Buf[0];
            Buf[1] <= Buf[1];
            Buf[2] <= Buf[2];
            Buf[3] <= Buf[3];
            Buf[4] <= Buf[4];
            Buf[5] <= Buf[5];
            Buf[6] <= Buf[6];
            Buf[7] <= Buf[7];
            Buf[8] <= Buf[8];
            Buf[9] <= Buf[9];
            Buf[10] <= Buf[10];
            Buf[11] <= Buf[11];
            Buf[12] <= Buf[12];
            Buf[13] <= Buf[13];
            Buf[14] <= Buf[14];
            Buf[15] <= Buf[15];
         end
      end
   end

endmodule