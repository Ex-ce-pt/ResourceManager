#include "../ExResource/WResourceLoader.h"

#include <stdio.h>

// Static functions for data processing

static std::wstring getTrimmedStringFromCharArray(wchar_t* str, const size_t& len) {
	wchar_t* beginResult = str;
	wchar_t* endResult = str;
	wchar_t* end = str + len;

	for (wchar_t* i = str; i != end; i++) {
		if (!iswspace(*i) && *i != (wchar_t) '\0') {
			beginResult = i;
			break;
		}
	}
	for (wchar_t* i = end - 1; i != beginResult; i--) {
		if (!iswspace(*i) && *i != (wchar_t) '\0') {
			endResult = i + 1;
			break;
		}
	}

	return std::wstring(beginResult, endResult);
}

static inline void findNextQuotationMark(FILE* file) {
	while (!feof(file) && fgetwc(file) != (wchar_t) '\"');
}

static std::tuple<std::wstring, long> getNextName(FILE* file) {
	long nameBegin, nameLen;
	wchar_t* buf;
	std::wstring name;

	findNextQuotationMark(file);
	nameBegin = ftell(file);
	findNextQuotationMark(file);
	nameLen = ftell(file) - nameBegin;
	fseek(file, nameBegin, SEEK_SET);
	buf = new wchar_t[nameLen + 1];
	fgetws(buf, nameLen, file);
	buf[nameLen] = '\0';
	name = buf;
	delete[] buf;
	fseek(file, 1, SEEK_CUR);

	return std::make_tuple(name, nameBegin - 1);
}

static size_t getNextResourceSize(FILE* file) {
	wchar_t* buf;

	while (fgetwc(file) != '\t');
	fseek(file, -1, SEEK_CUR);
	buf = new wchar_t[15];       // TODO: un-hard-code
	while (!feof(file)) {
		fgetws(buf, 15, file);
		wchar_t attributeSignature[] = { buf[0], buf[1], '\0' };
		if (!wcscmp(attributeSignature, L"\t_")) {
			if (!wcscmp(buf, L"\t_resourceSize")) {
				delete[] buf;
				if (fgetwc(file) != ':') {
					printf("ERROR: no colon after a _resourceSize attribute.");
					exit(-1);
				}

				while (!feof(file) && iswspace(fgetwc(file)));
				fseek(file, -1, SEEK_CUR);
				long sizeBegin = ftell(file);
				while (!feof(file) && iswdigit(fgetwc(file)));
				long sizeLen = ftell(file) - sizeBegin;
				fseek(file, sizeBegin, SEEK_SET);
				buf = new wchar_t[sizeLen];
				fgetws(buf, sizeLen, file);
				size_t size = wcstoul(buf, NULL, 10);
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

static ExResource::WResourceLoader::ResourceSignature getNextResourceSignature(FILE* file) {
	auto [name, pos] = getNextName(file);
	if (fgetwc(file) != ':') {
		printf("ERROR: no colon after the resource name.");
		exit(-1);
	}

	size_t size = getNextResourceSize(file);
	if (fgetwc(file) != '\n') {
		printf("ERROR: no new line character before the resource's data");
		exit(-1);
	}

	fseek(file, size, SEEK_CUR);

	return std::make_tuple(name, size, pos);
}

/////////////

ExResource::WResourceLoader::WResourceLoader() :
	WResourceLoader("")
{}

ExResource::WResourceLoader::WResourceLoader(const wchar_t* path) :
	path(path)
{}

ExResource::WResourceLoader::WResourceLoader(const std::wstring& path) :
	WResourceLoader(path.c_str())
{}

ExResource::WResourceLoader::WResourceLoader(const std::filesystem::path& path) :
	WResourceLoader(path.c_str())
{}

void ExResource::WResourceLoader::scanFile() {

	FILE* file;
	if (_wfopen_s(&file, path, L"rb")) {
		printf("ERROR: couldn't open a file for scanning.");
		exit(-1);
	}
	fseek(file, 0, SEEK_SET);

	while (true) {
		auto resourceSignature = getNextResourceSignature(file);
		resourcesInCurrentFile.push_back(resourceSignature);
		
		if (feof(file) || fgetwc(file) <= '\0') break;
		fseek(file, -1, SEEK_CUR);
	}

	fclose(file);

}

bool ExResource::WResourceLoader::loadResource(const std::wstring& name) {
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
	_wfopen_s(&file, path, L"rb");
	fseek(file, std::get<2>(*signature), SEEK_SET);

	// Resource name checks
	if (name.compare(std::get<0>(getNextName(file)))) {
		printf("ERROR: found resource's name differs from the passed name.");
		return false;
	}
	if (fgetwc(file) != ':') {
		printf("ERROR: no colon after the resource name.");
		return false;
	}
	fseek(file, 1, SEEK_CUR);

	// Get attributes
	std::unordered_map<std::wstring, std::wstring> attributes;
	wchar_t* buf;
	while (!feof(file)) {
		if (fgetwc(file) != '\t') {
			printf("ERROR: no tab before an attribute.");
			return false;
		}

		wchar_t ch;
		long keyBegin, keyLen, valueBegin, valueLen;
		std::wstring key, value;

		keyBegin = ftell(file);
		while ((ch = fgetwc(file)) != ':') {
			if (ch == '\n') {
				printf("ERROR: no colon after an attribute key.");
				return false;
			}
		}
		keyLen = ftell(file) - keyBegin;
		fseek(file, keyBegin, SEEK_SET);
		buf = new wchar_t[keyLen];
		memset(buf, 0, keyLen);
		fgetws(buf, keyLen, file);
		key = getTrimmedStringFromCharArray(buf, keyLen);
		delete[] buf;

		fseek(file, 1, SEEK_CUR);

		valueBegin = ftell(file);
		while (fgetc(file) != '\n');
		valueLen = ftell(file) - valueBegin;
		fseek(file, valueBegin, SEEK_SET);
		buf = new wchar_t[valueLen];
		memset(buf, 0, valueLen);
		fgetws(buf, valueLen, file);
		value = getTrimmedStringFromCharArray(buf, valueLen);
		delete[] buf;

		attributes.emplace(key, value);

		if (!key.compare(L"_resourceSize")) { // un-hard-code
			break;
		}
	}
	fseek(file, 1, SEEK_CUR);

	// Get the data
	size_t size = std::get<1>(*signature) - 1;
	buf = new wchar_t[size + 1];
	for (size_t i = 0; i < size; i++)
		buf[i] = fgetwc(file);
	buf[size] = '\0';
	
	// Store the resource
	ResourcePtr resource = std::make_shared<WResource>();
	resource->name = name;
	resource->dataSize = size;
	resource->attributes.swap(attributes);
	resource->data = buf;
	loadedResources.emplace(resource->uuid, resource);

	fclose(file);

	return true;
}

bool ExResource::WResourceLoader::loadResource(const wchar_t* name) {
	return loadResource(std::wstring(name));
}

void ExResource::WResourceLoader::releaseResource(const std::wstring& name) {
	for (auto i : loadedResources) {
		if (!name.compare(i.second->getName())) {
			loadedResources.erase(i.first);
			return;
		}
	}
}

void ExResource::WResourceLoader::releaseResource(const wchar_t* name) {
	for (auto i : loadedResources) {
		if (!wcscmp(name, i.second->getNameRaw())) {
			loadedResources.erase(i.first);
			return;
		}
	}
}

void ExResource::WResourceLoader::releaseResource(const ResourcePtr& res) {
	for (auto& i : loadedResources) {
		if (i.second == res)
			loadedResources.erase(i.first);
	}
}

void ExResource::WResourceLoader::clearResources() {
	loadedResources.clear();
}

ExResource::WResourceLoader::WeakResourcePtr 
ExResource::WResourceLoader::getResource(const std::wstring& entryName) {
	for (auto& i : loadedResources) {
		if (!entryName.compare(i.second->getName())) {
			return i.second;
		}
	}
	return WeakResourcePtr();
}

ExResource::WResourceLoader::WeakResourcePtr  ExResource::WResourceLoader::getResource(const wchar_t* entryName) {
	for (auto& i : loadedResources) {
		if (!wcscmp(entryName, i.second->getNameRaw())) {
			return i.second;
		}
	}
	return WeakResourcePtr();
}

void ExResource::WResourceLoader::setPath(const std::wstring& path) {
	setPath(path.c_str());
}

void ExResource::WResourceLoader::setPath(const wchar_t* path) {
	clearResources();
	this->path = path;
}

std::wstring ExResource::WResourceLoader::getPath() const {
	return std::wstring(path);
}

const wchar_t* ExResource::WResourceLoader::getPathRaw() const {
	return path;
}

std::filesystem::path ExResource::WResourceLoader::getPathFS() const {
	return std::filesystem::path(path);
}

const std::vector<ExResource::WResourceLoader::ResourceSignature>& ExResource::WResourceLoader::getResourceSignaturesInCurrentFile() const {
	return resourcesInCurrentFile;
}