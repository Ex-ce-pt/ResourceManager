#include <iostream>

#define DEBUG
#include <ExResource/ExResource.h>

std::shared_ptr<ExResource::ResourceStorageLoader> resourceLoader;

int main() {

	std::string filename = "res/test.resource";
	resourceLoader = std::make_shared<ExResource::ResourceStorageLoader>(filename);
	
	std::cout << resourceLoader->getPath() << std::endl;

	return 0;
}