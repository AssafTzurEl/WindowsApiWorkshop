#include <Windows.h>
#include <iostream>
#include <vector>
#include <exception>
#include <memory>
#include <stdint.h>

class FileFindException
{
public:
    FileFindException(const std::wstring &msg)
        : message(msg)
    {}

    const std::wstring message;
};

std::wstring GetLastWinApiError(DWORD err = ::GetLastError())
{
    LPVOID lpMsgBuf;

    ::FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        err,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR) (&lpMsgBuf),
        0,
        NULL);

    std::wstring result(static_cast<wchar_t *>(lpMsgBuf));

    LocalFree(lpMsgBuf);

    return result;
}

// Using the Composite design pattern

class FileSystemObject
{
public:
    FileSystemObject(const std::wstring &name)
        : m_name(name)
    {}

    virtual ~FileSystemObject()
    {}

    virtual void Print() abstract;

protected:
    std::wstring m_name;
};

typedef std::shared_ptr<FileSystemObject> FileSystemObjectPtr;

class File : public FileSystemObject
{
public:
    File(const std::wstring &name, uint64_t size)
        : FileSystemObject(name), m_size(size)
    {}

    void Print()
    {
        std::wcout << m_name << L"(" << m_size << L"B)" << std::endl;
    }

private:
    uint64_t m_size;
};

class Folder : FileSystemObject
{
public:
    Folder(const std::wstring &name)
        : FileSystemObject(name)
    {
        PopulateFolderContents();
    }

    void Print()
    {
        std::wcout << L"<" << m_name << L">" << std::endl;

        for (auto item : m_contents)
        {
            item->Print();
        }
    }

private:
    static const std::wstring CURRENT_FOLDER;
    static const std::wstring PARENT_FOLDER;

    void PopulateFolderContents()
    {
        WIN32_FIND_DATA fileData;

        HANDLE hFind = FindFirstFile((m_name + L"\\*.*").c_str(), &fileData);

        if (hFind == INVALID_HANDLE_VALUE)
        {
            DWORD error = GetLastError();

            if (error != ERROR_FILE_NOT_FOUND)
            {
                throw FileFindException(GetLastWinApiError(error));
            }
        }
        // else...
        do
        {
            // skip "." and "..":
            if (CURRENT_FOLDER != fileData.cFileName && PARENT_FOLDER != fileData.cFileName)
            {
                std::wstring findName = m_name + L"\\" + fileData.cFileName;

                m_contents.push_back(FileSystemObjectPtr(
                    fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ?
                    static_cast<FileSystemObject *>(new Folder(findName)) :
                    static_cast<FileSystemObject *>(new File(findName,
                        (static_cast<uint64_t>(fileData.nFileSizeHigh) << 32) + fileData.nFileSizeLow))));
            }
        } while (FindNextFile(hFind, &fileData));

        // finally
        FindClose(hFind);
    }

    std::vector<FileSystemObjectPtr> m_contents;
};

const std::wstring Folder::CURRENT_FOLDER(L".");
const std::wstring Folder::PARENT_FOLDER(L"..");

int main()
{
    Folder f(L"C:\\Dev\\WindowsApiWorkshop\\ListFiles");

    f.Print();

    return 0;
}
