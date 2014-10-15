#include "main.hpp"

#include "MainWindow.hpp"
#include "PingImage.hpp"
#include "PingParseError.hpp"

#include <QApplication>
#include <iostream>
#include <unistd.h>

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
	options opts = {"", false, false, false};
	opterr = 0;
	int c = 0;

	while((c = getopt(argc, argv, "zhdif:")) != -1)
	{
		switch(c)
		{
			case 'z': cout << "Using developer flags." << endl; return {"/home/dan/PING/rgbalpha.png", true, true, false};
			case 'h': opts.printHeaders = true; break;
			case 'd': opts.printData = true; break;
			case 'i': opts.printIHDR = true; break;
			case 'f': opts.filename = optarg; break;
			case '?': cerr << "Unrecognised option '" << optopt << "'" << endl; break;
		}
	}

	return opts;
}

