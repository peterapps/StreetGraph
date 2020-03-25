#include <getopt.h>
#include <stdlib.h> // atoi
#include <iostream>
#include "parser.h"

void printHelp(char * argv[]){
	std::cout << "Usage: " << argv[0];
	std::cout << " [--image IMAGE_PATH] [--graph GRAPH_PATH] [--load] [--save] [--mode MODE]\n";
	std::cout << "	IMAGE_PATH (default): img/map.png\n";
	std::cout << "  GRAPH_PATH (default): graph.txt\n";
	std::cout << "  MODE: 0 (CREATE_NODES), 1 (CREATE_EDGES), 2 (DEFAULT)\n";
}

Settings parseOptions(int argc, char * argv[]){
	Settings result{"img/map.png", "graph.txt", Mode::DEFAULT, false, false}; // Will be returned
	opterr = true; // Helps with errors
	int choice = -1; // Keeps track of while loop below
	int option_index = 0; // An artifact of getopt_long
	option long_options[] = { // List of command-line options
		{ "image", required_argument, nullptr, 'i' },
		{ "graph", required_argument, nullptr, 'g' },
		{ "mode", required_argument, nullptr, 'm' },
		{ "load", no_argument, nullptr, 'l'},
		{ "save", no_argument, nullptr, 's'},
		{ "help", no_argument, nullptr, 'h'},
		{ nullptr, 0, nullptr, '\0'}
	};
	// Parse each option passed
	while ((choice = getopt_long(argc, argv, "i:g:m:h", long_options, &option_index)) != -1){
		switch (choice){
			case 'i':
				result.image_path = optarg;
				break;
			case 'g':
				result.graph_path = optarg;
				break;
			case 'm':
				result.mode = (Mode) atoi(optarg);
				break;
			case 'l':
				result.load = true;
				break;
			case 's':
				result.save = true;
				break;
			case 'h':
			default:
				printHelp(argv);
				exit(0);
				break;
		}
	}
	return result;
}
