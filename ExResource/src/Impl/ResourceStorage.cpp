#include "ResourceStorage.h"

#include <stdio.h>

// Static functions for data processing

static inline void findNextQuotationMark(FILE* file) {
	while (!feof(file) && fgetc(file) != '\"');
}

static std::string getNextName(FILE* file) {
	long nameBegin, nameLen;
	char* buf;
	std::string name;

	findNextQuotationMark(file);
	nameBegin = ftell(file);
	findNextQuotationMark(file);
	nameLen = ftell(file) - nameBegin;
	fseek(file, nameBegin, SEEK_SET);
	buf = new char[nameLen + 1];
	fgets(buf, nameLen, file);
	buf[nameLen] = '\0';
	name = buf;
	delete[] buf;
	fseek(file, 1, SEEK_CUR);

	return name;
}

static size_t getNextResourceSize(FILE* file) {
	char* buf;

	while (fgetc(file) != '\t');
	fseek(file, -1, SEEK_CUR);
	buf = new char[15];       // TODO: un-hard-code
	while (!feof(file)) {
		fgets(buf, 15, file);
		char signature[] = { buf[0], buf[1], '\0' };
		if (!strcmp(signature, "\t_")) {
			if (!strcmp(buf, "\t_resourceSize")) {
				delete[] buf;
				if (fgetc(file) != ':') {
					printf("ERROR: no colon after a _resourceSize attribute.");
					exit(-1);
				}

				while (!feof(file) && isspace(fgetc(file)));
				fseek(file, -1, SEEK_CUR);
				long sizeBegin = ftell(file);
				while (!feof(file) && isdigit(fgetc(file)));
				long sizeLen = ftell(file) - sizeBegin;
				fseek(file, sizeBegin, SEEK_SET);
				buf = new char[sizeLen];
				fgets(buf, sizeLen, file);
				size_t size = atoi(buf);
				delete[] buf;
				return size;

			}
		} else {
			printf("ERROR: didn't find a _resourceSize attribute.");
			exit(-1);
		}
	}

}

static std::pair<std::string, size_t> getNextResourceSignature(FILE* file) {
	std::string name = getNextName(file);
	if (fgetc(file) != ':') {
		printf("ERROR: no colon after the resource name.");
		exit(-1);
	}

	size_t size = getNextResourceSize(file);
	if (fgetc(file) != '\n') {
		printf("ERROR: no new line character before the resource's data");
		exit(-1);
	}

	fseek(file, size, SEEK_CUR);

	return std::make_pair(name, size);
}

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

	char* buf = new char[size];
	fgets(buf, size * sizeof(char), inputFile);
	fwrite(buf, sizeof(char), size, outputFile);

	delete[] buf;

}

/////////////

ExResource::ResourceStorage::ResourceStorage() :
	ResourceStorage("")
{}

ExResource::ResourceStorage::ResourceStorage(const char* path) :
	path(path)
{}

ExResource::ResourceStorage::ResourceStorage(const std::string& path) :
	ResourceStorage(path.c_str())
{}

void ExResource::ResourceStorage::storeAsResources(const std::string& folderPath, const std::string& output, const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& attributes) {
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
			return (char) c;
		});
		//

		storeResource(inputFile, outputFile, name, {}, first);

		if (first) first = false;

		fclose(inputFile);
	}

	fclose(outputFile);

}

void ExResource::ResourceStorage::scanFile() {

	FILE* file;
	if (fopen_s(&file, path, "rb")) {
		printf("ERROR: couldn't open a file for scanning.");
		exit(-1);
	}
	fseek(file, 0, SEEK_SET);
	
	while (true) {
		auto resourceSignature = getNextResourceSignature(file);
		resourcesInCurrentFile.insert(resourceSignature);
		
		if (feof(file) || fgetc(file) <= '\0') break;
	}

	fclose(file);

}

bool ExResource::ResourceStorage::loadResource(const std::string& name) {
	return loadResource(name.c_str());
}

bool ExResource::ResourceStorage::loadResource(const char* name) {
	if (path == nullptr) return false;

	// TODO: implement loading
	
	return false;
}

void ExResource::ResourceStorage::releaseResource(const std::string& name) {
	for (auto i : loadedResources) {
		if (!name.compare(i.second->getName())) {
			loadedResources.erase(i.first);
			return;
		}
	}
}

void ExResource::ResourceStorage::releaseResource(const char* name) {
	for (auto i : loadedResources) {
		if (!strcmp(name, i.second->getNameRaw())) {
			loadedResources.erase(i.first);
			return;
		}
	}
}

void ExResource::ResourceStorage::releaseResource(const ResourcePtr& res) {
	for (auto& i : loadedResources) {
		if (i.second == res)
			loadedResources.erase(i.first);
	}
}

void ExResource::ResourceStorage::clearResources() {
	loadedResources.clear();
}

ExResource::ResourceStorage::WeakResourcePtr 
ExResource::ResourceStorage::getResource(const std::string& entryName) {
	for (auto& i : loadedResources) {
		if (!entryName.compare(i.second->getName())) {
			return i.second;
		}
	}
	return WeakResourcePtr();
}

ExResource::ResourceStorage::WeakResourcePtr  ExResource::ResourceStorage::getResource(const char* entryName) {
	for (auto& i : loadedResources) {
		if (!strcmp(entryName, i.second->getNameRaw())) {
			return i.second;
		}
	}
	return WeakResourcePtr();
}

void ExResource::ResourceStorage::setPath(const std::string& path) {
	setPath(path.c_str());
}

void ExResource::ResourceStorage::setPath(const char* path) {
	clearResources();
	this->path = path;
}

std::string ExResource::ResourceStorage::getPath() const {
	return std::string(path);
}

const char* ExResource::ResourceStorage::getPathRaw() const {
	return path;
}