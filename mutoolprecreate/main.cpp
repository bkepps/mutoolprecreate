#include "mainHead.h"
#include <string>
#include <vector>

class Input {
	
	FILE* jpg;


public:
	int rotation = NULL;
	int* h = new int;
	int* w = new int;
	std::string jpgPath;

	Input() {};
	int InJpg(std::string inPath, std::filesystem::path basePath) {
		jpgPath = basePath.string();
		jpgPath.append(PATHSEP);
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
	printf("mupcreate [options]\noptions:\n\t-h\tshow this message\n\t-i\tinput image, .jpg\n\t-r\tamount of rotation in degrees\n\t\tif not assosiated with a -i, will set a global rotation value for all proceeding images\nex: mupcreate -ir input1.jpg 90 -i input2.jpg\n\n");
}

int main(int argc, char* argv[]) {
	int tempint;
	int argvToSkip = 0;
	std::filesystem::path outPath;
	std::filesystem::path tempPath;
	std::filesystem::path basePath;
	FILE** inputjpgs;
	FILE* tempFile;
	int globalRot = 0;
	bool rotArg = false;
	
	std::vector<Input*> inputs;

	//get current working directory
	basePath = std::filesystem::current_path();

	if (argc < 2)
		help();

	//parse option arguments
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			//step through single dash arguments
			for (int j = 1; j < strlen(argv[i]); j++) {
				switch (argv[i][j]) {
				case 'o':
					outPath = argv[i + 1 + argvToSkip];
					argvToSkip++;	//add one to count of processed arguments, mupcreate -oki C:\Users\lol\Documents\ 1.jpg, asdfg.pdf is the output, so don't interpret it as an input
					break; \
				case 'i':			//input image
					inputs.push_back(new Input);
					if (inputs.back()->InJpg(argv[i + 1 + argvToSkip], basePath))
						return 3;
					argvToSkip++;
					break;
				case 'r':			//rotation for current image
					rotArg = true;
					if (j == 1) {	//if r is 1st arg in group (-r )
						globalRot = atoi(argv[i + 1 + argvToSkip++]);
						break;
					}
					if (inputs.back()->Rotate(atoi(argv[i + 1 + argvToSkip]))) //if setting rotation fails
						return 2;
					argvToSkip++;
					break;
				case 'h':			//help
					help();
					break;
				default:
					printf("invalid argument -%c\n\n", argv[i][j]);
					help();
					break;
				}
			}
		}
		if (rotArg == false && !(inputs.back()->rotation)) {
			inputs.back()->Rotate(globalRot);
		}
		rotArg = false;
		i += argvToSkip; // skip processed arguments
		argvToSkip = 0;
	}
	int i = 0;
	for (Input* asdfghjkl : inputs) {
#pragma warning(suppress : 4996)
		tempFile = fopen(/*(const char*)tempPath.c_str()*/std::to_string(i).c_str(), "w");
		fprintf(tempFile, "%%%%MediaBox 0 0 %i %i\n", *asdfghjkl->w, *asdfghjkl->h);
		fprintf(tempFile, "%%%%Rotate %i\n", asdfghjkl->rotation);
		fprintf(tempFile, "%%%%Image I %s\n", asdfghjkl->jpgPath.c_str());
		fprintf(tempFile, "q\n%i 0 0 %i 0 0 cm\n/I Do\nQ\n", *asdfghjkl->w, *asdfghjkl->h);
		fclose(tempFile);
		i++;
	}
	return 0;
}