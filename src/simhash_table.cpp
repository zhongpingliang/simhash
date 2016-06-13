#include "simhash_table.h"

#include <map>
#include <set>
#include <algorithm>
#include <cstdio>
#include <fstream>

#include "simhash.h"

namespace simhash
{
/*
 * class SimhashContainer
 */
class SimhashContainer
{
public :
    virtual ~SimhashContainer();
protected :
    SimhashContainer(uint_t maxHamDist, uint_t level);
    SimhashContainer(const SimhashContainer &another);
    SimhashContainer& operator= (const SimhashContainer &another);
public :
    virtual bool Insert         (hash_t hash)                       = 0;
    virtual bool Remove         (hash_t hash)                       = 0;
    virtual bool Search         (hash_t hash)                       = 0;
    virtual bool HasNearDups    (hash_t hash, hash_t mask = 0UL)    = 0;
    virtual bool FindFirstNearDup(hash_t hash, hash_t &nearDup,
        hash_t mask = 0UL)      = 0;
    virtual bool FindNearDups   (hash_t hash, FindAnswerType &ans,
        hash_t mask = 0UL)      = 0;
    virtual void    Clear()     = 0;
    virtual uint_t  GetSize()   = 0;
    virtual bool SaveToFile(const std::string &filename, bool binary) = 0;
protected:
    uint_t mMaxHamDist;
    uint_t mLevel;
};
typedef std::tr1::shared_ptr<SimhashContainer> SimhashContainerPtr;

/*
* class SimhashContainerFactory
*/
class SimhashContainerFactory
{
public:
    static SimhashContainerPtr CreateSimhashContainer(uint_t maxHamDist,
        uint_t level, uint_t maskBeginPos = 0U, uint_t maskEndPos = HASH_WIDTH);
};


/*
* class SimhashSequentialContainner
*/
class SimhashSequentialContainner : public SimhashContainer
{
public:
    typedef std::set<hash_t> ContainerType;
public:
    virtual ~SimhashSequentialContainner();
protected :
    SimhashSequentialContainner(uint_t maxHamDist, uint_t level);
    SimhashSequentialContainner(const SimhashSequentialContainner &another);
    SimhashSequentialContainner& operator= (const SimhashSequentialContainner
        &another);
public:
    virtual bool Insert         (hash_t hash);
    virtual bool Remove         (hash_t hash);
    virtual bool Search         (hash_t hash);
    virtual bool HasNearDups    (hash_t hash, hash_t mask = 0UL);
    virtual bool FindFirstNearDup(hash_t hash, hash_t &nearDup,
        hash_t mask = 0UL);
    virtual bool FindNearDups   (hash_t hash, FindAnswerType &ans,
        hash_t mask = 0UL);
    virtual void    Clear();
    virtual uint_t  GetSize();
    virtual bool SaveToFile(const std::string &filename, bool binary);
protected:
    ContainerType mContainer;
    friend class SimhashContainerFactory;
};

/*
* class SimhashIndexedContainer
*/
class SimhashIndexedContainer : public SimhashContainer
{
protected:
    /*
    * This struct helps permute.
    * Suppose that we want to permute:
    *      x = X | AA | BBB | CCCC | DDDDD
    * to :
    *      y = X | BBB | AA | CCCC | DDDDD
    * Than the valus of props is list bellow:
    *     leftForwardMask      =
    *          0 | 11 | 000 | 0000 | 00000
    *     rightForwardMask     =
    *          0 | 00 | 111 | 0000 | 00000
    *     leftBackwardMask     =
    *          0 | 111 | 00 | 0000 | 00000
    *     rightBackwardMask    =
    *          0 | 000 | 11 | 0000 | 00000
    *     surroundMask         =
    *          1 | 00 | 000 | 1111 | 11111
    *     leftWidth            = 2
    *     rightWidth           = 3
    */
    typedef struct _SingleContainerProps
    {
        hash_t leftForwardMask;
        hash_t rightForwardMask;
        hash_t leftBackwardMask;
        hash_t rightBackwardMask;   //also the index mask
        hash_t surroundMask;
        uint_t leftWidth;
        uint_t rightWidth;
    } SingleContainerProps;
public:
    typedef std::vector<SimhashContainerPtr> ContainerType;
    typedef std::vector<SingleContainerProps> PropsType;
public:
    virtual ~SimhashIndexedContainer();
protected:
    SimhashIndexedContainer(uint_t maxHamDist, uint_t level,
        uint_t maskBeginPos, uint_t maskEndPos);
    SimhashIndexedContainer(const SimhashIndexedContainer &another);
    SimhashIndexedContainer& operator= (const SimhashIndexedContainer
        &another);
public:
    virtual bool Insert         (hash_t hash);
    virtual bool Remove         (hash_t hash);
    virtual bool Search         (hash_t hash);
    virtual bool HasNearDups    (hash_t hash, hash_t mask = 0UL);
    virtual bool FindFirstNearDup(hash_t hash, hash_t &nearDup,
        hash_t mask = 0UL);
    virtual bool FindNearDups   (hash_t hash, FindAnswerType &ans,
        hash_t mask = 0UL);
    virtual void    Clear();
    virtual uint_t  GetSize();
    virtual bool SaveToFile(const std::string &filename, bool binary);
protected:
    bool Init();
    void GetForwardPermutes(hash_t hash, std::vector<hash_t> &ans);

    inline hash_t ForwardPermute(hash_t hash, const SingleContainerProps &props)
    {
        //Generate forward permutes.
        /*
        * Suppose that we want to forward permute:
        *      x = X | A | B | C | D
        * to :
        *      y = X | B | A | C | D
        * This can be achieved be the following steps:
        *   1. Using leftForwardMask to x, we get:
        *      i = 0 | A | 0 | 0 | 0
        *   2. Using rightForwardMask to x, we get
        *      j = 0 | 0 | B | 0 | 0
        *   3. Using surroundMask to x, we get
        *      k = X | 0 | 0 | C | D
        *   4. Get y by expression:
        *      y = i >> rightWidth | j << leftWidth | k
        */
        return ((hash & props.leftForwardMask) >> props.rightWidth)
            | ((hash & props.rightForwardMask) << props.leftWidth)
            | (hash & props.surroundMask);
    }
    inline hash_t BackwardPermute(hash_t hash, const SingleContainerProps 
        &props)
    {
        //Generate forward permutes.
        /*
        * Suppose that we want to backward permute:
        *      x = X | B | A | C | D
        * to :
        *      y = X | A | B | C | D
        * This can be achieved be the following steps:
        *   1. Using leftBackwardMask to x, we get:
        *      i = 0 | B | 0 | 0 | 0
        *   2. Using rightBackwardMask to x, we get
        *      j = 0 | 0 | A | 0 | 0
        *   3. Using surroundMask to x, we get
        *      k = X | 0 | 0 | C | D
        *   4. Get y by expression:
        *      y = i >> leftWidth | j << rightWidth | k
        */
        return ((hash & props.leftBackwardMask) >> props.leftWidth)
            | ((hash & props.rightBackwardMask) << props.rightWidth)
            | (hash & props.surroundMask);
    }
protected:
    uint_t mBlockNum;           // The number of blocks, equals mMaxHammDist + 1
    ContainerType  mContainer;  // The containers.
    PropsType mProps;           // The property for each single container
    /*
    * The positions where mask bits begin and end, for example :
    *      x = 0 | 0 | 1 | 1 | 0 | 0 | 0 | 0
    * in this case :
    *   mMaskBeginPos = 5, mMaskEndPos = 7
    */
    uint_t mMaskBeginPos;
    uint_t mMaskEndPos;

    friend class SimhashContainerFactory;
};

SimhashContainer::SimhashContainer(uint_t maxHamDist, uint_t level)
    : mMaxHamDist(maxHamDist)
    , mLevel(level)
{}

SimhashContainer::~SimhashContainer()
{}

SimhashSequentialContainner::SimhashSequentialContainner(uint_t maxHamDist,
    uint_t level)
    : SimhashContainer(maxHamDist, level)
{}

SimhashSequentialContainner::~SimhashSequentialContainner()
{}

void SimhashSequentialContainner::Clear()
{
    mContainer.clear();
}

uint_t SimhashSequentialContainner::GetSize()
{
    return static_cast<uint_t>(mContainer.size());
}

bool SimhashSequentialContainner::Insert(hash_t hash)
{
    return mContainer.insert(hash).second;
}
bool SimhashSequentialContainner::Remove(hash_t hash)
{
    return 0 != mContainer.erase(hash);
}

bool SimhashSequentialContainner::Search(hash_t hash)
{
    return mContainer.end() != mContainer.find(hash);
}

bool SimhashSequentialContainner::HasNearDups(hash_t hash, hash_t mask)
{
    hash_t tmp;
    return FindFirstNearDup(hash, tmp, mask);
}

bool SimhashSequentialContainner::FindFirstNearDup(hash_t hash, hash_t &nearDup,
    hash_t mask)
{
    const ContainerType::iterator lower = mContainer.lower_bound(hash &  mask );
    const ContainerType::iterator upper = mContainer.upper_bound(hash |(~mask));
    for (ContainerType::iterator it = lower; upper != it; ++it)
    {
        if (Simhash::IsNearDups(hash, *it, mMaxHamDist))
        {
            nearDup = *it;
            return true;
        }
    }
    return false;
}

bool SimhashSequentialContainner::FindNearDups(hash_t hash, FindAnswerType &ans,
    hash_t mask)
{
    ans.clear();
    const ContainerType::iterator lower = mContainer.lower_bound(hash &  mask );
    const ContainerType::iterator upper = mContainer.upper_bound(hash |(~mask));
    for (ContainerType::iterator it = lower; upper != it; ++it)
    {
        if (Simhash::IsNearDups(hash, *it, mMaxHamDist))
        {
            ans.push_back(*it);
        }
    }
    return !ans.empty();
}

bool SimhashSequentialContainner::SaveToFile(const std::string &filename,
    bool binary)
{
    std::ofstream fout(filename.c_str(),
        std::fstream::out | std::fstream::binary);
    if (!fout.good())
    {
        return false;
    }
    if (binary)     //save in binary mode.
    {
        static const uint_t BYTES = static_cast<uint_t>(sizeof(hash_t));
        static const uint_t BUFF_SIZE = 10000U;
        hash_t buff[BUFF_SIZE];
        uint_t count = 0U;
        for (ContainerType::iterator it = mContainer.begin();
            mContainer.end() != it; ++it)
        {
            buff[count++] = *it;
            if (BUFF_SIZE == count)
            {
                fout.write(reinterpret_cast<char*>(buff), BYTES * BUFF_SIZE);
                count = 0U;
            }
        }
        if (count > 0)
        {
            fout.write(reinterpret_cast<char*>(buff), BYTES * count);
        }
    }
    else            //save in string mode.
    {
        std::string binaryStr;
        for (ContainerType::iterator it = mContainer.begin();
            mContainer.end() != it; ++it)
        {
            Simhash::HashToBinaryString(*it, binaryStr);
            fout << binaryStr << std::endl;
        }
    }
    fout.close();
    return true;
}

SimhashIndexedContainer::SimhashIndexedContainer(uint_t maxHamDist,
    uint_t level,uint_t maskBeginPos, uint_t maskEndPos)
    : SimhashContainer(maxHamDist, level)
    , mBlockNum(        maxHamDist + 1U )
    , mContainer(       maxHamDist + 1U )
    , mProps(           maxHamDist + 1U )
    , mMaskBeginPos(    maskBeginPos    )
    , mMaskEndPos(      maskEndPos      )
{
    bool ret = Init();
    if (!ret)
    {
        throw std::bad_alloc();
    }
}

SimhashIndexedContainer::~SimhashIndexedContainer()
{}

bool SimhashIndexedContainer::Init()
{
    uint_t maskWidth = mMaskEndPos - mMaskBeginPos;
    uint_t blockWidth = maskWidth / mBlockNum;
    uint_t remainLen = maskWidth - blockWidth * mBlockNum;
    maskWidth = mMaskEndPos;
    for (uint_t i = 0; i < mBlockNum; ++i)
    {
        //Set props.
        if (0 == i)
        {
            mProps.at(i).leftWidth = 0U;
            mProps.at(i).leftForwardMask = 0UL;
        }
        else
        {
            mProps.at(i).leftWidth = mProps.at(i - 1U).leftWidth
                + mProps.at(i - 1U).rightWidth;
            mProps.at(i).leftForwardMask = mProps.at(i - 1U).leftForwardMask
                | mProps.at(i - 1U).rightForwardMask;
        }
        if (remainLen > 0)
        {
            mProps.at(i).rightWidth = blockWidth + 1U;
            --remainLen;
        }
        else
        {
            mProps.at(i).rightWidth = blockWidth;
        }
        mProps.at(i).rightForwardMask = 0UL;
        for (uint_t j = maskWidth - mProps.at(i).rightWidth; j < maskWidth; ++j)
        {
            mProps.at(i).rightForwardMask |= HASH_1 << j;
        }
        maskWidth -= mProps.at(i).rightWidth;
        mProps.at(i).leftBackwardMask
            = mProps.at(i).rightForwardMask << mProps.at(i).leftWidth;
        mProps.at(i).rightBackwardMask
            = mProps.at(i).leftForwardMask  >> mProps.at(i).rightWidth;
        mProps.at(i).surroundMask
            = ~(mProps.at(i).leftForwardMask | mProps.at(i).rightForwardMask);

        //Create container.
        mContainer.at(i) = SimhashContainerFactory::CreateSimhashContainer(
            mMaxHamDist, mLevel - 1U, 0U,
            mMaskEndPos - mProps.at(i).rightWidth);
        if (!mContainer.at(i))
        {
            throw std::bad_alloc();
            return false;
        }
    }
    return true;
}

void SimhashIndexedContainer::Clear()
{
    for (ContainerType::iterator it = mContainer.begin();
        it != mContainer.end(); ++it)
    {
        (*it)->Clear();
    }
}

uint_t SimhashIndexedContainer::GetSize()
{
    return static_cast<uint_t>(mContainer.front()->GetSize());
}

bool SimhashIndexedContainer::Insert(hash_t hash)
{
    //Attempt insert hash into the front container.
    if (!mContainer.front()->Insert(hash))
    {
        return false;
    }
    //Insert into all redundancy containers.
    //Generate forward permutes.
    std::vector<hash_t> forwordPermutes;
    GetForwardPermutes(hash, forwordPermutes);
    //Insert hash into all redundancy containers.
    for (uint_t i = 1U; i < mBlockNum; ++i)
    {
        //Should return true.
        mContainer.at(i)->Insert(forwordPermutes.at(i));
    }
    return true;
}

bool SimhashIndexedContainer::Remove(hash_t hash)
{
    //Attempt remove hash from the front container.
    if (!mContainer.front()->Remove(hash))
    {
        return false;
    }
    //Remove from all redundancy containers.
    //Generate forward permutes.
    std::vector<hash_t> forwordPermutes;
    GetForwardPermutes(hash, forwordPermutes);
    for (uint_t i = 1U; i < mBlockNum; ++i)
    {
        //Should return true.
        mContainer.at(i)->Remove(forwordPermutes.at(i));
    }
    return true;
}

bool SimhashIndexedContainer::Search(hash_t hash)
{
    //Find hash from the front container.
    return mContainer.front()->Search(hash);
}

bool SimhashIndexedContainer::FindNearDups(hash_t hash, FindAnswerType &ans,
    hash_t mask)
{
    //Init.
    ans.clear();
    //Generate forward permutes.
    std::vector<hash_t> forwordPermutes;
    GetForwardPermutes(hash, forwordPermutes);
    //Find hash from all redundancy containers.
    FindAnswerType subAns;
    for (uint_t i = 0; i < mBlockNum; ++i)
    {
        mContainer.at(i)->FindNearDups(forwordPermutes.at(i), subAns,
            mProps.at(i).leftBackwardMask | mask);
        for (FindAnswerType::iterator iter = subAns.begin();
            subAns.end() != iter; ++iter)
        {
            ans.push_back(BackwardPermute(*iter, mProps.at(i)));
        }
    }
    return !ans.empty();
}

bool SimhashIndexedContainer::HasNearDups(hash_t hash, hash_t mask)
{
    hash_t tmp;
    return FindFirstNearDup(hash, tmp, mask);
}

bool SimhashIndexedContainer::FindFirstNearDup(hash_t hash, hash_t &nearDup,
    hash_t mask)
{
    //Generate forward permutes.
    std::vector<hash_t> forwordPermutes;
    GetForwardPermutes(hash, forwordPermutes);
    //Find hash from all redundancy containers.
    for (uint_t i = 0; i < mBlockNum; ++i)
    {
        if (mContainer.at(i)->FindFirstNearDup(hash, nearDup,
            mProps.at(i).leftBackwardMask | mask))
        {
            return true;
        }
    }
    return false;
}

bool SimhashIndexedContainer::SaveToFile(const std::string &filename,
    bool binary)
{
    return mContainer.front()->SaveToFile(filename, binary);
}

void SimhashIndexedContainer::GetForwardPermutes(hash_t hash,
    std::vector<hash_t> &ans)
{
    //Init.
    ans.resize(mBlockNum);
    //Generate forward permutes.
    for (uint_t i = 0; i < mBlockNum; ++i)
    {
        ans.at(i) = ForwardPermute(hash, mProps.at(i));
    }
}

SimhashContainerPtr SimhashContainerFactory::CreateSimhashContainer(
        uint_t maxHamDist, uint_t level, uint_t maskBeginPos, uint_t maskEndPos)
{
    if (!level)
    {
        return SimhashContainerPtr(new SimhashSequentialContainner(
            maxHamDist, level));
    }
    else
    {
        return SimhashContainerPtr(new SimhashIndexedContainer(
            maxHamDist, level, maskBeginPos, maskEndPos));
    }
}

SimhashTable::SimhashTable()
{}

SimhashTable::~SimhashTable()
{}

class SimhashTableImpl : public SimhashTable
{
private:
    SimhashTableImpl(uint_t maxHamDist, uint_t level);
    SimhashTableImpl(const SimhashTableImpl&);
    SimhashTableImpl& operator=(const SimhashTableImpl&);
public :
    virtual ~SimhashTableImpl();
public : 
    virtual bool Insert         (hash_t hash);
    virtual bool Remove         (hash_t hash);
    virtual bool Search         (hash_t hash);
    virtual bool HasNearDups    (hash_t hash);
    virtual bool FindFirstNearDup(hash_t hash, hash_t &nearDup);
    virtual bool FindNearDups   (hash_t hash, FindAnswerType &ans);
    virtual void Clear();
    virtual uint_t GetSize();
    virtual bool SaveToFile     (const std::string &filename, bool binary);
    virtual bool LoadFromFile   (const std::string &filename, bool binary);
private :
    SimhashContainerPtr mContainerPtr;
    friend SimhashTablePtr CreateSimhashTable(uint_t maxHamDist, uint_t level);
};

SimhashTableImpl::SimhashTableImpl(uint_t maxHamDist, uint_t level)
{
    mContainerPtr = SimhashContainerFactory::CreateSimhashContainer(maxHamDist,
        level, 0U, HASH_WIDTH);
}

SimhashTableImpl::~SimhashTableImpl()
{}

void SimhashTableImpl::Clear()
{
    return mContainerPtr->Clear();
}

uint_t SimhashTableImpl::GetSize()
{
    return mContainerPtr->GetSize();
}

bool SimhashTableImpl::Insert(hash_t hash)
{
    return mContainerPtr->Insert(hash);
}

bool SimhashTableImpl::Remove(hash_t hash)
{
    return mContainerPtr->Remove(hash);
}

bool SimhashTableImpl::Search(hash_t hash)
{
    return mContainerPtr->Search(hash);
}

bool SimhashTableImpl::HasNearDups(hash_t hash)
{
    return mContainerPtr->HasNearDups(hash);
}

bool SimhashTableImpl::FindFirstNearDup(hash_t hash, hash_t &nearDup)
{
    return mContainerPtr->FindFirstNearDup(hash, nearDup);
}

bool SimhashTableImpl::FindNearDups(hash_t hash, FindAnswerType &ans)
{
    bool ret = mContainerPtr->FindNearDups(hash, ans);
    std::sort(ans.begin(), ans.end());
    ans.resize(std::distance(ans.begin(), std::unique(ans.begin(), ans.end())));
    return ret;
}


bool SimhashTableImpl::SaveToFile(const std::string &filename, bool binary)
{
    return mContainerPtr->SaveToFile(filename, binary);
}

bool SimhashTableImpl::LoadFromFile(const std::string &filename, bool binary)
{
    std::ifstream fin(filename.c_str(),
        std::fstream::in | std::fstream::binary);
    if (!fin.good())
    {
        return false;
    }
    mContainerPtr->Clear();
    if (binary)     //load in binary mode.
    {
        static const uint_t BYTES = static_cast<uint_t>(sizeof(hash_t));
        static const uint_t BUFF_SIZE = 10000U;
        hash_t buff[BUFF_SIZE];
        uint_t count = 0U;
        do
        {
            fin.read(reinterpret_cast<char *>(buff), BYTES * BUFF_SIZE);
            count = static_cast<uint_t>(fin.gcount()) / BYTES;
            for (uint_t i = 0; i < count; ++i)
            {
                mContainerPtr->Insert(buff[i]);
            }
        } while (count);
    }
    else            //load in string mode.
    {
        std::string binaryStr;
        while (std::getline(fin, binaryStr))
        {
            if (!binaryStr.empty())
            {
                mContainerPtr->Insert(Simhash::BinaryStringToHash(binaryStr));
            }
        }
    }
    fin.close();
    return true;
}

SimhashTablePtr CreateSimhashTable(uint_t maxHamDist, uint_t level)
{
    return SimhashTablePtr(new SimhashTableImpl(maxHamDist, level));
}

} // namespace simhash
