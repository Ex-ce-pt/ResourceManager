#pragma once

#include <string>

namespace ExResource {

	class Resource {
	private:
		void* data;
		size_t dataSize;
		std::string entryName;

		inline void tryDelete();

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
		std::string getEntryName() const;
		const char* getEntryNameRaw() const;

		template<typename T>
		T* castPtr() {
			return static_cast<T*>(data);
		}
	};

}