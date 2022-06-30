#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>

namespace ExResource {

	class WResourceSaver {
	private:
		WResourceSaver();

	public:
		/*
		Stores a folder of files as a resource file.
		Args:
		• const std::wstring& folderPath - a path to a folder to store
		• const std::wstring& outputFile - a path to a file the folder will be stored in
		• const std::unordered_map<std::wstring, std::pair<std::wstring, std::wstring>>& attributes - a map, where key is a file path and value is a map of attributes for this file
		*/
		static void storeAsResources(const std::filesystem::path& folderPath, const std::filesystem::path& output, const std::unordered_map<std::wstring, std::unordered_map<std::wstring, std::wstring>>& attributes = {});

	};

}