iverilog-vpi register.cpp module.cpp
iverilog -o tb.vvp tb.v
vvp -M. -m register tb.vvp