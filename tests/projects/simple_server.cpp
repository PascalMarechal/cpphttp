#include "cpphttp.h"

using namespace cpphttp::server;
using namespace cpphttp::request;
using namespace cpphttp::response;

int main(void)
{
    // Create a server at port 8080
    server myserver(8080);

    // Create a simple router with several routes
    // One main page
    // Form get & post path
    router myrouter;
    myrouter.onGet("/", [](cpphttp::request::request &, cpphttp::response::response &res, error_callback) {
        res.send("<h1>Welcome to simple server</h1><a href='/form'>Go to form page</a>");
    });

    myrouter.onGet("/form", [](cpphttp::request::request &, cpphttp::response::response &res, error_callback) {
        res.send("<h1>Test form</h1>"
                 "<form action='/form' method='post'><div><label for='name'>Name :</label>"
                 "<input type='text' id='name' name='user_name'>"
                 "</div>"
                 "<div>"
                 "<label for='mail'>Email :</label>"
                 "<input type='email' id='mail' name='user_email'>"
                 "</div>"
                 "<div class='button'>"
                 "<button type='submit'>Send message</button>"
                 "</div>"
                 "</form>"
                 "<a href='/'>Go back to home page</a>");
    });

    myrouter.onPost("/form", [](cpphttp::request::request &req, cpphttp::response::response &res, error_callback) {
        res.send("<h1>Form has been sent to the server</h1>"
                 "<div>Name was : " +
                 req.getParam("user_name") +
                 "</div>"
                 "<div>Email was : " +
                 req.getParam("user_email") +
                 "</div>"
                 "<a href='/'>Go back to home page</a>");
    });

    // Set the router to the server
    myserver.setRouter(std::move(myrouter));

    // Start the server
    myserver.start();
    return 0;
}
