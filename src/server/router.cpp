#include "router.h"
#include "request/method.h"
#include "internal/tools/string.h"

#include <regex>

using namespace cpphttp;
using namespace response;
using namespace server;
using namespace request;

class router::impl
{
public:
    inline std::string process(cpphttp::request::request &req) const
    {
        if (!req.isReady())
            throw std::invalid_argument("Invalid requests are not allowed");

        response::response res;
        std::string errorValue;
        callFunctions(req, res, errorValue);
        if (res.hasEnded())
            return res.toString();

        callErrorFunctions(errorValue, req, res);

        if (!res.hasEnded())
            defaultError(res);

        return res.toString();
    }

    inline void onError(error_function f) noexcept
    {
        m_errorFunctions.push_back(f);
    }

    inline void onAll(const std::string &pathStartingWith, router_function function) noexcept
    {
        m_functions.push_back({pathStartingWith, std::regex(pathStartingWith), function, method::UNKNOWN});
    }

    inline void onGet(const std::string &path, router_function function) noexcept
    {
        m_functions.push_back({path, extractRegexFromPath(path), function, method::GET});
    }

    inline void onPost(const std::string &path, router_function function) noexcept
    {
        m_functions.push_back({path, extractRegexFromPath(path), function, method::POST});
    }

private:
    struct functionInfo
    {
        std::string path;
        std::regex regex;
        router_function function;
        method functionMethod;
    };

    std::vector<error_function> m_errorFunctions;
    std::vector<functionInfo> m_functions;

    static inline bool validPath(const request::header &header, const functionInfo &info)
    {
        if (info.functionMethod == method::UNKNOWN)
            return std::regex_search(header.getPath(), info.regex, std::regex_constants::match_continuous);
        return header.getMethod() == info.functionMethod && std::regex_match(header.getPath(), info.regex);
    }

    inline void callFunctions(request::request &req, response::response &res, std::string &errorValue) const noexcept
    {
        auto errorCallback = [&errorValue](std::string value) {
            if (value.size())
                errorValue = value;
            else
                errorValue = DEFAULT_SERVER_ERROR;
        };
        for (auto &funcInfo : m_functions)
        {
            if (res.hasEnded() || errorValue.size())
                return;

            if (validPath(req.header(), funcInfo))
            {
                req.loadParamFromUrl(funcInfo.path);
                funcInfo.function(req, res, errorCallback);
            }
        }
    }

    inline void callErrorFunctions(const std::string &error, request::request &req, response::response &res) const noexcept
    {
        for (auto f : m_errorFunctions)
        {
            if (res.hasEnded())
                return;
            f(error, req, res);
        }
    }

    static inline void defaultError(response::response &res) noexcept
    {
        res.status(status::_500);
        res.send(DEFAULT_SERVER_ERROR);
    }

    static inline std::regex extractRegexFromPath(const std::string &path) noexcept
    {
        auto splittedPath = tools::split(path, "/");
        std::string regexPath = "";
        for (auto &value : splittedPath)
        {
            regexPath += "/";
            if (value.size() && value[0] == ':')
                regexPath += "(.*)";
            else
                regexPath += value;
        }
        if (regexPath.empty())
            regexPath = "/";
        return std::regex(regexPath);
    }
};

router::router() : m_impl(std::make_unique<impl>()) {}

router::~router() {}

std::string router::process(cpphttp::request::request &req) const
{
    return m_impl->process(req);
}

void router::onError(error_function f) noexcept
{
    m_impl->onError(f);
}

void router::onAll(const std::string &pathStartingWith, router_function function) noexcept
{
    m_impl->onAll(pathStartingWith, function);
}

void router::onGet(const std::string &pathStartingWith, router_function function) noexcept
{
    m_impl->onGet(pathStartingWith, function);
}

void router::onPost(const std::string &pathStartingWith, router_function function) noexcept
{
    m_impl->onPost(pathStartingWith, function);
}

router::router(router &&toCopy)
{
    m_impl = std::move(toCopy.m_impl);
    toCopy.m_impl = std::make_unique<impl>();
}

router &router::operator=(router &&toCopy)
{
    if (this == &toCopy)
        return *this;
    m_impl = std::move(toCopy.m_impl);
    toCopy.m_impl = std::make_unique<impl>();
    return *this;
}
