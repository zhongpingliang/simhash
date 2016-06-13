/*==============================================================================
 *   Copyright (C) 2016 All rights reserved.
 *
 *  File Name    : simhash_table.h
 *  Author       : Zhongping Liang
 *  Date         : 2016-05-19
 *  Version      : 1.0
 *  Description  : This file provides declaration of the SimhashTable.
 * ============================================================================*/

#ifndef SIMHASH_SIMHASH_TABLE_H_
#define SIMHASH_SIMHASH_TABLE_H_

#include <tr1/memory>   //for shared_ptr
#include <vector>
#include <string>

#include "common.h"

namespace simhash
{

// type of FindNearDups result
typedef std::vector<hash_t> FindAnswerType;

/*
* class SimhashTable.
* SimhashTable is a container of simhash values. It provides several methods to
* access the table. User can insert a simhash value into the table; remove a
* simhash value from the table; search a simhash value from the table; find all
* near-duplicates from the table and judge if a given simhash value has any near
* -duplicates in the table.
* Let's review the simhash problem. Suppose that the the max Hamming distance
* is mMaxHamDist. Two simhash values (x, y) are near-duplicates if and only if
* the Hamming distance between them is not greater than mMaxHamDist. If the bit
* number of x and y is HASH_WIDTH, and we split x and y into mBlockNum =
* mMaxHamDist + 1 blocks, and x and y are near-duplicates, at least one of the
* mBlockNum blocks is the same, and at most mMaxHamDist of the mBlockNum blocks
* is different. So when we choose one of the mBlockNum blocks as key, we can
* build a indexed container, so that the time complex can be less. But the space
* will be large, because we must build a container for each key, in this case,
* the space will be mBlockNum times.
*
* As an example, mMaxHamDist=5, and HASH_WIDTH=64, so mBlockNum = 6, when x is :
*     |63---53|52---42|41---31|30---20|19---10|09---00|
*     |   A   |   B   |   C   |   D   |   E   |   F   |

* x can be permuted to 6 permutes:
*     |63---53|52---42|41---31|30---20|19---10|09---00|
*     |   A   |   B   |   C   |   D   |   E   |   F   |
*
*     |63---53|52---42|41---31|30---20|19---10|09---00|
*     |   B   |   A   |   C   |   D   |   E   |   F   |
*
*     |63---53|52---42|41---31|30---20|19---10|09---00|
*     |   C   |   A   |   B   |   D   |   E   |   F   |
*
*     |63---53|52---42|41---31|30---20|19---10|09---00|
*     |   D   |   A   |   B   |   C   |   E   |   F   |
*
*     |63---54|53---43|42---32|31---21|20---10|09---00|
*     |   E   |   A   |   B   |   C   |   D   |   F   |
*
*     |63---54|53---43|42---32|31---21|20---10|09---00|
*     |   F   |   A   |   B   |   C   |   D   |   E   |
*
* If the Hamming distance of x and y is mMaxHamDist, we know that the
* mMaxHamDist different bits will locate in no more than mMaxHamDist blocks.
* Again, we can merge the mMaxHamDist blocks as a whole, and split it into new
* mBlockNum blocks, and we can build a secondary indexed container. The time
* complex is reduced again, but the space will be mBlockNum * mBlockNum times.
*
* The func CreateSimhashTable takes two parameters, one is maxHamDist, another
* is level. The meaning of level is :
*       0 - don't use index
*       1 - use one level index
*       2 - use two levels index.
*       ...
* Note that, when level is higher, the space will have a exponential growth, the
* suggested value is 1 or 2.
*/
class SimhashTable
{
//constructors
public :
    virtual ~SimhashTable();
protected :
    SimhashTable();
private :
    SimhashTable(const SimhashTable &another);
    SimhashTable& operator=(const SimhashTable &another);
//public functions
public:
    /*
    *   @brief      This func inserts a simhash value into table.
    *   @author     Zhongping Liang
    *   @date       2016-05-19
    *   @param      hash: the input simhash value to be inserted.
    *   @return     false, if hash is already in the table; true, otherwise.
    */
    virtual bool Insert         (hash_t hash) = 0;
    /*
    *   @brief      This func removes a simhash value from table.
    *   @author     Zhongping Liang
    *   @date       2016-05-19
    *   @param      hash: the input simhash value to be removed.
    *   @return     false, if hash is not in the table; true, otherwise.
    */
    virtual bool Remove         (hash_t hash) = 0;
    /*
    *   @brief      This func searches a simhash value from table. The condition
    *           of this func is equal, but not is near-duplicate.
    *   @author     Zhongping Liang
    *   @date       2016-05-19
    *   @param      hash: the input simhash value to be searched.
    *   @return     false, if hash is not in the table; true, otherwise.
    */
    virtual bool Search         (hash_t hash) = 0;
    /*
    *   @brief      This func judges whether a simhash value has any near-
    *           duplicate in table.
    *   @author     Zhongping Liang
    *   @date       2016-05-19
    *   @param      hash: the input simhash value.
    *   @return     true, if there is some near-duplicates; false, otherwise.
    */
    virtual bool HasNearDups    (hash_t hash) = 0;
    /*
    *   @brief      This func judges whether a simhash value has any near
    *           duplicate in table, if true, the near duplicate first found
    *           is saved in nearDup.
    *   @author     Zhongping Liang
    *   @date       2016-05-19
    *   @param      hash: the input simhash value.
    *   @param      nearDup: the output near duplicate value.
    *   @return     true, if there is some near-duplicates; false, otherwise.
    */
    virtual bool FindFirstNearDup(hash_t hash, hash_t &nearDup) = 0;
    /*
    *   @brief      This func finds all simhash values from table that is near-
    *           duplicate with the given simhash value.
    *   @author     Zhongping Liang
    *   @date       2016-05-19
    *   @param      hash: the input simhash value.
    *   @param      ans : the simhash values found.
    *   @return     true, if there is some near-duplicates; false, otherwise.
    */
    virtual bool FindNearDups   (hash_t hash, FindAnswerType &ans) = 0;
    /*
    *   @brief      This func clears table.
    *   @author     Zhongping Liang
    *   @date       2016-05-19
    *   @return     void.
    */
    virtual void Clear() = 0;
    /*
    *   @brief      This func returns the size of table.
    *   @author     Zhongping Liang
    *   @date       2016-05-19
    *   @return     void.
    */
    virtual uint_t GetSize() = 0;
    /*
    *   @brief      This func saves all simhash values into file.
    *   @author     Zhongping Liang
    *   @date       2016-05-19
    *   @param      filename: the output filename.
    *   @param      binary  : if true, save to file in binary mode; otherwise
    *           save to file in string mode.
    *   @return     true, if success; false, otherwise.
    */
    virtual bool SaveToFile(const std::string &filename, bool binary = true)
        = 0;
    /*
    *   @brief      This func loads all simhash values from file.
    *   @author     Zhongping Liang
    *   @date       2016-05-19
    *   @param      filename: the input filename.
    *   @param      binary  : if true, load from file in binary mode; otherwise
    *           load from file in string mode.
    *   @return     true, if success; false, otherwise.
    */
    virtual bool LoadFromFile(const std::string &filename, bool binary = true)
        = 0;
};

typedef std::tr1::shared_ptr<SimhashTable> SimhashTablePtr;

/*
*   @brief      This func creates a SimhashTable instance.
*   @author     Zhongping Liang
*   @date       2016-05-19
*   @param      maxHamDist  : the max Hamming distance can be tolerated.
*   @param      level       : the index level. value means:
*                   0 - don't use index
*                   1 - use one level index
*                   2 - use two levels index.
*                   ...
*   @return     SimhashTable instance.
*/
SimhashTablePtr CreateSimhashTable(uint_t maxHamDist = 3U, uint_t level = 1U);

} // namespace simhash

#endif // SIMHASH_SIMHASH_TABLE_H_
