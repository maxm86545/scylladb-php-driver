/**
 * Copyright 2015-2017 DataStax, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "php_driver.h"
#include "php_driver_types.h"
#include "util/ref.h"
#include "util/result.h"
#include "util/types.h"

#include "DefaultFunction.h"

zend_class_entry *php_driver_default_function_ce = NULL;

php5to7_zval
php_driver_create_function(php_driver_ref* schema,
                              const CassFunctionMeta *meta TSRMLS_DC)
{
  php5to7_zval result;
  php_driver_function *function;
  const char *full_name;
  size_t full_name_length;

  PHP5TO7_ZVAL_UNDEF(result);

  PHP5TO7_ZVAL_MAYBE_MAKE(result);
  object_init_ex(PHP5TO7_ZVAL_MAYBE_P(result), php_driver_default_function_ce);

  function = PHP_DRIVER_GET_FUNCTION(PHP5TO7_ZVAL_MAYBE_P(result));
  function->schema = php_driver_add_ref(schema);
  function->meta   = meta;

  cass_function_meta_full_name(function->meta, &full_name, &full_name_length);
  PHP5TO7_ZVAL_MAYBE_MAKE(function->signature);
  PHP5TO7_ZVAL_STRINGL(PHP5TO7_ZVAL_MAYBE_P(function->signature), full_name, full_name_length);

  return result;
}

PHP_METHOD(DefaultFunction, name)
{
  php_driver_function *self;

  if (zend_parse_parameters_none() == FAILURE)
    return;

  self = PHP_DRIVER_GET_FUNCTION(getThis());

  RETURN_ZVAL(PHP5TO7_ZVAL_MAYBE_P(self->signature), 1, 0);
}

PHP_METHOD(DefaultFunction, simpleName)
{
  php_driver_function *self;

  if (zend_parse_parameters_none() == FAILURE)
    return;

  self = PHP_DRIVER_GET_FUNCTION(getThis());
  if (PHP5TO7_ZVAL_IS_UNDEF(self->simple_name)) {
    const char *name;
    size_t name_length;
    cass_function_meta_name(self->meta, &name, &name_length);
    PHP5TO7_ZVAL_MAYBE_MAKE(self->simple_name);
    PHP5TO7_ZVAL_STRINGL(PHP5TO7_ZVAL_MAYBE_P(self->simple_name), name, name_length);
  }

  RETURN_ZVAL(PHP5TO7_ZVAL_MAYBE_P(self->simple_name), 1, 0);
}

PHP_METHOD(DefaultFunction, arguments)
{
  php_driver_function *self;

  if (zend_parse_parameters_none() == FAILURE)
    return;

  self = PHP_DRIVER_GET_FUNCTION(getThis());
  if (PHP5TO7_ZVAL_IS_UNDEF(self->arguments)) {
    size_t i, count = cass_function_meta_argument_count(self->meta);
    PHP5TO7_ZVAL_MAYBE_MAKE(self->arguments);
    array_init(PHP5TO7_ZVAL_MAYBE_P(self->arguments));
    for (i = 0; i < count; ++i) {
      const char *name;
      size_t name_length;
      const CassDataType* data_type;
      if (cass_function_meta_argument(self->meta, i, &name, &name_length, &data_type) == CASS_OK) {
        php5to7_zval type = php_driver_type_from_data_type(data_type TSRMLS_CC);
        if (!PHP5TO7_ZVAL_IS_UNDEF(type)) {
          PHP5TO7_ADD_ASSOC_ZVAL_EX(PHP5TO7_ZVAL_MAYBE_P(self->arguments),
                                    name, name_length + 1,
                                    PHP5TO7_ZVAL_MAYBE_P(type));
        }
      }
    }
  }

  RETURN_ZVAL(PHP5TO7_ZVAL_MAYBE_P(self->arguments), 1, 0);
}

PHP_METHOD(DefaultFunction, returnType)
{
  php_driver_function *self;

  if (zend_parse_parameters_none() == FAILURE)
    return;

  self = PHP_DRIVER_GET_FUNCTION(getThis());
  if (PHP5TO7_ZVAL_IS_UNDEF(self->return_type)) {
    const CassDataType* data_type = cass_function_meta_return_type(self->meta);
    if (!data_type) {
      return;
    }
    self->return_type = php_driver_type_from_data_type(data_type TSRMLS_CC);
  }

  RETURN_ZVAL(PHP5TO7_ZVAL_MAYBE_P(self->return_type), 1, 0);
}

PHP_METHOD(DefaultFunction, signature)
{
  php_driver_function *self;

  if (zend_parse_parameters_none() == FAILURE)
    return;

  self = PHP_DRIVER_GET_FUNCTION(getThis());
  RETURN_ZVAL(PHP5TO7_ZVAL_MAYBE_P(self->signature), 1, 0);
}

PHP_METHOD(DefaultFunction, language)
{
  php_driver_function *self;

  if (zend_parse_parameters_none() == FAILURE)
    return;

  self = PHP_DRIVER_GET_FUNCTION(getThis());
  if (PHP5TO7_ZVAL_IS_UNDEF(self->language)) {
    const char *language;
    size_t language_length;
    cass_function_meta_language(self->meta, &language, &language_length);
    PHP5TO7_ZVAL_MAYBE_MAKE(self->language);
    PHP5TO7_ZVAL_STRINGL(PHP5TO7_ZVAL_MAYBE_P(self->language), language, language_length);
  }

  RETURN_ZVAL(PHP5TO7_ZVAL_MAYBE_P(self->language), 1, 0);
}

PHP_METHOD(DefaultFunction, body)
{
  php_driver_function *self;

  if (zend_parse_parameters_none() == FAILURE)
    return;

  self = PHP_DRIVER_GET_FUNCTION(getThis());
  if (PHP5TO7_ZVAL_IS_UNDEF(self->body)) {
    const char *body;
    size_t body_length;
    cass_function_meta_body(self->meta, &body, &body_length);
    PHP5TO7_ZVAL_MAYBE_MAKE(self->body);
    PHP5TO7_ZVAL_STRINGL(PHP5TO7_ZVAL_MAYBE_P(self->body), body, body_length);
  }

  RETURN_ZVAL(PHP5TO7_ZVAL_MAYBE_P(self->body), 1, 0);
}

PHP_METHOD(DefaultFunction, isCalledOnNullInput)
{
  php_driver_function *self;

  if (zend_parse_parameters_none() == FAILURE)
    return;

  self = PHP_DRIVER_GET_FUNCTION(getThis());
  RETURN_BOOL((int)cass_function_meta_called_on_null_input(self->meta));
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_none, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_driver_default_function_methods[] = {
  PHP_ME(DefaultFunction, name, arginfo_none, ZEND_ACC_PUBLIC)
  PHP_ME(DefaultFunction, simpleName, arginfo_none, ZEND_ACC_PUBLIC)
  PHP_ME(DefaultFunction, arguments, arginfo_none, ZEND_ACC_PUBLIC)
  PHP_ME(DefaultFunction, returnType, arginfo_none, ZEND_ACC_PUBLIC)
  PHP_ME(DefaultFunction, signature, arginfo_none, ZEND_ACC_PUBLIC)
  PHP_ME(DefaultFunction, language, arginfo_none, ZEND_ACC_PUBLIC)
  PHP_ME(DefaultFunction, body, arginfo_none, ZEND_ACC_PUBLIC)
  PHP_ME(DefaultFunction, isCalledOnNullInput, arginfo_none, ZEND_ACC_PUBLIC)
  PHP_FE_END
};

static zend_object_handlers php_driver_default_function_handlers;

static HashTable *
php_driver_type_default_function_gc(
#if PHP_MAJOR_VERSION >= 8
        zend_object *object,
#else
        zval *object,
#endif
        php5to7_zval_gc table, int *n TSRMLS_DC
)
{
  *table = NULL;
  *n = 0;
  return zend_std_get_properties(object TSRMLS_CC);
}

static HashTable *
php_driver_default_function_properties(
#if PHP_MAJOR_VERSION >= 8
        zend_object *object
#else
        zval *object TSRMLS_DC
#endif
)
{
  HashTable *props = zend_std_get_properties(object TSRMLS_CC);

  return props;
}

static int
php_driver_default_function_compare(zval *obj1, zval *obj2 TSRMLS_DC)
{
#if PHP_MAJOR_VERSION >= 8
  ZEND_COMPARE_OBJECTS_FALLBACK(obj1, obj2);
#endif
  if (Z_OBJCE_P(obj1) != Z_OBJCE_P(obj2))
    return 1; /* different classes */

  return Z_OBJ_HANDLE_P(obj1) != Z_OBJ_HANDLE_P(obj1);
}

static void
php_driver_default_function_free(php5to7_zend_object_free *object TSRMLS_DC)
{
  php_driver_function *self = PHP5TO7_ZEND_OBJECT_GET(function, object);

  PHP5TO7_ZVAL_MAYBE_DESTROY(self->simple_name);
  PHP5TO7_ZVAL_MAYBE_DESTROY(self->arguments);
  PHP5TO7_ZVAL_MAYBE_DESTROY(self->return_type);
  PHP5TO7_ZVAL_MAYBE_DESTROY(self->signature);
  PHP5TO7_ZVAL_MAYBE_DESTROY(self->language);
  PHP5TO7_ZVAL_MAYBE_DESTROY(self->body);

  if (self->schema) {
    php_driver_del_ref(&self->schema);
    self->schema = NULL;
  }
  self->meta = NULL;

  zend_object_std_dtor(&self->zval TSRMLS_CC);
  PHP5TO7_MAYBE_EFREE(self);
}

static php5to7_zend_object
php_driver_default_function_new(zend_class_entry *ce TSRMLS_DC)
{
  php_driver_function *self =
      PHP5TO7_ZEND_OBJECT_ECALLOC(function, ce);

  PHP5TO7_ZVAL_UNDEF(self->simple_name);
  PHP5TO7_ZVAL_UNDEF(self->arguments);
  PHP5TO7_ZVAL_UNDEF(self->return_type);
  PHP5TO7_ZVAL_UNDEF(self->signature);
  PHP5TO7_ZVAL_UNDEF(self->language);
  PHP5TO7_ZVAL_UNDEF(self->body);

  self->schema = NULL;
  self->meta = NULL;

  PHP5TO7_ZEND_OBJECT_INIT_EX(function, default_function, self, ce);
}

void php_driver_define_DefaultFunction(TSRMLS_D)
{
  zend_class_entry ce;

  INIT_CLASS_ENTRY(ce, PHP_DRIVER_NAMESPACE "\\DefaultFunction", php_driver_default_function_methods);
  php_driver_default_function_ce = zend_register_internal_class(&ce TSRMLS_CC);
  zend_class_implements(php_driver_default_function_ce TSRMLS_CC, 1, php_driver_function_ce);
  php_driver_default_function_ce->ce_flags     |= PHP5TO7_ZEND_ACC_FINAL;
  php_driver_default_function_ce->create_object = php_driver_default_function_new;

  memcpy(&php_driver_default_function_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  php_driver_default_function_handlers.get_properties  = php_driver_default_function_properties;
#if PHP_VERSION_ID >= 50400
  php_driver_default_function_handlers.get_gc          = php_driver_type_default_function_gc;
#endif
#if PHP_MAJOR_VERSION >= 8
  php_driver_default_function_handlers.compare = php_driver_default_function_compare;
#else
  php_driver_default_function_handlers.compare_objects = php_driver_default_function_compare;
#endif
  php_driver_default_function_handlers.clone_obj = NULL;
}
