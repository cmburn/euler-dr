/* SPDX-License-Identifier: ISC */

#include "euler/runtime/dragonruby_state.h"

#include <vector>

#include <dragonruby.h>

#undef alloca

using euler::runtime::DragonRubyState;

DragonRubyState::MRuby::MRuby(mrb_state *mrb, drb_api_t *api)
    : mrb(mrb)
    , _api(*api)
{
}

void
DragonRubyState::MRuby::raise(RClass *c, const char *msg)
{
	_api.mrb_raise(mrb, c, msg);
}

void
DragonRubyState::MRuby::raisef(RClass *c, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	auto value = vformat(fmt, ap);
	va_end(ap);
	const char *message = string_cstr(value);
	raise(c, message);
}

RClass *
DragonRubyState::MRuby::module_get(const char *name)
{
	return _api.mrb_module_get(mrb, name);
}

RClass *
DragonRubyState::MRuby::module_get_under(RClass *outer, const char *name)
{
	return _api.mrb_module_get_under(mrb, outer, name);
}

RClass *
DragonRubyState::MRuby::define_module_under(RClass *outer, const char *name)
{
	return _api.mrb_define_module_under(mrb, outer, name);
}

RClass *
DragonRubyState::MRuby::class_get_under(RClass *outer, const char *name)
{
	return _api.mrb_class_get_under(mrb, outer, name);
}

RClass *
DragonRubyState::MRuby::define_class_under(RClass *outer, const char *name,
    RClass *super)
{
	return _api.mrb_define_class_under(mrb, outer, name, super);
}

void
DragonRubyState::MRuby::define_module_function(RClass *cla, const char *name,
    mrb_func_t fun, mrb_aspec aspec)
{
	return _api.mrb_define_module_function(mrb, cla, name, fun, aspec);
}

void
DragonRubyState::MRuby::define_method(RClass *cla, const char *name,
    mrb_func_t func, mrb_aspec aspec)
{
	return _api.mrb_define_method(mrb, cla, name, func, aspec);
}

void
DragonRubyState::MRuby::define_class_method(RClass *cla, const char *name,
    mrb_func_t fun, mrb_aspec aspec)
{
	return _api.mrb_define_class_method(mrb, cla, name, fun, aspec);
}

mrb_int
DragonRubyState::MRuby::get_args(mrb_args_format format, ...)
{
	/* TODO, this gets messy */
}

mrb_value
DragonRubyState::MRuby::str_new_cstr(const char *str)
{
	return _api.mrb_str_new_cstr(mrb, str);
}

RData *
DragonRubyState::MRuby::data_object_alloc(RClass *klass, void *datap,
    const mrb_data_type *type)
{
	return _api.mrb_data_object_alloc(mrb, klass, datap, type);
}

mrb_value
DragonRubyState::MRuby::Float(mrb_value val)
{
	return _api.mrb_Float(mrb, val);
}

mrb_value
DragonRubyState::MRuby::Integer(mrb_value val)
{
	return _api.mrb_Integer(mrb, val);
}

mrb_irep *
DragonRubyState::MRuby::add_irep()
{
	return _api.mrb_add_irep(mrb);
}

void
DragonRubyState::MRuby::alias_method(RClass *c, mrb_sym a, mrb_sym b)
{
	_api.mrb_alias_method(mrb, c, a, b);
}

mrb_value
DragonRubyState::MRuby::any_to_s(mrb_value obj)
{
	return _api.mrb_any_to_s(mrb, obj);
}

void
DragonRubyState::MRuby::argnum_error(mrb_int argc, int min, int max)
{
	_api.mrb_argnum_error(mrb, argc, min, max);
}

mrb_value
DragonRubyState::MRuby::ary_clear(mrb_value self)
{
	return _api.mrb_ary_clear(mrb, self);
}

void
DragonRubyState::MRuby::ary_concat(mrb_value self, mrb_value other)
{
	_api.mrb_ary_concat(mrb, self, other);
}

mrb_value
DragonRubyState::MRuby::ary_entry(mrb_value ary, mrb_int offset)
{
	return _api.mrb_ary_entry(ary, offset);
}

mrb_value
DragonRubyState::MRuby::ary_join(mrb_value ary, mrb_value sep)
{
	return _api.mrb_ary_join(mrb, ary, sep);
}

void
DragonRubyState::MRuby::ary_modify(RArray *ary)
{
	_api.mrb_ary_modify(mrb, ary);
}

mrb_value
DragonRubyState::MRuby::ary_new()
{
	return _api.mrb_ary_new(mrb);
}

mrb_value
DragonRubyState::MRuby::ary_new_capa(mrb_int capa)
{
	return _api.mrb_ary_new_capa(mrb, capa);
}

mrb_value
DragonRubyState::MRuby::ary_new_from_values(mrb_int size, const mrb_value *vals)
{
	return _api.mrb_ary_new_from_values(mrb, size, vals);
}

mrb_value
DragonRubyState::MRuby::ary_pop(mrb_value ary)
{
	return _api.mrb_ary_pop(mrb, ary);
}

void
DragonRubyState::MRuby::ary_push(mrb_value array, mrb_value value)
{
	_api.mrb_ary_push(mrb, array, value);
}

mrb_value
DragonRubyState::MRuby::ary_ref(mrb_value ary, mrb_int n)
{
	return _api.mrb_ary_ref(mrb, ary, n);
}

void
DragonRubyState::MRuby::ary_replace(mrb_value self, mrb_value other)
{
	return _api.mrb_ary_replace(mrb, self, other);
}

mrb_value
DragonRubyState::MRuby::ary_resize(mrb_value ary, mrb_int new_len)
{
	return _api.mrb_ary_resize(mrb, ary, new_len);
}

void
DragonRubyState::MRuby::ary_set(mrb_value ary, mrb_int n, mrb_value val)
{
	return _api.mrb_ary_set(mrb, ary, n, val);
}

mrb_value
DragonRubyState::MRuby::ary_shift(mrb_value self)
{
	return _api.mrb_ary_shift(mrb, self);
}

mrb_value
DragonRubyState::MRuby::ary_splat(mrb_value value)
{
	return _api.mrb_ary_splat(mrb, value);
}

mrb_value
DragonRubyState::MRuby::ary_splice(mrb_value self, mrb_int head, mrb_int len,
    mrb_value rpl)
{
	return _api.mrb_ary_splice(mrb, self, head, len, rpl);
}

mrb_value
DragonRubyState::MRuby::ary_unshift(mrb_value self, mrb_value item)
{
	return _api.mrb_ary_unshift(mrb, self, item);
}

mrb_value
DragonRubyState::MRuby::assoc_new(mrb_value car, mrb_value cdr)
{
	return _api.mrb_assoc_new(mrb, car, cdr);
}

mrb_value
DragonRubyState::MRuby::attr_get(mrb_value obj, mrb_sym id)
{
	return _api.mrb_attr_get(mrb, obj, id);
}

void
DragonRubyState::MRuby::bug(const char *fmt, ...)
{
	_api.mrb_bug(mrb, fmt);
}

void *
DragonRubyState::MRuby::calloc(size_t count, size_t size)
{
	return _api.mrb_calloc(mrb, count, size);
}

mrb_value
DragonRubyState::MRuby::check_array_type(mrb_value self)
{
	return _api.mrb_check_array_type(mrb, self);
}

mrb_value
DragonRubyState::MRuby::check_hash_type(mrb_value hash)
{
	return _api.mrb_check_hash_type(mrb, hash);
}

mrb_value
DragonRubyState::MRuby::check_intern(const char *str, size_t len)
{
	return _api.mrb_check_intern(mrb, str, len);
}

mrb_value
DragonRubyState::MRuby::check_intern_cstr(const char *str)
{
	return _api.mrb_check_intern_cstr(mrb, str);
}

mrb_value
DragonRubyState::MRuby::check_intern_str(mrb_value str)
{
	return _api.mrb_check_intern_str(mrb, str);
}

mrb_value
DragonRubyState::MRuby::check_string_type(mrb_value str)
{
	return _api.mrb_check_string_type(mrb, str);
}

void
DragonRubyState::MRuby::check_type(mrb_value x, mrb_vtype t)
{
	_api.mrb_check_type(mrb, x, t);
}

mrb_bool
DragonRubyState::MRuby::class_defined(const char *name)
{
	return _api.mrb_class_defined(mrb, name);
}

mrb_bool
DragonRubyState::MRuby::class_defined_id(mrb_sym name)
{
	return _api.mrb_class_defined_id(mrb, name);
}

mrb_bool
DragonRubyState::MRuby::class_defined_under(RClass *outer, const char *name)
{
	return _api.mrb_class_defined_under(mrb, outer, name);
}

mrb_bool
DragonRubyState::MRuby::class_defined_under_id(RClass *outer, mrb_sym name)
{
	return _api.mrb_class_defined_under_id(mrb, outer, name);
}

RClass *
DragonRubyState::MRuby::class_get(const char *name)
{
	return _api.mrb_class_get(mrb, name);
}

RClass *
DragonRubyState::MRuby::class_get_id(mrb_sym name)
{
	return _api.mrb_class_get_id(mrb, name);
}

RClass *
DragonRubyState::MRuby::class_get_under_id(RClass *outer, mrb_sym name)
{
	return _api.mrb_class_get_under_id(mrb, outer, name);
}

const char *
DragonRubyState::MRuby::class_name(RClass *klass)
{
	return _api.mrb_class_name(mrb, klass);
}

RClass *
DragonRubyState::MRuby::class_new(RClass *super)
{
	return _api.mrb_class_new(mrb, super);
}

mrb_value
DragonRubyState::MRuby::class_path(RClass *c)
{
	return _api.mrb_class_path(mrb, c);
}

RClass *
DragonRubyState::MRuby::class_real(RClass *cl)
{
	return _api.mrb_class_real(cl);
}

void
DragonRubyState::MRuby::close()
{
	return _api.mrb_close(mrb);
}

RProc *
DragonRubyState::MRuby::closure_new_cfunc(mrb_func_t func, int nlocals)
{
	return _api.mrb_closure_new_cfunc(mrb, func, nlocals);
}

mrb_int
DragonRubyState::MRuby::cmp(mrb_value obj1, mrb_value obj2)
{
	return _api.mrb_cmp(mrb, obj1, obj2);
}

mrb_bool
DragonRubyState::MRuby::const_defined(mrb_value value, mrb_sym sym)
{
	return _api.mrb_const_defined(mrb, value, sym);
}

mrb_bool
DragonRubyState::MRuby::const_defined_at(mrb_value mod, mrb_sym id)
{
	return _api.mrb_const_defined_at(mrb, mod, id);
}

mrb_value
DragonRubyState::MRuby::const_get(mrb_value value, mrb_sym sym)
{
	return _api.mrb_const_get(mrb, value, sym);
}

void
DragonRubyState::MRuby::const_remove(mrb_value value, mrb_sym sym)
{
	_api.mrb_const_remove(mrb, value, sym);
}

void
DragonRubyState::MRuby::const_set(mrb_value value, mrb_sym sym,
    mrb_value new_value)
{
	_api.mrb_const_set(mrb, value, sym, new_value);
}

mrb_value
DragonRubyState::MRuby::convert_to_integer(mrb_value val, mrb_int base)
{
	return _api.mrb_convert_to_integer(mrb, val, base);
}

double
DragonRubyState::MRuby::cstr_to_dbl(const char *s, mrb_bool badcheck)
{
	return _api.mrb_cstr_to_dbl(mrb, s, badcheck);
}

mrb_value
DragonRubyState::MRuby::cstr_to_inum(const char *s, mrb_int base,
    mrb_bool badcheck)
{
	return _api.mrb_cstr_to_inum(mrb, s, base, badcheck);
}

mrb_bool
DragonRubyState::MRuby::cv_defined(mrb_value mod, mrb_sym sym)
{
	return _api.mrb_cv_defined(mrb, mod, sym);
}

mrb_value
DragonRubyState::MRuby::cv_get(mrb_value mod, mrb_sym sym)
{
	return _api.mrb_cv_get(mrb, mod, sym);
}

void
DragonRubyState::MRuby::cv_set(mrb_value mod, mrb_sym sym, mrb_value v)
{
	_api.mrb_cv_set(mrb, mod, sym, v);
}

void *
DragonRubyState::MRuby::data_check_get_ptr(mrb_value value,
    const mrb_data_type *type)
{
	return _api.mrb_data_check_get_ptr(mrb, value, type);
}

void
DragonRubyState::MRuby::data_check_type(mrb_value value,
    const mrb_data_type *type)
{
	return _api.mrb_data_check_type(mrb, value, type);
}

void *
DragonRubyState::MRuby::data_get_ptr(mrb_value value, const mrb_data_type *type)
{
	return _api.mrb_data_get_ptr(mrb, value, type);
}

const char *
DragonRubyState::MRuby::debug_get_filename(const mrb_irep *irep, uint32_t pc)
{
	return _api.mrb_debug_get_filename(mrb, irep, pc);
}

int32_t
DragonRubyState::MRuby::debug_get_line(const mrb_irep *irep, uint32_t pc)
{
	return _api.mrb_debug_get_line(mrb, irep, pc);
}

mrb_irep_debug_info *
DragonRubyState::MRuby::debug_info_alloc(mrb_irep *irep)
{
	return _api.mrb_debug_info_alloc(mrb, irep);
}

mrb_irep_debug_info_file *
DragonRubyState::MRuby::debug_info_append_file(mrb_irep_debug_info *info,
    const char *filename, uint16_t *lines, uint32_t start_pos, uint32_t end_pos)
{
	return _api.mrb_debug_info_append_file(mrb, info, filename, lines,
	    start_pos, end_pos);
}

void
DragonRubyState::MRuby::debug_info_free(mrb_irep_debug_info *d)
{
	_api.mrb_debug_info_free(mrb, d);
}

void *
DragonRubyState::MRuby::default_allocf(void *data, size_t size, void *ptr)
{
	return _api.mrb_default_allocf(mrb, data, size, ptr);
}

void
DragonRubyState::MRuby::define_alias(RClass *c, const char *a, const char *b)
{
	_api.mrb_define_alias(mrb, c, a, b);
}

void
DragonRubyState::MRuby::define_alias_id(RClass *c, mrb_sym a, mrb_sym b)
{
	_api.mrb_define_alias_id(mrb, c, a, b);
}

RClass *
DragonRubyState::MRuby::define_class(const char *name, RClass *super)
{
	return _api.mrb_define_class(mrb, name, super);
}

RClass *
DragonRubyState::MRuby::define_class_id(mrb_sym name, RClass *super)
{
	return _api.mrb_define_class_id(mrb, name, super);
}

void
DragonRubyState::MRuby::define_class_method_id(RClass *cla, mrb_sym name,
    mrb_func_t fun, mrb_aspec aspec)
{
	return _api.mrb_define_class_method_id(mrb, cla, name, fun, aspec);
}

RClass *
DragonRubyState::MRuby::define_class_under_id(RClass *outer, mrb_sym name,
    RClass *super)
{
	return _api.mrb_define_class_under_id(mrb, outer, name, super);
}

void
DragonRubyState::MRuby::define_const(RClass *cla, const char *name,
    mrb_value val)
{
	_api.mrb_define_const(mrb, cla, name, val);
}

void
DragonRubyState::MRuby::define_const_id(RClass *cla, mrb_sym name,
    mrb_value val)
{
	_api.mrb_define_const_id(mrb, cla, name, val);
}

void
DragonRubyState::MRuby::define_global_const(const char *name, mrb_value val)
{
	_api.mrb_define_global_const(mrb, name, val);
}

void
DragonRubyState::MRuby::define_method_id(RClass *c, mrb_sym mid,
    mrb_func_t func, mrb_aspec aspec)
{
	_api.mrb_define_method_id(mrb, c, mid, func, aspec);
}

void
DragonRubyState::MRuby::define_method_raw(RClass *c, mrb_sym mid, mrb_method_t meth)
{
	_api.mrb_define_method_raw(mrb, c, mid, meth);
}

RClass *
DragonRubyState::MRuby::define_module(const char *name)
{
	_api.mrb_define_module(mrb, name);
}

void
DragonRubyState::MRuby::define_module_function_id(RClass *cla, mrb_sym name,
    mrb_func_t fun, mrb_aspec aspec)
{
	_api.mrb_define_module_function_id(mrb, cla, name, fun, aspec);
}

RClass *
DragonRubyState::MRuby::define_module_id(mrb_sym name)
{
	return _api.mrb_define_module_id(mrb, name);
}

RClass *
DragonRubyState::MRuby::define_module_under_id(RClass *outer, mrb_sym name)
{
	return _api.mrb_define_module_under_id(mrb, outer, name);
}

void
DragonRubyState::MRuby::define_singleton_method(RObject *cla, const char *name,
    mrb_func_t fun, mrb_aspec aspec)
{
	_api.mrb_define_singleton_method(mrb, cla, name, fun, aspec);
}

void
DragonRubyState::MRuby::define_singleton_method_id(RObject *cla, mrb_sym name,
    mrb_func_t fun, mrb_aspec aspec)
{
	_api.mrb_define_singleton_method_id(mrb, cla, name, fun, aspec);
}

mrb_value
DragonRubyState::MRuby::ensure(mrb_func_t body, mrb_value b_data,
    mrb_func_t ensure, mrb_value e_data)
{
	return _api.mrb_ensure(mrb, body, b_data, ensure, e_data);
}

mrb_value
DragonRubyState::MRuby::ensure_array_type(mrb_value self)
{
	return _api.mrb_ensure_array_type(mrb, self);
}

mrb_value
DragonRubyState::MRuby::ensure_hash_type(mrb_value hash)
{
	return _api.mrb_ensure_hash_type(mrb, hash);
}

mrb_value
DragonRubyState::MRuby::ensure_string_type(mrb_value str)
{
	return _api.mrb_ensure_string_type(mrb, str);
}

mrb_bool
DragonRubyState::MRuby::eql(mrb_value obj1, mrb_value obj2)
{
	return _api.mrb_eql(mrb, obj1, obj2);
}

mrb_bool
DragonRubyState::MRuby::equal(mrb_value obj1, mrb_value obj2)
{
	return _api.mrb_equal(mrb, obj1, obj2);
}

mrb_value
DragonRubyState::MRuby::exc_backtrace(mrb_value exc)
{
	 return _api.mrb_exc_backtrace(mrb, exc);
}

RClass *
DragonRubyState::MRuby::exc_get_id(mrb_sym name)
{
	return _api.mrb_exc_get_id(mrb, name);
}

mrb_value
DragonRubyState::MRuby::exc_new(RClass *c, const char *ptr, size_t len)
{
	 return _api.mrb_exc_new(mrb, c, ptr, len);
}

mrb_value
DragonRubyState::MRuby::exc_new_str(RClass *c, mrb_value str)
{
	return _api.mrb_exc_new_str(mrb, c, str);
}

void
DragonRubyState::MRuby::exc_raise(mrb_value exc)
{
	return _api.mrb_exc_raise(mrb, exc);
}

mrb_value
DragonRubyState::MRuby::f_raise(mrb_value val)
{
	return _api.mrb_f_raise(mrb, val);
}

mrb_value
DragonRubyState::MRuby::fiber_alive_p(mrb_value fib)
{
	return _api.mrb_fiber_alive_p(mrb, fib);
}

mrb_value
DragonRubyState::MRuby::fiber_resume(mrb_value fib, mrb_int argc,
    const mrb_value *argv)
{
	return _api.mrb_fiber_resume(mrb, fib, argc, argv);
}

mrb_value
DragonRubyState::MRuby::fiber_yield(mrb_int argc, const mrb_value *argv)
{
	return _api.mrb_fiber_yield(mrb, argc, argv);
}

void
DragonRubyState::MRuby::field_write_barrier(RBasic *b1, RBasic * b2)
{
	_api.mrb_field_write_barrier(mrb, b1, b2);
}

mrb_value
DragonRubyState::MRuby::fixnum_to_str(mrb_value x, mrb_int base)
{
	return _api.mrb_fixnum_to_str(mrb, x, base);
}

mrb_value
DragonRubyState::MRuby::flo_to_fixnum(mrb_value val)
{
	return _api.mrb_flo_to_fixnum(mrb, val);
}

double
DragonRubyState::MRuby::float_read(const char *str, char **endptr)
{
	return _api.mrb_float_read(str, endptr);
}

int
DragonRubyState::MRuby::float_to_cstr(char *buf, size_t len, const char *fmt,
    mrb_float f)
{
	return _api.mrb_float_to_cstr(mrb, buf, len, fmt, f);
}

mrb_value
DragonRubyState::MRuby::float_to_str(mrb_value x, const char *fmt)
{
	return _api.mrb_float_to_str(mrb, x, fmt);
}

mrb_value
DragonRubyState::MRuby::format(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	auto value = vformat(format, ap);
	va_end(ap);
	return str_new_cstr(string_cstr(value));
}

void
DragonRubyState::MRuby::free(void *ptr)
{
	_api.mrb_free(mrb, ptr);
}

void
DragonRubyState::MRuby::free_context(mrb_context *c)
{
	_api.mrb_free_context(mrb, c);
}

void
DragonRubyState::MRuby::frozen_error(void *frozen_obj)
{
	_api.mrb_frozen_error(mrb, frozen_obj);
}

void
DragonRubyState::MRuby::full_gc()
{
	_api.mrb_full_gc(mrb);
}

mrb_bool
DragonRubyState::MRuby::func_basic_p(mrb_value obj, mrb_sym mid,
    mrb_func_t func)
{
	return _api.mrb_func_basic_p(mrb, obj, mid, func);
}

mrb_value
DragonRubyState::MRuby::funcall(mrb_value val, const char *name, mrb_int argc,
    ...)
{
	va_list ap;
	va_start(ap, argc);
	std::vector<mrb_value> argv;
	argv.reserve(argc);
	for (mrb_int i = 0; i < argc; ++i) {
		argv.push_back(va_arg(ap, mrb_value));
	}
	va_end(ap);
	mrb_sym sym_name = intern_cstr(name);
	return _api.mrb_funcall_argv(mrb, val, name, argc, argv.data());
}

mrb_value
DragonRubyState::MRuby::funcall_argv(mrb_value val, mrb_sym name, mrb_int argc,
    const mrb_value *argv)
{
}

mrb_value
DragonRubyState::MRuby::funcall_id(mrb_value val, mrb_sym mid, mrb_int argc,
    ...)
{
}

mrb_value
DragonRubyState::MRuby::funcall_with_block(mrb_value val, mrb_sym name,
    mrb_int argc, const mrb_value *argv, mrb_value block)
{
}

void
DragonRubyState::MRuby::garbage_collect()
{
}

void
DragonRubyState::MRuby::gc_mark(RBasic *)
{
}

void
DragonRubyState::MRuby::gc_protect(mrb_value obj)
{
}

void
DragonRubyState::MRuby::gc_register(mrb_value obj)
{
}

void
DragonRubyState::MRuby::gc_unregister(mrb_value obj)
{
}

RProc *
DragonRubyState::MRuby::generate_code(mrb_parser_state *)
{
}

mrb_value
DragonRubyState::MRuby::get_arg1()
{
}

mrb_int
DragonRubyState::MRuby::get_argc()
{
}

const mrb_value *
DragonRubyState::MRuby::get_argv()
{
}

mrb_value
DragonRubyState::MRuby::get_backtrace()
{
}

mrb_value
DragonRubyState::MRuby::gv_get(mrb_sym sym)
{
}

void
DragonRubyState::MRuby::gv_remove(mrb_sym sym)
{
}

void
DragonRubyState::MRuby::gv_set(mrb_sym sym, mrb_value val)
{
}

mrb_value
DragonRubyState::MRuby::hash_clear(mrb_value hash)
{
}

mrb_value
DragonRubyState::MRuby::hash_delete_key(mrb_value hash, mrb_value key)
{
}

mrb_value
DragonRubyState::MRuby::hash_dup(mrb_value hash)
{
}

mrb_bool
DragonRubyState::MRuby::hash_empty_p(mrb_value self)
{
}

mrb_value
DragonRubyState::MRuby::hash_fetch(mrb_value hash, mrb_value key, mrb_value def)
{
}

void
DragonRubyState::MRuby::hash_foreach(RHash *hash, mrb_hash_foreach_func *func,
    void *p)
{
}

mrb_value
DragonRubyState::MRuby::hash_get(mrb_value hash, mrb_value key)
{
}

mrb_bool
DragonRubyState::MRuby::hash_key_p(mrb_value hash, mrb_value key)
{
}

mrb_value
DragonRubyState::MRuby::hash_keys(mrb_value hash)
{
}

void
DragonRubyState::MRuby::hash_merge(mrb_value hash1, mrb_value hash2)
{
}

mrb_value
DragonRubyState::MRuby::hash_new()
{
}

mrb_value
DragonRubyState::MRuby::hash_new_capa(mrb_int capa)
{
}

void
DragonRubyState::MRuby::hash_set(mrb_value hash, mrb_value key, mrb_value val)
{
}

mrb_int
DragonRubyState::MRuby::hash_size(mrb_value hash)
{
}

mrb_value
DragonRubyState::MRuby::hash_values(mrb_value hash)
{
}

void
DragonRubyState::MRuby::include_module(RClass *cla, RClass *included)
{
}

void
DragonRubyState::MRuby::incremental_gc()
{
}

mrb_value
DragonRubyState::MRuby::inspect(mrb_value obj)
{
}

mrb_value
DragonRubyState::MRuby::instance_new(mrb_value cv)
{
}

mrb_sym
DragonRubyState::MRuby::intern(const char *, size_t)
{
}

mrb_sym
DragonRubyState::MRuby::intern_check(const char *, size_t)
{
}

mrb_sym
DragonRubyState::MRuby::intern_check_cstr(const char *)
{
}

mrb_sym
DragonRubyState::MRuby::intern_check_str(mrb_value)
{
}

mrb_sym
DragonRubyState::MRuby::intern_cstr(const char *str)
{
}

mrb_sym
DragonRubyState::MRuby::intern_static(const char *, size_t)
{
}

mrb_sym
DragonRubyState::MRuby::intern_str(mrb_value)
{
}

void
DragonRubyState::MRuby::iv_copy(mrb_value dst, mrb_value src)
{
}

mrb_bool
DragonRubyState::MRuby::iv_defined(mrb_value, mrb_sym)
{
}

void
DragonRubyState::MRuby::iv_foreach(mrb_value obj, mrb_iv_foreach_func *func,
    void *p)
{
}

mrb_value
DragonRubyState::MRuby::iv_get(mrb_value obj, mrb_sym sym)
{
}

void
DragonRubyState::MRuby::iv_name_sym_check(mrb_sym sym)
{
}

mrb_bool
DragonRubyState::MRuby::iv_name_sym_p(mrb_sym sym)
{
}

mrb_value
DragonRubyState::MRuby::iv_remove(mrb_value obj, mrb_sym sym)
{
}

void
DragonRubyState::MRuby::iv_set(mrb_value obj, mrb_sym sym, mrb_value v)
{
}

mrb_value
DragonRubyState::MRuby::load_detect_file_cxt(FILE *fp, mrbc_context *c)
{
}

mrb_value
DragonRubyState::MRuby::load_exec(mrb_parser_state *p, mrbc_context *c)
{
}

mrb_value
DragonRubyState::MRuby::load_file(FILE *)
{
}

mrb_value
DragonRubyState::MRuby::load_file_cxt(FILE *, mrbc_context *cxt)
{
}

mrb_value
DragonRubyState::MRuby::load_irep(const uint8_t *)
{
}

mrb_value
DragonRubyState::MRuby::load_irep_buf(const void *, size_t)
{
}

mrb_value
DragonRubyState::MRuby::load_irep_buf_cxt(const void *, size_t, mrbc_context *)
{
}

mrb_value
DragonRubyState::MRuby::load_irep_cxt(const uint8_t *, mrbc_context *)
{
}

mrb_value
DragonRubyState::MRuby::load_irep_file(FILE *)
{
}

mrb_value
DragonRubyState::MRuby::load_irep_file_cxt(FILE *, mrbc_context *)
{
}

mrb_value
DragonRubyState::MRuby::load_nstring(const char *s, size_t len)
{
}

mrb_value
DragonRubyState::MRuby::load_nstring_cxt(const char *s, size_t len,
    mrbc_context *cxt)
{
}

mrb_value
DragonRubyState::MRuby::load_proc(const RProc *proc)
{
}

mrb_value
DragonRubyState::MRuby::load_string(const char *s)
{
}

mrb_value
DragonRubyState::MRuby::load_string_cxt(const char *s, mrbc_context *cxt)
{
}

mrb_value
DragonRubyState::MRuby::make_exception(mrb_int argc, const mrb_value *argv)
{
}

void *
DragonRubyState::MRuby::malloc(size_t)
{
}

void *
DragonRubyState::MRuby::malloc_simple(size_t)
{
}

mrb_method_t
DragonRubyState::MRuby::method_search(RClass *, mrb_sym)
{
}

mrb_method_t
DragonRubyState::MRuby::method_search_vm(RClass **, mrb_sym)
{
}

void
DragonRubyState::MRuby::mod_cv_set(RClass *c, mrb_sym sym, mrb_value v)
{
}

RClass *
DragonRubyState::MRuby::module_get_id(mrb_sym name)
{
}

RClass *
DragonRubyState::MRuby::module_get_under_id(RClass *outer, mrb_sym name)
{
}

RClass *
DragonRubyState::MRuby::module_new()
{
}

void
DragonRubyState::MRuby::mt_foreach(RClass *, mrb_mt_foreach_func *, void *)
{
}

void
DragonRubyState::MRuby::name_error(mrb_sym id, const char *fmt, ...)
{
}

void
DragonRubyState::MRuby::no_method_error(mrb_sym id, mrb_value args,
    const char *fmt, ...)
{
}

void
DragonRubyState::MRuby::notimplement()
{
}

mrb_value
DragonRubyState::MRuby::notimplement_m(mrb_value)
{
}

mrb_value
DragonRubyState::MRuby::num_minus(mrb_value x, mrb_value y)
{
}

mrb_value
DragonRubyState::MRuby::num_mul(mrb_value x, mrb_value y)
{
}

mrb_value
DragonRubyState::MRuby::num_plus(mrb_value x, mrb_value y)
{
}

RBasic *
DragonRubyState::MRuby::obj_alloc(mrb_vtype, RClass *)
{
}

mrb_value
DragonRubyState::MRuby::obj_as_string(mrb_value obj)
{
}

RClass *
DragonRubyState::MRuby::obj_class(mrb_value obj)
{
}

const char *
DragonRubyState::MRuby::obj_classname(mrb_value obj)
{
}

mrb_value
DragonRubyState::MRuby::obj_clone(mrb_value self)
{
}

mrb_value
DragonRubyState::MRuby::obj_dup(mrb_value obj)
{
}

mrb_bool
DragonRubyState::MRuby::obj_eq(mrb_value a, mrb_value b)
{
}

mrb_bool
DragonRubyState::MRuby::obj_equal(mrb_value a, mrb_value b)
{
}

mrb_value
DragonRubyState::MRuby::obj_freeze(mrb_value)
{
}

mrb_int
DragonRubyState::MRuby::obj_id(mrb_value obj)
{
}

mrb_value
DragonRubyState::MRuby::obj_inspect(mrb_value self)
{
}

mrb_bool
DragonRubyState::MRuby::obj_is_instance_of(mrb_value obj, RClass *c)
{
}

mrb_bool
DragonRubyState::MRuby::obj_is_kind_of(mrb_value obj, RClass *c)
{
}

mrb_bool
DragonRubyState::MRuby::obj_iv_defined(RObject *obj, mrb_sym sym)
{
}

mrb_value
DragonRubyState::MRuby::obj_iv_get(RObject *obj, mrb_sym sym)
{
}

void
DragonRubyState::MRuby::obj_iv_set(RObject *obj, mrb_sym sym, mrb_value v)
{
}

mrb_value
DragonRubyState::MRuby::obj_new(RClass *c, mrb_int argc, const mrb_value *argv)
{
}

mrb_bool
DragonRubyState::MRuby::obj_respond_to(RClass *c, mrb_sym mid)
{
}

mrb_sym
DragonRubyState::MRuby::obj_to_sym(mrb_value name)
{
}

mrb_bool
DragonRubyState::MRuby::object_dead_p(RBasic *object)
{
}

mrb_state *
DragonRubyState::MRuby::open_allocf(mrb_allocf f, void *ud)
{
}

mrb_state *
DragonRubyState::MRuby::open_core(mrb_allocf f, void *ud)
{
}

void
DragonRubyState::MRuby::p(mrb_value)
{
}

mrb_parser_state *
DragonRubyState::MRuby::parse_file(FILE *, mrbc_context *)
{
}

mrb_parser_state *
DragonRubyState::MRuby::parse_nstring(const char *, size_t, mrbc_context *)
{
}

mrb_parser_state *
DragonRubyState::MRuby::parse_string(const char *, mrbc_context *)
{
}

void
DragonRubyState::MRuby::parser_free(mrb_parser_state *)
{
}

mrb_sym
DragonRubyState::MRuby::parser_get_filename(mrb_parser_state *, uint16_t idx)
{
}

mrb_parser_state *
DragonRubyState::MRuby::parser_new()
{
}

void
DragonRubyState::MRuby::parser_parse(mrb_parser_state *, mrbc_context *)
{
}

void
DragonRubyState::MRuby::parser_set_filename(mrb_parser_state *, const char *)
{
}

void *
DragonRubyState::MRuby::pool_alloc(mrb_pool *, size_t)
{
}

mrb_bool
DragonRubyState::MRuby::pool_can_realloc(mrb_pool *, void *, size_t)
{
}

void
DragonRubyState::MRuby::pool_close(mrb_pool *)
{
}

mrb_pool *
DragonRubyState::MRuby::pool_open()
{
}

void *
DragonRubyState::MRuby::pool_realloc(mrb_pool *, void *, size_t oldlen,
    size_t newlen)
{
}

void
DragonRubyState::MRuby::prepend_module(RClass *cla, RClass *prepended)
{
}

void
DragonRubyState::MRuby::print_backtrace()
{
}

void
DragonRubyState::MRuby::print_error()
{
}

mrb_value
DragonRubyState::MRuby::proc_cfunc_env_get(mrb_int idx)
{
}

RProc *
DragonRubyState::MRuby::proc_new_cfunc(mrb_func_t)
{
}

RProc *
DragonRubyState::MRuby::proc_new_cfunc_with_env(mrb_func_t func, mrb_int argc,
    const mrb_value *argv)
{
}

mrb_value
DragonRubyState::MRuby::protect(mrb_func_t body, mrb_value data,
    mrb_bool *state)
{
}

mrb_value
DragonRubyState::MRuby::ptr_to_str(void *p)
{
}

enum mrb_range_beg_len
DragonRubyState::MRuby::range_beg_len(mrb_value range, mrb_int *begp,
    mrb_int *lenp, mrb_int len, mrb_bool trunc)
{
}

mrb_value
DragonRubyState::MRuby::range_new(mrb_value start, mrb_value end,
    mrb_bool exclude)
{
}

RRange *
DragonRubyState::MRuby::range_ptr(mrb_value range)
{
}

mrb_irep *
DragonRubyState::MRuby::read_irep(const uint8_t *)
{
}

mrb_irep *
DragonRubyState::MRuby::read_irep_buf(const void *, size_t)
{
}

void *
DragonRubyState::MRuby::realloc(void *, size_t)
{
}

void *
DragonRubyState::MRuby::realloc_simple(void *, size_t)
{
}

void
DragonRubyState::MRuby::remove_method(RClass *c, mrb_sym sym)
{
}

mrb_value
DragonRubyState::MRuby::rescue(mrb_func_t body, mrb_value b_data,
    mrb_func_t rescue, mrb_value r_data)
{
}

mrb_value
DragonRubyState::MRuby::rescue_exceptions(mrb_func_t body, mrb_value b_data,
    mrb_func_t rescue, mrb_value r_data, mrb_int len, RClass **classes)
{
}

mrb_bool
DragonRubyState::MRuby::respond_to(mrb_value obj, mrb_sym mid)
{
}

void
DragonRubyState::MRuby::show_copyright()
{
}

void
DragonRubyState::MRuby::show_version()
{
}

mrb_value
DragonRubyState::MRuby::singleton_class(mrb_value val)
{
}

RClass *
DragonRubyState::MRuby::singleton_class_ptr(mrb_value val)
{
}

void
DragonRubyState::MRuby::stack_extend(mrb_int)
{
}

void
DragonRubyState::MRuby::state_atexit(mrb_atexit_func func)
{
}

mrb_value
DragonRubyState::MRuby::str_append(mrb_value str, mrb_value str2)
{
}

mrb_value
DragonRubyState::MRuby::str_cat(mrb_value str, const char *ptr, size_t len)
{
}

mrb_value
DragonRubyState::MRuby::str_cat_cstr(mrb_value str, const char *ptr)
{
}

mrb_value
DragonRubyState::MRuby::str_cat_str(mrb_value str, mrb_value str2)
{
}

int
DragonRubyState::MRuby::str_cmp(mrb_value str1, mrb_value str2)
{
}

void
DragonRubyState::MRuby::str_concat(mrb_value self, mrb_value other)
{
}

mrb_value
DragonRubyState::MRuby::str_dup(mrb_value str)
{
}

mrb_bool
DragonRubyState::MRuby::str_equal(mrb_value str1, mrb_value str2)
{
}

mrb_int
DragonRubyState::MRuby::str_index(mrb_value str, const char *p, mrb_int len,
    mrb_int offset)
{
}

mrb_value
DragonRubyState::MRuby::str_intern(mrb_value self)
{
}

void
DragonRubyState::MRuby::str_modify(RString *s)
{
}

void
DragonRubyState::MRuby::str_modify_keep_ascii(RString *s)
{
}

mrb_value
DragonRubyState::MRuby::str_new(const char *p, size_t len)
{
}

mrb_value
DragonRubyState::MRuby::str_new_capa(size_t capa)
{
}

mrb_value
DragonRubyState::MRuby::str_new_static(const char *p, size_t len)
{
}

mrb_value
DragonRubyState::MRuby::str_plus(mrb_value a, mrb_value b)
{
}

mrb_value
DragonRubyState::MRuby::str_resize(mrb_value str, mrb_int len)
{
}

mrb_int
DragonRubyState::MRuby::str_strlen(RString *)
{
}

mrb_value
DragonRubyState::MRuby::str_substr(mrb_value str, mrb_int beg, mrb_int len)
{
}

char *
DragonRubyState::MRuby::str_to_cstr(mrb_value str)
{
}

double
DragonRubyState::MRuby::str_to_dbl(mrb_value str, mrb_bool badcheck)
{
}

mrb_value
DragonRubyState::MRuby::str_to_inum(mrb_value str, mrb_int base,
    mrb_bool badcheck)
{
}

const char *
DragonRubyState::MRuby::string_cstr(mrb_value str)
{
}

mrb_value
DragonRubyState::MRuby::string_type(mrb_value str)
{
}

const char *
DragonRubyState::MRuby::string_value_cstr(mrb_value *str)
{
}

mrb_int
DragonRubyState::MRuby::string_value_len(mrb_value str)
{
}

const char *
DragonRubyState::MRuby::string_value_ptr(mrb_value str)
{
}

const char *
DragonRubyState::MRuby::sym_dump(mrb_sym)
{
}

const char *
DragonRubyState::MRuby::sym_name(mrb_sym)
{
}

const char *
DragonRubyState::MRuby::sym_name_len(mrb_sym, mrb_int *)
{
}

mrb_value
DragonRubyState::MRuby::sym_str(mrb_sym)
{
}

void
DragonRubyState::MRuby::sys_fail(const char *mesg)
{
}

mrb_float
DragonRubyState::MRuby::to_flo(mrb_value x)
{
}

mrb_value
DragonRubyState::MRuby::to_int(mrb_value val)
{
}

mrb_value
DragonRubyState::MRuby::to_str(mrb_value val)
{
}

mrb_value
DragonRubyState::MRuby::top_run(const RProc *proc, mrb_value self,
    mrb_int stack_keep)
{
}

mrb_value
DragonRubyState::MRuby::top_self()
{
}

mrb_value
DragonRubyState::MRuby::type_convert(mrb_value val, mrb_vtype type,
    mrb_sym method)
{
}

mrb_value
DragonRubyState::MRuby::type_convert_check(mrb_value val, mrb_vtype type,
    mrb_sym method)
{
}

void
DragonRubyState::MRuby::undef_class_method(RClass *cls, const char *name)
{
}

void
DragonRubyState::MRuby::undef_class_method_id(RClass *cls, mrb_sym name)
{
}

void
DragonRubyState::MRuby::undef_method(RClass *cla, const char *name)
{
}

void
DragonRubyState::MRuby::undef_method_id(RClass *, mrb_sym)
{
}

mrb_value
DragonRubyState::MRuby::vformat(const char *format, va_list ap)
{
}

mrb_value
DragonRubyState::MRuby::vm_const_get(mrb_sym)
{
}

void
DragonRubyState::MRuby::vm_const_set(mrb_sym, mrb_value)
{
}

mrb_value
DragonRubyState::MRuby::vm_cv_get(mrb_sym)
{
}

void
DragonRubyState::MRuby::vm_cv_set(mrb_sym, mrb_value)
{
}

RClass *
DragonRubyState::MRuby::vm_define_class(mrb_value, mrb_value, mrb_sym)
{
}

RClass *
DragonRubyState::MRuby::vm_define_module(mrb_value, mrb_sym)
{
}

mrb_value
DragonRubyState::MRuby::vm_exec(const RProc *proc, const mrb_code *iseq)
{
}

mrb_value
DragonRubyState::MRuby::vm_run(const RProc *proc, mrb_value self,
    mrb_int stack_keep)
{
}

mrb_value
DragonRubyState::MRuby::vm_special_get(mrb_sym)
{
}

void
DragonRubyState::MRuby::vm_special_set(mrb_sym, mrb_value)
{
}

void
DragonRubyState::MRuby::warn(const char *fmt, ...)
{
}

mrb_value
DragonRubyState::MRuby::word_boxing_cptr_value(void *)
{
}

mrb_value
DragonRubyState::MRuby::word_boxing_float_value(mrb_float)
{
}

mrb_value
DragonRubyState::MRuby::word_boxing_int_value(mrb_int)
{
}

void
DragonRubyState::MRuby::write_barrier(RBasic *)
{
}

mrb_value
DragonRubyState::MRuby::yield(mrb_value b, mrb_value arg)
{
}

mrb_value
DragonRubyState::MRuby::yield_argv(mrb_value b, mrb_int argc,
    const mrb_value *argv)
{
}

mrb_value
DragonRubyState::MRuby::yield_with_class(mrb_value b, mrb_int argc,
    const mrb_value *argv, mrb_value self, RClass *c)
{
}

mrb_value
DragonRubyState::MRuby::obj_value(void *p)
{
}

mrb_value
DragonRubyState::MRuby::int_value(mrb_int i)
{
}

mrb_value
DragonRubyState::MRuby::float_value(mrb_float f)
{
}

mrb_value
DragonRubyState::MRuby::symbol_value(mrb_sym i)
{
}

