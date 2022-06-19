# ResourceManager

A resource manager written in **C++**. It's able to compress data into one file and then load it when needed.
It works just like an ordinary file system.

By **Exedice**\
`v. 1.0` - *19.06.2022*\
Status: `stable build`

## Documentation

### Resource class
**Resource** class contains data about the resource loaded as well as some methods for accessing that data.\
You are not meant to create an instance of the class yourself, it's a work for **ResourceLoader** class.\
**NB** - In the end of every resource's data there is a null-termination character ('\0'), it's **included** into the size of the resource. This was made for easier usage of text resources. Please be aware of that.

<table><tbody>
<tr><th>Method</th><th>Description</th></tr>
<tr><td>`Resource()`</td><td>A default constructor.</td></tr>
<tr><td>`Resource(const Resource& other)`</td><td>A copy constructor that accepts an lvalue refference.</td></tr>
<tr><td>`Resource(Resource&& other) noexcept`</td><td>A copy constructor that accepts an rvalue refference.</td></tr>
<tr><td>`void operator=(const Resource& other)`</td><td>An assignment operator for lvalue refferences.</td></tr>
<tr><td>`void operator=(Resource&& other) noexcept`</td><td>An assignment operator for rvalue refferences.</td></tr>
<tr><td>`bool operator==(const Resource& other) const`</td><td>An *equal* operator, return value is determined comparing resources' names, sized, pointers to the data and UUIDs.</td></tr>
<tr><td>`Resource copy() noexcept`</td><td>A function returning a copy of the resource it has been called on (the data gets copied as well).</td></tr>
<tr><td>`void* getPtr() const`</td><td>Returns a pointer to the resource's data.</td></tr>
<tr><td>`const size_t& getSize() const`</td><td>Returns the size of the resource's data.</td></tr>
<tr><td>`std::string getName() const`</td><td>Returns the resource's name (is equal to the name of the file this resource was created of).</td></tr>
<tr><td>`const char* getNameRaw() const`</td><td>Returns the resource's name as a C string (see `std::string getName() const`).</td></tr>
<tr><td>`const std::unordered_map<std::string, std::string>& getAttributes() const`</td><td>Returns the resource's attributes.</td></tr>
<tr><td>`const UUID& getUUID() const`</td><td>Returns the resource's UUID.</td></tr>
<tr><td>`template<typename T> T* castPtr()`</td><td>Returns the pointer to the resource's data (see `void* getPtr() const`) casted to the type `T*`.</td></tr>
</tbody></table>
---
### ResourceSaver class
**ResourceSaver** class is a class with one single meaning - to pack files into a `.resource` file.\
<hr>
`static void storeAsResources(const std::string& folderPath, const std::string& output, const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& attributes = {});`\
Stores a folder of files as a resource file.\
Args:
* folderPath - a path to a folder to store.
* outputFile - a path to a file the folder will be stored in.
* attributes - a map, where key is file path and value is a map of attributes for this file.
