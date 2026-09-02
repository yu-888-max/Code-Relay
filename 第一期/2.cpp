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

void in_Max(){
	cin>>maxUserNumber>>maxServerFlux>>maxUserFlux;
}

stack<Folder> st_Folder;

void in_Object(){
	string object_Name;
	cin>>object_Name;
	if(object_Name=="-"){
		st_Folder.pop();
		return ;
	}
	int object_Size;
	cin>>object_Size;
	Folder now_Folder=st_Folder.top();
	if(object_Size!=0){
		File new_File(object_Name,object_Size,NORMAL,&now_Folder);
		now_Folder.addFile(new_File);
	}
	else{
		Folder new_Folder(object_Name,NORMAL,&now_Folder,false);
		st_Folder.push(new_Folder);
		now_Folder.addFolder(new_Folder);
		in_Object();
	}
}

#define WA cout<<"unsuccess\n"
#define AC cout<<"success\n"

#define Operation1 "connect"
#define Operation2 "quit"
#define Operation3 "cd"
#define Operation4 "cd.."
#define Operation5 "download"
#define Operation6 "upload"

void Op1(string User_Name){
	int User_type;
	cin>>User_type;
	auto had_this_User=userMap.find(User_Name);
	if(had_this_User!=userMap.end()){
		WA;
		return ;
	}
	if(userTotal==maxUserNumber){
		WA;
		return ;
	}
	AC;
	++userTotal;
	switch(User_type){
	case 1:
		userMap.emplace(User_Name,User(User_Name,UPLOAD_USER));
		break;
	case 2:
		userMap.emplace(User_Name,User(User_Name,DOWNLOAD_USER));
		break;
	case 3:
		userMap.emplace(User_Name,User(User_Name,GUEST));
		break;
	}
	return ;
}

void Op2(string User_Name){
	auto had_this_User=userMap.find(User_Name);
	if(had_this_User==userMap.end()){
		WA;
		return ;
	}
	AC;
	userMap.erase(User_Name);
	--userTotal;
	return ;
}

void in_User(){
	while(1){
		string User_Name,Ope;
		cin>>User_Name;
		
		if(User_Name=="down"){
			return ;
		}
		
		cin>>Ope;
		
		if(Ope==Operation1){
			Op1(User_Name);
		}
		
		if(Ope==Operation2){
			Op2(User_Name);
		}
	}
}

void solve(){
	rootFolder=Folder("/",NORMAL,nullptr,true);
	st_Folder.push(rootFolder);
	
	in_Max();
	in_Object();
	in_User();
}

signed main()
{
	ios::sync_with_stdio(0);
	cin.tie(0),cout.tie(0);
	int _T=1;
//	cin>>_T;
	while(_T--){
		solve();
	}
	return 0;
}
