#include "mainHead.h"
#include <string>
#include <vector>
#include <filesystem>

class Input {
	std::string jpgPath;
	FILE* jpg;
	int* h = new int;
	int* w = new int;
	int rotation;

public:
	Input();
	int InJpg(std::string inPath, std::string basePath) {
		jpgPath = inPath.append(basePath);
		jpg = fopen(jpgPath.c_str(), "r");
		scanhead(jpg, w, h);
	}
	int Rotate(int rot) {
		rotation = rot;
		return 0;
	}
};

void help() {
	printf("mupcreate [options] 1.jpg 2.jpg ...\noptions:\n-o\tdirectory to output to\n");
}

int main(int argc, char* argv[]) {
	int tempint;
	int argvToSkip = 0;
	std::string outPath;
	std::string basePath;
	FILE** inputjpgs;
	
	std::vector<Input*> inputs;

	//get current working directory
	std::filesystem::path cwd = std::filesystem::current_path();

	//parse option arguments
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			//step through single dash arguments
			for (int j = 1; i < strlen(argv[i]); i++) {
				switch (argv[i][1]) {
				case 'o':
					outPath = argv[i + 1 + argvToSkip];
					argvToSkip++;	//add one to count of processed arguments, mupcreate -oki C:\Users\lol\Documents\ 1.jpg, asdfg.pdf is the output, so don't interpret it as an input
					break;\
				case 'i':			//input image
					inputs.push_back(new Input);
					inputs.back()->InJpg(argv[i + 1 + argvToSkip], basePath);
					argvToSkip++;
					break;
				case 'r':			//rotation for current image
					if (!inputs.back()->Rotate(atoi(argv[i + 1 + argvToSkip])))
						return 1;
					argvToSkip++;
					break;
				default:
					printf("invalid argument -%c\n\n", argv[i][j]);
					help();
					break;
				}
			}
		}
		i += argvToSkip; // skip processed arguments
	}
	//parse inputs

	return 0;
}