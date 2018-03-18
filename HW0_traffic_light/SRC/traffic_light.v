module traffic_light(clk,rst,pass,R,G,Y);
input clk,rst,pass;
output R,G,Y;
reg R,G,Y;

reg [1:0]curstate,nextstate;
reg [3:0]cyclecount;

parameter [1:0] R_light=0,G_light=1,Y_light=2;

always@(posedge clk or posedge rst or pass)
begin
	
	if(rst)
	begin
		curstate<=R_light;
		cyclecount<=1;
	end
	else
	begin
		
		if(!pass)
		begin
			curstate<=nextstate;
			cyclecount<=cyclecount+1;
 		end
		else
		begin
			if(curstate!=G_light)
			begin	
				curstate<=G_light;
				cyclecount<=1;
			end
			else
			begin
				curstate<=curstate;
				cyclecount<=cyclecount+1;
			end
		end
	end
end
always@(curstate,cyclecount)
begin
/*out*/
	case(curstate)
		R_light:
		begin
			R=1'b1;
			G=1'b0;
			Y=1'b0;
			if(cyclecount==10)
			begin
				nextstate=G_light;
				cyclecount=0;
			end
			else 
				nextstate=R_light;
		end
		G_light:
		begin
			R=1'b0;
			G=1'b1;
			Y=1'b0;
			if(cyclecount==12)
			begin
				nextstate=Y_light;
				cyclecount=0;
			end
			else 
				nextstate=G_light;
		end
		Y_light:
		begin
			R=1'b0;
			G=1'b0;
			Y=1'b1;
			if(cyclecount==5)
			begin
				nextstate=R_light;
				cyclecount=0;
			end
			else 
				nextstate=Y_light;
		end
	endcase
end
endmodule

