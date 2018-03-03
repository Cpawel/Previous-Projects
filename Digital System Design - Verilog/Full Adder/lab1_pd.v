module lab1_pd(
	A,
	B,
	Cin,
	Y,
	Cout
);


input wire	A;
input wire	B;
input wire	Cin;
output wire	Y;
output wire	Cout;


assign	Y = (A ^ B) ^ Cin;

assign	Cout = (Cin & (B | A)) | (A & B);


endmodule
