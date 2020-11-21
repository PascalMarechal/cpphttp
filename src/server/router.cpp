#include "router.h"
#include "request/method.h"
#include "internal/tools/string.h"

#include <deque>
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

        callFunctions(req, res);
        if (res.hasEnded())
            return res.toString();

        callErrorFunctions("error", req, res);

        if (!res.hasEnded())
            defaultError(res);

        return res.toString();
    }

    inline void error(error_function f) noexcept
    {
        m_errorFunctions.push_back(f);
    }

    inline void use(std::string pathStartingWith, router_function function) noexcept
    {
        m_functions.push_back({pathStartingWith, std::regex(pathStartingWith), function, method::UNKNOWN});
    }

    inline void get(std::string path, router_function function) noexcept
    {
        m_functions.push_back({path, extractRegexFromPath(path), function, method::GET});
    }

private:
    struct functionInfo
    {
        std::string path;
        std::regex regex;
        router_function function;
        method functionMethod;
    };

    std::deque<error_function> m_errorFunctions;
    std::deque<functionInfo> m_functions;

    static inline bool validPath(const std::string &path, const functionInfo &info)
    {
        if (info.functionMethod == method::UNKNOWN)
            return std::regex_search(path, info.regex, std::regex_constants::match_continuous);
        return std::regex_match(path, info.regex);
    }

    inline void callFunctions(request::request &req, response::response &res) const noexcept
    {
        std::string errorVal;
        auto errorCallback = [&errorVal](std::string value) {
            if (value.size())
                errorVal = value;
            else
                errorVal = "Server Error!";
        };
        for (auto &funcInfo : m_functions)
        {
            if (res.hasEnded() || errorVal.size())
                return;

            if (validPath(req.header().getPath(), funcInfo))
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
        res.send("Internal Server Error Detected.");
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
        return std::regex(regexPath);
    }
};

router::router() : m_impl(std::make_unique<impl>()) {}

router::~router() {}

std::string router::process(cpphttp::request::request &req) const
{
    return m_impl->process(req);
}

void router::error(error_function f) noexcept
{
    m_impl->error(f);
}

void router::use(std::string pathStartingWith, router_function function) noexcept
{
    m_impl->use(pathStartingWith, function);
}

void router::get(std::string pathStartingWith, router_function function) noexcept
{
    m_impl->get(pathStartingWith, function);
}
