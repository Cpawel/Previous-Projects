`timescale 1ps / 1ps

module testbench_pd;

reg clk, reset, left, right;
reg la, lb, lc, ra, rb, rc;
int i, j;
reg[7:0] seq1, seq2;

lab3_pd dut(clk, reset, left, right, la, lb, lc, ra, rb, rc);

initial
begin

	seq1 = 8'b0110_1001;
	seq2 = 8'b0101_1010;
	
	clk = 0;
	reset = 1;
	left = 0;
	right = 0;
	
	#50
	clk = 1;
	
	#50
	reset = 0;
	
	// Above should show both that output updates on posedge and that reset performs correctly
	
	// Initialize clock
	j = 0;
	for (i = 0; i < 18; i++)
		begin
			#50;
			clk = ~clk;
			if (clk == 0)
			begin
				left = seq1[j];
				right = seq2[j];
				j++;
			end
			
		end			
	
end
endmodule
	