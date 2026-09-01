#include <bits/stdc++.h>
#define ll long long
using namespace std;

const int NORMAL = 0, UPLOADING = 1;
const int UPLOAD_USER = 1, DOWNLOAD_USER = 2, GUEST = 3;
const int OP_SCAN = 1, OP_DOWNLOAD = 2, OP_UPLOAD = 3;

struct Folder;

struct File
{
    string name;
    int size;
    int state;
    Folder *fa;
    File(const string &n, int s, int st = NORMAL, Folder *p = nullptr)
    {
    	name = n;
    	size = s;
    	state = st;
    	fa = p;
    }
};

struct Folder
{
    vector<File> file;
    vector<Folder> folder;
    string name;
    int size;
    int state;
    Folder *fa;
    bool root;
    Folder(const string &n, int st = NORMAL, Folder *p = nullptr, bool r = 0)
    {
    	name = n;
    	state = st;
    	fa = p;
    	root = r;
    }
    void addFile(const File &child)
    {
        File newFile = child;
        newFile.fa = this;
        file.push_back(newFile);
    }
    void addFolder(const Folder &child)
    {
        Folder newFolder = child;
        newFolder.fa = this;
        folder.push_back(newFolder);
    }
    Folder *findFolder(const string &fname)
    {
        for (auto &f : folder)
        {
        	if (f.name == fname)
            {
            	return &f;
            }
        }
        return nullptr;
    }
    File *findFile(const string &fname)
    {
        for (auto &f : file)
        {
        	if (f.name == fname)
            {
            	return &f;
            }
        }
        return nullptr;
    }
    int calcSize()
    {
        int tot = 0;
        for (auto &f : file)
        {
        	tot += f.size;
        }
        for (auto &f : folder)
        {
        	tot += f.calcSize();
        }
        size = tot;
        return tot;
    }
	void updateState()
    {
        bool uploading = 0;
        for (auto &f : file)
        {
        	if (f.state == UPLOADING)
        	{
        		uploading = 1;
        	}
        }
        for (auto &f : folder)
        {
            f.updateState();
            if (f.state == UPLOADING)
            {
            	uploading = 1;
            }
        }
        state = uploading ? UPLOADING : NORMAL;
    }
};

struct User
{
    string name;
    int type;
    int state;
    Folder *position;
    User(const string &n, int t)
    {
    	name = n;
    	type = t;
    	state = 0;
    	position = nullptr;
    }
};

int maxUserNumber, maxServerFlux, maxUserFlux;
int userTotal = 0;
map<string, User> userMap;
Folder rootFolder;

int getUserFlux(const User& u)
{
    if (userTotal == 0) return maxServerFlux;
    int present = maxServerFlux / userTotal;
    return min(present, maxUserFlux);
}

signed main()
{
    ios::sync_with_stdio(false), cin.tie(0), cout.tie(0);
    rootFolder = Folder("/", NORMAL, nullptr, true);
	
    return 0;
}