/* carefree-objects
 *
 * a thread-safe object manager extension for c++
 *
 * Copyright (C) 2011-2013 Stefan Zimmermann <zimmermann.code@googlemail.com>
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

#include <carefree-python/import.hpp>

namespace cfo { namespace python { namespace import
{
  namespace __builtin__
  {
    static const boost::python::object module
    = boost::python::import("__builtin__");

    const boost::python::object
      type = module.attr("type"),

      property = module.attr("property"),

      int_ = module.attr("int"),
      hash = module.attr("hash"),
      iter = module.attr("iter"),
      repr = module.attr("repr");
  }

  namespace functools
  {
    static const boost::python::object module
    = boost::python::import("functools");

    const boost::python::object
      partial = module.attr("partial");
  }

  namespace datetime
  {
    static const boost::python::object module
    = boost::python::import("datetime");

    const boost::python::object
      timedelta = module.attr("timedelta"),
      tzinfo = module.attr("tzinfo"),
      datetime = module.attr("datetime");
  }
} } }