#include "../ExResource/Resource.h"

ExResource::Resource::Resource()
	: data(nullptr), dataSize(0), name(), uuid(createUUID())
{}

ExResource::Resource::~Resource() {
	delete[] data;
}

ExResource::Resource::Resource(const Resource& other) {
	if (*this == other) return;

	this->data = other.data;
	this->dataSize = other.dataSize;
	this->name = other.name;
	this->uuid = other.uuid;
}

ExResource::Resource::Resource(Resource&& other) noexcept {
	if (*this == other) return;

	this->data = other.data;
	this->dataSize = other.dataSize;
	this->name = std::string(other.name);
	this->uuid = other.uuid;

	other.data = nullptr;
}

void ExResource::Resource::operator=(const Resource& other) {
	if (*this == other) return;

	//delete[] data;
	this->data = other.data;
	this->dataSize = other.dataSize;
	this->name = other.name;
	this->uuid = other.uuid;
}

void ExResource::Resource::operator=(Resource&& other) noexcept {
	if (*this == other) return;

	//delete[] data;
	this->data = other.data;
	this->dataSize = other.dataSize;
	this->name = std::string(other.name);
	this->uuid = other.uuid;

	other.data = nullptr;
}

bool ExResource::Resource::operator==(const Resource& other) const {
	return data == other.data &&
			dataSize == other.dataSize &&
			name.compare(other.name) == 0 &&
			uuid == other.uuid;
}

ExResource::Resource ExResource::Resource::copy() noexcept {
	return Resource(*this);
}

void* ExResource::Resource::getPtr() const {
	return data;
}

const size_t& ExResource::Resource::getSize() const {
	return dataSize;
}

std::string ExResource::Resource::getName() const {
	return name;
}

const char* ExResource::Resource::getNameRaw() const {
	return name.c_str();
}

const std::unordered_map<std::string, std::string>& ExResource::Resource::getAttributes() const {
	return attributes;
}

const UUID& ExResource::Resource::getUUID() const {
	return uuid;
}