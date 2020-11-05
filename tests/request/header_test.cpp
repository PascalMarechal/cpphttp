#include "request/header.h"
#include <gtest/gtest.h>
#include "../tools/data_reader.h"
#include "tools/string.h"

using namespace cpphttp::request;

std::string postRequest = readFile("./data/post_request.txt");
std::string jsonPostRequest = readFile("./data/post_json_request.txt");
std::string postRequestWithIncorrectLength = readFile("./data/post_request_incorrect_length.txt");

TEST(Header, EmptyHeader)
{
    header head;
    EXPECT_EQ(head.isReady(), false);
}

TEST(Header, CompleteHeader)
{
    header head;
    head.read("GET /index HTTP/1.0\n\n");
    EXPECT_EQ(head.isReady(), true);
}

TEST(Header, CompleteWindowsHeader)
{
    header head;
    head.read("GET /index HTTP/1.0\r\n\r\n");
    EXPECT_EQ(head.isReady(), true);
}

TEST(Header, IncompleteHeader)
{
    header head;
    head.read("GET /index HTTP/1.0");
    EXPECT_EQ(head.isReady(), false);
}

TEST(Header, MissingRequestLine)
{
    header head;
    head.read("User-Agent: HTTPTool/1.0\n\n");
    EXPECT_EQ(head.isReady(), false);
}

TEST(Header, IncompleteRequestLine)
{
    header head;
    head.read("GET HTTP/1.0\n\n");
    EXPECT_EQ(head.isReady(), false);
}

TEST(Header, WrongOrderRequestLine)
{
    header head;
    head.read("GET HTTP/1.0 /hello\n\n");
    EXPECT_EQ(head.isReady(), false);
}

TEST(Header, CompleteHeaderData)
{
    header head;
    head.read("GET /index HTTP/1.0\n\n");
    EXPECT_EQ(head.isReady(), true);
    EXPECT_EQ(head.getMethod(), method::GET);
    EXPECT_EQ(head.getPath(), "/index");
    EXPECT_EQ(head.getVersion(), version::_1);
}

TEST(Header, ChunkedHeaderData)
{
    header head;
    head.read("GET /ind");
    head.read("ex HTTP/1.0\n\n");
    EXPECT_EQ(head.isReady(), true);
    EXPECT_EQ(head.getMethod(), method::GET);
    EXPECT_EQ(head.getPath(), "/index");
    EXPECT_EQ(head.getVersion(), version::_1);
}

TEST(Header, RemoveHeaderFromRemainingIncomingData)
{
    header head;
    auto body = head.read(postRequest);
    EXPECT_EQ(body, "\n\nhome=Cosby&favorite+flavor=flies\n");
}

TEST(Header, LetRemainingJSONBody)
{
    header head;
    auto splittedRequest = split(jsonPostRequest, '[');
    std::string lastBodyChunk = "";
    for(const auto& value : splittedRequest)
        lastBodyChunk = head.read(value);
    EXPECT_EQ(lastBodyChunk, "\"diesel\",\"benzin\"]\n}\n");
}

TEST(Header, ReadExpectedBodySize)
{
    header head;
    auto body = head.read(postRequest);
    EXPECT_EQ(head.getExpectedBodySize(), 32);
}

TEST(Header, IncorrectBodySizeValue)
{
    header head;
    auto body = head.read(postRequestWithIncorrectLength);
    EXPECT_EQ(head.getExpectedBodySize(), 0);
}
