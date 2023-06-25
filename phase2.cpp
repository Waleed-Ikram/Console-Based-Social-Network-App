#include <iostream>
#include <fstream>
using namespace std;

template<class myClass>
void Dealloc(myClass** dealloc)                        // Template for deallocation of double pointers of any class
{
	for (int i = 0; dealloc[i] != nullptr; i++)
		   delete dealloc[i];

	delete[] dealloc;
}

class Object;
class Page;
class User;
class Comment;
class Post;
class Activity;
class Facebook;

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
	char* buffer = new char[len + 1];
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


// All the above functions are used for string manipulation

class Object    // Abstract class
{
private:
	char* id;
public:

	virtual void AddPostToTimeline(Post* ptr) {};   // Abstract functions
	virtual void printName() {};
	virtual void printListView() {};
	virtual ~Object() {};
};

class Date
{
private:
	int day;
	int month;
	int year;
public:

	static Date currdate;

	void ReadDateFromFile(ifstream& obj)
	{
		obj >> day;
		obj >> month;
		obj >> year;
	}

	static void SetStaticDate(int d, int m, int y)
	{
		currdate.day = d;
		currdate.month = m;
		currdate.year = y;

	}

	void PrintDate()
	{
		cout << "(" << day << "/" << month << "/" << year << ")" << endl;
	}

	int GetDay()
	{
		return day;
	}

	int GetMonth()
	{
		return month;
	}

	int GetYear()
	{
		return year;
	}

	static bool compareDate(Date date1, Date date2)          // This function sees if two dates are either the same or have just a difference of one day
	{
		int day1 = date1.GetDay();
		int day2 = date2.GetDay();

		if (day1 == day2 && date1.GetMonth() == date2.GetMonth() && date1.GetYear() == date2.GetYear())  // checks for same day
			return true;
		else if (day1 - day2 == 1 && date1.GetMonth() == date2.GetMonth() && date1.GetYear() == date2.GetYear())   // checks for previous day
			return true;
		else
			return false;
	}

	static bool CheckYear(Date date1, Date date2)    // This function checks wether the two dates are of the same day 
	{
		if (date1.GetDay() == date2.GetDay() && date1.GetMonth() == date2.GetMonth())
		{
			return true;
		}
		else
			return false;
	}

	static int yeardifference(Date date1, Date date2)     // Gives difference between two dates.
	{
		int diff;

		diff = date1.GetYear() - date2.GetYear();
		return diff;
	}

	~Date() {};    // Destructor does nothing
};

class Activity
{
private:
	int type = 0;
	char* value;

public:
	void ReadActivityFromFile(ifstream& fin)
	{
		fin >> type;
		char buffer[80];
		fin.getline(buffer, 80);
		value = GetStringFromBuffer(buffer);
	}

	void PrintActivity()
	{
		if (type == 1)                          // sees the type of activity and prints accordingly
			cout << " is Feeling ";
		else if (type == 2)
			cout << " is Thinking About ";
		else if (type == 3)
			cout << " is Making ";
		else if (type == 4)
			cout << " is Celebrating ";

		cout << value;

	}

	~Activity()
	{
		if (value)
			delete[] value;
	}
};

class Comment
{
private:
	char* comment_id;
	char* text;
	Post* post_id;
	Object* comment_by;


public:

	static int num_ofcomments;

	Comment()  // Default constructor
	{
		comment_id = nullptr;
		text = nullptr;
		post_id = nullptr;
		comment_by = nullptr;
	}  

	Comment(Object* obj, char* txt)    // Overloaded Constructor. Used when adding comment from a user
	{
		num_ofcomments++;
		comment_by = obj;
		text = txt;
		post_id = nullptr;
	}

	~Comment()
	{
		if (comment_id)
			delete[] comment_id;

		if (text)
			delete[] text;
	}


	void ReadCommentsFromFile(ifstream& fin)
	{
		char buffer[80];
		fin >> buffer;
		comment_id = GetStringFromBuffer(buffer);
	}

	void SetPost(Post* id)  // Association
	{
		post_id = id;
	}

	void Setcommentby(Object* obj)     // Association
	{
		comment_by = obj;
	}

	void Readtext(ifstream& fin)
	{
		char buffer[250];
		fin.getline(buffer, 250);
		text = GetStringFromBuffer(buffer);
	}

	void PrintComments()
	{
		comment_by->printName();
		cout << " wrote : ";
		cout << text << endl;
	}
};

int Comment::num_ofcomments = 0;

Date Date::currdate;

class Post : public Object   // Inherits the class Object
{
	char* post_id;
	char* text;
	int num_likes = 0;
	Date shared_date;
	Comment** comment;
	Object* shared_by;
	Object** liked_by;
	Activity* activity;
	int no_ofcomments = 0;

public:

	static int Total_posts;  // This keeps a count of all the posts available
	 
	// Post Constructor
	Post()
	{
		post_id = nullptr;
		text = nullptr;

		liked_by = new Object * [10];
		for (int i = 0; i < 10; i++)
		{
			liked_by[i] = nullptr;
		}

		comment = new Comment * [10];
		for (int i = 0; i < 10; i++)
		{
			comment[i] = nullptr;
		}

		shared_by = nullptr;
		activity = nullptr;

	}

	//Post Destructor
	virtual ~Post()
	{
		if (post_id)
			delete[] post_id;

		if (text)
		{
			delete[] text;
		}
		 
		Dealloc(comment);  // Template used over here
		
		delete[] liked_by;

		if (activity)
			delete activity;
	}

	void SetNoofLikes(int n)
	{
		num_likes = n;
	}

	void ReadPostFromFile(ifstream& fin)
	{
		int temp;
		fin >> temp;

		char buffer[80];
		fin >> buffer;
		post_id = GetStringFromBuffer(buffer);

		shared_date.ReadDateFromFile(fin);
		fin.ignore();
		fin.getline(buffer, 80, '\n');
		text = GetStringFromBuffer(buffer);

		if (temp == 2)
		{
			activity = new Activity;
			activity->ReadActivityFromFile(fin);
		}
	}

	void Setsharedby(Object* ptr)
	{
		shared_by = ptr;
	}

	Object* GetSharedByID()
	{
		return shared_by;
	}

	char* GetText()
	{
		return text;
	}

	void SetLikedBy(Object* ptr)
	{
		liked_by[num_likes] = ptr;
		num_likes++;
	}

	void SetText(char* str)
	{
		text = str;
	}

	char* GetId()
	{
		return post_id;
	}

	Date GetSharedDate()
	{
		return shared_date;
	}

	void Setcomment(Comment* com)
	{
		comment[no_ofcomments] = com;
		no_ofcomments++;
	}

	void SetSharedDate(Date date)
	{
		shared_date = date;
	}

	virtual void PrintPost()     // This function is also used in Memory class to print memory, thats why its virtual.
	{
		shared_by->printName();
		
		if (activity)
		{
			activity->PrintActivity();
		}

		cout << "\n" << text << "...";
		shared_date.PrintDate();
		
		for (int i = 0; i < no_ofcomments; i++)
		{
			if (comment[i])
			{
				comment[i]->PrintComments();
			}
		}
	}

	void ViewLikedList()
	{
		cout << "Post is Liked By : \n" << endl;

		for (int i = 0; i < num_likes; i++)
		{
			liked_by[i]->printListView();  // printListView is a virtual function of Object Class 
			cout << endl;
		}
	}
};

int Post::Total_posts = 0;  // This keeps a total count of all the posts

class Memory : public Post   // A memory is a type of Post
{
	Post* OrigPost;

public:

	Memory()   // Default Constructor
	{
		OrigPost = nullptr;
	}
	
	Memory(Post* ptr, char* txt, Object* obj)   // Overloaded Constructor
	{
		OrigPost = ptr;
		Setsharedby(obj);
		SetText(txt);
		SetSharedDate(Date::currdate);
	}

	~Memory() {};

	void PrintPost()  // Polymorphism used here. The same function exixts in Post class
	{
		GetSharedByID()->printName();
		cout << " shared a memory ---";
		cout << endl << GetText() << endl;
		OrigPost->PrintPost();
	}

};

class Page : public Object
{
private:
	char* page_id;
	char* title;
	Post** Timeline;
	int num_ofposts = 0;

public:
	static int total_pages;

	Page()
	{
		page_id = NULL;
		title = NULL;

		Timeline = new Post * [10];
		for (int i = 0; i < 10; i++)
		{
			Timeline[i] = nullptr;
		}
	}

	void AddPostToTimeline(Post* ptr)
	{
		Timeline[num_ofposts] = ptr;
		num_ofposts++;
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

		Dealloc(Timeline);

	}

	void Read_PagesFromFile(ifstream& fin)
	{
		char buffer[80];
		fin >> buffer;
		page_id = GetStringFromBuffer(buffer);

		fin.getline(buffer, 80);
		title = GetStringFromBuffer(buffer);

	}

	int Getnumofposts()
	{
		return num_ofposts;
	}

	char* GetID()
	{
		return page_id;
	}

	void printListView()  // Polymorphism used here as well. This is a function of Object class
	{
		cout << GetID() << "---" << title << endl;
	}

	void DisplayForHomePage()
	{
		Page* likedpage = this;
		bool to_print = false;

		for (int i = 0; i < num_ofposts; i++)
		{
			if (likedpage->Timeline[i])
			{
				to_print = Date::compareDate(Date::currdate, Timeline[i]->GetSharedDate());  // Checks weather the day is same or previous day.
			}

			if (to_print == true)  // If condition met then it prints.
			{
				likedpage->Timeline[i]->PrintPost();
				cout << endl;
			}

			to_print = false;
		}
	}

	void ViewPageTimeline()
	{
		for (int i = 0; i < num_ofposts; i++)
		{
			if (Timeline[i])
			{
				Timeline[i]->PrintPost();
				cout << endl;
			}
		}
	}

	void printName() // Polymorphism used here. Function is originally of Object Class.
	{
		cout << title << " " << "shared :";
	}
};

int Page::total_pages = 0;  // Keeps a count of all pages

class User : public Object
{
private:

	char* user_id;
	char* first_name;
	char* last_name;
	User** friend_list;
	Page** liked_pages;
	int total_friends = 0;
	int total_likedpages = 0;
	Post** Timeline;
	int num_ofposts = 0;

public:

	static int total_users;

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

		Timeline = new Post * [10];
		for (int i = 0; i < 10; i++)
		{
			Timeline[i] = nullptr;
		}

	}

	void AddPostToTimeline(Post* ptr)
	{
		Timeline[num_ofposts] = ptr;
		num_ofposts++;
	}

	~User()
	{
		if (user_id)
			delete[] user_id;
		if (first_name)
			delete[] first_name;
		if (last_name)
			delete[] last_name;
		delete[] friend_list;
		delete[] liked_pages;
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

	void Like_page(Page*& reference, int i)  // Association
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

	void printName() // Polymorphism used 
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
				friend_list[i]->printListView();
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

	void DisplayForHomePage()
	{
		User* friends = this;
		bool to_print = false;
		for (int j = 0; j < friends->num_ofposts; j++)
		{
			if (friends->Timeline[j])
			{
				to_print = Date::compareDate(Date::currdate, Timeline[j]->GetSharedDate());
			}

			if (to_print == true)
			{
				friends->Timeline[j]->PrintPost();
				cout << endl;
			}

			to_print = false;
		}
	}

	void DisplayHomePage()
	{
		for (int i = 0; i < total_friends; i++)
		{
			if (friend_list[i])
			{
				friend_list[i]->DisplayForHomePage();
				cout << endl;
			}
		}

		cout << "\n";

		for (int i = 0; i < total_likedpages; i++)
		{
			if (liked_pages[i])
			{
				liked_pages[i]->DisplayForHomePage();
				cout << endl;
			}
		}
	}


	void ViewTimeline()
	{
		for (int i = 0; i < num_ofposts; i++)
		{
			Timeline[i]->PrintPost();
			cout << endl;
		}
	}

	void printListView()  // polymorphism used.
	{
		cout << this->user_id << "---";
		this->printName();
	}

	void SeeMemory()
	{
		bool is_able = false;

		for (int i = 0; i < num_ofposts; i++)
		{

			if (Timeline[i])
			{
				is_able = Date::CheckYear(Date::currdate, Timeline[i]->GetSharedDate());
			}

			if (is_able == true)
			{
				cout << "\nOn this day\n";
				int difference;
				difference = Date::yeardifference(Date::currdate, Timeline[i]->GetSharedDate());
				cout << difference << " years ago.\n\n";
				Timeline[i]->PrintPost();
			}
		}
	}

	void AddMemoryToTimeline(Memory* memory)
	{
		num_ofposts++;

		Post** temp;

		temp = Timeline;

		Timeline = new Post * [num_ofposts];
		Timeline[0] = memory;

		if (temp)
		{
			for (int i = 0; i < num_ofposts - 1; i++)
			{
				Timeline[i + 1] = temp[i];
			}

			delete[] temp;
		}
	}
};

int User::total_users = 0;

class Facebook
{
private:
	Page** pages;
	User** user;
	Post** posts;
	Comment** comments;

public:

	void load_data(ifstream& fin, ifstream& obj, ifstream& file, ifstream& obj2)
	{
		load_pages(fin);
		load_users(obj);
		load_posts(file);
		load_comments(obj2);
	}

	void load_pages(ifstream& file)
	{
		int total_pages = 0;
		file >> total_pages;
		Page::total_pages = total_pages;

		pages = new Page * [total_pages+1];
		pages[total_pages] = nullptr;

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

	Post* search_PostById(char* id)
	{
		for (int i = 0; i < Post::Total_posts; i++)
		{
			if (strcmp(posts[i]->GetId(), id) == 1)
			{
				return posts[i];
			}
		}
		return nullptr;
	}

	void load_users(ifstream& file)
	{
		int total_users = 0;
		file >> total_users;
		User::total_users = total_users;

		char buffer[80];

		user = new User * [total_users+1];
		user[total_users] = nullptr;
		char*** frnds_arr = new char** [total_users];   // this is used to later manipulate and store friends array

		for (int i = 0; i < total_users; i++)
		{
			user[i] = new User;

			user[i]->Read_UserFromFile(file);
			// This only stores the name and ID

			frnds_arr[i] = new char* [10];

			for (int k = 0; k < 10; k++)  // allocation for frnds array
			{
				frnds_arr[i][k] = nullptr;
			}

			int j = 0;
			file >> buffer;

			while (buffer[0] != '-')  // loop only runs till first -1
			{
				frnds_arr[i][j] = GetStringFromBuffer(buffer);  // storing friends in the array
				j++;
				file >> buffer;
			}

			user[i]->SetTotalFriends(j); // J tells us the total friends. Because it increments with the loop.
			user[i]->Allocate_friendlist();


			// Pages stored from below this 
			file >> buffer;
			int k = 0;
			Page* pntr;
			while (buffer[0] != '-')
			{
				pntr = search_pagebyID(buffer, Page::total_pages);
				user[i]->Like_page(pntr, k);  // This is the association of pages
				k++;
				file >> buffer;
			}
			user[i]->SetTotalLikedPages(k); // K tells the total number of pages liked.

		}

		//Code for friends association
		User* pntr_u;
		for (int i = 0; i < total_users; i++)
		{
			for (int j = 0; frnds_arr[i][j] != nullptr; j++)
			{
				pntr_u = search_userByID(frnds_arr[i][j], total_users);
				user[i]->Add_frnd(pntr_u, j);
			}
		}

		//Deallocation of triple pointer

		for (int i = 0; i < total_users; i++)
		{
			for (int j = 0; frnds_arr[i][j] != nullptr; j++)
			{
				if (frnds_arr[i][j])
				{
					delete[] frnds_arr[i][j];
				}

			}
			if (frnds_arr[i])
			{
				delete[] frnds_arr[i];
			}
			
		}
		delete[] frnds_arr;
	}

	void Run()
	{
		int day = 15;
		int month = 11;
		int year = 2017;


		cout << "Command\t\tSet current system Date as 15/11/2017.\n";
		Date::SetStaticDate(day, month, year);
		cout << "System Date : ";
		Date::currdate.PrintDate();


		cout << "\n\nCommand : \t\tSet current user u7" << endl;
		User* curr_user = search_userByID("u7", User::total_users);
		cout << "\n";
		curr_user->printName();
		cout << " successfully set as user.\n";


		cout << "\n" << "Command : \t\tView Friend List.\n";
		curr_user->ViewFriendList();


		cout << "\nCommand : \t\tView Liked Pages.\n";
		curr_user->ViewPagesList();


		cout << "\n\nCommand:\t\tView Homepage";
		cout << "\n------------------------------------------------------------\n";
		curr_user->DisplayHomePage();


		cout << "\n------------------------------------------------------------";
		cout << "\nCommand :\t\tView Timeline.\n";
		cout << "------------------------------------------------------------\n\n";
		curr_user->ViewTimeline();


		char buffer[] = { "post5" };
		Post* postptr;
		postptr = search_PostById(buffer);
		cout << "\n------------------------------------------------------------\n";
		cout << "Command :\t\tView Liked List (post5)" << endl;
		cout << "------------------------------------------------------------\n\n";
		postptr->ViewLikedList();


		cout << "\n------------------------------------------------------------\n";
		cout << "Command :\t\tLike Post (post5)";
		postptr->SetLikedBy(curr_user);
		cout << "\nCommand :\t\tView Liked List (post5)" << endl << endl;
		postptr->ViewLikedList();


		cout << "\n------------------------------------------------------------\n";
		char buffer2[] = { " Good Luck for your Result" };
		Comment* com;
		com = new Comment(curr_user, buffer2);
		char buffer3[] = { "post4" };
		postptr = search_PostById(buffer3);
		postptr->Setcomment(com);
		cout << "Command :\t\t PostComment(post4, Good Luck for your Result)\n";
		cout << "Command :\t\tView Post (Post4)\n";
		postptr->PrintPost();

		
		cout << "\n------------------------------------------------------------\n";
		cout << "Command :\t\t See Your Memories\n\n";
		cout << "We hope you enjoy looking back and sharing your memories on Facebook, from the most recent to those long ago.\n";
		curr_user->SeeMemory();


		cout << "\n------------------------------------------------------------\n";
		cout << "\nCommand :\t\tShareMemory(post10, Never thought I will be specialist in this field)\n";
		cout << "Command :\t\tView Timeline.\n\n";
		char buffer5[] = { "post10" };
		char buffer6[] = { "Never thought I will be specialist in this field" };
		postptr = search_PostById(buffer5);
		Memory* mem = new Memory(postptr, buffer6, curr_user);
		curr_user->AddMemoryToTimeline(mem);
		curr_user->ViewTimeline();

		
		cout << "\n------------------------------------------------------------\n";
		cout << "Command :\t\tView Page (P1)\n\n";
		char buffer4[] = { "p1" };
		Page* pageptr;
		pageptr = search_pagebyID(buffer4, Page::total_pages);
		pageptr->ViewPageTimeline();

	}

	void load_posts(ifstream& file)
	{
		int total_posts = 0;
		file >> total_posts;
		Post::Total_posts = total_posts;

		posts = new Post * [total_posts];

		for (int i = 0; i < total_posts; i++)
		{
			posts[i] = new Post;
			posts[i]->ReadPostFromFile(file);


			// Code for shared by
			char buffer[80];
			file >> buffer;
			Object* ptr = nullptr;

			if (buffer[0] == 'u')
			{
				ptr = search_userByID(buffer, User::total_users);    // returns a user 
			}
			else
			{
				ptr = search_pagebyID(buffer, Page::total_pages);    // returns a page
			}

			posts[i]->Setsharedby(ptr);
			ptr->AddPostToTimeline(posts[i]);    // Polymorphism


			// Code for liked by
			file >> buffer;
			int count = 0;
			while (buffer[0] != '-')
			{
				if (buffer[0] == 'u')
				{
					ptr = search_userByID(buffer, User::total_users);
				}
				else if (buffer[0] == 'p')
				{
					ptr = search_pagebyID(buffer, Page::total_pages);
				}
				posts[i]->SetLikedBy(ptr);
				count++;
				posts[i]->SetNoofLikes(count);
				file >> buffer;
			}

		}

	}

	void load_comments(ifstream& file)
	{
		int no_ofcomments = 0;

		file >> no_ofcomments;
		Comment::num_ofcomments = no_ofcomments;
		comments = new Comment * [no_ofcomments];

		for (int i = 0; i < no_ofcomments; i++)
		{
			comments[i] = new Comment;

			comments[i]->ReadCommentsFromFile(file);

			char buffer[80];
			file >> buffer;

			Post* post;
			post = search_PostById(buffer);

			post->Setcomment(comments[i]);


			file >> buffer;
			Object* ptr = NULL;

			if (buffer[0] == 'u')
			{
				ptr = search_userByID(buffer, User::total_users);
			}
			else if (buffer[0] == 'p')
			{
				ptr = search_pagebyID(buffer, Page::total_pages);
			}

			comments[i]->Setcommentby(ptr);

			comments[i]->Readtext(file);
		}
	}

	~Facebook()
	{

		Dealloc(user);
		Dealloc(pages);
		delete[] comments;
		delete[] posts;
	}
};


int main()
{
	Facebook fb;
	ifstream obj("Pages.txt");
	ifstream obj1("User.txt");
	ifstream obj2("Posts.txt");
	ifstream obj3("Comments.txt");
	fb.load_data(obj, obj1, obj2, obj3);
	fb.Run();
}