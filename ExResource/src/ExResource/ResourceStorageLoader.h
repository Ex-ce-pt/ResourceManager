#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <memory>

#include "Resource.h"

namespace ExResource {

	class ResourceStorageLoader {
	private:
		using WeakResourceRef = std::weak_ptr<Resource>;
		using ResourceRef = std::shared_ptr<Resource>;
		using ResourceMap = std::unordered_map<std::string, ResourceRef>;
		using InitFunc = std::function<std::pair<void*, size_t> (std::pair<void*, size_t>)>;

		const char* path;
		ResourceMap loadedResources;
		std::unordered_map<std::string, size_t> entriesInCurrentFile;

	private:
		char skipNextWhitespaces(std::ifstream& stream) const;
		std::string getNextEntryName(std::ifstream& stream) const;

		// scanning

	public:
		ResourceStorageLoader();
		ResourceStorageLoader(const std::string& path);
		ResourceStorageLoader(const char* path);

		void scanFile();
		bool loadResource(const std::string& name);
		bool loadResource(const char* name);
		bool loadResource(const std::string& name, const InitFunc& initFunc);
		bool loadResource(const char* name, const InitFunc& initFunc);

		void releaseResource(const std::string& name);
		void releaseResource(const ResourceRef& res);
		void clearResources();
		WeakResourceRef getResource(const std::string& entryName);
		WeakResourceRef getResource(const char* entryName);

		void setPath(const std::string& path);
		void setPath(const char* path);

		std::string getPath() const;
		const char* getPathRaw() const;
	};

}