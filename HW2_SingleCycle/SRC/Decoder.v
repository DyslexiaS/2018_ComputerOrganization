`timescale 1ns / 1ps

module Decoder(OP, Reg_Dst, Jump, Branch, Mem_Read, Mem_to_Reg, ALU_OP, Mem_Write, ALU_Src, Reg_Write);

	input [5:0] OP;
	output Reg_Dst, Jump, Branch, Mem_Read, Mem_to_Reg, Mem_Write, ALU_Src, Reg_Write;
	output [1:0] ALU_OP;
	
	/////  OP Code  /////
	//  R-type 000000  //
	//  load   100011  //
	//  store  101011  //
	//  beq    000100  //
	//  addi   001000  //
	//  j      000010  //
	/////////////////////
	
	assign Reg_Dst = (OP == 6'b000000) ? 1 : 0;
	assign Mem_Read = (OP == 6'b100011) ? 1 : 0;
	assign Mem_to_Reg = (OP == 6'b100011) ? 1 : 0;
	assign Mem_Write = (OP == 6'b101011) ? 1 : 0;
	
	////  ALUOP  ////
	//  R-type 10  //
	//  beq    01  //
	//  else   00  //
	/////////////////
	
	/* add your code here */
	reg [1:0] ALU_OP;
	reg Jump, Branch, ALU_Src, Reg_Write; 
	always@(*)begin
		Jump = 0;
		Branch = 0;
		case(OP)
			6'b000000: begin ALU_Src = 0; Reg_Write = 1; ALU_OP = 2'b10; end	// R-type
			6'b100011: begin ALU_Src = 1; Reg_Write = 1; ALU_OP = 2'b00; end	// load
			6'b101011: begin ALU_Src = 1; Reg_Write = 0; ALU_OP = 2'b00; end	// store
			6'b000100: begin ALU_Src = 0; Reg_Write = 0; ALU_OP = 2'b01; Branch = 1; end	// beq
			6'b001000: begin ALU_Src = 1; Reg_Write = 1; ALU_OP = 2'b00; end	// I-type
			6'b000010: begin ALU_Src = 0; Reg_Write = 0; ALU_OP = 2'b00; Jump = 1; end	// jump
			default: begin ALU_OP = 2'b00; end
		endcase
	end

	
endmodule
