#ifndef BPT_MEMORYRIVER_HPP
#define BPT_MEMORYRIVER_HPP

#include <fstream>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<class T, int info_len = 2>
class MemoryRiver
{
private:
    int num = 0, empty_pos = 0;
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
public:
    MemoryRiver() = default;

    MemoryRiver(const string& file_name) : file_name(file_name) {}

    void initialise(string FN = "")
    {
        if (FN != "") file_name = FN;
        file.open(file_name, std::ios::out);
        int tmp = 0;
        for (int i = 0; i < info_len; ++i)
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    //������n��int��ֵ����tmp��1_base
    void get_info(int &tmp, int n)
    {
        if (n > info_len) return;
        file.open(file_name);
        file.seekg((n-1)*sizeof(int));
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    //��tmpд���n��int��λ�ã�1_base
    void write_info(int tmp, int n)
    {
        if (n > info_len) return;
        file.open(file_name);
        file.seekp((n-1)*sizeof(int));
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    //���ļ�����λ��д�������t��������д���λ������index
    //λ��������ζ�ŵ�������ȷ��λ������index�����������������ж���˳�����ҵ�Ŀ�������в���
    //λ������index����ȡΪ����д�����ʼλ��
    int write(T &t)
    {
        file.open(file_name);
        int index = empty_pos, pos = info_len*sizeof(int) + empty_pos*(sizeof(int)+sizeofT);
        if(empty_pos == num) //��β��д��
            pos += sizeof(int);
        else //��ɾ����д��
        {
            file.seekg(pos);
            file.read(reinterpret_cast<char *>(&empty_pos), sizeof(int));
            pos += sizeof(int);
        }
        file.seekp(pos);
        file.write(reinterpret_cast<char *>(&t), sizeof(int));
        ++num;
        return index;
    }

    //��t��ֵ����λ������index��Ӧ�Ķ��󣬱�֤���õ�index������write��������
    void update(T &t, const int index)
    {
        file.open(file_name);
        int pos = info_len*sizeof(int) + index*(sizeof(int)+sizeofT) + sizeof(int);
        file.seekp(pos);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }

    //����λ������index��Ӧ��T�����ֵ����ֵ��t����֤���õ�index������write��������
    void read(T &t, const int index)
    {
        file.open(file_name);
        int pos = info_len*sizeof(int) + index*(sizeof(int)+sizeofT) + sizeof(int);
        file.seekg(pos);
        file.read(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }

    //ɾ��λ������index��Ӧ�Ķ���(���漰�ռ����ʱ���ɺ��Դ˺���)����֤���õ�index������write��������
    void Delete(int index)
    {
        file.open(file_name);
        --num;
        int pos = info_len*sizeof(int) + index*(sizeof(int)+sizeofT);
        file.seekp(pos);
        file.write(reinterpret_cast<char *>(&empty_pos), sizeof(int));
        empty_pos = index;
        file.close();
    }
};


#endif //BPT_MEMORYRIVER_HPP