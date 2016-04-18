// FILE: DPQueue.cpp
// IMPLEMENTS: p_queue (see DPQueue.h for documentation.)
//
// INVARIANT for the p_queue class:
//   1. The number of items in the p_queue is stored in the member
//      variable used.
//   2. The items themselves are stored in a dynamic array (partially
//      filled in general) organized to follow the usual heap storage
//      rules.
//      2.1 The member variable heap stores the starting address
//          of the array (i.e., heap is the array's name). Thus,
//          the items in the p_queue are stored in the elements
//          heap[0] through heap[used - 1].
//      2.2 The member variable capacity stores the current size of
//          the dynamic array (i.e., capacity is the maximum number
//          of items the array currently can accommodate).
//          NOTE: The size of the dynamic array (thus capacity) can
//                be resized up or down where needed or appropriate
//                by calling resize(...).
// NOTE: Private helper functions are implemented at the bottom of
// this file along with their precondition/postcondition contracts.

#include <cassert>   // provides assert function
#include <iostream>  // provides cin, cout
#include <iomanip>   // provides setw
#include <cmath>     // provides log2
#include "DPQueue.h"

using namespace std;

namespace CS3358_SP2016_Assign08
{
   // EXTRA MEMBER FUNCTIONS FOR DEBUG PRINTING
   void p_queue::print_tree(const char message[], size_type i) const
   // Pre:  (none)
   // Post: If the message is non-empty, it has first been written to
   //       cout. After that, the portion of the heap with root at
   //       node i has been written to the screen. Each node's data
   //       is indented 4*d, where d is the depth of the node.
   //       NOTE: The default argument for message is the empty string,
   //             and the default argument for i is zero. For example,
   //             to print the entire tree of a p_queue p, with a
   //             message of "The tree:", you can call:
   //                p.print_tree("The tree:");
   //             This call uses the default argument i=0, which prints
   //             the whole tree.
   {
      const char NO_MESSAGE[] = "";
      size_type depth;

      if (message[0] != '\0')
         cout << message << endl;

      if (i >= used)
         cout << "(EMPTY)" << endl;
      else
      {
         depth = size_type( log( double(i+1) ) / log(2.0) + 0.1 );
         if (2*i + 2 < used)
            print_tree(NO_MESSAGE, 2*i + 2);
         cout << setw(depth*3) << "";
         cout << heap[i].data;
         cout << '(' << heap[i].priority << ')' << endl;
         if (2*i + 1 < used)
            print_tree(NO_MESSAGE, 2*i + 1);
      }
   }

   void p_queue::print_array(const char message[]) const
   // Pre:  (none)
   // Post: If the message is non-empty, it has first been written to
   //       cout. After that, the contents of the array representing
   //       the current heap has been written to cout in one line with
   //       values separated one from another with a space.
   //       NOTE: The default argument for message is the empty string.
   {
      if (message[0] != '\0')
         cout << message << endl;

      if (used == 0)
         cout << "(EMPTY)" << endl;
      else
         for (size_type i = 0; i < used; i++)
            cout << heap[i].data << ' ';
   }

   // CONSTRUCTORS AND DESTRUCTOR

   p_queue::p_queue()
   : capacity(0), used(0)
   {
      heap = new ItemType[capacity];
   }

   p_queue::p_queue(const p_queue& src)
   : capacity(src.capacity), used(src.used)
   {
      heap = new ItemType[capacity];
      for (size_type i = 0; i < used; ++i) {
         heap[i].data = src.heap[i].data;
         heap[i].priority = src.heap[i].priority;
      }
   }

   p_queue::~p_queue()
   {
      delete [] heap;
      heap = 0;
   }

   // MODIFICATION MEMBER FUNCTIONS
   p_queue& p_queue::operator=(const p_queue& rhs)
   {
      if (this != &rhs) { // If sequence is not being set to itself
         used = rhs.used;
         capacity = rhs.capacity;

         heap = new ItemType[capacity]; // Allocate space
         if (heap == NULL) // Ensure memory is allocated
         {
            cerr << "*** Error: Memory not available. Exiting program. ***";
            exit(0);
         }

         for (size_type i = 0; i < used; ++i) {
            heap[i].data = rhs.heap[i].data;
            heap[i].priority = rhs.heap[i].priority;
         }
      }

      return *this;
   }

   void p_queue::push(const value_type& entry, size_type priority)
   {
      cout << "Cap: " << capacity << " | used: " << used << endl;
      if (used == capacity) { // If adding to the sequence will exceed capacity
         cout << "resizing: " << used;
         size_type new_capacity = (int)((capacity * 1.5) + 1);
         cout << " to: " << new_capacity << endl;
         resize(new_capacity);
      }

      heap[used].data = entry;
      heap[used].priority = priority;

      used++;

      // Logic for balanced tree goes here, see specs
   }

   void p_queue::pop()
   {
      cerr << "pop() not implemented yet" << endl;
   }

   // CONSTANT MEMBER FUNCTIONS

   p_queue::size_type p_queue::size() const
   {
      return used;
   }

   bool p_queue::empty() const
   {
      if (used == 0) {
         return true;
      }

      return false;
   }

   p_queue::value_type p_queue::front() const
   {
   //     Pre:  size() > 0.
   //     Post: The return value is the data of the highest priority
   //           item in the p_queue, but the p_queue is unchanged.
   //           (If several items have equal priority, then the
   //           implementation may decide which one to return.)
      assert(used > 0);

      size_type maxPriority;
      size_type atIndex;
      value_type highPriData;

      for (size_type i = 0; i < used; i++) {
         if (i == 0) {
            atIndex = i;
            maxPriority = heap[atIndex].priority;
         } else {
            if (heap[i].priority > maxPriority) {
               atIndex = i;
               maxPriority = heap[atIndex].priority;
            }
         }
      }

      highPriData = heap[atIndex].data;

      return highPriData;
   }

   // PRIVATE HELPER FUNCTIONS
   void p_queue::resize(size_type new_capacity)
   // Pre:  (none)
   // Post: The size of the dynamic array pointed to by heap (thus
   //       the capacity of the p_queue) has been resized up or down
   //       to new_capacity, but never less than used (to prevent
   //       loss of existing data).
   //       NOTE: All existing items in the p_queue are preserved and
   //             used remains unchanged.
   {
      assert (new_capacity > 0);
      capacity = new_capacity;

      ItemType *temp = new ItemType[capacity]; // Allocate space
      if (temp == NULL) // Ensure memory is allocated
      {
         cerr << "*** Error: Memory not available. Exiting program. ***";
         exit(0);
      }

      for (size_type i = 0; i < used; ++i) {
         temp[i].data = heap[i].data; // Copy existing values
         temp[i].priority = heap[i].priority;
      }

      delete [] heap; // Free up old space
      heap = temp; // Update sequence::data to new memory location
      cout << "Done resize." << endl;
   }

   bool p_queue::is_leaf(size_type i) const
   // Pre:  (i < used)
   // Post: If the item at heap[i] has no children, true has been
   //       returned, otherwise false has been returned.
   {
      assert(i < used);

      if (used > ((2*i) + 1)) { // If the node has at least a left-child
         return false;
      }

      return true; // The node has no children so it's is a leaf
   }

   p_queue::size_type
   p_queue::parent_index(size_type i) const
   // Pre:  (i > 0) && (i < used)
   // Post: The index of "the parent of the item at heap[i]" has
   //       been returned.
   {
      assert(i > 0);
      assert(i < used);

      size_type parentIndex = (int)((i-1)/2);

      return parentIndex;
   }

   p_queue::size_type
   p_queue::parent_priority(size_type i) const
   // Pre:  (i > 0) && (i < used)
   // Post: The priority of "the parent of the item at heap[i]" has
   //       been returned.
   {
      assert(i > 0);
      assert(i < used);

      size_type parentPriority = heap[parent_index(i)].priority;

      return parentPriority;
   }

   p_queue::size_type
   p_queue::big_child_index(size_type i) const
   // Pre:  is_leaf(i) returns false
   // Post: The index of "the bigger child of the item at heap[i]"
   //       has been returned.
   //       (The bigger child is the one whose priority is no smaller
   //       than that of the other child, if there is one.)
   {
      assert(!is_leaf(i));

      if (used <= ((2*i) + 2)) { // If no right child exists
         return ((2*i) + 1); // Return left child index
      } else { // If both children exist
         if (heap[((2*i) + 1)].priority >= heap[((2*i) + 2)].priority) {
            return ((2*i) + 1); // Return left child index
         } else {
            return ((2*i) + 2); // Return right child index
         }
      }
   }

   p_queue::size_type
   p_queue::big_child_priority(size_type i) const
   // Pre:  is_leaf(i) returns false
   // Post: The priority of "the bigger child of the item at heap[i]"
   //       has been returned.
   //       (The bigger child is the one whose priority is no smaller
   //       than that of the other child, if there is one.)
   {
      assert(!is_leaf(i));

      return (heap[big_child_index(i)].priority);
   }

   void p_queue::swap_with_parent(size_type i)
   // Pre:  (i > 0) && (i < used)
   // Post: The item at heap[i] has been swapped with its parent.
   {
      assert(i > 0);
      assert(i < used);

      value_type tempData = heap[i].data; // Store child data in temp
      size_type tempPriority = heap[i].priority;

      heap[i].data = heap[i/2].data; // Write data from parent into child
      heap[i].priority = heap[i/2].priority;

      heap[i/2].data = tempData;      // Write data from child into parent
      heap[i/2].priority = tempPriority;
   }
}

