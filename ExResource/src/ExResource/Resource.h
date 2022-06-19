#pragma once

#include <string>
#include <unordered_map>

#include "../Lib/UUID.h"

namespace ExResource {

	class Resource {
	public:
		Resource();
		Resource(const Resource& other);
		Resource(Resource&& other) noexcept;
		~Resource();

		void operator=(const Resource& other);
		void operator=(Resource&& other) noexcept;

		bool operator==(const Resource& other) const;

		Resource copy() noexcept;

		void* getPtr() const;
		const size_t& getSize() const;
		std::string getName() const;
		const char* getNameRaw() const;
		const std::unordered_map<std::string, std::string>& getAttributes() const;
		const UUID& getUUID() const;

		template<typename T>
		T* castPtr() {
			return static_cast<T*>(data);
		}

	private:
		void* data;
		size_t dataSize;
		std::string name;
		std::unordered_map<std::string, std::string> attributes;
		UUID uuid;

		friend class ResourceLoader;

	};

}