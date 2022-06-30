#include "../ExResource/WResourceSaver.h"

#include <stdio.h>

// Static helper functions

// NOTE: "first" variable is a bad idea, there should be another way...
static void storeResource(FILE* inputFile, FILE* outputFile, const std::wstring& name, const std::unordered_map<std::wstring, std::wstring>& attributes, bool first) {

	fseek(inputFile, 0, SEEK_END);
	size_t size = ftell(inputFile) + 1;
	fseek(inputFile, 0, SEEK_SET);

	if (!first) fputwc('\n', outputFile);

	fwprintf(outputFile, L"\"%s\":\n", name.c_str());
	for (const auto& i : attributes) {
		fwprintf(outputFile, L"\t%s: %s\n", i.first.c_str(), i.second.c_str());
	}
	fwprintf(outputFile, L"\t%s: %i\n", L"_resourceSize", size); // TODO: un-hard-code

	// No fgetws()
	// fgetws() stops reading data after a new line character.
	for (size_t i = 0; i < size; i++) {
		char ch = fgetwc(inputFile);
		if (ch != -1)
			fputwc(ch, outputFile);
	}

}

///////////

ExResource::WResourceSaver::WResourceSaver() {}

void ExResource::WResourceSaver::storeAsResources(const std::filesystem::path& folderPath, const std::filesystem::path& output, const std::unordered_map<std::wstring, std::unordered_map<std::wstring, std::wstring>>& attributes) {
	using namespace std::filesystem;
	if (!is_directory(folderPath)) {
		printf("ERROR: folderPath folder does not exist.");
		return;
	}

	FILE* outputFile;
	_wfopen_s(&outputFile, output.c_str(), L"wb");

	bool first = true;
	for (const auto& i : recursive_directory_iterator(folderPath)) {
		if (!i.is_regular_file() || !i.exists()) continue;
		FILE* inputFile;
		_wfopen_s(&inputFile, i.path().c_str(), L"rb");

		storeResource(inputFile, outputFile, i.path().c_str(), {}, first);

		if (first) first = false;

		fclose(inputFile);
	}

	fclose(outputFile);
}
