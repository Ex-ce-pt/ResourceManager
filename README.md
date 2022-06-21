<h1>ResourceManager</h1>

<p>A resource manager written in <b>C++</b>. It's able to compress data into one file and then load it when needed.</p>
<p>It works just like an ordinary file system.</p>

<p>By <b>Exedice</b></p>
<p><code>v. 1.0</code> - <i>19.06.2022</i></p>
<p>Status: <code>stable build</code></p>

<h2>Documentation</h2>

<h3>Resource class</h3>
<p><b>Resource</b> class contains data about the resource loaded as well as some methods for accessing that data.</p>
<p>You are not meant to create an instance of the class yourself, it's a work for <b>ResourceLoader</b> class.</p>
<p><b>NB</b> - In the end of every resource's data there is a null-termination character ('\0'), it's <b>included</b> into the size of the resource. This was made for easier usage of text resources. Please be aware of that.</p>

<table>
<tbody>
    <tr>
        <th>Method</th>
        <th>Description</th>
    </tr>
    <tr>
        <td><code>Resource()</code></td>
        <td>A default constructor.</td>
    </tr>
    <tr>
        <td><code>Resource(const Resource& other)</code></td>
        <td>A copy constructor that accepts an lvalue refference.</td>
    </tr>
    <tr>
        <td><code>Resource(Resource&& other) noexcept</code></td>
        <td>A copy constructor that accepts an rvalue refference.</td>
    </tr>
    <tr>
        <td><code>void operator=(const Resource& other)</code></td>
        <td>An assignment operator for lvalue refferences.</td>
    </tr>
    <tr>
        <td><code>void operator=(Resource&& other) noexcept</code></td>
        <td>An assignment operator for rvalue refferences.</td>
    </tr>
    <tr>
        <td><code>bool operator==(const Resource& other) const</code></td>
        <td>An <i>equal</i> operator, return value is determined comparing resources' names, sized, pointers to the data and UUIDs.</td>
    </tr>
    <tr>
        <td><code>Resource copy() noexcept</code></td>
        <td>A function returning a copy of the resource it has been called on (the data gets copied as well).</td>
    </tr>
    <tr>
        <td><code>void* getPtr() const</code></td>
        <td>Returns a pointer to the resource's data.</td>
        </tr>
    <tr>
        <td><code>const size_t& getSize() const</code></td>
        <td>Returns the size of the resource's data.</td>
    </tr>
    <tr>
        <td><code>std::string getName() const</code></td>
        <td>Returns the resource's name (is equal to the name of the file this resource was created of).</td>
    </tr>
    <tr>
        <td><code>const char* getNameRaw() const</code></td>
        <td>Returns the resource's name as a C string (see <code>std::string getName() const</code>).</td>
    </tr>
    <tr>
        <td><code>const std::unordered_map&lt;std::string, std::string&gt;& getAttributes() const</code></td>
        <td>Returns the resource's attributes.</td>
    </tr>
    <tr>
        <td><code>const UUID& getUUID() const</code></td>
        <td>Returns the resource's UUID.</td>
    </tr>
    <tr>
        <td><code>template&lt;typename T&gt; T* castPtr()</code></td>
        <td>Returns the pointer to the resource's data (see <code>void* getPtr() const</code>) casted to the type <code>T*</code>.</td>
    </tr>
</tbody></table>

<hr>

<h3>ResourceSaver class</h3>
<p><b>ResourceSaver</b> class is a class with one single meaning - to pack files into a <code>.resource</code> file.<p>

<table><tbody>
<tr>
    <td><code>static void storeAsResources(const std::string& folderPath, const std::string& output, const std::unordered_map&lt;std::string, std::unordered_map&lt;std::string, std::string&gt;&gt;& attributes = {})</code></td>
    <td>Stores a folder of files as a resource file.
        <br>
        Args:
        <ul>
            <li><code>folderPath</code> - a path to a folder to store.</li>
            <li><code>outputFile</code> - a path to a file the folder will be stored in.</li>
            <li><code>attributes</code> - a map, where key is file path and value is a map of attributes for this file.</li>
        </ul>
    </td>
</tr>
</tbody></table>

<hr>

<h3>ResourceLoader class</h3>
<p><b>ResourceLoader</b> class is a class that can load and store resources from a <code>.resource</code> file.</p>

<table><tbody>
<tr>
    <th>Types used</th>
    <th>Definitions</th>
</tr>
<tr>
    <td><code>WeakResourcePtr</code></td>
    <td><code>std::weak_ptr&lt;Resource&gt;</code</td>
</tr>
<tr>
    <td><code>ResourcePtr</code></td>
    <td><code>std::shared_ptr&lt;Resource&gt;</code</td>
</tr>
<tr>
    <td><code>ResourceMap</code></td>
    <td><code>std::unordered_map&lt;UUID, ResourcePtr&gt;</code</td>
</tr>
<tr>
    <td><code>ResourceSignature</code></td>
    <td><code>std::tuple&lt;std::string, size_t, long&gt;</code</td>
</tr>
</tbody></table>

<table><tbody>
<tr>
    <th>Method</th>
    <th>Description</th>
</tr>
<tr>
    <td><code>ResourceLoader()</code></td>
    <td>A default constructor.</td>
</tr>
<tr>
    <td><code>ResourceLoader(const std::string& path)</code></td>
    <td>A constructor that sets the path.</td>
</tr>
<tr>
    <td><code>ResourceLoader(const char* path)</code></td>
    <td>A constructor that sets the path (accepts C string).</td>
</tr>
<tr>
    <td><code>void scanFile()</code></td>
    <td>Scans the file to determine the resources stored in the file.<br><b>N.B. - setPath() must be called before using!<b></td>
</tr>
<tr>
    <td><code>bool loadResource(const std::string& name)</code></td>
    <td>Load a resource using a resource name.
        <br>
        Args:
        <ul>
		    <li><code>const std::string& name</code> - the name of the resource (corresponds to the path to the file in the original folder)</li>
        </ul>
		Returns:
        <ul>
		    <li>Loading success</li>
        </ul>
    </td>
</tr>
<tr>
    <td><code>bool loadResource(const char* name)</code></td>
    <td>Load a resource using a resource name.
        <br>
		Args:
        <ul>
		    <li><code>const char* name</code> - the name of the resource (corresponds to the path to the file in the original folder)</li>
        </ul>
		Returns:
        <ul>
		    <li>Loading success</li>
        </ul>
    </td>
</tr>
<tr>
    <td><code>void releaseResource(const std::string& name)</code></td>
    <td>Release the resource thus freeing the memory.
        <br>
		Args:
        <ul>
		    <li><code>const std::string& name</code> - the name of the resource to release.</li>
        </ul>
		<b>N.B. - Make sure that there's no shared pointers of the resource before releasing it!</b>
    </td>
</tr>
<tr>
    <td><code>void releaseResource(const char* name)</code></td>
    <td>Release the resource thus freeing the memory.
        <br>
		Args:
        <ul>
		    <li><code>const char* name</code> - the name of the resource to release.</li>
        </ul>
		<b>N.B. - Make sure that there's no shared pointers of the resource before calling!</b>
    </td>
</tr>
<tr>
    <td><code>void releaseResource(const ResourcePtr& res)</code></td>
    <td>Release the resource thus freeing the memory.
        <br>
		Args:
		<ul>
            <li><code>const ResoucePtr& res</code> - the resource to release.</li>
        </ul>
		<b>N.B. - Make sure that there's no shared pointers of the resource before calling!</b>
    </td>
</tr>
<tr>
    <td><code>void clearResources()</code></td>
    <td>Release all loaded resources.
        <br>
		<b>N.B. - Make sure that there's no shared pointers of any of the resources before calling!</b>
    </td>
</tr>
<tr>
    <td><code>WeakResourcePtr getResource(const std::string& resourceName)</code></td>
    <td>Get a loaded resource.
        <br>
		Args:
        <ul>
		    <li><code>const std::string& entryName</code> - the name of a loaded resource.</li>
        </ul>
		Returns:
        <ul>
		    <li>A weak pointer to the resource.</li>
        </ul>
    </td>
</tr>
<tr>
    <td><code>WeakResourcePtr getResource(const char* resourceName)</code></td>
    <td>Get a loaded resource.
        <br>
		Args:
        <ul>
		    <li><code>const char* entryName</code> - the name of a loaded resource.</li>
        </ul>
		Returns:
        <ul>
		    <li>A weak pointer to the resource.</li>
        </ul>
    </td>
</tr>
<tr>
    <td><code>void setPath(const std::string& path)</code></td>
    <td>Set a path to a resouce file.
        <br>
		Args:
        <ul>
		    <li><code>const std::string& path</code> - a path to a resource file.</li>
        </ul>
    </td>
</tr>
<tr>
    <td><code>void setPath(const char* path)</code></td>
    <td>Set a path to a resouce file.
        <br>
		Args:
        <ul>
		    <li><code>const std::string& path</code> - a path to a resource file</li>
        </ul>
    </td>
</tr>
<tr>
    <td><code>std::string getPath() const</code></td>
    <td>Get the path to the current resouce file.
        <br>
		Returns:
        <ul>
		    <li>The path in <code>std:::string</code> class</li>
        </ul>
    </td>
</tr>
<tr>
    <td><code>const char* getPathRaw() const</code></td>
    <td>Get the path to the current resouce file.
        <br>
		Returns:
        <ul>
		    <li>The path as a C string</li>
        </ul>
    </td>
</tr>
<tr>
    <td><code>const std::vector&lt;ResourceSignature&gt;& getResourceSignaturesInCurrentFile() const</code></td>
    <td>Get the resource signatures from the current file.
        <br>
		Returns:
        <ul>
		    <li>A vector of resource signatures</li>.
        </ul>
		<b>N.B. - scanFile() must be called before using!</b>
    </td>
</tr>

</tbody></table>

<!--
<img src="https://images.squarespace-cdn.com/content/v1/54acb7a2e4b0170492f6554d/1461692832097-WX6HF4AOGGBNYS1YRAM7/image-asset.png">

<tr>
    <td><code></code></td>
    <td></td>
</tr>
-->