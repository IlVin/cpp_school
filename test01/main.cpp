// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>


struct DomPath {
    std::string Domain;
    std::string Path;
};

using DpVector = std::vector<DomPath>;
using DpPair = std::pair<std::string, int>;

void ToLower(std::string& str){
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

int isHttp(std::string str) {
    if (str.substr(0, 4) == "http")
        return 4;
    return 0;
}

int isHttps(std::string str) {
    if (str.substr(0, 5) == "https")
        return 5;
    return 0;
}

int CntUrlPrefix(std::string str) {
    if (isHttps(str) && str.substr(5, 3) == "://") {
        return 8;
    } else if (isHttp(str) && str.substr(4, 3) == "://") {
        return 7;
    }
    return 0;
}

int CntDomainChars(std::string str) {
    int i = 0;
    while (
        (str[i] >= 'a' && str[i] <= 'z')
        || (str[i] >= 'A' && str[i] <= 'Z')
        || (str[i] >= '0' && str[i] <= '9')
        || (str[i] == '.')
        || (str[i] == '-')) {
        i++;
    }
    return i;
}

int CntPathChars(std::string str) {
    int i = 0;
    while (
        (str[i] >= 'a' && str[i] <= 'z')
        || (str[i] >= 'A' && str[i] <= 'Z')
        || (str[i] >= '0' && str[i] <= '9')
        || (str[i] == '.')
        || (str[i] == ',')
        || (str[i] == '/')
        || (str[i] == '+')
        || (str[i] == '_')
    ) {
        i++;
    }
    return i;
}

void FindDomPath (std::string str, DpVector& DP) {

    int pos, cnt;
    do {
        pos = str.find("http");
        if (pos != std::string::npos) {
            if (pos == 0 || str[pos-1] == ' ') {
                str = std::string(str, pos);
                cnt = CntUrlPrefix(str);
                if (cnt > 0) {
                    str = std::string(str, cnt);
                    cnt = CntDomainChars(str);
                    if (cnt > 0) {
                        DomPath dp;
                        dp.Domain = std::string(str, 0, cnt);
                        ToLower(dp.Domain);
                        str = std::string(str,cnt);
                        cnt = CntPathChars(str);
                        if (cnt > 0 && str[0] == '/') {
                            dp.Path = std::string(str, 0, cnt);
                            str = std::string(str, cnt);
                        } else {
                            dp.Path = '/';
                        }
                        DP.push_back(dp);
                        continue;
                    }
                }
                str = std::string(str, 4);
                continue;
            }
            str = std::string(str, pos + 4);
        }
    } while (pos != std::string::npos);
}

int main(int argc, char *argv[]) {

    int N = 0;
    std::string ifn;
    std::string ofn;

    if (argc == 5) {
        if (std::string(argv[1]) != "-n")
            return -1;
        N = std::stoi(argv[2]);
        ifn = argv[3];
        ofn = argv[4];
    } else if (argc == 3) {
        ifn = argv[1];
        ofn = argv[2];
    } else {
        return -1;
    }

    std::ifstream ifs{ ifn };
    if (!ifs.is_open())
        return -1;

    std::ofstream ofs{ ofn };
    if (!ofs.is_open())
        return -1;

    int urls_cnt = 0;
    std::map<std::string, int> domains;
    std::map<std::string, int> paths;

    std::string buf;

    while (!ifs.eof()) {
        getline(ifs, buf);
        DpVector DP;
        FindDomPath(buf, DP);
        for (const auto& dp: DP) {
            urls_cnt++;
            domains[dp.Domain]++;
            paths[dp.Path]++;
        }
    };

    ofs << "total urls " << urls_cnt << ", domains " << domains.size() << ", paths " << paths.size() << std::endl << std::endl;

    ofs << "top domains" << std::endl;
    std::vector<DpPair> dpDomains;
    for (const auto& domainPair: domains) {
        dpDomains.push_back(domainPair);
    }
    std::sort(dpDomains.begin(), dpDomains.end(), [](DpPair a, DpPair b){
        return (a.second > b.second) || (a.second == b.second && a.first < b.first);
    });
    for (int i = 0; i < N && i < dpDomains.size(); ++i) {
        ofs << dpDomains[i].second << " " << dpDomains[i].first << std::endl;
    }
    ofs << std::endl;

    ofs << "top paths" << std::endl;
    std::vector<DpPair> dpPaths;
    for (const auto& pathPair: paths) {
        dpPaths.push_back(pathPair);
    }
    std::sort(dpPaths.begin(), dpPaths.end(), [](DpPair a, DpPair b){
            return (a.second > b.second) || (a.second == b.second && a.first < b.first);
    });
    for (int i = 0; i < N && i < dpPaths.size(); ++i) {
        ofs << dpPaths[i].second << " " << dpPaths[i].first << std::endl;
    }


    return 0;
}

