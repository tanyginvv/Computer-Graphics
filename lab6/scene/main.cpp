#include "CMyApplication.h"
#include "CGdiplusInitializer.h"

CMyApplication app("test", 800, 600);
CGdiplusInitializer gdiplusInitializer;

int main(int argc, char* argv[])
{
	glewInit();
	app.MainLoop();
	return 0;
}