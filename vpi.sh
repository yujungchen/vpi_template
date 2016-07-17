iverilog-vpi register.cpp module.cpp
iverilog -o tb.vvp vtxbuffer.v tb.v
vvp -M. -m register tb.vvp