/* carefree-objects
 *
 * a thread-safe object manager extension for c++
 *
 * Copyright (C) 2011-2012 Stefan Zimmermann <zimmermann.code@googlemail.com>
 *
 * carefree-objects is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * carefree-objects is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with carefree-objects.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __CFO_MANAGED_FORWARD_HPP
#define __CFO_MANAGED_FORWARD_HPP

#include "common.hpp"

#include "basic_manager.hpp"
#include "methods.hpp"

namespace cfo { namespace intern
{
  template<typename T, bool INIT_NULL, typename... BASES>
  class forward<T, true, INIT_NULL, BASES...> :
    private std::unique_ptr<managed<T, true, INIT_NULL, BASES...> >
  {
  private:
    typedef managed<T, true, INIT_NULL, BASES...> manager_type;
    typedef std::unique_ptr<manager_type> basic_type;

  protected:
    inline forward(T *obj) :
      basic_type(new manager_type(obj))
    {}

  public:
    // typedef typename T::template cfo_managed_const_methods
    //   <const T, true, BASES...>
    //   const_accessor;

    // typedef typename T::template cfo_managed_methods<T, true, BASES...>
    //   accessor;

    typedef forward<T, true, true, BASES...> null;

    inline forward() :
      basic_type(new manager_type)
    {}

    template<typename... A>
    inline forward(A... args) :
      basic_type(new manager_type(args...))
    {}

    inline forward(const managed<T, true, false, BASES...> &manager) :
      basic_type(new manager_type(manager))
    {}

    template
    <typename T_other, bool INIT_NULL_other, typename... BASES_other>
    inline forward
    (const managed<T_other, true, INIT_NULL_other, BASES_other...>
     &other_manager) :

      basic_type(new manager_type(other_manager))
    {}

    // inline const_accessor caccess() const
    // {
    //   return const_accessor(**this);
    // }

    // inline accessor access()
    // {
    //   return accessor(**this);
    // }

    inline T* unmanaged() const
    {
      return (*this)->unmanaged();
    }

    inline operator bool() const
    {
      return bool(*this);
    }

    inline bool operator!() const
    {
      return !*this;
    }

    inline const manager_type& const_manager() const
    {
      return **this;
    }

    inline manager_type& manager()
    {
      return **this;
    }

    inline operator const manager_type&() const
    {
      return **this;
    }

    inline operator manager_type&()
    {
      return **this;
    }

    inline operator manager_type()
    {
      return **this;
    }
  };

  template<typename T, bool INIT_NULL, typename... BASES>
  class forward<T, false, INIT_NULL, BASES...> :
    private std::unique_ptr<managed<T, false, INIT_NULL, BASES...> >
  {
  private:
    typedef managed<T, false, INIT_NULL, BASES...> manager_type;
    typedef std::unique_ptr<manager_type> basic_type;

  protected:
    inline forward(T *obj) :
      basic_type(new manager_type(obj))
    {}

  public:
    typedef forward<T, false, true, BASES...> null;

    inline forward() :
      basic_type(new manager_type)
    {}

    template<typename... A>
    inline forward(A... args) :
      basic_type(new manager_type(args...))
    {}

    inline forward(const managed<T, false, false, BASES...> &manager) :
      basic_type(new manager_type(manager))
    {}

    template
    <typename T_other, bool INIT_NULL_other, typename... BASES_other>
    inline forward
    (const managed<T_other, false, INIT_NULL_other, BASES_other...>
     &other_manager) :

      basic_type(new manager_type(other_manager))
    {}

    inline std::size_t refcount() const
    {
      return (**this).refcount();
    }

    inline T* operator->() const
    {
      return (**this).operator->();
    }

    inline T* unmanaged() const
    {
      return (**this).unmanaged();
    }

    inline const manager_type& const_manager() const
    {
      return **this;
    }

    inline manager_type& manager()
    {
      return **this;
    }

    inline operator const manager_type&() const
    {
      return **this;
    }

    inline operator manager_type&()
    {
      return **this;
    }

    inline operator manager_type()
    {
      return **this;
    }
  };
} }

#endif