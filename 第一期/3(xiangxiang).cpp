#include <bits/stdc++.h>
using namespace std;
#define int long long

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
    // 2026/9/2 12:52
    int active;
    int transfer_type;
    File *transfer_file;
    Folder *transfer_folder;
    int transfer_size;
    //
    User(const string &n, int t)
    {
        name = n;
        type = t;
        state = 0;
        position = nullptr;
        active = 0;
        transfer_type = -1;
        transfer_file = nullptr;
        transfer_folder = nullptr;
        transfer_size = 0;
    }
};

int maxUserNumber, maxServerFlux, maxUserFlux;
int userTotal = 0;
map<string, User> userMap;
Folder rootFolder;
int activeTransfers = 0;
int currentTime = 0;

struct Event
{
    int finishTime;
    string userName;
    int eventType;
    bool operator>(const Event &other) const
    {
        return finishTime > other.finishTime;
    }
};
priority_queue<Event, vector<Event>, greater<Event>> eventQueue;

int getUserFlux(const User &u)
{
    if (userTotal == 0)
        return maxServerFlux;
    int present = maxServerFlux / userTotal;
    return min(present, maxUserFlux);
}

void in_Max()
{
    cin >> maxUserNumber >> maxServerFlux >> maxUserFlux;
}

stack<Folder> st_Folder;

int getCurrentFlux()
{
    if (activeTransfers == 0)
        return maxServerFlux;
    int present = maxServerFlux / activeTransfers;
    return min(present, maxUserFlux);
}

void in_Object()
{
    string object_Name;
    cin >> object_Name;
    if (object_Name == "-")
    {
        st_Folder.pop();
        return;
    }
    int object_Size;
    cin >> object_Size;
    Folder now_Folder = st_Folder.top();
    if (object_Size != 0)
    {
        File new_File(object_Name, object_Size, NORMAL, &now_Folder);
        now_Folder.addFile(new_File);
    }
    else
    {
        Folder new_Folder(object_Name, NORMAL, &now_Folder, false);
        st_Folder.push(new_Folder);
        now_Folder.addFolder(new_Folder);
        in_Object();
    }
}

#define WA cout << "unsuccess\n"
#define AC cout << "success\n"

#define Operation1 "connect"
#define Operation2 "quit"
#define Operation3 "cd"
#define Operation4 "cd.."
#define Operation5 "download"
#define Operation6 "upload"

void Op1(string User_Name)
{
    int User_type;
    cin >> User_type;
    auto had_this_User = userMap.find(User_Name);
    if (had_this_User != userMap.end())
    {
        WA;
        return;
    }
    if (userTotal == maxUserNumber)
    {
        WA;
        return;
    }
    AC;
    ++userTotal;
    User newUser(User_Name, User_type);
    newUser.position = &rootFolder; // 2026/9/2 10:33 想想
    userMap.emplace(User_Name, newUser);
    return;
}

void Op2(string User_Name)
{
    auto had_this_User = userMap.find(User_Name);
    if (had_this_User == userMap.end())
    {
        WA;
        return;
    }
    AC;
    userMap.erase(User_Name);
    --userTotal;
    return;
}
/*Thinking日志:
2026/9/2：
    10:00 开始理解
    10:09 启动原神
    10:12 怎么要指针写法，玩原神
    10:29 Op3初步完成，发现in_User，Op1貌似有锅 修改in_User,Op1（
    10:37 修锅结束，启动O神4
    10:56 Op4基本完成，最简单的一集，看看Op5
    10:57 失去所有写题欲望，什么叫你下载的流量是动态的还要建一个文件夹树，倒闭了，跑路了，打算吃完饭回来再搞
    11:14 搞Op5,Op6前置
    11:17 倒闭了，先去干点别的
    12:52 继续搞前置，打算建完树交了
*/
/*
11:22:
不修了，跑路了说实在的早就有这个念头了 没觉得自己对不起谁 对不起的只有自己吧 也早就不在乎操作了因为我觉得自己有俩个人格 在大家面前的是大家所知道的认真摸鱼的人 另外一个就是我不在乎成绩的一个学生 很多事情只有只在乎自己了才舒服 每天考虑这顾及那的早就不想学了 糟糕的心情也很多现在是彻底解放了也很感谢学校很感谢老师很感谢同学哈哈哈没有你们我哪来的现在的大门不出二门不迈的躺平生活这就是我想要的躺平生活 谢谢哈看在这些的份上也更加不在乎你们的看法了 谢谢你们哈最后也就是接龙的东西我还没交 交完就彻底算人消失了也别说我交的慢 爱要不要免费的东西别急看我心情懂吗 老子才是老大
*/

// 我的注释除了日志随便删吧

void Op3(string User_Name)
{
    string folder_Name;
    cin >> folder_Name;
    auto it = userMap.find(User_Name);
    if (it == userMap.end())
    {
        WA;
        return;
    }
    User &u = it->second;
    if (u.position == nullptr)
    {
        WA;
        return;
    }
    Folder *cur = u.position;
    Folder *target = cur->findFolder(folder_Name);
    if (target == nullptr || target->state == UPLOADING)
    {
        WA;
        return;
    }
    u.position = target;
    AC;
}
void Op4(string User_Name)
{
    auto it = userMap.find(User_Name);
    if (it == userMap.end())
    {
        WA;
        return;
    }
    User &u = it->second;
    if (u.position == nullptr || u.position->root)
    {
        WA;
        return;
    }
    u.position = u.position->fa;
    AC;
}
void Op5(string User_name)
{
}
void in_User()
{
    while (1)
    {
        string token;
        cin >> token;
        if (token == "down")
            return;
        int t = stoi(token);
        string User_Name, Ope;
        cin >> User_Name;
        if (User_Name == "down")
        {
            return;
        }
        cin >> Ope;
        if (Ope == Operation1)
            Op1(User_Name);
        else if (Ope == Operation2)
            Op2(User_Name);
        else if (Ope == Operation3)
            Op3(User_Name);
        else if (Ope == Operation4)
            Op4(User_Name);
        else if (Ope == Operation5)
            Op5(User_Name);
    }
}

/*原in_User
void in_User()
{
    while (1)
    {

        string User_Name, Ope;
        cin >> User_Name;

        if (User_Name == "down")
        {
            return;
        }

        cin >> Ope;

        if (Ope == Operation1)
        {
            Op1(User_Name);
        }

        if (Ope == Operation2)
        {
            Op2(User_Name);
        }
    }
}
*/
void solve()
{
    rootFolder = Folder("/", NORMAL, nullptr, true);
    st_Folder.push(rootFolder);

    in_Max();
    in_Object();
    in_User();
}

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);
    int _T = 1;
    //	cin>>_T;
    while (_T--)
    {
        solve();
    }
    return 0;
}
