#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <iostream>
#include <httplib.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <thread>
#include <vector>

using json = nlohmann::json;

int main()
{
    std::ifstream file("config.json");
    json config = json::parse(file);
    
    std::string host = config["host"];
    int port = config["port"];


    std::cout << "Listening to " << host << ":" << port << std::endl;
    
    std::string token = config["github_fine_grained_personal_access_token"].get<std::string>();

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
    
    http.Get("/api/usr/repos", [&](const httplib::Request& req, httplib::Response& res)
        {
            std::string cookie = req.get_header_value("Cookie");
            std::string username;
            size_t start = 0;
            while (start < cookie.size())
            {
                size_t end = cookie.find(';', start);
                std::string item = cookie.substr(start, end == std::string::npos ? std::string::npos : end - start);
                size_t name_start = item.find_first_not_of(" \t");
                if (name_start != std::string::npos)
                {
                    item.erase(0, name_start);
                    size_t equals = item.find('=');
                    if (equals != std::string::npos && item.substr(0, equals) == "user")
                    {
                        username = httplib::decode_uri_component(item.substr(equals + 1));
                        break;
                    }
                }
                if (end == std::string::npos)
                    break;
                start = end + 1;
            }

            if (username.empty())
            {
                res.status = 400;
                res.set_content("Missing user cookie", "text/plain");
                return;
            }

            httplib::Client api("https://api.github.com");
            api.set_default_headers({
                {"User-Agent", "GH Embassy"},
                {"Authorization", "Bearer " + token},
                {"Accept", "application/vnd.github+json"}
                });

            auto res2 = api.Get("/users/" + httplib::encode_uri_component(username) + "/repos?type=owner&per_page=100");
            if (!res2)
            {
                res.status = 502;
                res.set_content("GitHub API request failed", "text/plain");
                return;
            }
            if (res2->status != 200)
            {
                res.status = res2->status;
                res.set_content(res2->body, "application/json");
                return;
            }

            json info = json::array();
            std::vector<std::thread> threads;
            
            for (const auto& repos : json::parse(res2->body))
            {
                info.push_back(json::object());
                info.back()["name"] = repos["name"];
                info.back()["full_name"] = repos["full_name"];
                info.back()["description"] = repos["description"];
                info.back()["stars"] = repos["stargazers_count"];
                info.back()["lang"] = repos["language"];
                info.back()["code"] = repos["svn_url"];
                

                size_t index = info.size() - 1;

                threads.emplace_back([&, index, repos]() {
                    httplib::Client rpi("https://api.github.com");
                    std::string full_name = repos["full_name"].get<std::string>();
                    std::string url = "/repos/" + full_name + "/releases";
                    rpi.set_default_headers({
                        {"User-Agent", "GH Embassy"},
                        {"Authorization", "Bearer " + token},
                        {"Accept", "application/vnd.github+json"}
                        });

                    auto res = rpi.Get(url);
                    if (res && res->status == 200) {
                        json releases = json::parse(res->body);
                        if (!releases.empty())
                            info[index]["releases"] = repos["svn_url"].get<std::string>() + "/releases";
                    }
                    auto pages = rpi.Get("/repos/" + full_name + "/pages");

                    if (pages && pages->status == 200) {
                        json pageData = json::parse(pages->body);

                        info[index]["pages"] = pageData["html_url"];
                    }
                    });
            }

            for (auto& t : threads)
                t.join();

            res.set_content(info.dump(), "application/json");
        });

    http.listen(host, port);
}