#include "Resource.h"

ExResource::Resource::Resource()
	: data(nullptr), dataSize(0), entryName()
{}

ExResource::Resource::~Resource() {
	tryDelete();
}

ExResource::Resource::Resource(const Resource& other) {
	this->data = other.data;
	this->dataSize = other.dataSize;
	this->entryName = other.entryName;
}

ExResource::Resource::Resource(Resource&& other) noexcept {
	this->data = other.data;
	this->dataSize = other.dataSize;
	this->entryName = std::string(other.entryName);

	other.data = nullptr;
}

void ExResource::Resource::operator=(const Resource& other) {
	tryDelete();
	this->data = other.data;
	this->dataSize = other.dataSize;
	this->entryName = other.entryName;
}

void ExResource::Resource::operator=(Resource&& other) noexcept {
	tryDelete();
	this->data = other.data;
	this->dataSize = other.dataSize;
	this->entryName = std::string(other.entryName);

	other.data = nullptr;
}

bool ExResource::Resource::operator==(const Resource& other) const {
	return data == other.data &&
			dataSize == other.dataSize &&
			entryName.compare(other.entryName) == 0;
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

std::string ExResource::Resource::getEntryName() const {
	return entryName;
}

const char* ExResource::Resource::getEntryNameRaw() const {
	return entryName.c_str();
}

inline void ExResource::Resource::tryDelete() {
	delete[] data;
}