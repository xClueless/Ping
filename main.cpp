#include "main.hpp"

#include "MainWindow.hpp"
#include "PingImage.hpp"
#include "PingParseError.hpp"

#include <QApplication>
#include <iostream>
#include <unistd.h>
#include <QLabel>

using namespace std;

int main(int argc, char *argv[])
{
	options opts = processArguments(argc, argv);

	try
	{
		if(opts.filename.empty())
		{
			throw PingParseError("A file must be provided with the -f option.");
		}

		PingImage png(opts.filename);
		png.readFile();
		png.verifySignature();
		png.readChunks();

		if(opts.printIHDR) png.ihdr()->print();

		for(PingChunk* chunk : png.chunks())
		{
			if(opts.printHeaders) chunk->printHeader();
			if(opts.printData) chunk->printData();
		}

		if(opts.displayImage)
		{
			png.readImage();

			QApplication a(argc, argv);
			MainWindow m;

			m.setImage(png.imageBuffer().buildQImage(png.ihdr()));
			m.show();
			a.exec();
		}
	}
	catch (const PingParseError& pngPE)
	{
		cerr << pngPE.what() << endl;
		return 1;
	}

	return 0;
}

options processArguments(int argc, char* argv[])
{
	options opts = {"", false, false, false, false};
	opterr = 0;
	int c = 0;

//	string devModeFile = "/home/dan/PING/rgbalpha.png";
	string devModeFile = "/home/dan/PING/panda.png";

	while((c = getopt(argc, argv, "zhdixf:")) != -1)
	{
		switch(c)
		{
			case 'z': cout << "Using developer flags." << endl; return {devModeFile, true, true, false, true};
			case 'h': opts.printHeaders = true; break;
			case 'd': opts.printData = true; break;
			case 'i': opts.printIHDR = true; break;
			case 'f': opts.filename = string(optarg); break;
			case 'x': opts.displayImage = true; break;
			case '?': cerr << "Unrecognised option '" << optopt << "'" << endl; break;
		}
	}

	return opts;
}

