#include "ResourceStorageLoader.h"

#include <fstream>

ExResource::ResourceStorageLoader::ResourceStorageLoader() :
	ResourceStorageLoader("")
{}

ExResource::ResourceStorageLoader::ResourceStorageLoader(const char* path) :
	path(path)
{}

ExResource::ResourceStorageLoader::ResourceStorageLoader(const std::string& path) :
	ResourceStorageLoader(path.c_str())
{}

void ExResource::ResourceStorageLoader::scanFile() {
	if (path == nullptr) return;
	if (!entriesInCurrentFile.empty()) entriesInCurrentFile.clear();

	std::ifstream stream;
	stream.open(path);
	if (stream.bad()) {
		stream.close();
		return;
	}

	std::string entryName;
	size_t entryLength;
	while (stream.good()) {
		entryName = getNextEntryName(stream);
		if (skipNextWhitespaces(stream) == ':') {
			//
		}
	}


	stream.close();
}

#define EX_RESOURCE_DEBUG
#ifdef EX_RESOURCE_DEBUG

bool ExResource::ResourceStorageLoader::loadResource(const std::string& name) {
	return loadResource(name.c_str());
}

bool ExResource::ResourceStorageLoader::loadResource(const char* name) {
	if (path == nullptr) return false;

	std::ifstream stream;
	stream.open(path);
	if (stream.bad()) {
		stream.close();
		return false;
	}
	
	return false;
}

bool ExResource::ResourceStorageLoader::loadResource(const std::string& name, const InitFunc& initFunc) {
	return loadResource(name, initFunc);
}

bool ExResource::ResourceStorageLoader::loadResource(const char* name, const InitFunc& initFunc) {
	return false;
}

#else

bool ExResource::ResourceStorageLoader::loadResource(const std::string& entryName) {
	return loadResource(entryName.c_str());
}

bool ExResource::ResourceStorageLoader::loadResource(const char* entryName) {
	
	return false;
}

void ExResource::ResourceStorageLoader::loadResource(const std::string& name, const InitFunc& initFunc) {

}
void ExResource::ResourceStorageLoader::loadResource(const ResourceRef& res, const InitFunc& initFunc) {

}

#endif

void ExResource::ResourceStorageLoader::releaseResource(const std::string& name) {
	loadedResources.erase(name);
}

void ExResource::ResourceStorageLoader::releaseResource(const ResourceRef& res) {
	for (auto& i : loadedResources) {
		if (i.second == res)
			loadedResources.erase(i.first);
	}
}

void ExResource::ResourceStorageLoader::clearResources() {
	loadedResources.clear();
}

ExResource::ResourceStorageLoader::WeakResourceRef  ExResource::ResourceStorageLoader::getResource(const std::string& entryName) {
	auto elem = loadedResources.find(entryName);
	return (elem != loadedResources.end()) ? elem->second : WeakResourceRef();
}

ExResource::ResourceStorageLoader::WeakResourceRef  ExResource::ResourceStorageLoader::getResource(const char* entryName) {
	return getResource(std::string(entryName));
}

void ExResource::ResourceStorageLoader::setPath(const std::string& path) {
	setPath(path.c_str());
}

void ExResource::ResourceStorageLoader::setPath(const char* path) {
	clearResources();
	this->path = path;
}

std::string ExResource::ResourceStorageLoader::getPath() const {
	return std::string(path);
}

const char* ExResource::ResourceStorageLoader::getPathRaw() const {
	return path;
}

std::string ExResource::ResourceStorageLoader::getNextEntryName(std::ifstream& stream) const {
	std::vector<char> name;
	while (stream.get() != '\"');
	char ch;
	while ((ch = stream.get()) != '\"') {
		name.push_back(ch);
	}
	name.push_back('\0');

	return std::string(name.data());
}

char ExResource::ResourceStorageLoader::skipNextWhitespaces(std::ifstream& stream) const {
	char ch;
	while (std::isspace(ch = stream.get()));
	return ch;
}