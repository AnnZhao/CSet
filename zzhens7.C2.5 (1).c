/** CS 2506 Fall 2014
 *  Assignment 2:CSet
 *  Student:      Zhenshu Zhao
 *  PID:          zzhens7
 */
#ifndef CSET_H
#define CSET_H
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>



#define FILLER INT32_MIN    // sentinel value for unused array cells

// CSet provides an implementation of a set type for storing a collection of
// signed 32-bit integer values (int32_t).
//
// The implementation imposes the following constraints:
//  - storage is array-based
//  - duplicate elements are not allowed in a CSet
//  - logically empty cells are set to INT32_MIN
//  - sets can contain up to UINT32_MAX elements
//  - unless noted to the contrary, the amortized cost of operations is O(N),
//    where N is the number of elements in the CSet object(s) involved
//  - amortized cost of union and difference is O(N)
//  - cost of checking size and empty tests are O(1)
//  - there are no memory leaks during any of the supported operations
//
// We say a CSet object A is proper if and only if it satisfies each of the 
// following conditions:
//
//  1.  If A.Capacity == 0 then A.Data == NULL.
//  2.  If A.Capacity > 0 then A.Data points to an array of dimension A.Capacity.
//  3.  A.Data[0 : A.Usage-1] are the values stored in the set 
//      (in unspecified order)
//  4.  A.Data[A.Usage : A.Capacity-1] equal INT32_MIN (FILLER)
//
// CSet objects that have not been initialized are said to be raw.
//
// With the sole exception of CSet_Init(), all CSet objects supplied as
// parameters are expected to be proper.
//

struct _CSet {

   uint32_t Capacity;    // dimension of the set's array
   uint32_t Usage;       // number of elements in the set
   int32_t* Data;        // pointer to the set's array
};

typedef struct _CSet CSet;

//helper functions
bool CSet_Contains(const CSet* const pSet, int32_t Value);
bool initialized_Array(int32_t** temp, uint32_t Sz);
int indexFinder(const CSet* const pSet, int32_t Value);
bool arrayExtender(CSet* pSet, int32_t size);
int indexFinderForRemove(const CSet* const pSet, int32_t Value);

/**
 * Initializes a raw pSet object, with capacity Sz.
 *
 * Pre:
 *    pSet points to a CSet object, but *pSet may be proper or raw
 *    Sz   has been initialized
 * Post:  
 *    If successful:
 *       pSet->Capacity == Sz
 *       pSet->Usage == 0
 *       pSet->Data points to an array of dimension Sz, or
 *          is NULL if Sz == 0
 *       if Sz != 0, pSet->Data[0 : Sz-1] == INT32_MIN
 *    else:
 *       pSet->Capacity == 0
 *       pSet->Usage == 0
 *       pSet->Data == NULL
 * Returns:
 *    true if successful, false otherwise
 */
bool CSet_Init(CSet* const pSet, uint32_t Sz){
     if (Sz == 0){
          pSet->Capacity = 0;
          pSet->Usage = 0;
          pSet->Data = NULL;
          return true;
     }
     else{
	if(initialized_Array(&(pSet->Data),Sz) == true){
       
          pSet->Capacity = Sz;
          pSet->Usage = 0;
          return true;
	}
          return false;
        }
     

};


/**
 * Adds Value to a pSet object.
 *
 * Pre:
 *    *pSet is proper
 *    Value has been initialized
 * Post:  
 *    If successful:
 *       Value is a member of *pSet
 *       pSet->Capacity has been increased, if necessary
 *       *pSet is proper
 *    else:
 *       *pSet is unchanged
 * Returns:
 *    true if successful, false otherwise
 */
bool CSet_Insert(CSet* const pSet, int32_t Value){
	if(pSet->Data == NULL){
	return false;
	}
	int a = indexFinder(pSet, Value);
	if(a == -1){
		return false;
	}
	if(pSet->Usage == (pSet->Capacity - 1)){
		bool arr = arrayExtender(pSet, (pSet->Capacity*2));
		if(arr == false){
			return false;
		}
	}
    int32_t b;
    while(a <= pSet->Usage)
    {
        b = pSet->Data[a];
        pSet->Data[a] = Value;
        Value = b;
        a++;
    }
    pSet->Usage++;
    return true;	
		
	
}

/**
 * Removes Value from a pSet object.
 *
 * Pre:
 *    *pSet is proper
 *    Value has been initialized
 * Post:  
 *    If Value was a member of *pSet:
 *       Value is no longer a member of *pSet
 *       pSet->Capacity is unchanged
 *       pSet->Usage is decremented
 *       *pSet is proper
 *    else:
 *       *pSet is unchanged
 * Returns:
 *    true if Value was removed, false otherwise
 */
 bool CSet_Remove(CSet* const pSet, int32_t Value){
	if(pSet->Data == NULL ){
		return false;
	}
	int i = indexFinderForRemove(pSet, Value);
        if(i == -1){
		return false;
	}
	while( i < (pSet->Usage - 1)){
		pSet->Data[i] = pSet->Data[i+1];
		i++;
	}
	pSet->Data[pSet->Usage - 1] = INT32_MIN;
	pSet->Usage--;
	return true;
}

/**
 * Determines if Value belongs to a pSet object.
 *
 * Pre:
 *    *pSet is proper
 *    Value has been initialized
 * Post:  
 *    *pSet is unchanged
 * Returns:
 *    true if Value belongs to *pSet, false otherwise
 */
bool CSet_Contains(const CSet* const pSet, int32_t Value){
	if( pSet->Data == NULL){
		return false;
	}
	int i = indexFinder(pSet, Value);
        if(i == -1)
        {
            return true;
        }
    return false;
}

/**
 * Determines if two CSet objects contain the same elements.
 *
 * Pre:
 *    *pA and *pB are proper
 * Post:  
 *    *pA is unchanged
 *    *pB is unchanged
 * Returns:
 *    true if sets contain same elements, false otherwise
 */
bool CSet_Equals(const CSet* const pA, const CSet* const pB){
	if(pA->Usage != pB->Usage){
		return false;
		}
	else if(!pA->Data && !pB->Data){
		return true;
		}
	else{
		for (int i = 0; i < pA->Usage; i++){
                	if( pA->Data[i] != pB->Data[i] ){
                    	return false;
			}
            	}
            	return true;
	
}
};

/**
 * Determines if one CSet object is a subset of another.
 *
 * Pre:
 *    *pA and *pB are proper
 * Post:  
 *    *pA is unchanged
 *    *pB is unchanged
 * Returns:
 *    true if *pB contains every element of *pA, false otherwise
 */
bool CSet_isSubsetOf(const CSet* const pA, const CSet* const pB){
	if(pB->Data == NULL){
			return false;
		}
		if(pA->Data == NULL){
			return true;
	}
		if(pA->Usage > pB->Usage){
			return false;
		}
		if(pA->Data == NULL && pB->Data == NULL){
			return true;
		}
		if(pA->Usage > pB->Usage)
	    	{
	        	return false;
	    	}
		int i = 0;
		int j = 0;
		while (j < pB->Usage){
			if (pB->Data[j] == pA->Data[i]){
				i++;
			}
			j++;
		}
		return i == pA->Usage;
	}

/**
 * Sets *pUnion to be the union of the sets *pA and *pB.
 *
 * Pre:
 *    *pUnion, *pA and *pB are proper
 *    pUnion != pA and pUnion != pB
 * Post:
 *    *pA and *pB are unchanged
 *    For every integer x, x is contained in *pUnion iff x is contained in
 *       *pA or *pB (or both).
 *    pUnion->Capacity == pA->Capacity + pB->Capacity
 *    pUnion->Usage    == pA->Usage + pB->Usage - number of elements that
 *                        occur in both *pA and *pB
 *    *pUnion is proper
 *
 * Returns:
 *    true if the union is successfully created; false otherwise
 */
bool CSet_Union(CSet* const pUnion, const CSet* const pA, const CSet* const pB){
    bool arr = CSet_Init(pUnion, pA->Capacity+pB->Capacity);

    if(pA->Usage == 0 && pB->Usage == 0 || !arr)
    {
        return false;
    }


    if(!arr)
    {
        return false;
    }
    else
    {
        if(pA->Usage == 0)
        {
            int i;
            for(i = 0; i < pB->Usage; i++)
            {
                CSet_Insert(pUnion, pB->Data[i]);
            }
            return true;
        }
        if(pB->Usage == 0)
        {
            int j;
            for(j = 0; j < pA->Usage; j++)
            {
                CSet_Insert(pUnion, pA->Data[j]);
            }
            return true;
        }
        
        int a = 0;
        int b = 0;
        bool done = false;
        while(!done)
        {
            if(pA->Data[a] > pB->Data[b])
            {
                CSet_Insert(pUnion, pB->Data[b]);
                if(b != pB->Usage - 1)
                {
                    b++;
                }
                else
                {
                    while(a < pA->Usage)
                    {
                        CSet_Insert(pUnion, pA->Data[a]);
                        a++;
                    }
                    done = true;
                }
            }
            else if(pA->Data[a] < pB->Data[b])
            {
                CSet_Insert(pUnion, pA->Data[a]);
                if(a != pA->Usage-1)
                {
                    a++;
                }
                else
                {
                    while(b < pB->Usage)
                    {
                        CSet_Insert(pUnion, pB->Data[b]);
                        b++;
                    }
                    done = true;
                }
            }
            else
            {
                CSet_Insert(pUnion, pA->Data[a]);
                if(a == pA->Usage - 1 && b == pB->Usage - 1)
                {
                    done = true;
                }
                else if(a == pA->Usage - 1 && b != pB->Usage - 1)
                {
                    b++;
                }
                else if(a != pA->Usage - 1 && b == pB->Usage - 1 )
                {
                    a++;
                }
                else 
                {
                    a++;
                    b++;
                }
            }
        }
        return true;
    }


}

/**
 * Sets *pDiff to be the difference of the sets *pA and *pB.
 *
 * Pre:
 *    *pDiff, *pA and *pB are proper
 *    pDiff != pA and pDiff != pB
 * Post:
 *    *pA and *pB are unchanged
 *    For every integer x, x is contained in *pDiff iff x is contained in
 *       *pA but not in *pB.
 *    pDiff->Capacity == pA->Capacity
 *    pDiff->Usage    == pA->Usage - number of elements that
 *                        occur in both *pA and *pB
 *    *pDiff is proper
 *
 * Returns:
 *    true if the difference is successfully created; false otherwise
 */
bool CSet_Difference(CSet* const pDiff, const CSet* const pA, const CSet* const pB){
if (pA->Data == NULL)
    {
        return false;
    }
    bool arr = CSet_Init(pDiff, pA->Capacity);
    
    if(!arr)
    {
        return false;
    }
    else
    {
        if(pB->Data == NULL)
        {
            int i;
            for(i = 0; i < pA->Usage; i++)
            {
                CSet_Insert(pDiff, pA->Data[i]);
            }
            return true;
        }
        int a = 0;
        int b = 0;
        bool done = false;
        while(!done)
        {
            if(pA->Data[a] < pB->Data[b])
            {
                CSet_Insert(pDiff, pA->Data[a]);
                if(a == pA->Usage - 1)
                {
                    done = true;
                }
                else
                {
                    a++;
                }
            }
            else if(pA->Data[a] > pB->Data[b])
            {
                if(b != pB->Usage-1)
                {
                    b++;
                }
                else
                {
                    while (a < pA->Usage)
                    {
                        CSet_Insert(pDiff, pA->Data[a]);
                        a++;
                    }
                    done = true;
                }
            }
            else
            {
                if(a == pA->Usage - 1)
                {
                    done = true;
                }
                else if(b == pB->Usage - 1)
                {
                    a++;
                }
                else
                {
                    a++;
                    b++;
                }
            }
        }
        return true;
    }
}

/**
 *  Determines whether a CSet object is empty.
 *
 *  Pre:
 *     *pSet is proper
 *  Post:
 *     *pSet is unchanged
 *  Returns:
 *     true if pSet->Usage == 0, false otherwise
 */
bool CSet_isEmpty(const CSet* const pSet){
	if(pSet->Usage == 0 || pSet->Data == NULL){
		return true;
	}
	return false;
};

/**
 *  Removes all elements from a CSet object.
 *
 *  Pre:
 *     *pSet is proper
 *  Post:
 *     *pSet contains no elements
 *     pSet->Capacity is unchanged
 *     *pSet is proper
 */
void CSet_makeEmpty(CSet* const pSet){
	free(pSet->Data);
	pSet->Usage = 0;
	pSet->Capacity = 0;
	pSet->Data = NULL;
};

/**
 * Makes a deep copy of a CSet object.
 *
 * Pre:
 *    *pCopy and *pSet are proper
 *    pSet != pCopy
 * Post:  
 *    *pSet is unchanged
 *    If successful:
 *       pCopy->Capacity == pSet->Capacity
 *       pCopy->Usage == pSet->Usage
 *       pCopy[0:pCopy->Capacity] ==  pSet[0:pSet->Capacity]
 *       pCopy->Data != pSet->Data
 *       *pCopy is proper.
 *    else:
 *       *pCopy is unchanged
 * Returns:
 *    true if successful, false otherwise
 */
bool CSet_Copy(CSet* const pTarget, const CSet* const pSource){
	if(pTarget->Capacity != pSource->Capacity){
		free(pTarget->Data);
		if(!CSet_Init(pTarget, pSource->Capacity)){
			return false;
		}
	}
	int temp = 0;
	while( temp< pSource->Usage){
		pTarget->Data[temp] = pSource->Data[temp];
		temp++;
	}
	pTarget->Usage = pSource->Usage;
	pTarget->Capacity = pSource->Capacity;
	return true;
};

bool initialized_Array(int32_t** temp, uint32_t Sz){
	int32_t* arr;
	 arr =(int32_t*) malloc( sizeof(int32_t) * Sz);
          if(arr!=NULL){
		int i = 0;
		while( i < Sz ){
			arr[i] = INT32_MIN;
			i++;
		}
		*temp = arr;
                return true;}
	return false;

}

bool arrayExtender(CSet* pSet, int32_t size){
	int32_t* temp = realloc(pSet->Data, (sizeof(int32_t) * size));
	if(!temp){
		return false;
	}
	int i = pSet->Usage;
	while(i < size){
		temp[i] = INT32_MAX;
		i++;
	}
	pSet->Data = temp;
	pSet->Capacity = (pSet->Capacity * 2);
	return true;
}

int indexFinder(const CSet* const pSet, int32_t Value){
	int tail = pSet->Usage;
	int head = 0;
	int i;
	while(head <= tail){
		if(pSet->Data[head] == INT32_MIN)
        {
            break;
        }
        if(pSet->Data[head] == Value )
        {
            head = -1;
            break;
        }
        else if(pSet->Data[head] > Value)
        {
            break;
        }
        else 
        {
            head++;
        }
    }
    i = head;
    return i;
}

int indexFinderForRemove(const CSet* const pSet, int32_t Value){
    int head = 0;
    int tail = pSet->Usage;
    int i;
    while(head <= tail)
    {
        if(pSet->Data[head] == INT32_MIN)
        {
            head = -1;  
            break;
        }
        else if(pSet->Data[head] == Value )
        {
            break;
        }
        else 
        {
            head++;
        }
    }
    i = head;
    return i;
}
#endif
// On my honor: 
// 
// - I have not discussed the C language code in my program with 
// anyone other than my instructor or the teaching assistants 
// assigned to this course. 
// 
// - I have not used C language code obtained from another student, 
// or any other unauthorized source, either modified or unmodified. 
// 
// - If any C language code or documentation used in my program 
// was obtained from another source, such as a text book or course 
// notes, that has been clearly noted with a proper citation in 
// the comments of my program. 
// 
// - I have not designed this program in such a way as to defeat or 
// interfere with the normal operation of the Curator System. 
// 
// <Zhenshu Zhao>
