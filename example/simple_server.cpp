#include <cpphttp.h>
#include <iostream>

using namespace cpphttp::server;
using namespace cpphttp::request;
using namespace cpphttp::response;

int main(void)
{
    // Create a server at port 8080
    server myserver(8080);

    // Create a simple router with several routes
    router myrouter;

    // main page
    myrouter.onGet("/", [](cpphttp::request::request &, cpphttp::response::response &res, error_callback) {
        res.send("<h1>Welcome to simple server</h1><a href='/form'>Go to form page</a>");
    });

    // Form get & post path
    myrouter.onGet("/form", [](cpphttp::request::request &, cpphttp::response::response &res, error_callback) {
        res.send("<!DOCTYPE html>"
                 "<html lang=\"en\">"
                 "<head>"
                 "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />"
                 "</head>"
                 "<body>"
                 "<h1>Test form</h1>"
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
                 "<a href='/'>Go back to home page</a>"
                 "</body>");
    });
    myrouter.onPost("/form", [](cpphttp::request::request &req, cpphttp::response::response &res, error_callback) {
        res.send("<!DOCTYPE html>"
                 "<html lang=\"en\">"
                 "<head>"
                 "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />"
                 "</head>"
                 "<body>"
                 "<h1>Form has been sent to the server</h1>"
                 "<div>Name was : " +
                 req.getParam("user_name") +
                 "</div>"
                 "<div>Email was : " +
                 req.getParam("user_email") +
                 "</div>"
                 "<a href='/'>Go back to home page</a>"
                 "</body>");
    });
    // Example of onAll function
    // All path except the three ones above will call this function (order matters!)
    myrouter.onAll("/", [](cpphttp::request::request &req, cpphttp::response::response &res, error_callback) {
        std::cout << "Visited path : " << req.header().getPath() << std::endl;
    });

    // Example of saving a parameter and use it later
    myrouter.onGet(
        "/param",
        [](cpphttp::request::request &req, cpphttp::response::response &res, error_callback) { req.set("myParam", 42); });
    myrouter.onGet(
        "/param", [](cpphttp::request::request &req, cpphttp::response::response &res, error_callback) {
            res.send("My param was : " + req.get("myParam"));
        });

    // Example of using variadic function
    myrouter.onGet(
        "/variadic",
        [](cpphttp::request::request &req, cpphttp::response::response &res, error_callback) { res.write("Something"); },
        [](cpphttp::request::request &req, cpphttp::response::response &res, error_callback) {
        // res.send is the same as res.write + res.end
        // on res.end no other functions are called and the result is sent to the user
        res.write(" has been written.");
        res.end(); });

    // Example of setting your own error function (you can test it by going to any wrong path)
    myrouter.onError([](const std::string &errorText, cpphttp::request::request &, cpphttp::response::response &res) {
        if (errorText.size())
            res.send("Simple server error : " + errorText);
        else
            res.send("Simple server error");
    });

    // Example of calling the error function from path function
    myrouter.onGet(
        "/error",
        [](cpphttp::request::request &req, cpphttp::response::response &res, error_callback error) { error("from /error path"); });

    // Example of param in URL (test with /item/42 for example)
    myrouter.onGet("/item/:id",
                   [](cpphttp::request::request &req, cpphttp::response::response &res, error_callback) { res.send("Item id is " + req.getParam("id")); });

    // Set the server public folder URL & path
    myserver.setPublicFolder("/public", "../images");

    // Set the router to the server
    myserver.setRouter(std::move(myrouter));

    // Start the server
    myserver.start();
    return 0;
}
