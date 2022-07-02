// Finished by MeteorVanish on 2022.6.19

#ifndef SJTU_BPLUSTREE_HPP
#define SJTU_BPLUSTREE_HPP

#include <fstream>
#include <functional>
#include <cstddef>
#include "exception.hpp"

using std::pair;
using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

enum TYPE {INTERNAL = 73/*I*/, LEAF = 76/*L*/, NIL = 78/*N*/, EMPTY = 69/*E*/};

const size_t ORDER = 10;          // max Child num of INTERNAL Node
const size_t CAPACITY_LEAF = 100; // max Data num of LEAF Node

namespace sjtu
{
    template <class Key, class Value>
    class BPlusTree
    {
    public:
        using DataType = pair<Key, Value>;

    private:
        const char *FileName;
        size_t FileFlag;
        fstream File;

        struct Node
        {
            size_t Type;                      // INTERNAL or LEAF or NIL
            size_t Num;                       // max: MAX_CHILD - 1
            size_t FatherIndex;
            size_t ChildIndex[ORDER + 1];     // 0 ~ Num (one extra space for overflow, only for INTERNAL Node)
            Key KeyIndex[ORDER + 1];          // 1 ~ Num (one extra space for overflow, only for INTERNAL Node)
            DataType Data[CAPACITY_LEAF + 2]; // 1 ~ Num (one extra space for overflow, only for LEAF Node)
            size_t PrevLeafIndex;             // only for LEAF Node
            size_t NextLeafIndex;             // only for LEAF Node
            size_t NextEmptyIndex;            // only for space recovery

            Node() = default;
            Node(TYPE Type_param)
                    :Type(Type_param), Num(0), NextEmptyIndex(-1), FatherIndex(-1), PrevLeafIndex(-1), NextLeafIndex(-1)
            {
                memset(ChildIndex, 0, sizeof(ChildIndex));
                memset(KeyIndex, 0, sizeof(KeyIndex));
                memset(Data, 0, sizeof(Data));
            }
            ~Node() = default;

            // insert InsertData to Data[] while keeping ascending order
            void LeafInsert(DataType InsertData)
            {
                Data[++Num] = InsertData;
                for(size_t i = Num; i >= 1; --i)
                {
                    if(i != 1 && InsertData.first < Data[i - 1].first)
                        Data[i] = Data[i - 1];
                    else { Data[i] = InsertData; break; }
                }
            }

            // insert InsertKey and NewNodeIndex while keeping KeyIndex[]'s ascending order
            void InternalInsert(Key InsertKey, size_t NewNodeIndex)
            {
                ++Num;
                KeyIndex[Num] = InsertKey;
                ChildIndex[Num] = NewNodeIndex;
                for(size_t i = Num; i >= 1; --i)
                {
                    if(i != 1 && InsertKey < KeyIndex[i - 1])
                    {
                        KeyIndex[i] = KeyIndex[i - 1];
                        ChildIndex[i] = ChildIndex[i - 1];
                    }
                    else
                    {
                        KeyIndex[i] = InsertKey;
                        ChildIndex[i] = NewNodeIndex;
                        break;
                    }
                }
            }

            // erase Data whose first == EraseKey in Data[]
            void LeafErase(Key EraseKey)
            {
                size_t EraseIndex;
                for(size_t i = 1; i <= Num; ++i)
                    if(Data[i].first == EraseKey)
                    { EraseIndex = i; break;}
                for(size_t i = EraseIndex; i <= Num - 1; ++i)
                    Data[i] = Data[i + 1];
                Data[Num--] = DataType();
            }

            // erase EraseKey and corresponding ChildIndex
            void InternalErase(Key EraseKey)
            {
                size_t EraseIndex = 0;
                for(size_t i = 1; i <= Num; ++i)
                    if(KeyIndex[i] == EraseKey)
                    { EraseIndex = i; break;}
                for(size_t i = EraseIndex; i <= Num - 1; ++i)
                    KeyIndex[i] = KeyIndex[i + 1], ChildIndex[i] = ChildIndex[i + 1];
                KeyIndex[0] = Key();
                KeyIndex[Num] = Key();
                ChildIndex[Num] = size_t();
                --Num;
            }

            void Reset()
            {
                Type = EMPTY;
                Num = 0;
                FatherIndex = -1;
                PrevLeafIndex = -1;
                NextLeafIndex = -1;
                memset(ChildIndex, 0, sizeof(ChildIndex));
                memset(KeyIndex, 0, sizeof(KeyIndex));
                memset(Data, 0, sizeof(Data));
            }
        };

        // to avoid opening file multiple times
        void OpenFile() { if(FileFlag++ == 0) File.open(FileName); }

        // to avoid closing file multiple times
        void CloseFile() { if(FileFlag-- == 1) File.close(); }

        size_t GetSize()
        {
            OpenFile();
            size_t Size;
            File.seekg(0);
            File.read(reinterpret_cast<char *>(&Size), sizeof(size_t));
            CloseFile();
            return Size;
        }

        void PutSize(size_t Size)
        {
            OpenFile();
            File.seekp(0);
            File.write(reinterpret_cast<char *>(&Size), sizeof(size_t));
            CloseFile();
        }

        size_t GetNodeMaxIndex()
        {
            OpenFile();
            size_t NodeMaxIndex;
            File.seekg(sizeof(size_t));
            File.read(reinterpret_cast<char *>(&NodeMaxIndex), sizeof(size_t));
            CloseFile();
            return NodeMaxIndex;
        }

        void PutNodeMaxIndex(size_t NodeMaxIndex)
        {
            OpenFile();
            File.seekp(sizeof(size_t));
            File.write(reinterpret_cast<char *>(&NodeMaxIndex), sizeof(size_t));
            CloseFile();
        }

        size_t GetRootIndex()
        {
            OpenFile();
            size_t RootIndex;
            File.seekg(2 * sizeof(size_t));
            File.read(reinterpret_cast<char *>(&RootIndex), sizeof(size_t));
            CloseFile();
            return RootIndex;
        }

        void PutRootIndex(size_t RootIndex)
        {
            OpenFile();
            File.seekp(2 * sizeof(size_t));
            File.write(reinterpret_cast<char *>(&RootIndex), sizeof(size_t));
            CloseFile();
        }

        size_t GetEmptyIndex()
        {
            OpenFile();
            size_t EmptyIndex;
            File.seekg(3 * sizeof(size_t));
            File.read(reinterpret_cast<char *>(&EmptyIndex), sizeof(size_t));
            CloseFile();
            return EmptyIndex;
        }

        void PutEmptyIndex(size_t EmptyIndex)
        {
            OpenFile();
            File.seekp(3 * sizeof(size_t));
            File.write(reinterpret_cast<char *>(&EmptyIndex), sizeof(size_t));
            CloseFile();
        }

        // return the Address corresponding to the Index
        size_t GetNodeAddress(size_t Index)
        {
            if(Index == -2) // Head
                return 4 * sizeof(size_t);
            if(Index == -3) // Tail
                return 4 * sizeof(size_t) + sizeof(Node);
            return 4 * sizeof(size_t) + (Index + 2) * sizeof(Node);
        }

        // return Index of new Node (with space recovery)
        size_t NewNodeIndex()
        {
            OpenFile();
            if(GetEmptyIndex() == -1)
            {
                PutNodeMaxIndex(GetNodeMaxIndex() + 1);
                return GetNodeMaxIndex();
            }
            else // space recovery
            {
                OpenFile();
                size_t Index = GetEmptyIndex();
                Node Pos;
                File.seekg(GetNodeAddress(Index));
                File.read(reinterpret_cast<char *>(&Pos), sizeof(Node));
                PutEmptyIndex(Pos.NextEmptyIndex);
                CloseFile();
                return Index;
            }
            CloseFile();
        }

        // return Index of the target's LEAF Node
        size_t FindLeaf(const Key &target)
        {
            OpenFile();
            size_t TargetIndex = GetRootIndex();
            Node Pos;
            while(true)
            {
                File.seekg(GetNodeAddress(TargetIndex));
                File.read(reinterpret_cast<char *>(&Pos), sizeof(Node));
                if(Pos.Type == LEAF) break; // root is LEAF
                TargetIndex = Pos.ChildIndex[Pos.Num];
                for(size_t i = 1; i <= Pos.Num; ++i)
                    if(target < Pos.KeyIndex[i])
                    { TargetIndex = Pos.ChildIndex[i - 1]; break; }
            }
            CloseFile();
            return TargetIndex;
        }

        // return the Node corresponding to NodeIndex in the iterator
        Node IterGetNode(size_t NodeIndex)
        {
            OpenFile();
            Node ret;
            File.seekg(GetNodeAddress(NodeIndex));
            File.read(reinterpret_cast<char *>(&ret), sizeof(Node));
            CloseFile();
            return ret;
        }

        // write Pos to file
        void IterPutNode(Node Pos, size_t NodeIndex)
        {
            OpenFile();
            File.seekp(GetNodeAddress(NodeIndex));
            File.write(reinterpret_cast<char *>(&Pos), sizeof(Node));
            CloseFile();
        }

    public:
        // write content at the head of file
        // content: Size(size_t), NodeMaxIndex(size_t), RootIndex(size_t), EmptyIndex(size_t), Head(Node), Tail(Node)
        BPlusTree(const char* FileName_param = "DataFile.txt") :FileName(FileName_param), FileFlag(0)
        {
            OpenFile();

            // write init data
            size_t Size = 0, NodeMaxIndex = 0, RootIndex = 0, EmptyIndex = -1;
            Node Root(LEAF), Head(NIL), Tail(NIL);
            Head.NextLeafIndex = RootIndex;
            Tail.PrevLeafIndex = RootIndex;
            Root.PrevLeafIndex = -2; // HeadIndex = -2
            Root.NextLeafIndex = -3; // TailIndex = -3
            File.seekp(0);
            File.write(reinterpret_cast<char *>(&Size), sizeof(size_t));
            File.write(reinterpret_cast<char *>(&NodeMaxIndex), sizeof(size_t));
            File.write(reinterpret_cast<char *>(&RootIndex), sizeof(size_t));
            File.write(reinterpret_cast<char *>(&EmptyIndex), sizeof(size_t));
            File.write(reinterpret_cast<char *>(&Head), sizeof(Node));
            File.write(reinterpret_cast<char *>(&Tail), sizeof(Node));
            File.write(reinterpret_cast<char *>(&Root), sizeof(Node));

            CloseFile();
        }

        ~BPlusTree() = default;

        // clear the BPlusTree
        void clear()
        {
            // clear the file
            fstream ClearFile(FileName, std::ios::out | std::ios::trunc);
            ClearFile.close();

            // rewrite init data
            OpenFile();
            size_t Size = 0, NodeMaxIndex = 0, RootIndex = 0, EmptyIndex = -1;
            Node Root(LEAF), Head(NIL), Tail(NIL);
            Head.NextLeafIndex = RootIndex;
            Tail.PrevLeafIndex = RootIndex;
            Root.PrevLeafIndex = -2; // HeadIndex = -2
            Root.NextLeafIndex = -3; // TailIndex = -3
            File.seekp(0);
            File.write(reinterpret_cast<char *>(&Size), sizeof(size_t));
            File.write(reinterpret_cast<char *>(&NodeMaxIndex), sizeof(size_t));
            File.write(reinterpret_cast<char *>(&RootIndex), sizeof(size_t));
            File.write(reinterpret_cast<char *>(&EmptyIndex), sizeof(size_t));
            File.write(reinterpret_cast<char *>(&Head), sizeof(Node));
            File.write(reinterpret_cast<char *>(&Tail), sizeof(Node));
            File.write(reinterpret_cast<char *>(&Root), sizeof(Node));
            CloseFile();
        }

        // return true if insert successfully, else return false
        // do nothing if key already exists
        bool insert(const Key &key, const Value &value)
        {
            OpenFile();

            // insert <key, value> to Leaf
            size_t LeafIndex = FindLeaf(key);
            Node Leaf;
            File.seekg(GetNodeAddress(LeafIndex));
            File.read(reinterpret_cast<char *>(&Leaf), sizeof(Node));
            for(size_t i = 1; i <= Leaf.Num; ++i)
                if(key == Leaf.Data[i].first) // already exist
                {
                    CloseFile();
                    return false;
                }
            PutSize(GetSize() + 1);
            Leaf.LeafInsert(DataType(key, value));
            if(Leaf.Num <= CAPACITY_LEAF) // don't need to split
            {
                File.seekp(GetNodeAddress(LeafIndex));
                File.write(reinterpret_cast<char *>(&Leaf), sizeof(Node));
                CloseFile();
                return true;
            }

            // split Leaf to NewLeaf
            size_t NewLeafIndex = NewNodeIndex(), LeafOriginalNextIndex = Leaf.NextLeafIndex;
            Node NewLeaf(LEAF), LeafOriginalNext;
            File.seekg(GetNodeAddress(LeafOriginalNextIndex));
            File.read(reinterpret_cast<char *>(&LeafOriginalNext), sizeof(Node));
            NewLeaf.PrevLeafIndex = LeafIndex;
            NewLeaf.NextLeafIndex = Leaf.NextLeafIndex;
            Leaf.NextLeafIndex = NewLeafIndex;
            LeafOriginalNext.PrevLeafIndex = NewLeafIndex;
            File.seekp(GetNodeAddress(LeafOriginalNextIndex));
            File.write(reinterpret_cast<char *>(&LeafOriginalNext), sizeof(Node));
            NewLeaf.Num = (Leaf.Num + 1) / 2;
            Leaf.Num /= 2;
            for(size_t i = 1; i <= NewLeaf.Num; ++i)
            {
                NewLeaf.Data[i] = Leaf.Data[Leaf.Num + i];
                Leaf.Data[Leaf.Num + i] = DataType();
            }

            // adjust up
            if(Leaf.FatherIndex == -1) // root is LEAF
            {
                size_t NewRootIndex = NewNodeIndex();
                Node NewRoot(INTERNAL);

                // adjust Leaf, NewRoot and NewLeaf
                NewRoot.Num = 1;
                NewRoot.ChildIndex[0] = LeafIndex;
                NewRoot.ChildIndex[1] = NewLeafIndex;
                NewRoot.KeyIndex[1] = NewLeaf.Data[1].first;
                Leaf.FatherIndex = NewRootIndex;
                NewLeaf.FatherIndex = NewRootIndex;

                // write to file
                PutRootIndex(NewRootIndex);
                File.seekp(GetNodeAddress(LeafIndex));
                File.write(reinterpret_cast<char *>(&Leaf), sizeof(Node));
                File.seekp(GetNodeAddress(NewRootIndex));
                File.write(reinterpret_cast<char *>(&NewRoot), sizeof(Node));
                File.seekp(GetNodeAddress(NewLeafIndex));
                File.write(reinterpret_cast<char *>(&NewLeaf), sizeof(Node));
            }
            else
            {
                NewLeaf.FatherIndex = Leaf.FatherIndex;

                // write to file
                File.seekp(GetNodeAddress(LeafIndex));
                File.write(reinterpret_cast<char *>(&Leaf), sizeof(Node));
                File.seekp(GetNodeAddress(NewLeafIndex));
                File.write(reinterpret_cast<char *>(&NewLeaf), sizeof(Node));

                size_t FatherIndex = Leaf.FatherIndex;
                Node Father;
                File.seekg(GetNodeAddress(FatherIndex));
                File.read(reinterpret_cast<char *>(&Father), sizeof(Node));

                Key SplitExtraKey = NewLeaf.Data[1].first;
                size_t PosIndex, NewPosIndex = NewLeafIndex;
                Node Pos;
                while(true)
                {
                    // adjust Father
                    Father.InternalInsert(SplitExtraKey, NewPosIndex);
                    if(Father.Num < ORDER) // don't need to continue to split
                    {
                        File.seekp(GetNodeAddress(FatherIndex));
                        File.write(reinterpret_cast<char *>(&Father), sizeof(Node));
                        CloseFile();
                        return true;
                    }

                    // level up
                    Pos = Father;
                    PosIndex = FatherIndex;
                    FatherIndex = Pos.FatherIndex;
                    if(FatherIndex == -1) break;
                    Node NewPos(INTERNAL);
                    NewPosIndex = NewNodeIndex();
                    File.seekg(GetNodeAddress(FatherIndex));
                    File.read(reinterpret_cast<char *>(&Father), sizeof(Node));

                    // split Pos to NewPos and record SplitExtraKey
                    SplitExtraKey = Pos.KeyIndex[Pos.Num / 2 + 1];
                    Pos.KeyIndex[Pos.Num / 2 + 1] = Key();
                    NewPos.Num = (Pos.Num - 1) / 2;
                    Pos.Num /= 2;
                    NewPos.FatherIndex = Pos.FatherIndex;
                    NewPos.ChildIndex[0] = Pos.ChildIndex[Pos.Num + 1];
                    Pos.ChildIndex[Pos.Num + 1] = size_t();
                    for(size_t i = 1; i <= NewPos.Num; ++i)
                    {
                        NewPos.KeyIndex[i] = Pos.KeyIndex[Pos.Num + 1 + i];
                        NewPos.ChildIndex[i] = Pos.ChildIndex[Pos.Num + 1 + i];
                        Pos.KeyIndex[Pos.Num + 1 + i] = Key();
                        Pos.ChildIndex[Pos.Num + 1 + i] = size_t();
                    }

                    // adjust Father of NewPos's Child
                    for(size_t i = 0; i <= NewPos.Num; ++i)
                    {
                        size_t TmpIndex = NewPos.ChildIndex[i];
                        Node Tmp;
                        File.seekg(GetNodeAddress(TmpIndex));
                        File.read(reinterpret_cast<char *>(&Tmp), sizeof(Node));
                        Tmp.FatherIndex = NewPosIndex;
                        File.seekp(GetNodeAddress(TmpIndex));
                        File.write(reinterpret_cast<char *>(&Tmp), sizeof(Node));
                    }

                    // write to file
                    File.seekp(GetNodeAddress(PosIndex));
                    File.write(reinterpret_cast<char *>(&Pos), sizeof(Node));
                    File.seekp(GetNodeAddress(NewPosIndex));
                    File.write(reinterpret_cast<char *>(&NewPos), sizeof(Node));
                }

                /*
                 * split the root
                 * only run when tree height will increase
                 */
                size_t NewRootIndex = NewNodeIndex();
                NewPosIndex = NewNodeIndex();
                Node NewRoot(INTERNAL), NewPos(INTERNAL);

                // split Pos to NewPos and record SplitExtraKey
                SplitExtraKey = Pos.KeyIndex[Pos.Num / 2 + 1];
                Pos.KeyIndex[Pos.Num / 2 + 1] = Key();
                NewPos.Num = (Pos.Num - 1) / 2;
                Pos.Num /= 2;
                Pos.FatherIndex = NewRootIndex;
                NewPos.FatherIndex = NewRootIndex;
                NewPos.ChildIndex[0] = Pos.ChildIndex[Pos.Num + 1];
                Pos.ChildIndex[Pos.Num + 1] = size_t();
                for(size_t i = 1; i <= NewPos.Num; ++i)
                {
                    NewPos.KeyIndex[i] = Pos.KeyIndex[Pos.Num + 1 + i];
                    NewPos.ChildIndex[i] = Pos.ChildIndex[Pos.Num + 1 + i];
                    Pos.KeyIndex[Pos.Num + 1 + i] = Key();
                    Pos.ChildIndex[Pos.Num + 1 + i] = size_t();
                }

                // adjust Father of NewPos's Child
                for(size_t i = 0; i <= NewPos.Num; ++i)
                {
                    size_t TmpIndex = NewPos.ChildIndex[i];
                    Node Tmp;
                    File.seekg(GetNodeAddress(TmpIndex));
                    File.read(reinterpret_cast<char *>(&Tmp), sizeof(Node));
                    Tmp.FatherIndex = NewPosIndex;
                    File.seekp(GetNodeAddress(TmpIndex));
                    File.write(reinterpret_cast<char *>(&Tmp), sizeof(Node));
                }

                // adjust NewRoot
                NewRoot.Num = 1;
                NewRoot.ChildIndex[0] = PosIndex;
                NewRoot.ChildIndex[1] = NewPosIndex;
                NewRoot.KeyIndex[1] = SplitExtraKey;

                // write to file
                PutRootIndex(NewRootIndex);
                File.seekp(GetNodeAddress(PosIndex));
                File.write(reinterpret_cast<char *>(&Pos), sizeof(Node));
                File.seekp(GetNodeAddress(NewPosIndex));
                File.write(reinterpret_cast<char *>(&NewPos), sizeof(Node));
                File.seekp(GetNodeAddress(NewRootIndex));
                File.write(reinterpret_cast<char *>(&NewRoot), sizeof(Node));
            }
            CloseFile();
            return true;
        }

        // return true if modify successfully, else return false
        bool modify(const Key &key, const Value &value)
        {
            OpenFile();
            bool ret = false;
            size_t ModifyIndex = FindLeaf(key);
            Node Pos;
            File.seekg(GetNodeAddress(ModifyIndex));
            File.read(reinterpret_cast<char *>(&Pos), sizeof(Node));
            for(size_t i = 1; i <= Pos.Num; ++i)
                if(key == Pos.Data[i].first)
                { Pos.Data[i].second = value; ret = true; }
            File.seekp(GetNodeAddress(ModifyIndex));
            File.write(reinterpret_cast<char *>(&Pos), sizeof(Node));
            CloseFile();
            return ret;
        }

        // return the Value corresponding to the Key
        // return Value() if Key doesn't exist
        Value at(const Key &key)
        {
            OpenFile();
            Value ret = Value();
            size_t AtIndex = FindLeaf(key);
            Node Pos;
            File.seekg(GetNodeAddress(AtIndex));
            File.read(reinterpret_cast<char *>(&Pos), sizeof(Node));
            for(size_t i = 1; i <= Pos.Num; ++i)
                if(key == Pos.Data[i].first)
                { ret = Pos.Data[i].second; break; }
            CloseFile();
            return ret;
        }

        // return true if erase successfully, else return false
        bool erase(const Key &key)
        {
            OpenFile();

            size_t LeafIndex = FindLeaf(key);
            Node Leaf;
            File.seekg(GetNodeAddress(LeafIndex));
            File.read(reinterpret_cast<char *>(&Leaf), sizeof(Node));
            bool ExistFlag = false, AdjustFlag = false;
            for(size_t i = 1; i <= Leaf.Num; ++i)
                if(key == Leaf.Data[i].first)
                {
                    ExistFlag = true;
                    if(i == 1) AdjustFlag = true;
                    break;
                }
            if(!ExistFlag) // don't exist
            {
                CloseFile();
                return false;
            }

            // get Leaf's Father, PrevLeaf and NextLeaf
            size_t FatherIndex = Leaf.FatherIndex,
                    PrevLeafIndex = Leaf.PrevLeafIndex,
                    NextLeafIndex = Leaf.NextLeafIndex;
            Node Father, PrevLeaf, NextLeaf;
            File.seekg(GetNodeAddress(FatherIndex));
            File.read(reinterpret_cast<char *>(&Father), sizeof(Node));
            File.seekg(GetNodeAddress(PrevLeafIndex));
            File.read(reinterpret_cast<char *>(&PrevLeaf), sizeof(Node));
            File.seekg(GetNodeAddress(NextLeafIndex));
            File.read(reinterpret_cast<char *>(&NextLeaf), sizeof(Node));

            // erase data whose first == key in Leaf
            PutSize(GetSize() - 1);
            Leaf.LeafErase(key);

            // adjust KeyIndex corresponding to Leaf
            if(AdjustFlag)
            {
                if(LeafIndex == Father.ChildIndex[0]) // adjust KeyIndex in Leaf's ancestor (not Father)
                {
                    size_t TmpIndex;
                    Node Tmp = Father;
                    while(Tmp.FatherIndex != -1)
                    {
                        bool BreakFlag = false;
                        TmpIndex = Tmp.FatherIndex;
                        File.seekg(GetNodeAddress(TmpIndex));
                        File.read(reinterpret_cast<char *>(&Tmp), sizeof(Node));
                        for(size_t i = 1; i <= Tmp.Num; ++i)
                        {
                            if(Tmp.KeyIndex[i] == key)
                            {
                                Tmp.KeyIndex[i] = Leaf.Data[1].first;
                                File.seekp(GetNodeAddress(TmpIndex));
                                File.write(reinterpret_cast<char *>(&Tmp), sizeof(Node));
                                BreakFlag = true;
                                break;
                            }
                            else if(Tmp.KeyIndex[i] > key) break;
                        }
                        if(BreakFlag) break;
                    }
                }
                else // adjust KeyIndex in Leaf's Father
                {
                    for(size_t i = 1; i <= Father.Num; ++i)
                        if(Father.KeyIndex[i] == key)
                        {
                            Father.KeyIndex[i] = Leaf.Data[1].first;
                            File.seekp(GetNodeAddress(FatherIndex));
                            File.write(reinterpret_cast<char *>(&Father), sizeof(Node));
                            break;
                        }
                }
            }

            // don't need to operate
            if(LeafIndex == GetRootIndex() || Leaf.Num >= (CAPACITY_LEAF + 1) / 2)
            {
                File.seekp(GetNodeAddress(LeafIndex));
                File.write(reinterpret_cast<char *>(&Leaf), sizeof(Node));
                CloseFile();
                return true;
            }

            if(PrevLeaf.FatherIndex == Leaf.FatherIndex && PrevLeaf.Num > (CAPACITY_LEAF + 1) / 2) // adopt from PrevLeaf
            {
                DataType AdoptData = PrevLeaf.Data[PrevLeaf.Num];
                PrevLeaf.Data[PrevLeaf.Num--] = DataType();
                Leaf.LeafInsert(AdoptData);
                for(size_t i = 1; i <= Father.Num; ++i)
                    if(Father.KeyIndex[i] == Leaf.Data[2].first)
                    {
                        Father.KeyIndex[i] = Leaf.Data[1].first;
                        break;
                    }

                // write to file
                File.seekp(GetNodeAddress(LeafIndex));
                File.write(reinterpret_cast<char *>(&Leaf), sizeof(Node));
                File.seekp(GetNodeAddress(PrevLeafIndex));
                File.write(reinterpret_cast<char *>(&PrevLeaf), sizeof(Node));
                File.seekp(GetNodeAddress(FatherIndex));
                File.write(reinterpret_cast<char *>(&Father), sizeof(Node));
            }
            else if(NextLeaf.FatherIndex == Leaf.FatherIndex && NextLeaf.Num > (CAPACITY_LEAF + 1) / 2) // adopt from NextLeaf
            {
                DataType AdoptData = NextLeaf.Data[1];
                for(size_t i = 1; i <= NextLeaf.Num - 1; ++i)
                    NextLeaf.Data[i] = NextLeaf.Data[i + 1];
                NextLeaf.Data[NextLeaf.Num--] = DataType();
                Leaf.Data[++Leaf.Num] = AdoptData;
                for(size_t i = 1; i <= Father.Num; ++i)
                    if(Father.KeyIndex[i] == AdoptData.first)
                    {
                        Father.KeyIndex[i] = NextLeaf.Data[1].first;
                        break;
                    }

                // write to file
                File.seekp(GetNodeAddress(LeafIndex));
                File.write(reinterpret_cast<char *>(&Leaf), sizeof(Node));
                File.seekp(GetNodeAddress(NextLeafIndex));
                File.write(reinterpret_cast<char *>(&NextLeaf), sizeof(Node));
                File.seekp(GetNodeAddress(FatherIndex));
                File.write(reinterpret_cast<char *>(&Father), sizeof(Node));
            }
            else // have to merge
            {
                Key MergeKey = Leaf.Data[1].first;
                if(PrevLeaf.FatherIndex != Leaf.FatherIndex) // Leaf is Father's first Child
                {
                    MergeKey = NextLeaf.Data[1].first;

                    // adopt from NextLeaf
                    DataType AdoptData = NextLeaf.Data[1];
                    for(size_t i = 1; i <= NextLeaf.Num - 1; ++i)
                        NextLeaf.Data[i] = NextLeaf.Data[i + 1];
                    NextLeaf.Data[NextLeaf.Num--] = DataType();
                    Leaf.Data[++Leaf.Num] = AdoptData;

                    // modify Leaf, PrevLeaf, NextLeaf
                    PrevLeafIndex = LeafIndex;
                    PrevLeaf = Leaf;
                    LeafIndex = NextLeafIndex;
                    Leaf = NextLeaf;
                    NextLeafIndex = Father.ChildIndex[2];
                    File.seekg(GetNodeAddress(NextLeafIndex));
                    File.read(reinterpret_cast<char *>(&NextLeaf), sizeof(Node));
                }

                // merge Leaf to PrevLeaf
                for(size_t i = 1; i <= Leaf.Num; ++i)
                    PrevLeaf.Data[PrevLeaf.Num + i] = Leaf.Data[i];
                PrevLeaf.Num += Leaf.Num;

                NextLeaf.PrevLeafIndex = Leaf.PrevLeafIndex;
                PrevLeaf.NextLeafIndex = Leaf.NextLeafIndex;
                Leaf.Reset();
                Leaf.NextEmptyIndex = GetEmptyIndex();
                PutEmptyIndex(LeafIndex);
                File.seekp(GetNodeAddress(LeafIndex));
                File.write(reinterpret_cast<char *>(&Leaf), sizeof(Node));
                File.seekp(GetNodeAddress(PrevLeafIndex));
                File.write(reinterpret_cast<char *>(&PrevLeaf), sizeof(Node));
                File.seekp(GetNodeAddress(NextLeafIndex));
                File.write(reinterpret_cast<char *>(&NextLeaf), sizeof(Node));

                size_t PosIndex;
                Node Pos;
                while(true)
                {
                    Father.InternalErase(MergeKey);
                    if((FatherIndex != GetRootIndex() && Father.Num >= (ORDER - 1) / 2) || (FatherIndex == GetRootIndex() && Father.Num >= 1))
                    {
                        File.seekp(GetNodeAddress(FatherIndex));
                        File.write(reinterpret_cast<char *>(&Father), sizeof(Node));
                        CloseFile();
                        return true;
                    }

                    Pos = Father;
                    PosIndex = FatherIndex;
                    FatherIndex = Pos.FatherIndex;
                    if(FatherIndex == -1) break; // Pos is Root
                    File.seekg(GetNodeAddress(FatherIndex));
                    File.read(reinterpret_cast<char *>(&Father), sizeof(Node));

                    Key FatherKeyLeft = Key(), FatherKeyRight = Key();
                    size_t PrevPosIndex = -1, NextPosIndex = -1;
                    Node PrevPos, NextPos;
                    for(size_t i = 0; i <= Father.Num; ++i)
                        if(Father.ChildIndex[i] == PosIndex)
                        {
                            if(i)
                            {
                                PrevPosIndex = Father.ChildIndex[i - 1];
                                FatherKeyLeft = Father.KeyIndex[i];
                            }
                            if(i != Father.Num)
                            {
                                NextPosIndex = Father.ChildIndex[i + 1];
                                FatherKeyRight = Father.KeyIndex[i + 1];
                            }
                            break;
                        }
                    if(PrevPosIndex != -1)
                    {
                        File.seekg(GetNodeAddress(PrevPosIndex));
                        File.read(reinterpret_cast<char *>(&PrevPos), sizeof(Node));
                    }
                    if(NextPosIndex != -1)
                    {
                        File.seekg(GetNodeAddress(NextPosIndex));
                        File.read(reinterpret_cast<char *>(&NextPos), sizeof(Node));
                    }

                    if(PrevPosIndex != -1 && PrevPos.FatherIndex == Pos.FatherIndex && PrevPos.Num > (ORDER - 1) / 2) // adopt from PrevPos
                    {
                        // adjust PrevPos
                        Key AdoptKey = PrevPos.KeyIndex[PrevPos.Num];
                        size_t AdoptIndex = PrevPos.ChildIndex[PrevPos.Num];
                        PrevPos.KeyIndex[PrevPos.Num] = Key();
                        PrevPos.ChildIndex[PrevPos.Num] = size_t();
                        --PrevPos.Num;

                        // adjust Pos
                        for(size_t i = ++Pos.Num; i >= 1; --i)
                        {
                            Pos.KeyIndex[i] = Pos.KeyIndex[i - 1];
                            Pos.ChildIndex[i] = Pos.ChildIndex[i - 1];
                        }
                        Pos.KeyIndex[1] = FatherKeyLeft;
                        Pos.ChildIndex[0] = AdoptIndex;

                        // adjust Father
                        for(size_t i = 1; i <= Father.Num; ++i)
                            if(Father.KeyIndex[i] == FatherKeyLeft)
                            { Father.KeyIndex[i] = AdoptKey; break; }

                        // adjust FatherIndex of the Node corresponding to AdoptIndex
                        Node Adopt;
                        File.seekg(GetNodeAddress(AdoptIndex));
                        File.read(reinterpret_cast<char *>(&Adopt), sizeof(Node));
                        Adopt.FatherIndex = PosIndex;
                        File.seekp(GetNodeAddress(AdoptIndex));
                        File.write(reinterpret_cast<char *>(&Adopt), sizeof(Node));

                        // write to file
                        File.seekp(GetNodeAddress(PosIndex));
                        File.write(reinterpret_cast<char *>(&Pos), sizeof(Node));
                        File.seekp(GetNodeAddress(PrevPosIndex));
                        File.write(reinterpret_cast<char *>(&PrevPos), sizeof(Node));
                        File.seekp(GetNodeAddress(FatherIndex));
                        File.write(reinterpret_cast<char *>(&Father), sizeof(Node));

                        CloseFile();
                        return true;
                    }
                    else if(NextPosIndex != -1 && NextPos.FatherIndex == Pos.FatherIndex && NextPos.Num > (ORDER - 1) / 2) // adopt from NextPos
                    {
                        // adjust NextPos
                        Key AdoptKey = NextPos.KeyIndex[1];
                        size_t AdoptIndex = NextPos.ChildIndex[0];
                        NextPos.ChildIndex[0] = NextPos.ChildIndex[1];
                        for(size_t i = 1; i <= NextPos.Num - 1; ++i)
                        {
                            NextPos.KeyIndex[i] = NextPos.KeyIndex[i + 1];
                            NextPos.ChildIndex[i] = NextPos.ChildIndex[i + 1];
                        }
                        NextPos.KeyIndex[NextPos.Num] = Key();
                        NextPos.ChildIndex[NextPos.Num] = size_t();
                        --NextPos.Num;

                        // adjust Pos
                        ++Pos.Num;
                        Pos.KeyIndex[Pos.Num] = FatherKeyRight;
                        Pos.ChildIndex[Pos.Num] = AdoptIndex;

                        // adjust Father
                        for(size_t i = 1; i <= Father.Num; ++i)
                            if(Father.KeyIndex[i] == FatherKeyRight)
                            { Father.KeyIndex[i] = AdoptKey; break; }

                        // adjust FatherIndex of the Node corresponding to AdoptIndex
                        Node Adopt;
                        File.seekg(GetNodeAddress(AdoptIndex));
                        File.read(reinterpret_cast<char *>(&Adopt), sizeof(Node));
                        Adopt.FatherIndex = PosIndex;
                        File.seekp(GetNodeAddress(AdoptIndex));
                        File.write(reinterpret_cast<char *>(&Adopt), sizeof(Node));

                        // write to file
                        File.seekp(GetNodeAddress(PosIndex));
                        File.write(reinterpret_cast<char *>(&Pos), sizeof(Node));
                        File.seekp(GetNodeAddress(NextPosIndex));
                        File.write(reinterpret_cast<char *>(&NextPos), sizeof(Node));
                        File.seekp(GetNodeAddress(FatherIndex));
                        File.write(reinterpret_cast<char *>(&Father), sizeof(Node));

                        CloseFile();
                        return true;
                    }
                    else // continue to merge
                    {
                        if(PrevPosIndex != -1 && PrevPos.FatherIndex == Pos.FatherIndex) // merge Pos to PrevPos
                        {
                            // adjust PrevPos
                            MergeKey = FatherKeyLeft;
                            PrevPos.KeyIndex[PrevPos.Num + 1] = FatherKeyLeft;
                            PrevPos.ChildIndex[PrevPos.Num + 1] = Pos.ChildIndex[0];
                            for(size_t i = 1; i <= Pos.Num; ++i)
                            {
                                PrevPos.KeyIndex[PrevPos.Num + 1 + i] = Pos.KeyIndex[i];
                                PrevPos.ChildIndex[PrevPos.Num + 1 + i] = Pos.ChildIndex[i];
                            }

                            PrevPos.Num += Pos.Num + 1;

                            // adjust Father of Pos's Child
                            for(size_t i = 0; i <= Pos.Num; ++i)
                            {
                                size_t TmpIndex = Pos.ChildIndex[i];
                                Node Tmp;
                                File.seekg(GetNodeAddress(TmpIndex));
                                File.read(reinterpret_cast<char *>(&Tmp), sizeof(Node));
                                Tmp.FatherIndex = PrevPosIndex;
                                File.seekp(GetNodeAddress(TmpIndex));
                                File.write(reinterpret_cast<char *>(&Tmp), sizeof(Node));
                            }

                            // adjust Pos
                            Pos.Reset();
                            Pos.NextEmptyIndex = GetEmptyIndex();
                            PutEmptyIndex(PosIndex);

                            // write to file
                            File.seekp(GetNodeAddress(PosIndex));
                            File.write(reinterpret_cast<char *>(&Pos), sizeof(Node));
                            File.seekp(GetNodeAddress(PrevPosIndex));
                            File.write(reinterpret_cast<char *>(&PrevPos), sizeof(Node));
                        }
                        else // merge Pos to NextPos
                        {
                            // adjust NextPos
                            MergeKey = FatherKeyLeft;
                            for(size_t i = NextPos.Num; i >= 1; --i)
                            {
                                NextPos.KeyIndex[Pos.Num + i + 1] = NextPos.KeyIndex[i];
                                NextPos.ChildIndex[Pos.Num + i + 1] = NextPos.ChildIndex[i];
                            }
                            NextPos.KeyIndex[Pos.Num + 1] = FatherKeyRight;
                            NextPos.ChildIndex[Pos.Num + 1] = NextPos.ChildIndex[0];
                            NextPos.ChildIndex[0] = Pos.ChildIndex[0];
                            for(size_t i = 1; i <= Pos.Num; ++i)
                            {
                                NextPos.KeyIndex[i] = Pos.KeyIndex[i];
                                NextPos.ChildIndex[i] = Pos.ChildIndex[i];
                            }
                            NextPos.Num += Pos.Num + 1;

                            // adjust Father of Pos's Child
                            for(size_t i = 0; i <= Pos.Num; ++i)
                            {
                                size_t TmpIndex = Pos.ChildIndex[i];
                                Node Tmp;
                                File.seekg(GetNodeAddress(TmpIndex));
                                File.read(reinterpret_cast<char *>(&Tmp), sizeof(Node));
                                Tmp.FatherIndex = NextPosIndex;
                                File.seekp(GetNodeAddress(TmpIndex));
                                File.write(reinterpret_cast<char *>(&Tmp), sizeof(Node));
                            }

                            // adjust Pos
                            Pos.Reset();
                            Pos.NextEmptyIndex = GetEmptyIndex();
                            PutEmptyIndex(PosIndex);

                            // write to file
                            File.seekp(GetNodeAddress(PosIndex));
                            File.write(reinterpret_cast<char *>(&Pos), sizeof(Node));
                            File.seekp(GetNodeAddress(NextPosIndex));
                            File.write(reinterpret_cast<char *>(&NextPos), sizeof(Node));
                        }
                    }
                }

                /*
                 * delete the root
                 * only run when tree height will decrease
                 */
                size_t ChildIndex = Pos.ChildIndex[0];
                Node Child;
                File.seekg(GetNodeAddress(ChildIndex));
                File.read(reinterpret_cast<char *>(&Child), sizeof(Node));
                Child.FatherIndex = -1;
                PutRootIndex(ChildIndex);
                Pos.Reset();
                Pos.NextEmptyIndex = GetEmptyIndex();
                PutEmptyIndex(PosIndex);
                File.seekp(GetNodeAddress(PosIndex));
                File.write(reinterpret_cast<char *>(&Pos), sizeof(Node));
                File.seekp(GetNodeAddress(ChildIndex));
                File.write(reinterpret_cast<char *>(&Child), sizeof(Node));
            }

            CloseFile();
            return true;
        }

        class iterator
        {
        public:
            friend class BPlusTree;

        private:
            BPlusTree* ptr;
            size_t NodeIndex;
            size_t OrderNum;

        public:
            iterator() :ptr(nullptr), NodeIndex(-1), OrderNum(0) {}
            iterator(BPlusTree* ptr_param, size_t NodeIndex_param, size_t OrderNum_param)
                    :ptr(ptr_param), NodeIndex(NodeIndex_param), OrderNum(OrderNum_param) {}
            iterator(const iterator& other)
                    :ptr(other.ptr), NodeIndex(other.NodeIndex), OrderNum(other.OrderNum) {}

            // modify by iterator
            bool modify(const Value& value)
            {
                Node Leaf = ptr->IterGetNode(NodeIndex);
                Leaf.Data[OrderNum].second = value;
                ptr->IterPutNode(Leaf, NodeIndex);
                return true;
            }

            Key getKey() const
            {
                Node Leaf = ptr->IterGetNode(NodeIndex);
                return Leaf.Data[OrderNum].first;
            }

            Value getValue() const
            {
                Node Leaf = ptr->IterGetNode(NodeIndex);
                return Leaf.Data[OrderNum].second;
            }

            // iter++
            iterator operator++(int)
            {
                if(ptr == nullptr)
                    throw invalid_iterator();
                iterator ret = *this;
                Node Leaf = ptr->IterGetNode(NodeIndex);
                if(OrderNum < Leaf.Num) ++OrderNum;
                else NodeIndex = Leaf.NextLeafIndex, OrderNum = 1;
                return ret;
            }

            // ++iter
            iterator& operator++()
            {
                if(ptr == nullptr)
                    throw invalid_iterator();
                Node Leaf = ptr->IterGetNode(NodeIndex);
                if(OrderNum < Leaf.Num) ++OrderNum;
                else NodeIndex = Leaf.NextLeafIndex, OrderNum = 1;
                return *this;
            }

            // iter--
            iterator operator--(int)
            {
                if(ptr == nullptr)
                    throw invalid_iterator();
                iterator ret = *this;
                Node Leaf = ptr->IterGetNode(NodeIndex);
                if(OrderNum > 1) --OrderNum;
                else
                {
                    Node PrevLeaf = ptr->IterGetNode(Leaf.PrevLeafIndex);
                    NodeIndex = Leaf.PrevLeafIndex, OrderNum = PrevLeaf.Num;
                }
                return ret;
            }

            // --iter
            iterator& operator--()
            {
                if(ptr == nullptr)
                    throw invalid_iterator();
                Node Leaf = ptr->IterGetNode(NodeIndex);
                if(OrderNum > 1) --OrderNum;
                else
                {
                    Node PrevLeaf = ptr->IterGetNode(Leaf.PrevLeafIndex);
                    NodeIndex = Leaf.PrevLeafIndex, OrderNum = PrevLeaf.Num;
                }
                return *this;
            }

            bool operator==(const iterator& rhs) const
            { return ptr == rhs.ptr && NodeIndex == rhs.NodeIndex && OrderNum == rhs.OrderNum; }
            bool operator!=(const iterator& rhs) const
            { return ptr != rhs.ptr || NodeIndex != rhs.NodeIndex || OrderNum != rhs.OrderNum; }
        };

        iterator begin() { return iterator(this, IterGetNode(-2).NextLeafIndex, 1);}
        iterator end() { return iterator(this, -3, 1); }

        iterator find(const Key &key)
        {
            OpenFile();
            size_t LeafIndex = FindLeaf(key);
            Node Leaf;
            File.seekg(GetNodeAddress(LeafIndex));
            File.read(reinterpret_cast<char *>(&Leaf), sizeof(Node));
            for(size_t i = 1; i <= Leaf.Num; ++i)
                if(Leaf.Data[i].first == key)
                {
                    CloseFile();
                    return iterator(this, LeafIndex, i);
                }
            throw runtime_error();
        }

        // return an iterator whose key is the smallest key greater or equal than 'key'
        iterator lower_bound(const Key &key)
        {
            OpenFile();
            size_t LeafIndex = FindLeaf(key);
            Node Leaf;
            File.seekg(GetNodeAddress(LeafIndex));
            File.read(reinterpret_cast<char *>(&Leaf), sizeof(Node));
            for(size_t i = 1; i <= Leaf.Num; ++i)
                if(Leaf.Data[i].first >= key)
                {
                    CloseFile();
                    return iterator(this, LeafIndex, i);
                }
            CloseFile();
            return iterator(this, Leaf.NextLeafIndex, 1);
        }
    };
} // namespace sjtu

#endif //SJTU_BPLUSTREE_HPP