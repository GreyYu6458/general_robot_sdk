#ifndef _DETERMINISTIC_FINITE_AUTOMATON_
#define _DETERMINISTIC_FINITE_AUTOMATON_

// Deterministic finite automaton description

#include <unordered_set>

/**
 * @brief DFA 确定性的有限状态自动机 的程序化 描述
 * 一般 DFA可以使用一个5元组进行描述 (K, ∑, F, S, Z) :
 * K : 有穷状态的集合
 * SIGMA (∑) :  有穷字母表
 * F : 状态转移函数
 * S : 状态的初态， 有且只有一个，S ∈ K
 * Z : 最终状态的集合， Z ∈ K
 */

// https://blog.csdn.net/u011630575/article/details/80837629    boost graph 模块的使用
// https://blog.csdn.net/qq_24451605/article/details/49795551   DFA NFA 确定化, 对确定化的解释不多

struct DFA
{
public:

};



#endif