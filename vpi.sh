iverilog-vpi register.cpp module.cpp
iverilog -o tb.vvp buffer.v tb.v
vvp -M. -m register tb.vvp