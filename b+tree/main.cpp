#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <sstream>
#include <vector>
#define safe_delete(ptr) if(ptr != nullptr){delete[](ptr),ptr=nullptr;};
#define N ((blocksize-4)/8) //Number of entries in one node
#define NH ((header.BlockSize - 4)/8) //Number of entries in on node using header.BlockSize
#define Nright (int)(floor((double)(NH+1)/2)) //Number of right leaf node
#define Nleft (int)(ceil((double)(NH+1)/2)) //Number of left leaf node
#define MOD ios::binary | ios::in | ios::out
#define UpdateHeader file.seekp(0, ios::beg), file.write((char*)&header, sizeof(Header))
#define open_error_handling(file) if(!file){cout<<"파일 오픈 실패";exit(1);}
using namespace std;
class Index_entry {
public:
	int key = 0;
	int NextLevelBID = 0;
	Index_entry() {};
};
class Data_entry {
public:
	int key = 0;
	int Value = 0;
	Data_entry() {};
};


class NLeafNode {
public:
	NLeafNode(int blocksize);
	int insert(int key, int NextLevelBID);
	void load(fstream &file, int BID);
	void save(fstream &file, int to_BID);
	int NextLevelBID;
	Index_entry* entries;
	int blocksize;
};

class LeafNode {
public:
	LeafNode(int blocksize);
	int insert(int key, int value);
	void load(fstream &file, int BID);
	void save(fstream &file, int to_BID);
	Data_entry* Data;
	int NextBID;
	int blocksize;
};
class Header {
public:
int BlockSize;
int RootBID;
int Depth;
Header(int BlockSize, int RootBID, int Depth) :BlockSize(BlockSize), RootBID(RootBID), Depth(Depth) {};
Header() {};
};

class Btree {
public:
	Btree(string fileName, int blocksize);
	bool empty();
	bool insert(int key, int value);
	void print(fstream& output);
	int search(int key); // point search
	void search(int startRange, int endRange, fstream&); // range search
	int get_lastBID(fstream &file);

	string filename;
	Header header;


};

int main(int argc, char* argv[]) {
	char command = argv[1][0];
	switch (command) {
	case 'c': { //헤더파일만 생성하여 저장한다.
		ofstream out;
		Header header(atoi(argv[3]), 0, 0);
		out.open(argv[2], ios::binary);
		open_error_handling(out);
		out.write((char*)&header, sizeof(header));
		out.close();
		break;
	}
	case 'i'://삽입
	{
		//blocksize 불러오기
		int blocksize;
		fstream file(argv[2], MOD);
		open_error_handling(file);
		file.read((char*)&blocksize, sizeof(int));
		file.close();		
		Btree btree(argv[2], blocksize);//불러온 blocksize로 부터 btree 생성
		ifstream insert_file(argv[3]);//삽입할 파일 열기
		open_error_handling(insert_file);		
		string temp;
		while (getline(insert_file,temp)) {
			if (temp == "") continue;
			istringstream line(temp);
			string key_string, ID_string;
			getline(line, key_string,','); getline(line, ID_string,',');
			int key = stoi(key_string); //불러온 값을 저장
			int ID = stoi(ID_string); //불러온 값을 저장		
			btree.insert(key, ID);// btree에 삽입 시행
		}
		insert_file.close();//삽입할 파일 닫기
	}

	break;
	case 's':
	{
		//blocksize 불러오기
		int blocksize;
		fstream file(argv[2], MOD);
		open_error_handling(file);
		file.read((char*)&blocksize, sizeof(int));
		file.close();
		Btree btree(argv[2], blocksize);//불러온 blocksize로 부터 btree 생성
		ifstream search_file(argv[3],ios::in);//탐색할 파일 열기
		open_error_handling(search_file);
		ofstream result_file(argv[4],ios::out);
		open_error_handling(result_file);
		string temp;		
		while (getline(search_file, temp)) {
			if (temp == "") continue;
			int key = stoi(temp); //불러온 값을 저장
			result_file << temp << ","<< to_string(btree.search(key)) << "\n";
		}
		search_file.close();//삽입할 파일 닫기
		result_file.close();
	}
		break;
	case 'r':
	{
		int blocksize;
		fstream file(argv[2], MOD);
		open_error_handling(file);
		file.read((char*)&blocksize, sizeof(int));
		file.close();
		Btree btree(argv[2], blocksize);
		ifstream search_file(argv[3], ios::in);//탐색할 파일 열기
		open_error_handling(search_file);
		fstream result_file(argv[4],ios::out);
		open_error_handling(result_file);

		string temp;
		while (getline(search_file, temp)) {
			if (temp == "") continue;
			istringstream line(temp);
			string start_string, end_string;
			getline(line, start_string, ','); getline(line, end_string, ',');
			int start_range = stoi(start_string); //불러온 값을 저장
			int end_range = stoi(end_string); //불러온 값을 저장	
			btree.search(start_range, end_range, result_file);
		}
		search_file.close();
		result_file.close();
	}
		break;
	case 'p':
	{
		int blocksize;
		fstream file(argv[2], MOD);
		open_error_handling(file);
		file.read((char*)&blocksize, sizeof(int));
		file.close();
		Btree btree(argv[2], blocksize);
		fstream result_file(argv[3], ios::out);
		open_error_handling(result_file);
		btree.print(result_file);
		result_file.close();
	}
		break;
	}
}

Btree::Btree(string fileName, int blocksize) :filename(fileName)
{
	ifstream file(fileName, ios::binary);//원본 파일 열기
	open_error_handling(file);
	file.read((char*)&header, sizeof(Header));//원본파일로 부터 헤더 불러오기
	file.close();
}

bool Btree::empty()
{return header.RootBID == 0;}

bool Btree::insert(int key, int value)
{
	fstream file(filename, MOD); //삽입할 파일 열기
	open_error_handling(file);	
	if (empty()) {//btree가 empty라면 아래와 같이 바로 삽입한다.
		//삽입할 노드를 생성
		LeafNode root(header.BlockSize);
		root.insert(key, value);
		//삽입(첫 삽입 이므로 split될 경우는 존재하지 않으므로 처리하지 않는다.)
		header.RootBID = 1;
		UpdateHeader;
		root.save(file, header.RootBID);
		return true;//삽입 완료후 true 반환
	}
	else {//btree가 empty가 아니라면 루트 노드를 불러온다.
		LeafNode node(header.BlockSize);
		node.load(file, header.RootBID);
		if (header.Depth == 0) {//루트노드가 리프노드일 경우 루트노드에 바로 삽입을 진행한다.
			int index = node.insert(key, value);
			if (index != -1) {//루트 노드가 full이라서 삽입에 실패한 경우 split을 진행한다.
				LeafNode right(header.BlockSize);//쪼개져서 새로 생긴 오른쪽 노드를 선언					
				int j = NH;
				int i = Nright - 1;
				while (i >= 0) {
					if (index == j) {
						right.Data[i].key = key;
						right.Data[i--].Value = value;
						index = -1;
					}
					else {
						right.Data[i--] = node.Data[j - 1];
						node.Data[j - 1] = Data_entry();//옮겨놓고 초기화
						--j;
					}
				}
				if (index != -1) {
					for (int i = Nleft - 1; i >= 0; --i) {
						if(i != index) node.Data[i] = node.Data[i - 1];
						else {
							node.Data[i].key = key;
							node.Data[i].Value = value;
							break;
						}
					}
				}
				node.NextBID = 2;
				node.save(file, 1);
				right.save(file, 2);
				NLeafNode parent(header.BlockSize);//루트 노드는 부모가 없으므로 새로 만들어 준다.
				parent.NextLevelBID = 1;
				parent.insert(right.Data[0].key, 2);			
				parent.save(file, 3);
				header.Depth++;
				header.RootBID = 3;
				UpdateHeader;
				return true;//삽입 완료후 true 반환
			}
			else {
				node.save(file,header.RootBID);				
				return true;//루트가 full이 아니어서 삽입이 정상적으로 일어난 경우 true를 반환한다.
			}
		}
		else {//루트노드가 non-leaf node 일 경우 (즉 btree의 depth가 0이 아닌 경우)
			vector<int> path;
			path.push_back(header.RootBID);
			//삽입할 위치를 알아내기 위해 리프노드까지 찾아내려 가보자.
			for (int i = 0; i < header.Depth; ++i) {
				NLeafNode node(header.BlockSize);
				node.load(file, path[path.size()-1]);
				int temp_BID = node.NextLevelBID;
				for (int j = 0; j < NH; ++j) {
					if (node.entries[j].key == 0 || node.entries[j].key > key) {
						break;
					}
					else temp_BID = node.entries[j].NextLevelBID;
				}
				path.push_back(temp_BID);
			}			
			//삽입할 위치를 찾았으니, 불러온다.		
			LeafNode node(header.BlockSize);			
			node.load(file, path[header.Depth]);
			int index = node.insert(key, value);//삽입한다.
			if (index != -1) {//리프 노드가 full이라서 삽입에 실패한 경우 split을 진행한다.
				LeafNode right(header.BlockSize);//쪼개져서 새로 생긴 오른쪽 노드를 선언	
				int j = NH;
				int i = Nright - 1;
				while (i >= 0) {
					if (index == j) {
						right.Data[i].key = key;
						right.Data[i--].Value = value;
						index = -1;
					}
					else {
						right.Data[i--] = node.Data[j - 1];
						node.Data[j - 1] = Data_entry();
						--j;
					}
				}
				if (index != -1) {
					for (int i = Nleft - 1; i >= 0; --i) {
						if (i != index) node.Data[i] = node.Data[i - 1];
						else {
							node.Data[i].key = key;
							node.Data[i].Value = value;
							break;
						}
					}
				}
				right.NextBID = node.NextBID;
				node.NextBID = get_lastBID(file);
				node.save(file, path[header.Depth]);
				right.save(file, node.NextBID);
				//이제 인덱스로 타고 올라가자
				int temp_key = right.Data[0].key;
				int temp_NextLevelBID = node.NextBID;
				for (auto iter = path.rbegin()+1; iter != path.rend(); ++iter) {
					NLeafNode parent(header.BlockSize);
					parent.load(file, *iter);
					int Nindex = parent.insert(temp_key, temp_NextLevelBID);
					//이제 스플릿
					if (Nindex != -1) {
						NLeafNode parent_right(header.BlockSize);//쪼개져서 새로 생긴 오른쪽 노드를 선언			
						int j = ((header.BlockSize - 4) / 8);
						int i = Nright - 1;
						while (i >= 0) {
							if (Nindex == j) {
								parent_right.entries[i].key = temp_key;
								parent_right.entries[i--].NextLevelBID = temp_NextLevelBID;
								Nindex = -1;
							}
							else {
								parent_right.entries[i--] = parent.entries[j - 1];
								parent.entries[j - 1] = Index_entry();
								--j;
							}
						}
						if (Nindex != -1) {
							for (int i = Nleft - 1; i >= 0; --i) {
								if (i != Nindex) parent.entries[i] = parent.entries[i - 1];
								else {
									parent.entries[i].key = temp_key;
									parent.entries[i].NextLevelBID = temp_NextLevelBID;
									break;
								}
							}
						}
						parent_right.NextLevelBID = parent.entries[Nleft - 1].NextLevelBID;
						temp_key = parent.entries[Nleft - 1].key;
						temp_NextLevelBID = get_lastBID(file);
						parent.entries[Nleft - 1] = Index_entry();
						parent_right.save(file, temp_NextLevelBID);
						parent.save(file, *iter);					
						if (*iter == path[0]) {
							NLeafNode new_root(header.BlockSize);
							new_root.insert(temp_key, temp_NextLevelBID);
							new_root.NextLevelBID = *iter;
							header.RootBID = get_lastBID(file);
							header.Depth++;
							new_root.save(file, header.RootBID);
							UpdateHeader;
							return true;
						}						
					}
					else {
						parent.save(file, *iter);
						return true;
					}
				}
				return false;
			}
			else {
				node.save(file, path[header.Depth]);
;				return true;//full이 아니어서 삽입이 정상적으로 일어난 경우 true를 반환한다.
			}
		}
	}
}

void Btree::print(fstream& output)
{
	fstream file(filename, MOD);
	open_error_handling(file);
	if (empty()) { cout << "데이터가 비어있습니다." << endl; return; }
	LeafNode Lnode(header.BlockSize);
	NLeafNode NLnode(header.BlockSize);
	output << "<0>\n\n";	
	if (header.Depth == 0) {
		Lnode.load(file, header.RootBID);
		for (int i = 0; i < NH; ++i) {
			if (Lnode.Data[i].key != 0) {
				if (i != 0) output << ",";
				output << Lnode.Data[i].key;
			}
		}
		file.close();
		return;
	}
	else {
		NLnode.load(file, header.RootBID);
		for (int i = 0; i < NH; ++i) {
			if (NLnode.entries[i].key != 0) {
				if (i != 0) output << ",";
				output << NLnode.entries[i].key;
			}
		}
		output << "\n\n<1>\n\n";
		if (header.Depth == 1) {
			Lnode.load(file, NLnode.NextLevelBID);
			bool front = false;
			while (true) {
				for (int i = 0; i < NH + 1; ++i) {
					if (Lnode.Data[i].key != 0) {
						if (front == false) front = true;
						else output << ",";
						output << Lnode.Data[i].key;
					}
				}
				if (Lnode.NextBID != 0) {
					Lnode.load(file, Lnode.NextBID);
				}
				else break;
			}
		}
		else {
			NLeafNode NLnode2(header.BlockSize);
			NLnode2.load(file, NLnode.NextLevelBID);
			bool front = false;
			for(int j = 0 ; j < NH + 1 ; ++j){
				for (int i = 0; i < NH; ++i) {
					if (NLnode2.entries[i].key != 0) {
						if (front == false) front = true;
						else output << ",";
						output << NLnode2.entries[i].key;
					}
				}
				if (NLnode.entries[j].NextLevelBID != 0) {
					NLnode2.load(file, NLnode.entries[j].NextLevelBID);
				}
				else break;
			}
		}
		

	}
	
	
}

int Btree::search(int key)
{
	fstream file(filename, MOD); //탐색할 파일 열기
	open_error_handling(file);
	if (empty()) { cout << "데이터가 비어있습니다." << endl; return 0; }
	int temp_BID = header.RootBID;
	
	for (int i = 0; i < header.Depth; ++i) {
		
		NLeafNode node(header.BlockSize);
		node.load(file, temp_BID);
		temp_BID = node.NextLevelBID;
		for (int j = 0; j < NH; ++j) {
			if (node.entries[j].key == 0 || node.entries[j].key > key) {
				break;
			}
			else temp_BID = node.entries[j].NextLevelBID;
		}
	}
	LeafNode result_node(header.BlockSize);
	result_node.load(file, temp_BID);
	for (int i = 0; i < NH; ++i) {
	
		if (result_node.Data[i].key == key) return result_node.Data[i].Value;
	}
	return -1;
}

void Btree::search(int startRange, int endRange, fstream &output)//output은 ios::out로 열어야 한다.
{
	fstream file(filename, MOD); //탐색할 파일 열기
	open_error_handling(file);
	if (empty()) { cout << "데이터가 비어있습니다." << endl; return; }
	int temp_BID = header.RootBID;

	for (int i = 0; i < header.Depth; ++i) {

		NLeafNode node(header.BlockSize);
		node.load(file, temp_BID);
		temp_BID = node.NextLevelBID;
		for (int j = 0; j < NH; ++j) {
			if (node.entries[j].key == 0 || node.entries[j].key > startRange) {
				break;
			}
			else temp_BID = node.entries[j].NextLevelBID;
		}
	}
	LeafNode result_node(header.BlockSize);
	result_node.load(file, temp_BID);
	int count = 0;
	while (true) {
		int i;
		for (i = 0; i < NH; ++i) {
			if (result_node.Data[i].key == 0) continue;
			if (result_node.Data[i].key >= startRange && result_node.Data[i].key <= endRange) {
				if (count != 0) output << "\t";
				output << result_node.Data[i].key << "," << result_node.Data[i].Value;
				count++;
			}
			else if (count == 0) continue;
			else break;			
		}
		if (i >= NH && result_node.NextBID != 0) {
			result_node.load(file, result_node.NextBID);
		}
		else break;
	}
	output << "\n";
	file.close();
	}

int Btree::get_lastBID(fstream & file)
{
	file.seekg(0, ios::end);
	return (((int)file.tellg() - 12) / header.BlockSize) + 1;
}

NLeafNode::NLeafNode(int blocksize):blocksize(blocksize)
{
	entries = new Index_entry[N];
}

int NLeafNode::insert(int key, int NextLevelBID)//노드가 full이면 들어갔어야할 인덱스를 반환, 그렇지 않으면 삽입후 -1을 반환
{
	int count = 0;
	int found = N;
	while (count < N) {
		if (entries[count].key == 0) {
			if (found == N) {
				entries[count].key = key, entries[count].NextLevelBID = NextLevelBID;
				return -1;
			}
			else {
				break;
			}
		}
		else if (entries[count].key > key && found == N) found = count++;
		else count++;
	}
	if (count == N) return found;
	else {
		for (int i = N - 1; i > found; --i) {
			entries[i] = entries[i - 1];
		}
		entries[found].key = key, entries[found].NextLevelBID = NextLevelBID;
		return -1;
	}
}

void NLeafNode::load(fstream & file, int BID)//스트림은 ios::in가 포함된 상태로 열려져 있어야 한다.
{
	char* buffer = new char[blocksize]; //버퍼 선언
	memset(buffer, 0, blocksize);//버퍼 초기화
	file.seekg(12 + (BID - 1)*blocksize, ios::beg); //커서를 해당 BID의 offset으로 이동
	file.read(buffer, blocksize);
	memcpy(&NextLevelBID, buffer, sizeof(int));
	safe_delete(entries);
	entries = new Index_entry[N];
	memcpy(entries, buffer + sizeof(int), N*sizeof(Index_entry));
	delete[] buffer;
}

void NLeafNode::save(fstream & file, int to_BID)//스트림은 ios::in|ios::out가 포함된 상태로 열려져 있어야 한다.
{
	char* buffer = new char[blocksize]; //버퍼 선언
	memset(buffer, 0, blocksize);//버퍼 초기화
	//버퍼에 노드를 저장
	memcpy(buffer,&NextLevelBID,sizeof(int));
	memcpy(buffer + sizeof(int), entries, N*sizeof(Index_entry));

	file.seekg(12 + (to_BID - 1)*blocksize, ios::beg); //커서를 해당 BID의 offset으로 이동
	file.write(buffer, blocksize); //덮어쓰기	
}

LeafNode::LeafNode(int blocksize):blocksize(blocksize)
{
	Data = new Data_entry[N];
}

int LeafNode::insert(int key, int value)//노드가 full이면 들어갔어야할 인덱스를 반환, 그렇지 않으면 삽입후 -1을 반환
{
	int count = 0;
	int found = N;
	while (count < N) {
		if (Data[count].key == 0) {
			if (found == N) {
				Data[count].key = key, Data[count].Value = value;
				return -1;
			}
			else {
				break;
			}
		}
		else if (Data[count].key > key && found == N) found = count++;
		else count++;
	}
	if (count == N) return found;
	else {
		for (int i = N - 1; i > found; --i) {
			Data[i] = Data[i - 1];
		}
		Data[found].key = key, Data[found].Value = value;
		return -1;
	}
}

void LeafNode::load(fstream & file, int BID)//스트림은 ios::in가 포함된 상태로 열려져 있어야 한다.
{
	char* buffer = new char[blocksize]; //버퍼 선언
	memset(buffer, 0, blocksize);//버퍼 초기화
	
	file.seekg(12 + (BID - 1)*blocksize, ios::beg); //커서를 해당 BID의 offset으로 이동
	file.read(buffer, blocksize);
	safe_delete(Data);
	Data = new Data_entry[N];
	memcpy(Data, buffer, N*sizeof(Data_entry));
	memcpy(&NextBID, buffer + N*sizeof(Data_entry), sizeof(int));
	delete[] buffer;
}

void LeafNode::save(fstream & file, int to_BID)
{
	char* buffer = new char[blocksize]; //버퍼 선언
	memset(buffer, 0, blocksize);//버퍼 초기화
	//버퍼에 노드를 저장
	memcpy(buffer, Data, N*sizeof(Data_entry));
	memcpy(buffer+ N*sizeof(Data_entry), &NextBID, sizeof(int));
	

	file.seekg(12 + (to_BID - 1)*blocksize, ios::beg); //커서를 해당 BID의 offset으로 이동
	file.write(buffer, blocksize); //덮어쓰기	
}
