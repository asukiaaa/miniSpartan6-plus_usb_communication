// Communication with python serial.
// 50,000,000clk /    115,200b = 434.03 clk/b -> 434 // ok
// 50,000,000clk /    921,600b =  54.25 clk/b ->  55 // ok
// 50,000,000clk /  1,843,200b =  27.12 clk/b ->  27 // soso
// 50,000,000clk /  3,686,400b =  13.56 clk/b ->  14 // buggy
// 50,000,000clk /  7,372,800b =   6.78 clk/b ->   7 // buggy
// 50,000,000clk / 14,745,600b =   3.39 clk/b ->   4 // failed
// 50,000,000clk / 29,491,200b =   1.70 clk/b ->   2 // failed

// 50,000,000clk /  1,500,000 =   33.33 clk/b ->  33 // ok
// 50,000,000clk / 12,000,000 =    4.16 clk/b ->   4 // ok

`define CLK_PER_BIT 4

module mojo_top (
  input clk,
  output[7:0]led,
  input rx,
  output tx
);

wire rst = 0;
wire block = 0;
wire busy;
wire new_data;
wire [7:0] data;

initial begin
  $display ("Hello world in uart program");
end

assign led = data;

serial_rx #(.CLK_PER_BIT(`CLK_PER_BIT)) instance_rx (
  .clk(clk),
  .rst(rst),
  .rx(rx),
  .data(data),
  .new_data(new_data)
);

serial_tx #(.CLK_PER_BIT(`CLK_PER_BIT)) instance_tx (
  .clk(clk),
  .rst(rst),
  .tx(tx),
  .block(block),
  .busy(busy),
  .data(data),
  .new_data(new_data)
);

endmodule
