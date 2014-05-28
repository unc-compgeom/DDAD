#include <iostream>

using namespace std;


template <class T>
class LinkedList;

template <class T>
class Node
{
public:
	Node<T> *next;
	Node<T> *prev;
	T *element;
	friend class LinkedList<T>;
	friend class TheAlgorithm;
public:
	Node()
	{
		next = NULL;
		prev = NULL;
	}
	Node(T &target): element(target)
	{
		next = NULL;
		prev = NULL;
	}
	
	template <class T>
	friend ostream& operator<<(ostream& os, const LinkedList<T>& list);

	friend ostream& operator<<(ostream& os, const Node<T>& node)
	{
		os << *node.element;
		return os;
	}

};


template <class T>
class LinkedList
{
public:
	Node<T> *head;
	Node<T> *tail;
public:
	LinkedList()
	{
		head = NULL;
		tail = NULL;
	}

	LinkedList(LinkedList<T> &rhs)
	{
		head = rhs.head;
		tail = rhs.tail;
	}

	LinkedList(Node<T> *h, Node<T> *t)
	{
		head = h;
		tail = t;
	}

	Node<T>* create_node(T &value)
	{
		Node<T> *newNode = new Node<T>;
		newNode->element = &value;
		newNode->next = NULL;
		newNode->prev = NULL;
		return newNode;

	}

	Node<T>* insert_first_element(T & value)
	{
		static Node<T> *newNode = NULL;
		Node<T> *rv = NULL;
		newNode = create_node(value);
		head = tail = newNode;
		rv = newNode;
		newNode = NULL;
		return rv;
	}

	Node<T>* insert_at_front(T &value)
	{
		static Node<T> *newNode = NULL;
		Node<T> *rv = NULL;

		if(head == NULL){
			return insert_first_element(value);
		}
		else{
			newNode = create_node(value);
			newNode->next = head;
			head->prev = newNode;
			head = newNode;
			rv = newNode;
		}
		newNode = NULL;
		return rv;
	}

	Node<T>* insert_at_rear(T &value)
	{
		static Node<T> *newNode = NULL;
		Node<T> *rv = NULL;
		if(head == NULL){
			return insert_first_element(value);
		}
		else{
			newNode = create_node(value);
			newNode->prev = tail;
			tail->next = newNode;
			tail = newNode;
		}
		rv = newNode;
		newNode = NULL;
		return rv;
	}

	void insert_ascend(T &value)
	{
		static Node<T> *newNode = NULL;
		if(head == NULL){
			insert_first_element(value);	
			return;
		}

		Node<T> *current = head;
		while((current != NULL) && (current->element < value))
		{
			current = current->next;
		}

		if(current == head)insert_at_front(value);
		else if(current == NULL)insert_at_rear(value);
		else{
			newNode = create_node(value);
			newNode->next = current;
			newNode->prev = current->prev;
			current->prev->next = newNode;
			current->prev = newNode;
		}
		newNode = NULL;
	}

	void insert_descend(T &value)
	{
		static Node<T> *newNode = NULL;
		if(head == NULL){
			insert_first_element(value);	
			return;
		}

		Node<T> *current = head;
		while((current != NULL) && (current->element > value))
		{
			current = current->next;
		}

		if(current == head)insert_at_front(value);
		else if(current == NULL)insert_at_rear(value);
		else{
			newNode = create_node(value);
			newNode->next = current;
			newNode->prev = current->prev;
			current->prev->next = newNode;
			current->prev = newNode;
		}
		newNode = NULL;
	}

	Node<T>* insert_before(Node<T> *nextNode, T &value)
	{
		static Node<T> *newNode = NULL;
		Node<T> *rv = NULL;
		if(head == nextNode)
			return insert_at_front(value);
		else
		{
			newNode = create_node(value);
			nextNode->prev->next = newNode;
			newNode->prev = nextNode->prev;
			newNode->next = nextNode;
			nextNode->prev = newNode;
		}
		rv = newNode;
		newNode = NULL;
		return rv;
	}

	
	Node<T>* insert_after(Node<T> *prevNode, T &value)
	{
		static Node<T> *newNode = NULL;
		Node<T> *rv = NULL;
		if(prevNode == NULL)
			return insert_first_element(value);
		if(tail == prevNode)
			return insert_at_rear(value);
		else
		{
			newNode = create_node(value);
			prevNode->next->prev = newNode;
			newNode->next = prevNode->next;
			newNode->prev = prevNode;
			prevNode->next = newNode;
		}
		rv = newNode;
		newNode = NULL;
		return rv;
	}

	int remove(T &value){
		Node<T> *current = head;
		while(current != NULL)
		{
			if(*current->element == value)
				break;
			current = current->next;
		}

		if(head == NULL || *current->element != value)
			return -1;

		if(current == tail)
			tail = tail->prev;
		if(current == head)
			head = head->next;
		
		current->prev->next = current->next;
		current->next->prev = current->prev;
		
		return 1;

	}

	int remove(Node<T> *thisNode)
	{

		if(head == NULL)
			return -1;
		if(head == tail)
		{
			head = tail = NULL;
			return 1;
		}
		else if(head == thisNode)
		{
			head = thisNode->next;
			thisNode->next->prev = NULL;
		}
		else if(thisNode->prev)
			thisNode->prev->next = thisNode->next;

		if(tail == thisNode)
		{
			tail = thisNode->prev;
			thisNode->prev->next = NULL;
		}
		else
			thisNode->next->prev = thisNode->prev;

		thisNode->next = thisNode->prev = NULL;
		return 1;
	}


	template <class T>
	friend ostream& operator<<(ostream& os, const LinkedList<T>& list)
	{
		Node<T> *current = list.head;
		if(list.head == NULL) os << "Empty List" << endl;
		while(current != NULL)
		{
			os << *current << " --> ";
			current = current -> next;
		}
		os << endl;
		return os;
	}

	void printList()
	{
		Node<T> *current = head;
		while(current)
		{
			cout << *current->element << endl;
			if(!current)break;
			current = current->next;
		}
	}
	void print_reverse()
	{
		Node<T> *current = tail;
		while(current)
		{
			cout << *current->element << endl;
			if(!current)break;
			current = current->prev;
		}
	}

	void swap_two_nodes(Node<T> *preceding, Node<T> *following)
	{
		if(head == preceding)//do something 
		{
			head = following;
		}
		else
		{
			preceding->prev->next = following;
		}
		
		if(tail == following)//do something
		{
			tail = preceding;	
		}
		else
		{
			following->next->prev = preceding;
		}
		preceding->next = following->next;
		following->prev = preceding->prev;
		following->next = preceding;
		preceding->prev = following;
	}

	bool is_empty()
	{
		return head == NULL;
	}
};