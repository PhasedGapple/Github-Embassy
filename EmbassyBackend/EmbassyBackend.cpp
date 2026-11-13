#include <iostream>
#include <httplib.h>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main()
{
    std::ifstream file("config.json");
    json config = json::parse(file);
    
    std::string host = config["host"];
    int port = config["port"];

    std::cout << "Listening to " << host << ":" << port << std::endl;
    
    httplib::Server http;

    http.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type, Authorization"}
        });
    
    http.Options(".*", [](const httplib::Request&, httplib::Response& res) {
        res.status = 204;
        });

    http.set_mount_point("/", "./static");
    
    http.Get("/api/usr/repos", [](const httplib::Request& req, httplib::Response& res)
        {
            std::string username = req.get_param_value("username");
            res.set_content("Test", "application/json")
        });

    http.listen(host, port);
}