#include "../ExResource/WResource.h"

ExResource::WResource::WResource()
	: data(nullptr), dataSize(0), name(), uuid(createUUID())
{}

ExResource::WResource::~WResource() {
	delete[] data;
}

ExResource::WResource::WResource(const WResource& other) {
	if (*this == other) return;

	this->data = other.data;
	this->dataSize = other.dataSize;
	this->name = other.name;
	this->uuid = other.uuid;
}

ExResource::WResource::WResource(WResource&& other) noexcept {
	if (*this == other) return;

	this->data = other.data;
	this->dataSize = other.dataSize;
	this->name = std::wstring(other.name);
	this->uuid = other.uuid;

	other.data = nullptr;
}

void ExResource::WResource::operator=(const WResource& other) {
	if (*this == other) return;

	//delete[] data;
	this->data = other.data;
	this->dataSize = other.dataSize;
	this->name = other.name;
	this->uuid = other.uuid;
}

void ExResource::WResource::operator=(WResource&& other) noexcept {
	if (*this == other) return;

	//delete[] data;
	this->data = other.data;
	this->dataSize = other.dataSize;
	this->name = std::wstring(other.name);
	this->uuid = other.uuid;

	other.data = nullptr;
}

bool ExResource::WResource::operator==(const WResource& other) const {
	return data == other.data &&
			dataSize == other.dataSize &&
			name.compare(other.name) == 0 &&
			uuid == other.uuid;
}

ExResource::WResource ExResource::WResource::copy() noexcept {
	return WResource(*this);
}

void* ExResource::WResource::getPtr() const {
	return data;
}

const size_t& ExResource::WResource::getSize() const {
	return dataSize;
}

std::wstring ExResource::WResource::getName() const {
	return name;
}

const wchar_t* ExResource::WResource::getNameRaw() const {
	return name.c_str();
}

const std::unordered_map<std::wstring, std::wstring>& ExResource::WResource::getAttributes() const {
	return attributes;
}

const UUID& ExResource::WResource::getUUID() const {
	return uuid;
}