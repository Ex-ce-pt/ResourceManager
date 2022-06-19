#include "../ExResource/ResourceSaver.h"

#include <stdio.h>
#include <filesystem>

// Static helper functions

// NOTE: "first" variable is a bad idea, there should be another way...
static void storeResource(FILE* inputFile, FILE* outputFile, const std::string& name, const std::unordered_map<std::string, std::string>& attributes, bool first) {

	fseek(inputFile, 0, SEEK_END);
	size_t size = ftell(inputFile) + 1;
	fseek(inputFile, 0, SEEK_SET);

	if (!first) fputc('\n', outputFile);

	fprintf(outputFile, "\"%s\":\n", name.c_str());
	for (const auto& i : attributes) {
		fprintf(outputFile, "\t%s: %s\n", i.first.c_str(), i.second.c_str());
	}
	fprintf(outputFile, "\t%s: %i\n", "_resourceSize", size); // TODO: un-hard-code

	// No fgets()
	// fgets() stops reading data after a new line character.
	for (size_t i = 0; i < size; i++) {
		char ch = fgetc(inputFile);
		if (ch != -1)
			fputc(ch, outputFile);
	}

}

///////////

ExResource::ResourceSaver::ResourceSaver() {}

void ExResource::ResourceSaver::storeAsResources(const std::string& folderPath, const std::string& output, const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& attributes) {
	using namespace std::filesystem;
	if (!is_directory(folderPath)) {
		printf("ERROR: folderPath folder does not exist.");
		return;
	}

	FILE* outputFile;
	fopen_s(&outputFile, output.c_str(), "wb");

	bool first = true;
	for (const auto& i : recursive_directory_iterator(folderPath)) {
		if (!i.is_regular_file() || !i.exists()) continue;
		FILE* inputFile;
		_wfopen_s(&inputFile, i.path().c_str(), L"rb");

		// bad!
		const std::wstring p = std::wstring(i.path().c_str());
		std::string name;

		std::transform(p.begin(), p.end(), std::back_inserter(name), [](wchar_t c) {
			return (char)c;
			});
		//

		storeResource(inputFile, outputFile, name, {}, first);

		if (first) first = false;

		fclose(inputFile);
	}

	fclose(outputFile);
}
