#pragma once

#include "Usings.h"
#include "nlohmann/json.hpp"

namespace Gengine
{
    template <typename T>
    class Persistence
	{
    public:
        bool TrySave(const string& fileName, const string& dirPath = string()) const
        {
            string path = dirPath.empty() ? GetAppDataPath() : dirPath;
            std::filesystem::create_directories(path);
            std::ofstream file(path + "/" + fileName + ".json");
            if (file.is_open())
            {
                file << GetJson().dump(4);
                file.close();
                return true;
            }
            else
            {
                return false;
            }
        }

        bool TryLoad(const string& fileName, T& output, const  string& dirPath = string())
        {
            string path = dirPath.empty() ? GetAppDataPath() : dirPath;
            nlohmann::json j;
            if (TryLoadJson(fileName, path, j))
            {
                output = CreateInstanceFromJson(j);
                return true;
            }
            else
                return false;
        }

	protected:
        virtual nlohmann::json GetJson() const = 0;
        virtual T CreateInstanceFromJson(nlohmann::json j) const = 0;
    private:
        static bool TryLoadJson(const string& fileName, string& dirPath, nlohmann::json& output)
        {
            std::ifstream file(dirPath + "/" + fileName + ".json");
            if (file.is_open())
            {
                file >> output;
                file.close();
                return true;
            }
            return false;
        }

        static string GetAppDataPath()
        {
#ifdef GG_PLATFORM_WINDOWS
            char* appDataPath = nullptr;
            size_t len;
            _dupenv_s(&appDataPath, &len, "APPDATA");
            string fullPath = string(appDataPath) + "/GENGINE/";
            free(appDataPath);
            return fullPath;
#elif __APPLE__
            return string(getenv("HOME")) + "/Library/Application Support";
#else
            return string(getenv("HOME")) + "/.local/share";
#endif
        }
	};
}