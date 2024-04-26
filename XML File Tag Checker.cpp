#include <iostream>
#include <fstream>
#include <string>
using namespace std;
class XMLData;
template <class T>
class stack {
	class node {
	public:
		T data;
		node* next;
		node() {
			next = NULL;
		}
	};
public:
	node* head;
	int stacksize;
	stack() {
		head = NULL;
		stacksize = 0;
	}
	int size() {
		return stacksize;
	}
	bool isEmpty() {
		return (size() == 0);
	}
	T const& top() {
		if (head != NULL) {
			return head->data;
		}
	}
	void pop() {
		if (head != NULL) {
			node* current = head;
			head = head->next;
			delete current;
		}
		stacksize--;
	}
	void push(T const& e) {
		node* temp = new node;
		temp->data = e;
		temp->next = head;
		head = temp;
		stacksize++;
	}
	void print() {
		if (!isEmpty()) {
			stack a;
			while (!isEmpty()) {
				cout << top() << " ";
				a.push(top());
				pop();
			}
			cout << endl;
			while (a.isEmpty() != true) {
				push(a.top());
				a.pop();
			}
		}
		else
			cout << "Stack is empty! :(" << endl;
	}
	bool search(T data) {
		if (!isEmpty()) {
			stack a;
			bool check = false;
			while (!isEmpty()) {
				if (data == top()) {
					pop();
					check = true;
				}
				else {
					a.push(top());
					pop();
				}
			}
			while (a.isEmpty() != true) {
				push(a.top());
				a.pop();
			}
			return check;
		}
		else
			return false;
	}
	~stack() {
		node* temp = head;
		while (head != NULL) {
			temp = head;
			head = head->next;
			delete temp;
		}
	}
};
int lengthh(string x) {
	int i = 0;
	while (x[i] != '\0') {
		i++;
	}
	return i;
}
class XMLData : stack<XMLData> {
	string data;
	int lineno;
public:
	XMLData() {
		data = '\0';
		lineno = 0;
	}
	XMLData(string temp, int y) {
		data = temp;
		lineno = y;
	}
	string retdata() {
		return data;
	}
	int retlineno() {
		return lineno;
	}
	bool operator == (XMLData x) {
		if (data == x.data)
			return true;
		else
			return false;
	}
	bool checkattributes() {
		int eqcount = 0, dqcount = 0, sqcount = 0;
		for (int i = 0; i < lengthh(data); i++) {
			if (data[i] == '=')
				eqcount++;
			if (data[i] == '"')
				dqcount++;
			if (data[i] == 39)
				sqcount++;
		}
		int check1 = 0, check2 = 0;
		bool flag = true;
		for (int i = 0; i < lengthh(data); i++) {
			if (data[i] == '"') {
				check1++;
				if (check1 == 2) {
					check1 = 0;
				}
			}
			if (data[i] == 39) {
				check2++;
				if (check2 == 2) {
					check2 = 0;
				}
			}
			if (check1 == 1 && check2 == 1) {
				flag = false;
			}
		}
		if (!flag)
			return false;
		if (dqcount % 2 != 0 || sqcount % 2 != 0)
			return false;
		else {
			eqcount *= 2;
			if (dqcount + sqcount == eqcount)
				return true;
			else
				return false;
		}
	}
};

int main() {
	ifstream file1("input.xml");
	if (!file1) {
		cout << "File not found." << endl;
		return 0;
	}
	bool finalattcheck = true;
	stack<XMLData> s1;
	int linenum = 1;
	string line;
	string hello1 = ">", hello2 = "/";
	while (!file1.eof()) {
		getline(file1, line);
		if (line[0] == '<' && line[1] == '?') {
			stack<char> s;
			s.push('>');
			s.push(line[1]);
			int size = lengthh(line);
			for (int i = 2; i < size; i++) {
				if (line[i] == s.top()) {
					s.pop();
				}
			}
			if (s.isEmpty() != true) {
				cout << "Error at line " << linenum << ": HEADER: " << line << endl;
			}
			XMLData atch(line, linenum);
			if (!atch.checkattributes()) {
				cout << "Error at line " << linenum << ": INVALID ATTRIBUTES: " << line << endl;
				finalattcheck = false;
			}
			linenum++;
		}
		else if (line[0] == '<' && line[1] == '!' && line[2] == '-' && line[3] == '-') {
			stack<char> s;
			s.push('>');
			s.push(line[2]);
			s.push(line[3]);
			int size = lengthh(line);
			for (int i = 4; i < size; i++) {
				if (line[i] == s.top()) {
					s.pop();
				}
			}
			if (s.isEmpty() != true) {
				cout << "Error at line " << linenum << ": COMMENT: " << line << endl;
			}
			XMLData atch(line, linenum);
			if (!atch.checkattributes()) {
				cout << "Error at line " << linenum << ": INVALID ATTRIBUTES: " << line << endl;
				finalattcheck = false;
			}
			linenum++;
		}
		else if (line[0] == '<' && line[1] != '/') {
			int size = lengthh(line);
			int indexforeq = 0, indexforoc = 0;
			bool flagoc = false, flageq = false;
			for (int i = 1; i < size; i++) {
				if (line[i] == '<') {
					flagoc = true;
					indexforoc = i;
				}
				if (line[i] == '=') {
					flageq = true;
					indexforeq = i;
				}
			}
			if (flagoc == false && flageq == false) {
				string temp = line;
				temp = temp.insert(1, hello2);
				int i = 0;
				for (i = 0; i < size; i++) {
					if (temp[i] == '>')
						break;
				}
				temp.erase(++i);
				XMLData lol(temp, linenum);
				s1.push(lol);
				linenum++;
				continue;
			}
			else if (flagoc == false && flageq == true) {
				string temp = line;
				int i = 0;
				for (i = 0; i < indexforeq; i++) {
					if (temp[i] == ' ')
						break;
				}
				temp.erase(i);
				temp += hello1;
				temp = temp.insert(1, hello2);
				XMLData lol(temp, linenum);
				XMLData lol2(line, linenum);
				if (!lol2.checkattributes()) {
					cout << "Error at line " << linenum << ": INVALID ATTRIBUTES: " << line << endl;
					finalattcheck = false;
				}
				s1.push(lol);
				linenum++;
				continue;
			}
			else if (flagoc == true && flageq == false) {
				string temp = line;
				int i = 0;
				for (i = 0; i < indexforoc; i++) {
					if (temp[i] == '>')
						break;
				}
				temp.erase(++i);
				temp = temp.insert(1, hello2);
				XMLData lol(temp, linenum);
				s1.push(lol);
				string temp2 = line;
				temp2.erase(temp2.begin() + 0, temp2.begin() + indexforoc);
				XMLData wow = s1.top();
				if (temp2 == wow.retdata() && linenum == wow.retlineno()) {
					s1.pop();
				}
				else {
					cout << "Error at line " << linenum << ": TAG: " << line << endl;
				}
				linenum++;
				continue;
			}
			else if (flagoc == true && flageq == true) {
				string temp = line;
				int i = 0;
				for (i = 0; i < indexforeq; i++) {
					if (temp[i] == ' ')
						break;
				}
				temp.erase(i);
				temp += hello1;
				temp = temp.insert(1, hello2);
				XMLData lol(temp, linenum);
				XMLData lol2(line, linenum);
				if (!lol2.checkattributes()) {
					cout << "Error at line " << linenum << ": INVALID ATTRIBUTES: " << line << endl;
					finalattcheck = false;
				}
				s1.push(lol);
				string temp2 = line;
				temp2.erase(temp2.begin() + 0, temp2.begin() + indexforoc);
				XMLData wow = s1.top();
				if (temp2 == wow.retdata() && linenum == wow.retlineno()) {
					s1.pop();
				}
				else {
					cout << "Error at line " << linenum << ": MISMATCHED TAGS: " << line << endl;
				}
				linenum++;
				continue;
			}
		}
		else if (line[0] == '<' && line[1] == '/') {
			XMLData wow = s1.top();
			if (line == wow.retdata()) {
				s1.pop();
			}
			else {
				XMLData temp(line, linenum);
				bool check = s1.search(temp);
				if (check == false) {
					cout << "Error at line " << linenum << ": MISSING TAG: " << line << endl;
				}
			}
			linenum++;
			continue;
		}
		else if (line[0] != '<') {
			string temp = line;
			int size = lengthh(temp);
			int i = 0;
			bool check = false;
			for (i = 0; i < size; i++) {
				if (temp[i] == '<') {
					check = true;
					break;
				}
			}
			XMLData hoh(line, linenum);
			if (!hoh.checkattributes()) {
				cout << "Error at line " << linenum << ": INVALID ATTRIBUTES: " << line << endl;
				finalattcheck = false;
			}
			if (check) {
				temp.erase(0, i);
				if (temp[1] == '/') {
					XMLData hehe = s1.top();
					if (temp == hehe.retdata()) {
						s1.pop();
					}
					else {
						XMLData wow1(line, linenum);
						bool check = s1.search(wow1);
						if (check == false) {
							cout << "Error at line " << linenum << ": MISSING TAG: " << line << endl;
						}
					}
					linenum++;
					continue;
				}
				else {
					XMLData heh(temp, linenum);
					s1.push(heh);
				}
			}
			else {
				cout << "INVALID LINE " << linenum << ": " << line << endl;
				linenum++;
				continue;
			}
		}
	}
	if (s1.isEmpty() == false) {
		cout << "Missing tags: " << endl;
		while (!s1.isEmpty()) {
			string l;
			int ln;
			XMLData wow = s1.top();
			l = wow.retdata();
			ln = wow.retlineno();
			cout << "Error at line " << ln << ": MISSING TAGS: " << l << endl;
			s1.pop();
		}
	}
	else if (finalattcheck) {
		cout << "XML File is OK! :)" << endl;
	}
	return 0;
}