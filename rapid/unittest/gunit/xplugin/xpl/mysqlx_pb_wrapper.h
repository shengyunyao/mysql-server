/* Copyright (c) 2015, 2017, Oracle and/or its affiliates. All rights reserved.

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2 of the License.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA */

#ifndef XPLUGIN_MYSQLX_PB_WRAPPER_H_
#define XPLUGIN_MYSQLX_PB_WRAPPER_H_

#include <cstddef>

#include <initializer_list>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "ngs_common/protocol_protobuf.h"

namespace xpl {
namespace test {

class Operator;
class FunctionCall;
class Object;
class Array;

class Identifier : public Mysqlx::Expr::Identifier {
 public:
  Identifier(const std::string &name = "",
             const std::string &schema_name = "");  // NOLINT(runtime/explicit)
};

using Document_path_item = ::Mysqlx::Expr::DocumentPathItem;

class Document_path
    : public ::google::protobuf::RepeatedPtrField<Document_path_item> {
 public:
  class Path
      : private std::vector<std::pair<Document_path_item::Type, std::string> > {
   public:
    Path() {}
    explicit Path(const std::string &value);
    Path &add_member(const std::string &value);
    Path &add_index(int index);
    Path &add_asterisk();
    Path &add_double_asterisk();
    friend class Document_path;
  };

  Document_path(const Path &path);  // NOLINT(runtime/explicit)
};

class ColumnIdentifier : public Mysqlx::Expr::ColumnIdentifier {
 public:
  ColumnIdentifier(const std::string &name = "",
                   const std::string &table_name = "",
                   const std::string &schema_name = "",
                   const Document_path::Path *path = NULL);
  ColumnIdentifier(const Document_path &path, const std::string &name = "",
                   const std::string &table_name = "",
                   const std::string &schema_name =
                       "");  // NOLINT(runtime/explicit)
};

class Scalar : public Mysqlx::Datatypes::Scalar {
 public:
  struct Null {};

  struct String : public Mysqlx::Datatypes::Scalar_String {
    String(const std::string &value);  // NOLINT(runtime/explicit)
  };

  struct Octets : public Mysqlx::Datatypes::Scalar_Octets {
    Octets(const std::string &value,
           unsigned type = 0);  // NOLINT(runtime/explicit)
  };

  Scalar() {}
  Scalar(int value);                             // NOLINT(runtime/explicit)
  Scalar(unsigned int value);                    // NOLINT(runtime/explicit)
  Scalar(bool value);                            // NOLINT(runtime/explicit)
  Scalar(float value);                           // NOLINT(runtime/explicit)
  Scalar(double value);                          // NOLINT(runtime/explicit)
  Scalar(const char *value, unsigned type = 0);  // NOLINT(runtime/explicit)
  Scalar(Scalar::Octets *value);                 // NOLINT(runtime/explicit)
  Scalar(const Scalar::Octets &value);           // NOLINT(runtime/explicit)
  Scalar(Scalar::String *value);                 // NOLINT(runtime/explicit)
  Scalar(const Scalar::String &value);           // NOLINT(runtime/explicit)
  Scalar(Null value);                            // NOLINT(runtime/explicit)
};

class Any : public Mysqlx::Datatypes::Any {
 public:
  class Array;

  class Object : public Mysqlx::Datatypes::Object {
   public:
    struct Fld;
    Object() = default;
    Object(std::initializer_list<Fld> list);
  };

  Any() = default;
  Any(const Scalar &scalar);  // NOLINT(runtime/explicit)
  Any(const Object &obj);     // NOLINT(runtime/explicit)
  Any(const Array &array);    // NOLINT(runtime/explicit)

  template <typename T>
  Any(const T &v)             // NOLINT(runtime/explicit)
      : Any(Scalar(v)) {}
};

struct Any::Object::Fld {
  std::string key;
  Any value;
};


class Any::Array : public Mysqlx::Datatypes::Array {
 public:
  Array() = default;
  Array(std::initializer_list<Any> list) {
    for (const Any &e : list) add_value()->CopyFrom(e);
  }
};

class Placeholder {
 public:
  explicit Placeholder(const ::google::protobuf::uint32 v) : value(v) {}
  const ::google::protobuf::uint32 value;
};

class Variable {
 public:
  Variable(const std::string &name)  // NOLINT(runtime/explicit)
      : value(name) {}
  const std::string value;
};

class Expr : public Mysqlx::Expr::Expr {
 public:
  Expr() {}

  template <typename T>
  Expr(T value) {    // NOLINT(runtime/explicit)
    Expr::initialize(this, value);
  }

  static void initialize(Mysqlx::Expr::Expr *expr, const Scalar &value);
  static void initialize(Mysqlx::Expr::Expr *expr, Operator *oper);
  static void initialize(Mysqlx::Expr::Expr *expr, const Operator &oper);
  static void initialize(Mysqlx::Expr::Expr *expr, const Identifier &ident);
  static void initialize(Mysqlx::Expr::Expr *expr, FunctionCall *func);
  static void initialize(Mysqlx::Expr::Expr *expr, const FunctionCall &func);
  static void initialize(Mysqlx::Expr::Expr *expr, ColumnIdentifier *id);
  static void initialize(Mysqlx::Expr::Expr *expr, const ColumnIdentifier &id);
  static void initialize(Mysqlx::Expr::Expr *expr, Object *obj);
  static void initialize(Mysqlx::Expr::Expr *expr, const Object &obj);
  static void initialize(Mysqlx::Expr::Expr *expr, Array *arr);
  static void initialize(Mysqlx::Expr::Expr *expr, const Array &arr);
  static void initialize(Mysqlx::Expr::Expr *expr, const Placeholder &ph);
  static void initialize(Mysqlx::Expr::Expr *expr, const Variable &var);
};

class Operator : public Mysqlx::Expr::Operator {
 public:
  Operator(const std::string &name) {   // NOLINT(runtime/explicit)
    set_name(name);
  }

  template <typename T1>
  Operator(const std::string &name, T1 param1) {
    set_name(name);
    add_param(param1);
  }

  template <typename T1, typename T2>
  Operator(const std::string &name, T1 param1, T2 param2) {
    set_name(name);
    add_param(param1);
    add_param(param2);
  }

  template <typename T1, typename T2, typename T3>
  Operator(const std::string &name, T1 param1, T2 param2, T3 param3) {
    set_name(name);
    add_param(param1);
    add_param(param2);
    add_param(param3);
  }

  template <typename T1, typename T2, typename T3, typename T4>
  Operator(const std::string &name, T1 param1, T2 param2, T3 param3,
           T4 param4) {
    set_name(name);
    add_param(param1);
    add_param(param2);
    add_param(param3);
    add_param(param4);
  }

  void add_param(Expr *value) { mutable_param()->AddAllocated(value); }

  void add_param(const Expr &value) { mutable_param()->Add()->CopyFrom(value); }
};

class FunctionCall : public Mysqlx::Expr::FunctionCall {
 public:
  FunctionCall(Identifier *name) {    // NOLINT(runtime/explicit)
    set_allocated_name(name); }

  FunctionCall(const Identifier &name) {   // NOLINT(runtime/explicit)
    mutable_name()->CopyFrom(name); }

  template <typename T1>
  FunctionCall(Identifier *name, T1 param1) {
    initialize(name, param1);
  }

  template <typename T1, typename T2>
  FunctionCall(Identifier *name, T1 param1, T2 param2) {
    initialize(name, param1);
    add_param(param2);
  }

  FunctionCall(const std::string &name) {  // NOLINT(runtime/explicit)
    set_allocated_name(new Identifier(name));
  }

  template <typename T1>
  FunctionCall(const Identifier &name, T1 param1) {
    mutable_name()->CopyFrom(name);
    add_param(param1);
  }

  template <typename T1, typename T2>
  FunctionCall(const Identifier &name, T1 param1, T2 param2) {
    mutable_name()->CopyFrom(name);
    add_param(param1);
    add_param(param2);
  }

  template <typename T1, typename T2, typename T3>
  FunctionCall(const Identifier &name, T1 param1, T2 param2, T3 param3) {
    mutable_name()->CopyFrom(name);
    add_param(param1);
    add_param(param2);
    add_param(param3);
  }

  template <typename T1>
  FunctionCall(const std::string &name, T1 param1) {
    initialize(new Identifier(name), param1);
  }

  template <typename T1, typename T2>
  FunctionCall(const std::string &name, T1 param1, T2 param2) {
    initialize(new Identifier(name), param1);
    add_param(param2);
  }

  template <typename T1, typename T2, typename T3>
  FunctionCall(const std::string &name, T1 param1, T2 param2, T3 param3) {
    initialize(new Identifier(name), param1);
    add_param(param2);
    add_param(param3);
  }

 private:
  template <typename T1>
  void initialize(Identifier *name, T1 param1) {
    set_allocated_name(name);
    add_param(param1);
  }

  template <typename T>
  void add_param(T value) {
    Expr::initialize(Mysqlx::Expr::FunctionCall::add_param(), value);
  }

  void add_param(Expr *value) { mutable_param()->AddAllocated(value); }
};

class Object : public Mysqlx::Expr::Object {
 public:
  struct Fld {
    std::string key;
    Expr value;
  };

  Object() {}
  Object(const std::string &key, Expr *value);
  Object(std::initializer_list<Fld> list);
};

class Array : public Mysqlx::Expr::Array {
 public:
  Array() {}
  Array(std::initializer_list<Expr> list) {
    for (const Expr &e : list) *mutable_value()->Add() = e;
  }
};

class Column : public ::Mysqlx::Crud::Column {
 public:
  Column(const std::string &name,
         const std::string &alias = "");  // NOLINT(runtime/explicit)
  Column(const Document_path &path, const std::string &name = "",
         const std::string &alias = "");  // NOLINT(runtime/explicit)
};

class Collection : public ::Mysqlx::Crud::Collection {
 public:
  Collection(const std::string &name,
             const std::string &schema = "");  // NOLINT(runtime/explicit)
};

typedef ::Mysqlx::Crud::DataModel Data_model;

class Projection : public ::Mysqlx::Crud::Projection {
 public:
  Projection(const Expr &source,
             const std::string &alias = "");  // NOLINT(runtime/explicit)
};

class Order : public ::Mysqlx::Crud::Order {
 public:
  Order(const Expr &expr, const ::Mysqlx::Crud::Order_Direction dir =
                              ::Mysqlx::Crud::Order_Direction_ASC);
};

class Limit : public ::Mysqlx::Crud::Limit {
 public:
  Limit(const uint64_t row_count = 0,
        const uint64_t offset = 0);   // NOLINT(runtime/explicit)
};

template <typename B, typename T>
class RepeatedPtrField : public ::google::protobuf::RepeatedPtrField<B> {
 public:
  RepeatedPtrField() {}
  RepeatedPtrField(const T &arg) {  // NOLINT(runtime/explicit)
    *this->Add() = arg;
  }
  RepeatedPtrField(std::initializer_list<T> list) {
    for (const T &e : list) *this->Add() = e;
  }
};

using Filter = Expr;
using Group = Expr;
using Grouping_criteria = Expr;
using Column_projection_list = RepeatedPtrField<Mysqlx::Crud::Column, Column>;
using Grouping_list = RepeatedPtrField<Mysqlx::Expr::Expr, Group>;
using Expression_args = RepeatedPtrField<Mysqlx::Datatypes::Scalar, Scalar>;
using Field_list = RepeatedPtrField<Mysqlx::Expr::Expr, Expr>;
using Order_list = RepeatedPtrField<Mysqlx::Crud::Order, Order>;
using Projection_list = RepeatedPtrField<Mysqlx::Crud::Projection, Projection>;
using Value_list = RepeatedPtrField<Mysqlx::Expr::Expr, Expr>;

class Row_list : public ::google::protobuf::RepeatedPtrField<
                     Mysqlx::Crud::Insert_TypedRow> {
 public:
  Row_list() {}
  Row_list(std::initializer_list<Value_list> list) {
    for (const Value_list &v : list) *this->Add()->mutable_field() = v;
  }
};

}  // namespace test
}  // namespace xpl

#endif  // _XPL_MYSQLX_PB_WRAPPER_H_
