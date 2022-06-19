#pragma once

#include <string>
#include <unordered_map>

namespace ExResource {

	class ResourceSaver {
	private:
		ResourceSaver();

	public:
		/*
		Stores a folder of files as a resource file.
		Args:
		• const std::string& folderPath - a path to a folder to store
		• const std::string& outputFile - a path to a file the folder will be stored in
		• const std::unordered_map<std::string, std::pair<std::string, std::string>>& attributes - a map, where key is a file path and value is a map of attributes for this file
		*/
		static void storeAsResources(const std::string& folderPath, const std::string& output, const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& attributes = {});

	};

}