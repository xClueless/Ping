#include "MainWindow.hpp"
#include <QApplication>
#include "PingImage.hpp"
#include "PingParseError.hpp"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
//	QApplication a(argc, argv);
//	MainWindow w;
//	w.show();


	PingImage png("/run/media/dan/Seagate/Pictures/Graphics/Panda.png");
//	PingImage png("/home/dan/grub.cfg");

	try
	{
		png.readFile();
		png.verifySignature();
		png.readChunks();
		png.ihdr()->print();

		for(PingChunk* chunk : png.chunks())
		{
			chunk->printHeader();
			chunk->printData();
		}
	}
	catch (const PingParseError& pngPE)
	{
		cerr << pngPE.what() << endl;
	}

//	return a.exec();
	return 0;
}
