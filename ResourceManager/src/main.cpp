#include <iostream>

#include <ExResource/ExResource.h>

std::shared_ptr<ExResource::ResourceLoader> resourceLoader;

void testScanning() {
	std::string filename = "res/test.resource";
	resourceLoader = std::make_shared<ExResource::ResourceLoader>(filename);

	std::cout << resourceLoader->getPath() << std::endl;

	resourceLoader->scanFile();
}

void testStoring() {
	std::string filename = "res/test/"; // not used
	resourceLoader = std::make_shared<ExResource::ResourceLoader>(filename);

	ExResource::ResourceSaver::storeAsResources(filename, "res/out.resource");
}

void testBoth() {
	std::string filename = "res/out.resource";
	resourceLoader = std::make_shared<ExResource::ResourceLoader>(filename);

	ExResource::ResourceSaver::storeAsResources("res/test/", filename);

	resourceLoader->scanFile();

	for (const auto& i : resourceLoader->getResourceSignaturesInCurrentFile()) {
		const std::string& resourceName = std::get<0>(i);

		resourceLoader->loadResource(resourceName);

		const auto& res = resourceLoader->getResource(resourceName).lock();

		printf("=========\n");
		printf("Name: \"%s\"\nSize: %lu\nUUID: %lu\nAttributes:\n", res->getNameRaw(), res->getSize(), res->getUUID());
		for (const auto& i : res->getAttributes()) {
			printf("%s - %s\n", i.first.c_str(), i.second.c_str());
		}
		printf("Data: %s\n", res->getPtr());
		printf("=========\n");

	}

	resourceLoader->clearResources();
}

int main() {

	//testScanning();
	//testStoring();
	testBoth();

	std::cin.get();

	return 0;
}

// TODO: make an exception system
// TODO: update the doc