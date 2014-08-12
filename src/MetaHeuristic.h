#pragma once
typedef void (*FnTunnel)(int, double, double, double);

extern "C"
{
	void Run(FnTunnel p_fnTunnel);
}