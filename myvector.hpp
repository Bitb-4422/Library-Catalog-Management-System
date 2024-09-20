//============================================================================
// Name         : myvector.h
// Author       : 
// Version      : 1.0
// Date Created : 
// Date Modified:
// Description  : Vector implementation in C++
//============================================================================
#ifndef MYVECTOR_H
#define MYVECTOR_H

#include<iostream>
#include<cstdlib>
#include<iomanip>
#include <stdexcept>
#include<sstream>

using namespace std;
template <typename T>
class MyVector
{
	private:
		T *data;						//pointer to int(array) to store elements
		int v_size;						//current size of vector (number of elements in vector)
		int v_capacity;					//capacity of vector
	public:
		MyVector();						//No argument constructor
		MyVector(int cap);				//One Argument Constructor
		MyVector(const MyVector& other);		//Copy Constructor
		~MyVector();					//Destructor
		void push_back(T element);		//Add an element at the end of vector
		void insert(int index, T element); //Add an element at the index 
		void erase(int index);			//Removes an element from the index
		T& operator[](int index);		//return reference of the element at index
		T& at(int index); 				//return reference of the element at index
		const T& front();				//Returns reference of the first element in the vector
		const T& back();				//Returns reference of the Last element in the vector
		int size() const;				//Return current size of vector
		int capacity() const;			//Return capacity of vector
		bool empty() const; 			//Return true if the vector is empty, False otherwise
		void shrink_to_fit();			//Reduce vector capacity to fit its size
};

//================================================================================
template <typename T>
MyVector<T>::MyVector()
{
	v_capacity = 0;
	data = new T[v_capacity];
	v_size = 0;
}
//================================================================================
template <typename T>
MyVector<T>::MyVector(int cap)
{	
	v_capacity = cap;  // Set the capacity of the vector
	data = new T[v_capacity];  // Allocate memory for the vector data
	v_size = 0;  // Initialize the vector size to 0
}
//================================================================================
template <typename T>
MyVector<T>::MyVector(const MyVector &other)
{
	// Copy the size and capacity from the other vector
    this->v_size = other.v_size;
    this->v_capacity = other.v_capacity;
    // Allocate memory and copy data from the other vector
    this->data = new T[v_capacity];
    for (int i = 0; i < v_size; ++i) 
    {
        this->data[i] = other.data[i];
    }
}
//================================================================================
template <typename T>
MyVector<T>::~MyVector()
{
	// Deallocate memory used by the vector data
	delete[] data;
}
//================================================================================
template <typename T>
int MyVector<T>::size() const
{
	return v_size;
}
//================================================================================
template <typename T>
int MyVector<T>::capacity() const
{
	return v_capacity;
}
//================================================================================
template <typename T>
bool MyVector<T>::empty() const
{
	return size() == 0;  // Check if the size is zero
}
//================================================================================
template <typename T>
void MyVector<T>::push_back(T element)
{	
	// Resize the vector if it's full
	if(v_capacity == v_size) 
	{
		int new_capacity = max(1,v_capacity*2);
	    // Create a temporary array with the new capacity
	    T *temp = new T[new_capacity];
	    // Copy elements from the current data array to the temporary array
	    for (int i = 0; i < v_size; ++i) 
	    {
	        temp[i] = data[i];
	    }
	    
	    delete[] data; // Deallocate old memory
	    data = temp;
	    v_capacity = new_capacity;  // Update the capacity of the vector

	}
	// Add the new element at the end
	data[v_size++] = element;

}
//================================================================================
template <typename T>	
void MyVector<T>::insert(int index,T element)
{
	// Throw an exception if the vector is empty
	if(empty())
	{
		throw runtime_error("Vector is empty");
	}
	// Throw an exception if the index is out of range
	if(index > v_size - 1 || index < 0)
	{
		throw out_of_range("Index is out of range");
	}
    // Resize the vector if it's full
	if(v_capacity == v_size) 
	{
		int new_capacity = max(1,v_capacity*2);
    
	    // Create a temporary array with the new capacity
	    T *temp = new T[new_capacity];
	    // Copy elements from the current data array to the temporary array
	    for (int i = 0; i < v_size; ++i) 
	    {
	        temp[i] = data[i];
	    }
	    
	    delete[] data; // Deallocate old memory
	    data = temp;
	    v_capacity = new_capacity;  // Update the capacity of the vector
	}
    // Shift elements to make space for the new one
	if(index < v_size)
	{
		for (int i = v_size; i > index; --i){
			data[i]=data[i-1];
		}
	}
	// Insert the new element at the specified index
	data[index]= element;
	++v_size;

}
//=================================================================================
template <typename T>   
void MyVector<T>::erase(int index)
{
    // Throw an exception if the vector is empty
    if (empty())
    {
        throw runtime_error("Vector is empty");
    }
    // Throw an exception if the index is out of range
    if (index >= v_size || index < 0)
    {
        throw out_of_range("Index is out of range");
    }
    
    // Shift elements to overwrite the element at the specified index
    for (int i = index; i < v_size - 1; ++i)
    {
        data[i] = data[i + 1];
    }

    // Decrement the size of the vector
    --v_size;
}

//==================================================================================
template <typename T>
T& MyVector<T>::operator[](int index)
{
	// Throw an exception if the vector is empty
	if(empty())
	{
		throw runtime_error("Vector is empty");
	}
	// Throw an exception if the index is out of range
	if(index >= v_size || index < 0)
	{
		throw out_of_range("Index is out of range");
	}
	// Return a reference to the element at the specified index
  	return data[index];
}
//===================================================================================
template <typename T>
T& MyVector<T>::at(int index)
{
	// Throw an exception if the index is out of range
	if(index >= v_size || index < 0)
	{
		throw out_of_range("Index is out of range");
	}
	// Return a reference to the element at the specified index
	return data[index];
}
//===================================================================================
template <typename T>
const T& MyVector<T>::front()
{
	// Throw an exception if the vector is empty
	if(empty())
	{
		throw runtime_error("Vector is empty");
	}
	// Return a reference to the first element
	return data[0];
}
//===================================================================================
template <typename T>
const T& MyVector<T>::back()
{
	// Throw an exception if the vector is empty
	if(empty())
	{
		throw runtime_error("Vector is empty");
	}
	// Return a reference to the last element
	return data[v_size-1];
}
//==================================================================================
template <typename T>
void MyVector<T>::shrink_to_fit()		//Reduce vector capacity to fit its size.
{
	// Check if the capacity is greater than the size
	if(v_capacity > v_size)
	{
		T *temp= new T[v_size];
		// Copy elements from the current data array to the temporary array
		for (int i = 0; i < v_size; ++i)
		{
			temp[i]=data[i];
		}
		delete[] data; // Deallocate old memory
		data = temp;
		v_capacity = v_size;  // Update the capacity of the vector
	}
}
#endif
