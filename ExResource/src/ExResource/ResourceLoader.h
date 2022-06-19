#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <tuple>
#include <memory>
#include <filesystem>

#include "Resource.h"
#include "../Lib/UUID.h"

namespace ExResource {

	class ResourceLoader {
	public:
		using WeakResourcePtr = std::weak_ptr<Resource>;
		using ResourcePtr = std::shared_ptr<Resource>;
		using ResourceMap = std::unordered_map<UUID, ResourcePtr>;
		using ResourceSignature = std::tuple<std::string, size_t, long>;

	public:
		ResourceLoader();
		ResourceLoader(const std::string& path);
		ResourceLoader(const char* path);

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

		const std::vector<ResourceSignature>& getResourceSignaturesInCurrentFile() const;

	private:
		const char* path;
		ResourceMap loadedResources;
		std::vector<ResourceSignature> resourcesInCurrentFile;

	};

}