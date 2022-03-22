#include "virtual-painter.h"



int main()
{
	VirtualPainter& instance = VirtualPainter::GetInstance();
	instance.Start();
}