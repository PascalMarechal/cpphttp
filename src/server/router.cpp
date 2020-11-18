#include "router.h"

using namespace cpphttp::server;

class router::impl
{
public:
    std::string process(cpphttp::request::request &req)
    {
        if (!req.isReady())
            throw std::invalid_argument("Invalid requests are not allowed");
        return "500 Internal Server Error";
    }
};

router::router() : m_impl(std::make_unique<impl>()) {}

router::~router() {}

std::string router::process(cpphttp::request::request &req)
{
    return m_impl->process(req);
}
