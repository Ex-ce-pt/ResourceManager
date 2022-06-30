#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <tuple>
#include <memory>
#include <filesystem>

#include "WResource.h"
#include "../Lib/UUID.h"

namespace ExResource {

	class WResourceLoader {
	public:
		using WeakResourcePtr = std::weak_ptr<WResource>;
		using ResourcePtr = std::shared_ptr<WResource>;
		using ResourceMap = std::unordered_map<UUID, ResourcePtr>;
		using ResourceSignature = std::tuple<std::wstring, size_t, long>;

	public:
		WResourceLoader();
		WResourceLoader(const std::wstring& path);
		WResourceLoader(const std::filesystem::path& path);
		WResourceLoader(const wchar_t* path);

		/*
		Scan the file to determine the resources stored in the file.
		N.B. - setPath() must be called before using!
		*/
		void scanFile();

		/*
		Load a resource using a resource name.
		Args:
		• const std::wstring& name - the name of the resource (corresponds to the path to the file in the original folder).	  
		Returns:
		• Loading success
		*/
		bool loadResource(const std::wstring& name);
		/*
		Load a resource using a resource name.
		Args:
		• const wchar_t* name - the name of the resource (corresponds to the path to the file in the original folder).
		Returns:
		• Loading success
		*/
		bool loadResource(const wchar_t* name);

		/*
		Release the resource thus freeing the memory.
		Args:
		• const std::wstring& name - the name of the resource to release.
		N.B. - Make sure that there's no shared pointers of the resource before releasing it!
		*/
		void releaseResource(const std::wstring& name);
		/*
		Release the resource thus freeing the memory.
		Args:
		• const wchar_t* name - the name of the resource to release.
		N.B. - Make sure that there's no shared pointers of the resource before calling!
		*/
		void releaseResource(const wchar_t* name);

		/*
		Release the resource thus freeing the memory.
		Args:
		• const ResoucePtr& res - the resource to release
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
		• const std::wstring& entryName - the name of a loaded resource.
		Returns:
		• A weak pointer to the resource.
		*/
		WeakResourcePtr getResource(const std::wstring& resourceName);

		/*
		Get a loaded resource.
		Args:
		• const wchar_t* entryName - the name of a loaded resource.
		Returns:
		• A weak pointer to the resource.
		*/
		WeakResourcePtr getResource(const wchar_t* resourceName);

		/*
		Set a path to a resouce file.
		Args:
		• const std::wstring& path - a path to a resource file.
		*/
		void setPath(const std::wstring& path);

		/*
		Set a path to a resouce file.
		Args:
		• const std::wstring& path - a path to a resource file.
		*/
		void setPath(const wchar_t* path);

		/*
		Get the path to the current resouce file.
		Returns:
		• The path in std:::wstring class.
		*/
		std::wstring getPath() const;

		/*
		Get the path to the current resouce file.
		Returns:
		The path as a C string.
		*/
		const wchar_t* getPathRaw() const;

		/*
		get the path to the current resource file.
		Returns:
		The path in std::filesystem::path class.
		*/
		std::filesystem::path getPathFS() const;

		/*
		Get the resource signatures from the current file.
		Returns:
		• a vector of resource signatures.
		N.B. - scanFile() must be called before using!
		*/
		const std::vector<ResourceSignature>& getResourceSignaturesInCurrentFile() const;

	private:
		const wchar_t* path;
		ResourceMap loadedResources;
		std::vector<ResourceSignature> resourcesInCurrentFile;

	};

}