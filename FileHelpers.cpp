#include "pch.h"
#include "FileHelpers.h"

std::string FileHelpers::OpenFileDialog()
{
    char filename[MAX_PATH] = "";

    OPENFILENAMEA ofn{};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;

    ofn.lpstrFilter = "Binary Files\0*.bin\0"
        "All Files\0*.*\0";

    ofn.nFilterIndex = 1;

    ofn.Flags =
        OFN_PATHMUSTEXIST |
        OFN_FILEMUSTEXIST;

    if (GetOpenFileNameA(&ofn))
    {
        return filename;
    }

    return "";
}

std::string FileHelpers::SaveFileDialog()
{
    char filename[MAX_PATH] = "";

    OPENFILENAMEA ofn{};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;

    ofn.lpstrFilter =
        "Binary Files\0*.bin\0"
        "All Files\0*.*\0";

    ofn.nFilterIndex = 1;

    ofn.lpstrDefExt = "world";

    ofn.Flags =
        OFN_PATHMUSTEXIST |
        OFN_OVERWRITEPROMPT;

    if (GetSaveFileNameA(&ofn))
    {
        return filename;
    }

    return "";
}
