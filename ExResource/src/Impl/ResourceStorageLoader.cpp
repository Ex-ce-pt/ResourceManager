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
	
	// TODO: implement scanning

}

bool ExResource::ResourceStorageLoader::loadResource(const std::string& name) {
	return loadResource(name.c_str());
}

bool ExResource::ResourceStorageLoader::loadResource(const char* name) {
	if (path == nullptr) return false;

	// TODO: implement loading
	
	return false;
}

void ExResource::ResourceStorageLoader::releaseResource(const std::string& name) {
	loadedResources.erase(name);
}

void ExResource::ResourceStorageLoader::releaseResource(const char* name) {
	releaseResource(std::string(name));
}

void ExResource::ResourceStorageLoader::releaseResource(const ResourcePtr& res) {
	for (auto& i : loadedResources) {
		if (i.second == res)
			loadedResources.erase(i.first);
	}
}

void ExResource::ResourceStorageLoader::clearResources() {
	loadedResources.clear();
}

ExResource::ResourceStorageLoader::WeakResourcePtr  ExResource::ResourceStorageLoader::getResource(const std::string& entryName) {
	auto elem = loadedResources.find(entryName);
	return (elem != loadedResources.end()) ? elem->second : WeakResourcePtr();
}

ExResource::ResourceStorageLoader::WeakResourcePtr  ExResource::ResourceStorageLoader::getResource(const char* entryName) {
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