#pragma once
#include "AHZFormLookup.h"
#include "AHZUtilities.h"
#include <vector>
#include <map>
#include <windows.h>

class CAHZExternalFormTable
{
public:
    static void LoadACTIForms(CAHZFormLookup& loader, std::vector<std::string>& mHudFiles)
    {
        std::string skyrimDataPath = CAHZUtilities::GetSkyrimDataPath();

        // Foreach mhud file, load in the lookup table entries
        std::vector<std::string>::iterator p;
        for (p = mHudFiles.begin(); p != mHudFiles.end(); p++) {
            logger::info("Loading ACTI Forms from '{}'", (*p).c_str());
            std::string fullPath = skyrimDataPath + *p;

            // Get the number of entries
            uint32_t iNumOfEntries = GetPrivateProfileIntA("LookupTable", "iNumOfEntries", 0, fullPath.c_str());
            logger::info("Loading {} ACTI Form Entries", iNumOfEntries);
            std::cout << iNumOfEntries << std::endl;

            // Get each entry and load into the lookup table
            for (uint32_t i = 0; i < iNumOfEntries; i++) {
                char value[32] = "";
                char returnValue[1024] = "";
                sprintf_s(value, (size_t)32, "%d", i + 1);
                std::string entrName("oEntry");
                entrName.append(value);
                GetPrivateProfileStringA("LookupTable", entrName.c_str(), "", returnValue, size_t(1024), fullPath.c_str());
                if (strlen(returnValue)) {
                    std::string  returnVal = returnValue;
                    AHZLUTObject lutObject = CAHZUtilities::ParseLUTObject(returnVal);

                    if (!lutObject.IsEmpty()) {
                        loader.AddFormID(lutObject.BaseMod, lutObject.BaseFormID, lutObject.TargetMod, lutObject.TargetFormID);
                    } else {
                        logger::info("Could not entry Entry: {}", returnValue);
                    }
                }
            }
        }
    };

    static void LoadVMVariables(CAHZFormLookup& loader, std::vector<std::string>& mHudFiles)
    {
        std::string skyrimDataPath = CAHZUtilities::GetSkyrimDataPath();

        std::vector<std::string>::iterator p;
        for (p = mHudFiles.begin(); p != mHudFiles.end(); p++) {
            logger::info("Loading Script Variables from '{}'", (*p).c_str());
            std::string fullPath = skyrimDataPath + *p;
            // Get the number of entries
            uint32_t iNumOfEntries = GetPrivateProfileIntA("ScriptVariables", "iNumOfVariables", 0, fullPath.c_str());
            logger::info("Loading {} Script Variable(s)", iNumOfEntries);
            std::cout << iNumOfEntries << std::endl;

            // Get each entry and load into the lookup table
            for (uint32_t i = 0; i < iNumOfEntries; i++) {
                char value[32] = "";
                char returnValue[1024] = "";
                sprintf_s(value, (size_t)32, "%d", i + 1);
                std::string entrName("sVariable");
                entrName.append(value);
                GetPrivateProfileStringA("ScriptVariables", entrName.c_str(), "", returnValue, size_t(1024), fullPath.c_str());

                if (strlen(returnValue)) {
                    loader.AddScriptVarable(std::string(returnValue));
                } else {
                    logger::info("Could not load script variable: {}", returnValue);
                }
            }
        }
    };

private:
    CAHZExternalFormTable() = default;
    ~CAHZExternalFormTable() = default;
};
