#include <iostream>
#include <cstdlib>
#include <fstream>
#include <time.h>


using namespace std;

// max length of the database
const int MAX_SIZE = 10000;

// iteration counter
static int iterationsCount = 0;

// database elements
struct Node
{
	int id;
};

// comparison of two elements from the data
int compare(const void* n1, const void* n2)
{
	const Node* x = (Node*)n1;
	const Node* y = (Node*)n2;
	if (x->id > y->id)
		return 1;
	if (x->id < y->id)
		return -1;
	return 0;
}


class DataBase
{
	Node* arr; // Array of pointers
	int count; // elements number
	bool isSorted; // Is the array sorted?

public:
	// const
	DataBase()
	{
		arr = new Node[MAX_SIZE];
		count = 0;
		isSorted = false;
	}

	// Dest
	~DataBase()
	{
		delete[] arr;
	}

	// Finding an element
	Node* search(const int& id)
	{
		if (isSorted)
		{
			int l = 0;
			int r = count - 1;
			int m = (l + r) / 2;
			while (l <= r)
			{
				iterationsCount++;
				int m = (l + r) / 2;
				if (arr[m].id < id)
					l = m + 1;
				else if (arr[m].id > id)
					r = m - 1;
				else
					return &arr[m];
			}
		}
		else
		{
			for (int i = 0; i < count; i++)
			{
				iterationsCount++;
				if (arr[i].id == id)
					return &arr[i];
			}
		}
		return nullptr;
	}

	// Adding an element
	void add(const int& id)
	{
		Node* n = new Node;
		n->id = id;
		if (isSorted)
		{
			int l = 0;
			int r = count - 1;
			int m = (l + r) / 2;
			while (l <= r)
			{
				iterationsCount++;
				int m = (l + r) / 2;
				if (arr[m].id < n->id)
					l = m + 1;
				else
					r = m - 1;
			}
			for (int i = count; i > m; i--)
			{
				iterationsCount++;
				arr[i] = arr[i - 1];
			}
			arr[m] = *n;
			count++;
		}
		else
		{
			iterationsCount++;
			arr[++count] = *n;
		}
	}

	// deleting an element
	void remove(const int& id)
	{
		if (isSorted)
		{
			int l = 0;
			int r = count - 1;
			int m = (l + r) / 2;
			while (l <= r)
			{
				iterationsCount++;
				int m = (l + r) / 2;
				if (arr[m].id < id)
					l = m + 1;
				else if (arr[m].id > id)
					r = m - 1;
				else
					break;
			}

			for (int j = m; j < count; j++)
			{
				iterationsCount++;
				arr[j] = arr[j + 1];
			}
			if (m < count)
				count--;
		}
		else
			for (int i = 0; i < count; i++)
			{
				iterationsCount++;
				if (arr[i].id == id)
				{
					for (int j = i + 1; j < count; j++)
					{
						iterationsCount++;
						arr[j] = arr[j + 1];
					}
					count--;
					return;
				}
			}
	}

	// bubble sort
	void bubble_sort()
	{
		for (int i = 0; i < count - 1; i++)
			for (int j = 0; j < count; j++)
				if (compare(&arr[i], &arr[j]) == 1)
				{
					Node tmp = arr[i];
					arr[i] = arr[j];
					arr[j] = tmp;
				}
		isSorted = true;
	}

	// Quick sort
	void quick_sort()
	{
		qsort(arr, count, sizeof(Node), compare);
		isSorted = true;
	}
};

// simulator of database working
void simulate(int records, int requests, double add, double remove)
{
	// number of adding
	int addRequests = (int)(requests * add);

	// number of deleting
	int removeRequests = (int)(requests * remove);

	// random ID creating
	int* ids = new int[records + requests];
	for (int i = 0; i < records + requests; i++)
		ids[i] = rand() % 10000000 + 1;

	// unsorted database
	DataBase* d = new DataBase();

	iterationsCount = 0;
	for (int i = 0; i < records; i++)
		d->add(ids[i]);
	for (int i = records; i < records + addRequests; i++)
		d->add(ids[i]);
	for (int i = records + addRequests; i < records + addRequests + removeRequests; i++)
		d->remove(ids[i]);
	for (int i = records + addRequests + removeRequests; i < records; i++)
		d->search(ids[i]);
	cout << "Unsorted database: " << iterationsCount << " iterations" << endl;

	// bubble sort
	DataBase* d2 = new DataBase();

	iterationsCount = 0;
	for (int i = 0; i < records; i++)
		d2->add(ids[i]);
	d2->bubble_sort();
	for (int i = records; i < records + addRequests; i++)
		d2->add(ids[i]);
	for (int i = records + addRequests; i < records + addRequests + removeRequests; i++)
		d2->remove(ids[i]);
	for (int i = records + addRequests + removeRequests; i < records; i++)
		d2->search(ids[i]);
	cout << "Bubble sort: " << iterationsCount << " iterations" << endl;

	// quick sort
	DataBase* d3 = new DataBase();

	iterationsCount = 0;
	for (int i = 0; i < records; i++)
		d3->add(ids[i]);
	d3->quick_sort();
	for (int i = records; i < records + addRequests; i++)
		d3->add(ids[i]);
	for (int i = records + addRequests; i < records + addRequests + removeRequests; i++)
		d3->remove(ids[i]);
	for (int i = records + addRequests + removeRequests; i < records; i++)
		d3->search(ids[i]);
	cout << "Quick sorting: " << iterationsCount << " iterations" << endl;
}

int main()
{
	srand(time(NULL));
	cout << "First data set: " << endl;
	simulate(10, 3000, 0.2, 0.3);
	cout << "\n\n\nSecond data set: " << endl;
	simulate(100, 1000, 0.1, 0.5);

	return 0;
}
