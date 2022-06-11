#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <memory>
#include <filesystem>

#include "Resource.h"
#include "../Lib/UUID.h"

namespace ExResource {

	class ResourceStorage {
	private:
		using WeakResourcePtr = std::weak_ptr<Resource>;
		using ResourcePtr = std::shared_ptr<Resource>;
		using ResourceMap = std::unordered_map<UUID, ResourcePtr>;
		using ResourceSignature = std::pair<std::string, size_t>;

		//using path_t = std::filesystem::path;
		//using cpath_t = const path_t&;

	public:
		ResourceStorage();
		ResourceStorage(const std::string& path);
		ResourceStorage(const char* path);

		/*
		Stores a folder of files as a resource file.
		Args:
		• const std::string& folderPath - a path to a folder to store
		• const std::string& outputFile - a path to a file the folder will be stored in
		• const std::unordered_map<std::string, std::pair<std::string, std::string>>& attributes - a map, where key is a file path and value is a map of attributes for this file
		*/
		void storeAsResources(const std::string& folderPath, const std::string& output, const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& attributes = {});

		/*
		Scan the file to determine the resources stored in the file.
		N.B. - setPath() must be called before using!
		*/
		void scanFile();

		/*
		Load a resource using a resource name
		Args:
		• const std::string& name - the name of the resource (corresponds to the path to the file in the original folder)		  
		Return:
		Loading success
		*/
		bool loadResource(const std::string& name);
		/*
		Load a resource using a resource name.
		Args:
		• const char* name - the name of the resource (corresponds to the path to the file in the original folder)
		Return:
		Loading success
		*/
		bool loadResource(const char* name);

		/*
		Release the resource thus freeing the memory.
		Args:
		• const std::string& name - the name of the resource to release
		N.B. - Make sure that there's no shared pointers of the resource before releasing it!
		*/
		void releaseResource(const std::string& name);
		/*
		Release the resource thus freeing the memory.
		Args:
		• const char* name - the name of the resource to release
		N.B. - Make sure that there's no shared pointers of the resource before calling!
		*/
		void releaseResource(const char* name);

		/*
		Release the resource thus freeing the memory.
		Args:
		• const ResoucePtr& res- the resource to release
		N.B. - Make sure that there's no shared pointers of the resource before calling!
		*/
		void releaseResource(const ResourcePtr& res);
		
		/*
		Release all loaded resources.
		N.B. - Make sure that there's no shared pointers of any of the resources before calling!
		*/
		void clearResources();

		/*
		Get a loaded resource.
		Args:
		• const std::string& entryName - the name of a loaded resource.
		Return:
		A weak pointer to the resource
		*/
		WeakResourcePtr getResource(const std::string& resourceName);

		/*
		Get a loaded resource.
		Args:
		• const char* entryName - the name of a loaded resource.
		Return:
		A weak pointer to the resource
		*/
		WeakResourcePtr getResource(const char* resourceName);

		/*
		Set a path to a resouce file.
		Args:
		• const std::string& path - a path to a resource file
		*/
		void setPath(const std::string& path);

		/*
		Set a path to a resouce file.
		Args:
		• const std::string& path - a path to a resource file
		*/
		void setPath(const char* path);

		/*
		Get the path to the current resouce file.
		Return:
		The path in std:::string class
		*/
		std::string getPath() const;

		/*
		Get the path to the current resouce file.
		Return:
		The path in raw form (const char*)
		*/
		const char* getPathRaw() const;

	private:
		const char* path;
		ResourceMap loadedResources;
		std::unordered_map<std::string, size_t> resourcesInCurrentFile;

	};

}