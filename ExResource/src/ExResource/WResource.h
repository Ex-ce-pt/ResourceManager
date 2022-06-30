#pragma once

#include <string>
#include <unordered_map>

#include "../Lib/UUID.h"

namespace ExResource {

	class WResource {
	public:
		WResource();
		WResource(const WResource& other);
		WResource(WResource&& other) noexcept;
		~WResource();

		void operator=(const WResource& other);
		void operator=(WResource&& other) noexcept;

		bool operator==(const WResource& other) const;

		WResource copy() noexcept;

		void* getPtr() const;
		const size_t& getSize() const;
		std::wstring getName() const;
		const wchar_t* getNameRaw() const;
		const std::unordered_map<std::wstring, std::wstring>& getAttributes() const;
		const UUID& getUUID() const;

		template<typename T>
		T* castPtr() {
			return static_cast<T*>(data);
		}

	private:
		void* data;
		size_t dataSize;
		std::wstring name;
		std::unordered_map<std::wstring, std::wstring> attributes;
		UUID uuid;

		friend class WResourceLoader;

	};

}