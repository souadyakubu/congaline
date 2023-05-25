/*
 * List.h
 *      Author: vtn2
 */

#ifndef LIST_H_
#define LIST_H_

#include <istream>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cassert>
using namespace std;

template <class Item>
class List
{
public:
	List();
	List(const List<Item> &original);
	virtual ~List();
	unsigned getSize() const { return mySize; }
	Item getFirst() const;
	Item getLast() const;
	void append(const Item &it);
	List<Item> &operator=(const List<Item> &original);
	bool operator==(const List<Item> &other) const;
	bool operator!=(const List<Item> &other) const { return (!(*this == other)); }
	void readFrom(istream &in);
	void readFrom(const string &filename);
	void writeTo(ostream &out, char separator = ' ') const;
	void writeTo(const string &filename, char separator = ' ') const;
	void prepend(const Item &it);
	int getIndexOf(const Item &it) const;
	void insert(const Item &it, int index);
	Item remove(int index);
	void print(ostream &out) const;
	bool insertAfter(const Item &inList, const Item &newToList);
	bool insertBefore(const Item &inList, const Item &newToList);

	// private:
public: // make the inside structure public for testing purposes.
	struct Node
	{
		Node();
		Node(Item it, Node *next);
		virtual ~Node();
		Item myItem;
		Node *myNext;
	};
	unsigned mySize;
	Node *myFirst;
	Node *myLast;

	void makeADeepCopyOf(const List<Item> &original);
};

template <class Item>
List<Item>::List()
{
	mySize = 0;
	myFirst = myLast = nullptr;
}

template <class Item>
List<Item>::~List()
{
	mySize = 0;
	delete myFirst;
	myFirst = myLast = nullptr;
}

template <class Item>
void List<Item>::makeADeepCopyOf(const List<Item> &original)
{
	myFirst = myLast = nullptr;	   //  set pointers
	mySize = 0;					   //   and size to 'empty' values
	Node *oPtr = original.myFirst; //  start at the first node
	while (oPtr != NULL)
	{						  //  while there are nodes to copy:
		append(oPtr->myItem); //   append the item in that node
		oPtr = oPtr->myNext;  //   advance to next node
	}
}

template <class Item>
List<Item>::List(const List<Item> &original)
{
	makeADeepCopyOf(original);
}

template <class Item>
List<Item>::Node::Node()
{
	myItem = Item();
	myNext = nullptr;
}

template <class Item>
List<Item>::Node::Node(Item it, Node *next)
{
	myItem = it;
	myNext = next;
}

template <class Item>
List<Item>::Node::~Node()
{
	//	cout << "~Node() is deallocating the node containing item "
	//			<< myItem << endl;
	delete myNext;
	myNext = nullptr;
}

template <class Item>
Item List<Item>::getFirst() const
{
	if (mySize == 0 || myFirst == nullptr)
	{
		throw underflow_error("list is empty");
	}
	return myFirst->myItem;
}

template <class Item>
Item List<Item>::getLast() const
{
	if (mySize == 0 || myLast == nullptr)
	{
		throw underflow_error("list is empty");
	}
	return myLast->myItem;
}

template <class Item>
void List<Item>::append(const Item &it)
{
	Node *tmpPtr = new Node(it, nullptr);
	if (mySize == 0)
	{
		myFirst = tmpPtr;
	}
	else
	{
		myLast->myNext = tmpPtr;
	}
	myLast = tmpPtr;
	mySize++;
}

template <class Item>
List<Item> &List<Item>::operator=(const List<Item> &original)
{
	if (this != &original)
	{
		delete myFirst;
		makeADeepCopyOf(original);
	}
	return *this;
}

template <class Item>
bool List<Item>::operator==(const List<Item> &other) const
{
	if (mySize != other.mySize)
	{
		return false;
	}
	Node *nPtr = myFirst;
	Node *oPtr = other.myFirst;
	while (nPtr != nullptr)
	{
		if (nPtr->myItem != oPtr->myItem)
		{
			return false;
		}
		nPtr = nPtr->myNext;
		oPtr = oPtr->myNext;
	}
	return true;
}

template <class Item>
void List<Item>::readFrom(istream &in)
{
	if (mySize != 0)
	{
		delete myFirst;
		myFirst = myLast = nullptr;
		mySize = 0;
	}
	Item it;
	in >> it;
	while (in)
	{
		append(it);
		if (in.peek() == '\n')
		{
			break;
		}
		in >> it;
	}
}

template <class Item>
void List<Item>::readFrom(const string &filename)
{
	ifstream fin(filename);
	assert(fin.is_open());
	readFrom(fin);
	fin.close();
}

template <class Item>
void List<Item>::writeTo(ostream &out, char separator) const
{
	for (Node *ptr = myFirst; ptr; ptr = ptr->myNext)
	{
		out << separator << ptr->myItem;
	}
}

template <class Item>
void List<Item>::writeTo(const string &filename, char separator) const
{
	ofstream fout(filename);
	assert(fout.is_open());
	writeTo(fout, separator);
	fout.close();
}

template <class Item>
void List<Item>::prepend(const Item &it)
{
	myFirst = new Node(it, myFirst);
	if (mySize == 0)
	{
		myLast = myFirst;
	}
	mySize++;
}

template <class Item>
int List<Item>::getIndexOf(const Item &it) const
{
	int currIndex = 0;
	for (Node *ptr = myFirst; ptr; ptr = ptr->myNext)
	{
		if (ptr->myItem == it)
		{
			return currIndex;
		}
		currIndex++;
	}
	return -1; // not found
}

template <class Item>
void List<Item>::insert(const Item &it, int index)
{
	if (index <= 0)
	{
		// insert at beginning.
		prepend(it);
	}
	else if ((unsigned)index >= mySize)
	{
		append(it);
	}
	else
	{
		Node *iPtr = myFirst;
		for (unsigned i = 0; i < (unsigned)index - 1; i++)
		{
			iPtr = iPtr->myNext;
		}
		iPtr->myNext = new Node(it, iPtr->myNext);
		mySize++;
	}
}

template <class Item>
Item List<Item>::remove(int index)
{
	if (mySize == 0)
	{
		throw underflow_error("list is empty");
	}
	// fix the index first
	if (index <= 0)
	{
		index = 0;
	}
	else if ((unsigned)index > mySize - 1)
	{
		index = mySize - 1; // last node
	}
	if (mySize == 1)
	{
		// index must be 0.
		Item ret = myFirst->myItem;
		delete myFirst;
		myFirst = myLast = nullptr;
		mySize--;
		return ret;
	}

	// Deal with middle or last node.
	Node *prevPtr = myFirst;
	// Move prevPtr to node before the node to delete.
	for (unsigned i = 0; i < index - 1; i++)
	{
		prevPtr = prevPtr->myNext;
	}
	Node *dPtr = prevPtr->myNext;
	prevPtr->myNext = dPtr->myNext; // skip over the node or set to null.
	if (index == mySize - 1)
	{
		// deleting the last node, so set myLast to 2nd-to-last.
		myLast = prevPtr;
	}
	Item ret = dPtr->myItem;
	// Do this so that deleting dPtr does not cascade into deleting the whole rest of the chain.
	dPtr->myNext = nullptr;
	delete dPtr;
	mySize--;
	return ret;
}

template <class Item>
void List<Item>::print(ostream &out) const
{
	out << "\nSize: " << mySize << ":: " << flush;
	for (Node *nPtr = myFirst; nPtr; nPtr = nPtr->myNext)
	{
		out << nPtr->myItem << " ";
	}
	out << endl
		<< endl;
}

template <class Item>
bool List<Item>::insertAfter(const Item &inList, const Item &newToList)
{
	Node *curr = myFirst;
	while (curr != nullptr)
	{
		if (curr->myItem == inList)
		{
			Node *newNode = new Node(newToList, nullptr);
			newNode->myNext = curr->myNext;
			curr->myNext = newNode;
			if (curr == myLast)
			{
				myLast = newNode;
			}
			mySize++;
			return true;
		}
		curr = curr->myNext;
	}
	return false;
}

template <class Item>
bool List<Item>::insertBefore(const Item &inList, const Item &newToList)
{
	Node *curr = myFirst;
	Node *previous = nullptr;
	while (curr != nullptr)
	{
		if (curr->myItem == inList)
		{
			Node *newNode = new Node(newToList, curr);
			if (previous == nullptr)
			{
				myFirst = newNode;
			}
			else
			{
				previous->myNext = newNode;
			}
			mySize++;
			return true;
		}
		previous = curr;
		curr = curr->myNext;
	}
	return false;
}
template <class Item>
ostream &operator<<(ostream &out, const List<Item> &myList)
{
	myList.writeTo(out, '=');
	return out;
}

#endif /* LIST_H_ */
