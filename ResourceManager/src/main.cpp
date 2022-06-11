#include <iostream>

#include <ExResource/ExResource.h>

std::shared_ptr<ExResource::ResourceStorage> resourceLoader;

void testScanning() {
	std::string filename = "res/test.resource";
	resourceLoader = std::make_shared<ExResource::ResourceStorage>(filename);

	std::cout << resourceLoader->getPath() << std::endl;

	resourceLoader->scanFile();
}

void testStoring() {
	std::string filename = "res/test/"; // not used
	resourceLoader = std::make_shared<ExResource::ResourceStorage>(filename);

	resourceLoader->storeAsResources(filename, "res/out.resource");
}

void testBoth() {
	std::string filename = "res/out.resource";
	resourceLoader = std::make_shared<ExResource::ResourceStorage>(filename);

	resourceLoader->storeAsResources("res/test/", filename);

	resourceLoader->scanFile();
}

int main() {

	//testScanning();
	//testStoring();
	testBoth();

	//std::cin.get();

	return 0;
}

// TODO: make an exception system
// TODO: switch to wchar_t (std::filesystem::path) paths