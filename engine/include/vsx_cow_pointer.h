/** @file
* Project: VSXu: Realtime visual programming language, music/audio visualizer, animation tool and much much more.
*
* @see The GNU Public License (GPL)
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/


/**
  @par General info.
  vsx_cow_pointer is a simple and lightweight shared pointer template class
  that realizes reference counting, copy on write and automatic deletion
  of pointers it holds. I focused on makeing it quick and simple so it also
  has many limitations. @b Be @b sure to read this documentation and don't
  blame me if you don't. It uses pointer semantics.

  @par
  vsx_cow_pointer stores a pointer to parameter type T and a pointer to
  integer containing reference count. The copy constructor just copies
  sources internal pointer and reference pointer and then increments
  the reference counter. On use of a non-constanT method it detaches
  copying the pointed to T object and reseting the reference counter to 1.
  The destructor decrements reference counter and in case it's value is zero
  deletes pointed to object (and reference pointer). This ensures automatic
  and safe deletion of unreferenced pointers.

  @par
  vsx_cow_pointer also holds a static object of self type. The default constructor
  is actually the copy constructor. It copies the default object which is very
  cheap comparing to constructing of more complicated objects. The [default object](@ref default_T)
  @b must be initialized before any use of the class (or the uniwerse will collapse :) ).
  It's the tradeoff for possibility to quickly construct large amounts of objects.
  Also deletion of objects that have reference count > 0 is very cheap.

  @par What vsx_cow_pointer can be used for.
  1. for shareing chunks of data that are expensive to copy if it's not
  known if users will want to write to them. Shared data is copied only when it's needed.
  2. for preventing leaks/dangling pointers in above situation as theres no need
  to pick owner of the data - it will be cleaned by auto.
  3. for storeing pointers in arrays and other containers that wil not free
  them by self.
  4. cheap construction/deletion also makes allocating/freeing arrays/containers of objects cheaper
  in case if it is not known if all of them will be used.

  @par Requirements for stored objects.
  1. Stored objects must have public copy constructor.
  2. Stored objects must be alocated with @b new (and not new[]).

  @par Drawbacks.
  1. no pointer arytmetics is possible with vsx_cow_pointer
  2. it must be initialized
  3. vsx_cow_pointer @b doesn't @b check if you use the class right (see below), so it's
  on you.

  @author Wojciech Karkoszka datar@o2.pl Copyright (C) 2012
  */

#ifndef VSX_COW_POINTER_H
#define VSX_COW_POINTER_H

//#define VSX_COW_POINTER_DEBUG

#ifdef VSX_COW_POINTER_DEBUG
#define INSPECT(x) inspect(x)
#else
#define INSPECT(x) ;
#endif


template <typename T>
class vsx_cow_pointer
{
private: //data

	/**
	  Pointer to reference counter.
	  It is assumed it is @b alvays valid!!
	  */
	unsigned long* ref_count;

	/**
	  Pointer to referenced data.
	  It is assumed it's @b always walid, but in case of inproper use of class
	  this may be wrong (and this also means the program will BUM....)
	  */
	T* ref;

	/**
	  Default value for constructing new objects.
	  Default ctor constructs objects by copying this value.
	  Set it this way (say T=int):

		  template<>
		  const vsx_cow_pointer<int> vsx_cow_pointer<int>::default_T =
		  vsx_cow_pointer<int>::vsx_cow_pointer(new int(5));
		  
	  in a cpp file. Using the class while it's not set will lead to probable crash!!
	*/
	static vsx_cow_pointer<T> default_T;

public: //methods

	/**
	  Method to inspect the default construction value.
	  */
	static const T& get_default_T()
	{
		return default_T;
	}

	/**
	  Sets default construction value
	  in case you wanted to change it after initialization.
	  After you set it newly default constructed objects will be it's copies.
	  Cost: very cheap, unless default_T is unique (so it's *ref_count == 1).
	  @warning Using this method before initializing default_T will lead to
	  probable crash.
	  */
	static void set_default_T(const vsx_cow_pointer& t)
	{
		default_T = t;
	}

	/**
	  Constructs vsx_cow_pointer from pointed to type.
	  Cost: very cheap.
	  In case of failure (ie bad alloc) parameter r will be deleted.
	  @warning
	  1. T* r @b must be fully constructed. Passing NULL will make a big boom.
	  2. After you passed the pointer to this method it's owned by vsx_cow_pointer,
	  so don't delete it yourself and also not use, unless you know what you are doing :P
	  Use vsx_cow_pointer instead.
	  3. Constructing 2 vsx_cow_pointers of same T* pointers will ensure crash.
	  */
	vsx_cow_pointer(T* r):
		ref (r)
	{
		try
		{
			ref_count = new unsigned long(1);
		}
		catch (...)
		{
			delete ref;
		}
		INSPECT("vsx_cow_pointer(T* r)");
	}

	/**
	  Copy and also default constructor.
	  Very cheap.
	  */
	vsx_cow_pointer(const vsx_cow_pointer& other = default_T):
		ref_count(other.ref_count),
		ref(other.ref)
	{
		++(*ref_count);
		INSPECT("vsx_cow_pointer(const vsx_cow_pointer& other)");
	}

	/**
	  Assigment operator.
	  Self assigment handled.
	  Cost: low, unless this is unique, what will call T's destructor.
	  */
	vsx_cow_pointer& operator=(const vsx_cow_pointer& other)
	{
		if( ref != other.ref) //self assigment would kill us if ref_count == 1
		{
			clear();
			ref_count = other.ref_count;
			ref = other.ref;
			++(*ref_count);
		}
		INSPECT("operator=(const vsx_cow_pointer& other)");
		return *this;
	}

	/**
	  const operator->
	  Cost: very cheap.
	  @warning use only for temporary objects.
	  */
	const T* operator->() const
	{
		INSPECT("const operator->()");
		return ref;
	}

	/**
	  operator->
	  Cost: can be expensive, depending on cost of T::T(T&)
	  */
	T* operator->()
	{
		detach();
		INSPECT("operator->()");
		return ref;
	}

	/**
	  const operator*
	  Cost: very cheap.
	  */
	const T& operator*() const
	{
		INSPECT("const operator*()");
		return *ref;
	}

	/**
	  operator*
	  Cost: can be expensive, depending on cost of T::T(T&)
	  */
	T& operator*()
	{
		detach();
		INSPECT("operator*()");
		return *ref;
	}

	/**
	  operator==. Returns true if both points to same T
	  Cost: very cheap.
	  */
	bool operator==(const vsx_cow_pointer& other) const
	{
		INSPECT("operator==()");
		return (other.ref == ref);
	}

	/**
	  operator!=. Returns true if both points not to same T
	  Cost: very cheap.
	  */
	bool operator!=(const vsx_cow_pointer& other) const
	{
		INSPECT("operator!=()");
		return !(*this == other);
	}

	/**
	  It sets the object to default and returns T*, that is no longer owned by vsx_cow_pointer.
	  It's the only option to take ownage over T object holded by vsx_cow_pointer.
	  Cost: can be big, depending on T's costs: it calls T::T(T&) if this is not unique.
	  @warning After take() this is reset to default, so it no longer references previous T.
	  @warning I HAVEN'T TESTED IT YET!!!!
	  */
	T* take()
	{
		detach();
		delete ref_count;
		ref_count = default_T.ref_count;
		ref_count++;
		T* to_ret = ref;
		ref = default_T.ref;
		INSPECT("take()");
		return to_ret;
	}

	/**
	  destructor.
	  Cost: low, unless this is unique, what will call T's destructor.
	  */
	~vsx_cow_pointer()
	{
		INSPECT("~vsx_cow_pointer()");
		clear();
	}

	/**
	  I used it to see if eth is OK.
	  */
	void inspect(std::string msg) const
	{
		std::cout<<"vsx_cow_pointer: this="<<this<<
				   "   *ref_count=" <<*ref_count<<
				   "   ref_count="<<ref_count<<
				   "   ref="<<ref<<"   "<<msg<<std::endl;
	}

private: //methods
	void clear()
	{
		INSPECT("before clear()");
		--(*ref_count);
		if(*ref_count == 0)
		{
			delete ref_count;
			delete ref;
			ref_count = 0;
			ref = 0;
		}
	}

	void detach()
	{
		INSPECT("before detach()");
		if(*ref_count > 1)
		{
			T* nw_ref = new T(*ref);
			unsigned long* nw_ref_count;
			try
			{
				nw_ref_count = new unsigned long(1);
			}
			catch (...)
			{
				delete nw_ref;
				throw;
			}
			--(*ref_count);
			ref_count = nw_ref_count;
			ref = nw_ref;
		}
		INSPECT("after detach()");
	}
};
#endif // VSX_COW_POINTER_H
