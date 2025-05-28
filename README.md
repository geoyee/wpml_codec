# wpml_codec

[![CMake on multiple platforms](https://github.com/geoyee/wpml_codec/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/geoyee/wpml_codec/actions/workflows/cmake-multi-platform.yml) [![CMake on ndk of android](https://github.com/geoyee/wpml_codec/actions/workflows/cmake-ndk-android.yml/badge.svg)](https://github.com/geoyee/wpml_codec/actions/workflows/cmake-ndk-android.yml)

大疆 WPML 航线文件编解码器

![input](./resource/img/input.png)

## 构建

可通过如下方式获取代码并构建

```shell
git clone https://github.com/geoyee/wpml_codec.git
cd wpml_codec
git submodule update --init
mkdir build
cd build
cmake ..
cmake --build .
```

## 示例

可参考示例代码 [sample/main.cpp](https://github.com/geoyee/wpml_codec/blob/main/sample/main.cpp)，示例数据以及生成的结果查看，可以使用[大疆司空 2](https://fh.dji.com/)在线平台。简单来说，主要流程如下

```c++
// 导入头文件
#include <wpml_codec.hpp>

int main()
{
    std::string kmzPath = "xxxxxx.kmz";
    std::string saveKmzPath = "yyyyyy.kmz";
    
    // 解析航线文件
    auto wpmzData = wcc::parseWPMZ(kmzPath);
    // 相关处理
    /* ... */
    // 保存为航线文件
    wcc::createWPMZ(wpmzData.value(), saveKmzPath);
}
```

## 说明

大疆上云 API 中的航线文件参数说明部分在大疆司空 2中导出的航线文件中未找到；同样的大疆司空 2中导出的航线文件中部分参数并没有在大疆上云 API 中的航线文件参数说明中找到。应该是大疆自己发布的和使用的版本存在不一致的问题。因此同一份航线文件导入再导出后会有所不同，但都可以成功在大疆司空 2中进行加载，界面上看起来结果相同，本实现仅参考大疆上云 API 中的航线文件参数说明

## 参考

- https://developer.dji.com/doc/cloud-api-tutorial/cn/api-reference/dji-wpml/overview.html
