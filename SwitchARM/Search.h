/*
 * Search.h
 *
 *  Created on: 2014��6��10��
 *      Author: Administrator
 */

#ifndef SEARCH_H_
#define SEARCH_H_

extern SearchDefine *Search;
extern void InitSearch();

extern cJSON * CreateAlivePacket(const char *departmentID, const char *switchgearID, const char *aliveMessage);
extern void thread_CommonSearch(const SearchDefine *sd);

#endif /* SEARCH_H_ */
