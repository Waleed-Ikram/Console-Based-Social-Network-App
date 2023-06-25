#include <iostream>
#include <fstream>
using namespace std;

int StringLenght(const char* str)
{
	int length = 0;
	int count = 0;
	while (str[count] != '\0')
	{
		length++;
		count++;
	}
	return length;
}
void StringCopy(char*& dest, const char*& src)
{
	int len = StringLenght(src);
	int i = 0;
	for (; i < len; i++)
	{
		dest[i] = src[i];
	}
	dest[i] = '\0';
}
char* GetStringFromBuffer(const char* str)
{
	int len = StringLenght(str);
	char* buffer = new char[len];
	StringCopy(buffer, str);
	return buffer;
}

int strcmp(char* Str1, const char* Str2)
{
	int len1 = StringLenght(Str1);
	int len2 = StringLenght(Str2);

	if (len1 != len2)
	{
		return 0;
	}
	else
	{
		for (int i = 0; i < len1; i++)
		{
			if (Str1[i] != Str2[i])
			{
				return 0;
			}
		}
	}
	return 1;
}



class Page
{
private:
	char* page_id;
	char* title;

public:
	static int total_pages;

	Page()
	{
		page_id = NULL;
		title = NULL;
	}

	~Page()
	{
		if (!page_id)
		{
			delete[] page_id;
		}

		if (!title)
		{
			delete[] title;
		}
	}

	void Read_PagesFromFile(ifstream& fin)
	{
		char buffer[80];
		fin >> buffer;
		page_id = GetStringFromBuffer(buffer);

		fin.getline(buffer, 80);
		title = GetStringFromBuffer(buffer);

	}

	char* GetID()
	{
		return page_id;
	}

	void printListView()
	{
		cout << GetID() << "---" << title << endl;
	}
};

int Page::total_pages = 0;


class User
{
private:

	char* user_id;
	char* first_name;
	char* last_name;
	User** friend_list;
	Page** liked_pages;
	int total_friends = 0;
	int total_likedpages = 0;

public:

	User()
	{
		user_id = NULL;
		first_name = NULL;
		last_name = NULL;
		friend_list = NULL;

		liked_pages = new Page * [10];
		for (int i = 0; i < 10; i++)
		{
			liked_pages[i] = nullptr;
		}

	}

	~User()
	{
		if (user_id != NULL)
			delete[] user_id;
		if (first_name != NULL)
			delete[] first_name;
		if (last_name != NULL)
			delete[] last_name;
	}

	void SetTotalFriends(int n)
	{
		total_friends = n;
	}

	void SetTotalLikedPages(int n)
	{
		total_likedpages = n;
	}

	void Read_UserFromFile(ifstream& fin)
	{
		char buffer[80];
		fin >> buffer;
		user_id = GetStringFromBuffer(buffer);

		fin >> buffer;
		first_name = GetStringFromBuffer(buffer);

		fin >> buffer;
		last_name = GetStringFromBuffer(buffer);

	}

	void Like_page(Page*& reference, int i)
	{
		liked_pages[i] = reference;
	}

	void Allocate_friendlist()
	{
		friend_list = new User * [total_friends];
		for (int i = 0; i < total_friends; i++)
		{
			friend_list[i] = new User;
		}
	}

	void Add_frnd(User*& reference, int i)
	{
		friend_list[i] = reference;
	}

	char* GetID()
	{
		return user_id;
	}

	void printName()
	{
		cout << first_name << " " << last_name;
	}

	void ViewFriendList()
	{
		cout << "------------------------------------------------------------\n";
		printName();
		cout << "--Friend List\n\n";
		for (int i = 0; i < total_friends; i++) {
			if (friend_list[i])
			{
				cout << friend_list[i]->GetID() << " - ";
				friend_list[i]->printName();
				cout << endl;
			}
		}
		cout << endl;
	}

	void ViewPagesList()
	{
		cout << "------------------------------------------------------------\n";
		cout << first_name << " " << last_name << "--Liked Pages\n";
		for (int i = 0; i < total_likedpages; i++)
		{
			if (liked_pages[i])
			{
				liked_pages[i]->printListView();
			}
		}

	}

};


class Facebook
{
private:
	Page** pages;
	User** user;

public:

	void load_data(ifstream& fin, ifstream& obj)
	{
		load_pages(fin);
		load_users(obj);
	}

	void load_pages(ifstream& file)
	{
		int total_pages = 0;
		file >> total_pages;
		Page::total_pages = total_pages;

		pages = new Page * [total_pages];

		for (int i = 0; i < total_pages; i++)
		{
			pages[i] = new Page;

			pages[i]->Read_PagesFromFile(file);
		}

		file.close();
	}

	Page* search_pagebyID(char* page_ID, int total_pages)
	{
		for (int i = 0; i < total_pages; i++)
		{
			if (strcmp(pages[i]->GetID(), page_ID) == 1)
			{
				return pages[i];
			}
		}
		return nullptr;
	}

	User* search_userByID(const char* friendID, int total_users)
	{
		for (int i = 0; i < total_users; i++)
		{
			if (strcmp(user[i]->GetID(), friendID) == 1)
			{
				return user[i];
			}
		}
		return nullptr;
	}

	User* search_userByID(char* friendID, int total_users)
	{
		for (int i = 0; i < total_users; i++)
		{
			if (strcmp(user[i]->GetID(), friendID) == 1)
			{
				return user[i];
			}
		}
		return nullptr;
	}

	void load_users(ifstream& file)
	{
		int total_users = 0;
		file >> total_users;

		char buffer[80];

		user = new User * [total_users];
		char*** frnds_arr = new char** [total_users];

		for (int i = 0; i < total_users; i++)
		{
			user[i] = new User;

			user[i]->Read_UserFromFile(file);

			//Reading friend list of user[i]
			frnds_arr[i] = new char* [10];

			for (int k = 0; k < 10; k++)
			{
				frnds_arr[i][k] = new char;
				frnds_arr[i][k] = 0;
			}

			int j = 0;
			file >> buffer;

			while (buffer[0] != '-')
			{
				frnds_arr[i][j] = GetStringFromBuffer(buffer);
				j++;
				file >> buffer;
			}

			/*frnds_arr[i][j] = nullptr;*/

			user[i]->SetTotalFriends(j);
			user[i]->Allocate_friendlist();

			file >> buffer;
			int k = 0;
			Page* pntr;

			while (buffer[0] != '-')
			{
				pntr = search_pagebyID(buffer, Page::total_pages);
				user[i]->Like_page(pntr, k);
				k++;
				file >> buffer;
			}
			user[i]->SetTotalLikedPages(k);

		}

		User* pntr_u;
		for (int i = 0; i < total_users; i++)
		{
			for (int j = 0; frnds_arr[i][j] != nullptr; j++)
			{
				pntr_u = search_userByID(frnds_arr[i][j], total_users);
				user[i]->Add_frnd(pntr_u, j);
			}
		}

	}

	void Run()
	{
		User* curr_user = search_userByID("u10", 20);
		curr_user->ViewFriendList();
		curr_user->ViewPagesList();
	}

};

int main()
{
	Facebook fb;
	ifstream obj("Pages.txt");
	ifstream obj2("User.txt");
	fb.load_data(obj, obj2);
	fb.Run();
}