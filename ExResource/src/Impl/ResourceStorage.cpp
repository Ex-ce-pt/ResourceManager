#include "ResourceStorage.h"

#include <stdio.h>

// Static functions for data processing

static std::string getTrimmedStringFromCharArray(char* str, const size_t& len) {
	char* beginResult = str;
	char* endResult = str;
	char *end = str + len;

	for (char* i = str; i != end; i++) {
		if (!isspace(*i) && *i != '\0') {
			beginResult = i;
			break;
		}
	}
	for (char* i = end - 1; i != beginResult; i--) {
		if (!isspace(*i) && *i != '\0') {
			endResult = i + 1;
			break;
		}
	}

	return std::string(beginResult, endResult);
}

static inline void findNextQuotationMark(FILE* file) {
	while (!feof(file) && fgetc(file) != '\"');
}

static std::tuple<std::string, long> getNextName(FILE* file) {
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

	return std::make_tuple(name, nameBegin - 1);
}

static size_t getNextResourceSize(FILE* file) {
	char* buf;

	while (fgetc(file) != '\t');
	fseek(file, -1, SEEK_CUR);
	buf = new char[15];       // TODO: un-hard-code
	while (!feof(file)) {
		fgets(buf, 15, file);
		char attributeSignature[] = { buf[0], buf[1], '\0' };
		if (!strcmp(attributeSignature, "\t_")) {
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

	printf("ERROR: file has ended before the resource size could be determined.");
	exit(-1);

}

static ExResource::ResourceStorage::ResourceSignature getNextResourceSignature(FILE* file) {
	auto [name, pos] = getNextName(file);
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

	return std::make_tuple(name, size, pos);
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

	// No fgets()
	// fgets() stops reading data after a new line character.
	for (size_t i = 0; i < size; i++) {
		fputc(fgetc(inputFile), outputFile);
	}

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
		resourcesInCurrentFile.push_back(resourceSignature);
		
		if (feof(file) || fgetc(file) <= '\0') break;
	}

	fclose(file);

}

bool ExResource::ResourceStorage::loadResource(const std::string& name) {
	if (path == nullptr) return false;

	// Get the signature
	const ResourceSignature* signature = nullptr;
	for (const auto& i : resourcesInCurrentFile) {
		if (!name.compare(std::get<0>(i))) {
			signature = &i;
			break;
		}
	}
	if (signature == nullptr) return false;

	FILE* file;
	fopen_s(&file, path, "rb");
	fseek(file, std::get<2>(*signature), SEEK_SET);

	// Resource name checks
	if (name.compare(std::get<0>(getNextName(file)))) {
		printf("ERROR: found resource's name differs from the passed name.");
		return false;
	}
	if (fgetc(file) != ':') {
		printf("ERROR: no colon after the resource name.");
		return false;
	}
	fseek(file, 1, SEEK_CUR);

	// Get attributes
	std::unordered_map<std::string, std::string> attributes;
	char* buf;
	while (!feof(file)) {
		if (fgetc(file) != '\t') {
			printf("ERROR: no tab before an attribute.");
			return false;
		}

		char ch;
		long keyBegin, keyLen, valueBegin, valueLen;
		std::string key, value;

		keyBegin = ftell(file);
		while ((ch = fgetc(file)) != ':') {
			if (ch == '\n') {
				printf("ERROR: no colon after an attribute key.");
				return false;
			}
		}
		keyLen = ftell(file) - keyBegin;
		fseek(file, keyBegin, SEEK_SET);
		buf = new char[keyLen];
		memset(buf, 0, keyLen);
		fgets(buf, keyLen, file);
		key = getTrimmedStringFromCharArray(buf, keyLen);
		delete[] buf;

		fseek(file, 1, SEEK_CUR);

		valueBegin = ftell(file);
		while (fgetc(file) != '\n');
		valueLen = ftell(file) - valueBegin;
		fseek(file, valueBegin, SEEK_SET);
		buf = new char[valueLen];
		memset(buf, 0, valueLen);
		fgets(buf, valueLen, file);
		value = getTrimmedStringFromCharArray(buf, valueLen);
		delete[] buf;

		attributes.emplace(key, value);

		if (!key.compare("_resourceSize")) { // un-hard-code
			break;
		}
	}
	
	// Store the resource
	ResourcePtr resource = std::make_shared<Resource>();
	resource->name = name;
	resource->dataSize = std::get<1>(*signature);
	resource->attributes.swap(attributes);
	resource->data = nullptr;
	loadedResources.emplace(resource->uuid, resource);

	fclose(file);

	return true;
}

bool ExResource::ResourceStorage::loadResource(const char* name) {
	return loadResource(std::string(name));
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

const std::vector<ExResource::ResourceStorage::ResourceSignature>& ExResource::ResourceStorage::getResourceSignaturesInCurrentFile() const {
	return resourcesInCurrentFile;
}