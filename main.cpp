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
#define open_error_handling(file) if(!file){cout<<"���� ���� ����";exit(1);}
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
	case 'c': { //������ϸ� �����Ͽ� �����Ѵ�.
		ofstream out;
		Header header(atoi(argv[3]), 0, 0);
		out.open(argv[2], ios::binary);
		open_error_handling(out);
		out.write((char*)&header, sizeof(header));
		out.close();
		break;
	}
	case 'i'://����
	{
		//blocksize �ҷ�����
		int blocksize;
		fstream file(argv[2], MOD);
		open_error_handling(file);
		file.read((char*)&blocksize, sizeof(int));
		file.close();		
		Btree btree(argv[2], blocksize);//�ҷ��� blocksize�� ���� btree ����
		ifstream insert_file(argv[3]);//������ ���� ����
		open_error_handling(insert_file);		
		string temp;
		while (getline(insert_file,temp)) {
			if (temp == "") continue;
			istringstream line(temp);
			string key_string, ID_string;
			getline(line, key_string,','); getline(line, ID_string,',');
			int key = stoi(key_string); //�ҷ��� ���� ����
			int ID = stoi(ID_string); //�ҷ��� ���� ����		
			btree.insert(key, ID);// btree�� ���� ����
		}
		insert_file.close();//������ ���� �ݱ�
	}

	break;
	case 's':
	{
		//blocksize �ҷ�����
		int blocksize;
		fstream file(argv[2], MOD);
		open_error_handling(file);
		file.read((char*)&blocksize, sizeof(int));
		file.close();
		Btree btree(argv[2], blocksize);//�ҷ��� blocksize�� ���� btree ����
		ifstream search_file(argv[3],ios::in);//Ž���� ���� ����
		open_error_handling(search_file);
		ofstream result_file(argv[4],ios::out);
		open_error_handling(result_file);
		string temp;		
		while (getline(search_file, temp)) {
			if (temp == "") continue;
			int key = stoi(temp); //�ҷ��� ���� ����
			result_file << temp << ","<< to_string(btree.search(key)) << "\n";
		}
		search_file.close();//������ ���� �ݱ�
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
		ifstream search_file(argv[3], ios::in);//Ž���� ���� ����
		open_error_handling(search_file);
		fstream result_file(argv[4],ios::out);
		open_error_handling(result_file);

		string temp;
		while (getline(search_file, temp)) {
			if (temp == "") continue;
			istringstream line(temp);
			string start_string, end_string;
			getline(line, start_string, ','); getline(line, end_string, ',');
			int start_range = stoi(start_string); //�ҷ��� ���� ����
			int end_range = stoi(end_string); //�ҷ��� ���� ����	
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
	ifstream file(fileName, ios::binary);//���� ���� ����
	open_error_handling(file);
	file.read((char*)&header, sizeof(Header));//�������Ϸ� ���� ��� �ҷ�����
	file.close();
}

bool Btree::empty()
{return header.RootBID == 0;}

bool Btree::insert(int key, int value)
{
	fstream file(filename, MOD); //������ ���� ����
	open_error_handling(file);	
	if (empty()) {//btree�� empty��� �Ʒ��� ���� �ٷ� �����Ѵ�.
		//������ ��带 ����
		LeafNode root(header.BlockSize);
		root.insert(key, value);
		//����(ù ���� �̹Ƿ� split�� ���� �������� �����Ƿ� ó������ �ʴ´�.)
		header.RootBID = 1;
		UpdateHeader;
		root.save(file, header.RootBID);
		return true;//���� �Ϸ��� true ��ȯ
	}
	else {//btree�� empty�� �ƴ϶�� ��Ʈ ��带 �ҷ��´�.
		LeafNode node(header.BlockSize);
		node.load(file, header.RootBID);
		if (header.Depth == 0) {//��Ʈ��尡 ��������� ��� ��Ʈ��忡 �ٷ� ������ �����Ѵ�.
			int index = node.insert(key, value);
			if (index != -1) {//��Ʈ ��尡 full�̶� ���Կ� ������ ��� split�� �����Ѵ�.
				LeafNode right(header.BlockSize);//�ɰ����� ���� ���� ������ ��带 ����					
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
						node.Data[j - 1] = Data_entry();//�Űܳ��� �ʱ�ȭ
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
				NLeafNode parent(header.BlockSize);//��Ʈ ���� �θ� �����Ƿ� ���� ����� �ش�.
				parent.NextLevelBID = 1;
				parent.insert(right.Data[0].key, 2);			
				parent.save(file, 3);
				header.Depth++;
				header.RootBID = 3;
				UpdateHeader;
				return true;//���� �Ϸ��� true ��ȯ
			}
			else {
				node.save(file,header.RootBID);				
				return true;//��Ʈ�� full�� �ƴϾ ������ ���������� �Ͼ ��� true�� ��ȯ�Ѵ�.
			}
		}
		else {//��Ʈ��尡 non-leaf node �� ��� (�� btree�� depth�� 0�� �ƴ� ���)
			vector<int> path;
			path.push_back(header.RootBID);
			//������ ��ġ�� �˾Ƴ��� ���� ���������� ã�Ƴ��� ������.
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
			//������ ��ġ�� ã������, �ҷ��´�.		
			LeafNode node(header.BlockSize);			
			node.load(file, path[header.Depth]);
			int index = node.insert(key, value);//�����Ѵ�.
			if (index != -1) {//���� ��尡 full�̶� ���Կ� ������ ��� split�� �����Ѵ�.
				LeafNode right(header.BlockSize);//�ɰ����� ���� ���� ������ ��带 ����	
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
				//���� �ε����� Ÿ�� �ö���
				int temp_key = right.Data[0].key;
				int temp_NextLevelBID = node.NextBID;
				for (auto iter = path.rbegin()+1; iter != path.rend(); ++iter) {
					NLeafNode parent(header.BlockSize);
					parent.load(file, *iter);
					int Nindex = parent.insert(temp_key, temp_NextLevelBID);
					//���� ���ø�
					if (Nindex != -1) {
						NLeafNode parent_right(header.BlockSize);//�ɰ����� ���� ���� ������ ��带 ����			
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
;				return true;//full�� �ƴϾ ������ ���������� �Ͼ ��� true�� ��ȯ�Ѵ�.
			}
		}
	}
}

void Btree::print(fstream& output)
{
	fstream file(filename, MOD);
	open_error_handling(file);
	if (empty()) { cout << "�����Ͱ� ����ֽ��ϴ�." << endl; return; }
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
	fstream file(filename, MOD); //Ž���� ���� ����
	open_error_handling(file);
	if (empty()) { cout << "�����Ͱ� ����ֽ��ϴ�." << endl; return 0; }
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

void Btree::search(int startRange, int endRange, fstream &output)//output�� ios::out�� ����� �Ѵ�.
{
	fstream file(filename, MOD); //Ž���� ���� ����
	open_error_handling(file);
	if (empty()) { cout << "�����Ͱ� ����ֽ��ϴ�." << endl; return; }
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

int NLeafNode::insert(int key, int NextLevelBID)//��尡 full�̸� ������� �ε����� ��ȯ, �׷��� ������ ������ -1�� ��ȯ
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

void NLeafNode::load(fstream & file, int BID)//��Ʈ���� ios::in�� ���Ե� ���·� ������ �־�� �Ѵ�.
{
	char* buffer = new char[blocksize]; //���� ����
	memset(buffer, 0, blocksize);//���� �ʱ�ȭ
	file.seekg(12 + (BID - 1)*blocksize, ios::beg); //Ŀ���� �ش� BID�� offset���� �̵�
	file.read(buffer, blocksize);
	memcpy(&NextLevelBID, buffer, sizeof(int));
	safe_delete(entries);
	entries = new Index_entry[N];
	memcpy(entries, buffer + sizeof(int), N*sizeof(Index_entry));
	delete[] buffer;
}

void NLeafNode::save(fstream & file, int to_BID)//��Ʈ���� ios::in|ios::out�� ���Ե� ���·� ������ �־�� �Ѵ�.
{
	char* buffer = new char[blocksize]; //���� ����
	memset(buffer, 0, blocksize);//���� �ʱ�ȭ
	//���ۿ� ��带 ����
	memcpy(buffer,&NextLevelBID,sizeof(int));
	memcpy(buffer + sizeof(int), entries, N*sizeof(Index_entry));

	file.seekg(12 + (to_BID - 1)*blocksize, ios::beg); //Ŀ���� �ش� BID�� offset���� �̵�
	file.write(buffer, blocksize); //�����	
}

LeafNode::LeafNode(int blocksize):blocksize(blocksize)
{
	Data = new Data_entry[N];
}

int LeafNode::insert(int key, int value)//��尡 full�̸� ������� �ε����� ��ȯ, �׷��� ������ ������ -1�� ��ȯ
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

void LeafNode::load(fstream & file, int BID)//��Ʈ���� ios::in�� ���Ե� ���·� ������ �־�� �Ѵ�.
{
	char* buffer = new char[blocksize]; //���� ����
	memset(buffer, 0, blocksize);//���� �ʱ�ȭ
	
	file.seekg(12 + (BID - 1)*blocksize, ios::beg); //Ŀ���� �ش� BID�� offset���� �̵�
	file.read(buffer, blocksize);
	safe_delete(Data);
	Data = new Data_entry[N];
	memcpy(Data, buffer, N*sizeof(Data_entry));
	memcpy(&NextBID, buffer + N*sizeof(Data_entry), sizeof(int));
	delete[] buffer;
}

void LeafNode::save(fstream & file, int to_BID)
{
	char* buffer = new char[blocksize]; //���� ����
	memset(buffer, 0, blocksize);//���� �ʱ�ȭ
	//���ۿ� ��带 ����
	memcpy(buffer, Data, N*sizeof(Data_entry));
	memcpy(buffer+ N*sizeof(Data_entry), &NextBID, sizeof(int));
	

	file.seekg(12 + (to_BID - 1)*blocksize, ios::beg); //Ŀ���� �ش� BID�� offset���� �̵�
	file.write(buffer, blocksize); //�����	
}
