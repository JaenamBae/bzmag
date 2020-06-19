#include "resource_test.h"

#include <map>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "core/primitivetype.h"
#include "core/uri.h"
#include "core/resource.h"
#include "core/resourcemanager.h"

using namespace bzmag;

//-----------------------------------------------------------------------------
void ResourceTestCase::test_Uri_split()
{
    Uri uri;
    // test managed protocol
    uri.set("managed://img#test.png");
    BZMAGUNIT_ASSERT(uri.getProtocol() == "managed");
    BZMAGUNIT_ASSERT(uri.getPackage() == "img");
    BZMAGUNIT_ASSERT(uri.getPath() == "test.png");

    // test http protocol (ip + port)
    uri.set("http://127.0.0.1:8000/test.png");
    BZMAGUNIT_ASSERT(uri.getProtocol() == "http");
    BZMAGUNIT_ASSERT(uri.getIp() == "127.0.0.1");
    BZMAGUNIT_ASSERT(uri.getPort() == "8000");
    BZMAGUNIT_ASSERT(uri.getPath() == "test.png");

    // test http protocol (only ip)
    uri.set("http://127.0.0.1/test.png");
    BZMAGUNIT_ASSERT(uri.getProtocol() == "http");
    BZMAGUNIT_ASSERT(uri.getIp() == "127.0.0.1");
    BZMAGUNIT_ASSERT(uri.getPort() == "");
    BZMAGUNIT_ASSERT(uri.getPath() == "test.png");

    // test file protocol
    // relative path
    uri.set("file://test.png");
    BZMAGUNIT_ASSERT(uri.getProtocol() == "file");
    BZMAGUNIT_ASSERT(uri.getPath() == "test.png");

    // absolute path
    uri.set("file://C:\\test.png");
    BZMAGUNIT_ASSERT(uri.getProtocol() == "file");
    BZMAGUNIT_ASSERT(uri.getPath() == "C:\\test.png");

    // 불완전한 Uri 테스트
    uri.set("managed");
    BZMAGUNIT_ASSERT(uri.getProtocol() == "");
    BZMAGUNIT_ASSERT(uri.getPackage() == "");
    BZMAGUNIT_ASSERT(uri.getPath() == "");
    BZMAGUNIT_ASSERT(uri.getIp() == "");
    BZMAGUNIT_ASSERT(uri.getPort() == "");
    uri.set("managed://");
    BZMAGUNIT_ASSERT(uri.getProtocol() == "managed");
    BZMAGUNIT_ASSERT(uri.getPackage() == "");
    BZMAGUNIT_ASSERT(uri.getPath() == "");
    uri.set("managed://img");
    BZMAGUNIT_ASSERT(uri.getProtocol() == "managed");
    BZMAGUNIT_ASSERT(uri.getPackage() == "img");
    BZMAGUNIT_ASSERT(uri.getPath() == "");
    uri.set("managed://img#");
    BZMAGUNIT_ASSERT(uri.getProtocol() == "managed");
    BZMAGUNIT_ASSERT(uri.getPackage() == "img");
    BZMAGUNIT_ASSERT(uri.getPath() == "");
    uri.set("managed://img#test.");
    BZMAGUNIT_ASSERT(uri.getProtocol() == "managed");
    BZMAGUNIT_ASSERT(uri.getPackage() == "img");
    BZMAGUNIT_ASSERT(uri.getPath() == "test.");

    uri.set("http://");
    BZMAGUNIT_ASSERT(uri.getProtocol() == "http");
    BZMAGUNIT_ASSERT(uri.getIp() == "");
    BZMAGUNIT_ASSERT(uri.getPort() == "");
    BZMAGUNIT_ASSERT(uri.getPackage() == "");
    BZMAGUNIT_ASSERT(uri.getPath() == "");
    uri.set("http://127.0.0.1");
    BZMAGUNIT_ASSERT(uri.getProtocol() == "http");
    BZMAGUNIT_ASSERT(uri.getIp() == "127.0.0.1");
    uri.set("http://127.0.0.1:8000");
    BZMAGUNIT_ASSERT(uri.getProtocol() == "http");
    BZMAGUNIT_ASSERT(uri.getIp() == "127.0.0.1");
    BZMAGUNIT_ASSERT(uri.getPort() == "8000");
    uri.set("http://127.0.0.1:8000/");
    BZMAGUNIT_ASSERT(uri.getProtocol() == "http");
    BZMAGUNIT_ASSERT(uri.getIp() == "127.0.0.1");
    BZMAGUNIT_ASSERT(uri.getPort() == "8000");
    BZMAGUNIT_ASSERT(uri.getPath() == "");
    uri.set("http://127.0.0.1:8000/test.png");
    BZMAGUNIT_ASSERT(uri.getProtocol() == "http");
    BZMAGUNIT_ASSERT(uri.getIp() == "127.0.0.1");
    BZMAGUNIT_ASSERT(uri.getPort() == "8000");
    BZMAGUNIT_ASSERT(uri.getPath() == "test.png");

    // get uri
    uri.set("managed://img#test.png");
    BZMAGUNIT_ASSERT(uri.get() == "managed://img#test.png");

    // extract
    uri.set("managed://img#test.png");
    BZMAGUNIT_ASSERT(uri.extractFile() == "test.png");
    uri.set("managed://img#test/test.png");
    BZMAGUNIT_ASSERT(uri.extractFile() == "test.png");

    uri.set("managed://img#test.png");
    BZMAGUNIT_ASSERT(uri.extractExtension() == "png");
    uri.set("managed://img#test/test.png");
    BZMAGUNIT_ASSERT(uri.extractExtension() == "png");

    uri.set("managed://img#test.png");
    BZMAGUNIT_ASSERT(uri.extractFileName() == "test");
    uri.set("managed://img#test/test.png");
    BZMAGUNIT_ASSERT(uri.extractFileName() == "test");

    uri.set("managed://img#.png");
    BZMAGUNIT_ASSERT(uri.extractFileName() == "");
    uri.set("managed://img#test/.png");
    BZMAGUNIT_ASSERT(uri.extractFileName() == "");

    uri.set("managed://img#test.");
    BZMAGUNIT_ASSERT(uri.extractExtension() == "");
    uri.set("managed://img#test/test.");
    BZMAGUNIT_ASSERT(uri.extractExtension() == "");

    uri.set("managed://img#test/test/test.png");
    BZMAGUNIT_ASSERT(uri.extractPath() == "test/test");

    uri.set("managed://shader#test_shader00.fx");
    BZMAGUNIT_ASSERT(uri.extractPath() == "");
    BZMAGUNIT_ASSERT(uri.extractFileName() == "test_shader00");
    BZMAGUNIT_ASSERT(uri.extractFile() == "test_shader00.fx");
    BZMAGUNIT_ASSERT(uri.extractExtension() == "fx");
}


//-----------------------------------------------------------------------------
#define DATA "test test test resource"
void ResourceTestCase::test_ResourceManager_addResource()
{
    ResourceManager::instance()->initialize("");

    Resource r("managed://img#test/test/test.png");
    BZMAGUNIT_ASSERT(r.open(Resource::OPEN_WRITE | Resource::OPEN_BINARY));

    char buffer[256];
    memset(buffer, 0, 256);
    memcpy(buffer, DATA, strlen(DATA));

    BZMAGUNIT_ASSERT(r.write(buffer, 256) == 256);
    r.close();

    memset(buffer, 0, 256);
    BZMAGUNIT_ASSERT(r.open(Resource::OPEN_READ | Resource::OPEN_BINARY));
    BZMAGUNIT_ASSERT(r.read(buffer, 256) == 256);
    BZMAGUNIT_ASSERT(strcmp(buffer, DATA) == 0);
    r.close();

}
#undef DATA

//-----------------------------------------------------------------------------
void ResourceTestCase::test_ResourceManager_removeResource()
{
}

//-----------------------------------------------------------------------------
void ResourceTestCase::test_ResourceManager_findResource()
{
}
