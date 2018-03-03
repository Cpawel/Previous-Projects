module lab3_pd(
input reg clk, reset, left, right,
output reg la, lb, lc, ra, rb, rc);

	reg [2:0] state;

	// State Formation
always @(posedge clk)
	begin
		if(reset)
			state <= 3'b000;
		else
			begin case(state)
			3'b000:
				begin
					if(left & right) state <= 3'b000;
					else if(left) state <= 3'b101;
					else if(right) state <= 3'b001;
					else state <= 3'b000;
				end
				
			3'b001:
				state <= 3'b010;
				
			3'b010:
				state <= 3'b011;
				
			3'b101:
				state <= 3'b110;
				
			3'b110:
				state <= 3'b111;
				
			3'b011:
				begin
					if(left & right) state <= 3'b000;
					else if(left) state <= 3'b101;
					else if(right) state <= 3'b001;
					else state <= 3'b000;
				end
				
			3'b111:
				begin
					if(left & right) state <= 3'b000;
					else if(left) state <= 3'b101;
					else if(right) state <= 3'b001;
					else state <= 3'b000;
				end
			default:
				state <= 3'b000;
			endcase
			end
	end

	// Output Formation
always @(posedge clk)
	begin
		if(reset)
			begin
				la <= 0;
				lb <= 0;
				lc <= 0;
				ra <= 0;
				rb <= 0;
				rc <= 0;
			end
		else if(state == 3'b000)
			begin
				la <= 0;
				lb <= 0;
				lc <= 0;
				ra <= 0;
				rb <= 0;
				rc <= 0;
			end
		else if(state == 3'b001)
			begin
				la <= 0;
				lb <= 0;
				lc <= 0;
				ra <= 1;
				rb <= 0;
				rc <= 0;
			end
		else if(state == 3'b010)
			begin
				la <= 0;
				lb <= 0;
				lc <= 0;
				ra <= 1;
				rb <= 1;
				rc <= 0;
			end
		else if(state == 3'b011)
			begin
				la <= 0;
				lb <= 0;
				lc <= 0;
				ra <= 1;
				rb <= 1;
				rc <= 1;
			end
		else if(state == 3'b101)
			begin
				la <= 1;
				lb <= 0;
				lc <= 0;
				ra <= 0;
				rb <= 0;
				rc <= 0;
			end
		else if(state == 3'b110)
			begin
				la <= 1;
				lb <= 1;
				lc <= 0;
				ra <= 0;
				rb <= 0;
				rc <= 0;
			end
		else if(state == 3'b111)
			begin
				la <= 1;
				lb <= 1;
				lc <= 1;
				ra <= 0;
				rb <= 0;
				rc <= 0;
			end
		else
			begin
				la <= 0;
				lb <= 0;
				lc <= 0;
				ra <= 0;
				rb <= 0;
				rc <= 0;
			end
	end
endmodule
