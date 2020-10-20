#include "mainHead.h"
#include <string>
#include <vector>

class Input {
	std::string jpgPath;
	FILE* jpg;
	int* h = new int;
	int* w = new int;
	int rotation;

public:
	Input() {};
	int InJpg(std::string inPath, std::filesystem::path basePath) {
		jpgPath = basePath.string();
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
		jpgPath.append("\\");
#else
		jpgPath.append("/");
#endif
		jpgPath.append(inPath);
#pragma warning(suppress : 4996)
		jpg = fopen(jpgPath.c_str(), "r");
		if (jpg)
			scanhead(jpg, w, h);
		else
			return 1;
		return 0;
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
	std::filesystem::path outPath;
	std::filesystem::path basePath;
	FILE** inputjpgs;
	
	std::vector<Input*> inputs;

	//get current working directory
	basePath = std::filesystem::current_path();

	//parse option arguments
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			//step through single dash arguments
			for (int j = 1; j < strlen(argv[i]); j++) {
				switch (argv[i][j]) {
				case 'o':
					outPath = argv[i + 1 + argvToSkip];
					argvToSkip++;	//add one to count of processed arguments, mupcreate -oki C:\Users\lol\Documents\ 1.jpg, asdfg.pdf is the output, so don't interpret it as an input
					break;\
				case 'i':			//input image
					inputs.push_back(new Input);
					if(inputs.back()->InJpg(argv[i + 1 + argvToSkip], basePath))
						return 3;
					argvToSkip++;
					break;
				case 'r':			//rotation for current image
					if (inputs.back()->Rotate(atoi(argv[i + 1 + argvToSkip])))
						return 2;
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
		argvToSkip = 0;
	}
	//parse inputs
	return 0;
}