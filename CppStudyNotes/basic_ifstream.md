  
  
  
  
# std::basic_ifstream

### td::basic_ifstream<CharT,Traits>::open
```cpp
void open( const char* filename,
           std::ios_base::openmode mode = std::ios_base::in );

void open( const std::filesystem::path::value_type* filename,
           std::ios_base::openmode mode = std::ios_base::in );

void open( const std::string& filename,
           std::ios_base::openmode mode = std::ios_base::in );

void open( const std::filesystem::path& filename,
           std::ios_base::openmode mode = std::ios_base::in );

```

**filename**	-	要打开的文件的名称
**mode**	-	指定流打开模式。它是一个 BitmaskType，定义了以下常量
常量	解释
-    app	每次写入前定位到流的末尾
-    binary	以二进制模式打开
-    in	打开以供读取
-    out	打开以供写入
-    trunc	打开时丢弃流的内容
-    ate	打开后立即定位到流的末尾
-    noreplace (C++23)	以独占模式打开


### std::basic_ifstream<CharT,Traits>::is_open
```cpp
bool is_open() const;
```

### std::basic_ifstream<CharT,Traits>::close

### std::basic_ifstream<CharT,Traits>::swap

```cpp
void swap( basic_ifstream& other );
```