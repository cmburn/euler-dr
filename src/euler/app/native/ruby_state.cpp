/* SPDX-License-Identifier: ISC */

#include "euler/app/native/ruby_state.h"

#include <cassert>
#include <vector>

#include <mruby.h>
#include <mruby/boxing_word.h>
#include <mruby/dump.h>
#include <mruby/error.h>
#include <mruby/internal.h>
#include <mruby/irep.h>
#include <mruby/numeric.h>
#include <mruby/object.h>
#include <mruby/proc.h>
#include <mruby/variable.h>

using euler::app::native::RubyState;

mrb_state *
RubyState::mrb() const
{
	return _mrb;
}

void
RubyState::raise(RClass *c, const char *msg)
{
	mrb_raise(_mrb, c, msg);
}

void
RubyState::raisef(RClass *c, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	const auto value = vformat(fmt, ap);
	va_end(ap);
	const char *message = string_cstr(value);
	raise(c, message);
}

RClass *
RubyState::module_get(const char *name)
{
	return mrb_module_get(_mrb, name);
}

RClass *
RubyState::module_get_under(RClass *outer, const char *name)
{
	return mrb_module_get_under(_mrb, outer, name);
}

RClass *
RubyState::define_module_under(RClass *outer, const char *name)
{
	return mrb_define_module_under(_mrb, outer, name);
}

RClass *
RubyState::class_get_under(RClass *outer, const char *name)
{
	return mrb_class_get_under(_mrb, outer, name);
}

mrb_bool
RubyState::class_ptr_p(const mrb_value obj)
{
	switch (mrb_type(obj)) {
	case MRB_TT_CLASS:
	case MRB_TT_SCLASS:
	case MRB_TT_MODULE: return true;
	default: return false;
	}
}

RClass *
RubyState::define_class_under(RClass *outer, const char *name, RClass *super)
{
	return mrb_define_class_under(_mrb, outer, name, super);
}

void
RubyState::define_module_function(RClass *cla, const char *name,
    const mrb_func_t fun, const mrb_aspec aspec)
{
	mrb_define_module_function(_mrb, cla, name, fun, aspec);
}

void
RubyState::define_method(RClass *cla, const char *name, const mrb_func_t func,
    const mrb_aspec aspec)
{
	mrb_define_method(_mrb, cla, name, func, aspec);
}

void
RubyState::define_class_method(RClass *cla, const char *name,
    const mrb_func_t fun, const mrb_aspec aspec)
{
	mrb_define_class_method(_mrb, cla, name, fun, aspec);
}

mrb_int
RubyState::get_args(const mrb_args_format format, ...)
{
	std::vector<void *> args;
	va_list ap;
	va_start(ap, format);
	const auto argc = get_argc();
	for (mrb_int i = 0; i < argc; ++i) args.push_back(va_arg(ap, void *));
	va_end(ap);
	return get_args_a(format, args.data());
}

mrb_value
RubyState::str_new_cstr(const char *str)
{
	return mrb_str_new_cstr(_mrb, str);
}

RData *
RubyState::data_object_alloc(RClass *klass, void *datap,
    const mrb_data_type *type)
{
	return mrb_data_object_alloc(_mrb, klass, datap, type);
}

mrb_value
RubyState::ensure_float_type(const mrb_value val)
{
	return mrb_ensure_float_type(_mrb, val);
}

mrb_value
RubyState::ensure_integer_type(const mrb_value val)
{
	return mrb_ensure_integer_type(_mrb, val);
}

mrb_irep *
RubyState::add_irep()
{
	return mrb_add_irep(_mrb);
}

void
RubyState::alias_method(RClass *c, const mrb_sym a, const mrb_sym b)
{
	mrb_alias_method(_mrb, c, a, b);
}

mrb_value
RubyState::any_to_s(const mrb_value obj)
{
	return mrb_any_to_s(_mrb, obj);
}

void
RubyState::argnum_error(const mrb_int argc, const int min, const int max)
{
	mrb_argnum_error(_mrb, argc, min, max);
}

mrb_value
RubyState::ary_clear(const mrb_value self)
{
	return mrb_ary_clear(_mrb, self);
}

void
RubyState::ary_concat(const mrb_value self, const mrb_value other)
{
	mrb_ary_concat(_mrb, self, other);
}

mrb_value
RubyState::ary_entry(const mrb_value ary, const mrb_int offset)
{
	return mrb_ary_entry(ary, offset);
}

mrb_value
RubyState::ary_join(const mrb_value ary, const mrb_value sep)
{
	return mrb_ary_join(_mrb, ary, sep);
}

void
RubyState::ary_modify(RArray *ary)
{
	mrb_ary_modify(_mrb, ary);
}

mrb_value
RubyState::ary_new()
{
	return mrb_ary_new(_mrb);
}

mrb_value
RubyState::ary_new_capa(const mrb_int n)
{
	return mrb_ary_new_capa(_mrb, n);
}

mrb_value
RubyState::ary_new_from_values(const mrb_int size, const mrb_value *vals)
{
	return mrb_ary_new_from_values(_mrb, size, vals);
}

mrb_value
RubyState::ary_pop(const mrb_value ary)
{
	return mrb_ary_pop(_mrb, ary);
}

void
RubyState::ary_push(const mrb_value array, const mrb_value value)
{
	mrb_ary_push(_mrb, array, value);
}

mrb_value
RubyState::ary_ref(const mrb_value ary, const mrb_int n)
{
	return mrb_ary_ref(_mrb, ary, n);
}

void
RubyState::ary_replace(const mrb_value self, const mrb_value other)
{
	mrb_ary_replace(_mrb, self, other);
}

mrb_value
RubyState::ary_resize(const mrb_value ary, const mrb_int new_len)
{
	return mrb_ary_resize(_mrb, ary, new_len);
}

void
RubyState::ary_set(const mrb_value ary, const mrb_int n, const mrb_value val)
{
	mrb_ary_set(_mrb, ary, n, val);
}

mrb_value
RubyState::ary_shift(const mrb_value self)
{
	return mrb_ary_shift(_mrb, self);
}

mrb_value
RubyState::ary_splat(const mrb_value value)
{
	return mrb_ary_splat(_mrb, value);
}

mrb_value
RubyState::ary_splice(const mrb_value self, const mrb_int head,
    const mrb_int len, const mrb_value rpl)
{
	return mrb_ary_splice(_mrb, self, head, len, rpl);
}

mrb_value
RubyState::ary_unshift(const mrb_value self, const mrb_value item)
{
	return mrb_ary_unshift(_mrb, self, item);
}

mrb_value
RubyState::assoc_new(const mrb_value car, const mrb_value cdr)
{
	return mrb_assoc_new(_mrb, car, cdr);
}

mrb_value
RubyState::attr_get(const mrb_value obj, const mrb_sym id)
{
	return mrb_attr_get(_mrb, obj, id);
}

void
RubyState::bug(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	const auto message = vformat(fmt, ap);
	va_end(ap);
	const auto cstr = string_cstr(message);
	mrb_bug(_mrb, cstr);
}

void *
RubyState::calloc(const size_t n, const size_t size)
{
	return mrb_calloc(_mrb, n, size);
}

mrb_value
RubyState::check_array_type(const mrb_value self)
{
	return mrb_check_array_type(_mrb, self);
}

mrb_value
RubyState::check_hash_type(const mrb_value hash)
{
	return mrb_check_hash_type(_mrb, hash);
}

mrb_value
RubyState::check_intern(const char *str, const size_t len)
{
	return mrb_check_intern(_mrb, str, len);
}

mrb_value
RubyState::check_intern_cstr(const char *str)
{
	return mrb_check_intern_cstr(_mrb, str);
}

mrb_value
RubyState::check_intern_str(const mrb_value str)
{
	return mrb_check_intern_str(_mrb, str);
}

mrb_value
RubyState::check_string_type(const mrb_value str)
{
	return mrb_check_string_type(_mrb, str);
}

void
RubyState::check_type(const mrb_value x, const mrb_vtype t)
{
	mrb_check_type(_mrb, x, t);
}

mrb_bool
RubyState::class_defined(const char *name)
{
	return mrb_class_defined(_mrb, name);
}

mrb_bool
RubyState::class_defined_id(const mrb_sym name)
{
	return mrb_class_defined_id(_mrb, name);
}

mrb_bool
RubyState::class_defined_under(RClass *outer, const char *name)
{
	return mrb_class_defined_under(_mrb, outer, name);
}

mrb_bool
RubyState::class_defined_under_id(RClass *outer, const mrb_sym name)
{
	return mrb_class_defined_under_id(_mrb, outer, name);
}

RClass *
RubyState::class_get(const char *name)
{
	return mrb_class_get(_mrb, name);
}

RClass *
RubyState::class_get_id(const mrb_sym name)
{
	return mrb_class_get_id(_mrb, name);
}

RClass *
RubyState::class_get_under_id(RClass *outer, const mrb_sym name)
{
	return mrb_class_get_under_id(_mrb, outer, name);
}

const char *
RubyState::class_name(RClass *klass)
{
	return mrb_class_name(_mrb, klass);
}

RClass *
RubyState::class_new(RClass *super)
{
	return mrb_class_new(_mrb, super);
}

mrb_value
RubyState::class_path(RClass *c)
{
	return mrb_class_path(_mrb, c);
}

RClass *
RubyState::class_real(RClass *cl)
{
	return mrb_class_real(cl);
}

void
RubyState::close()
{
	mrb_close(_mrb);
	_mrb = nullptr;
}

RProc *
RubyState::closure_new_cfunc(const mrb_func_t func, const int nlocals)
{
	return mrb_closure_new_cfunc(_mrb, func, nlocals);
}

mrb_int
RubyState::cmp(const mrb_value obj1, const mrb_value obj2)
{
	return mrb_cmp(_mrb, obj1, obj2);
}

mrb_bool
RubyState::const_defined(const mrb_value mod, const mrb_sym id)
{
	return mrb_const_defined(_mrb, mod, id);
}

mrb_bool
RubyState::const_defined_at(const mrb_value mod, const mrb_sym id)
{
	return mrb_const_defined_at(_mrb, mod, id);
}

mrb_value
RubyState::const_get(const mrb_value mod, const mrb_sym id)
{
	return mrb_const_get(_mrb, mod, id);
}

void
RubyState::const_remove(const mrb_value mod, const mrb_sym id)
{
	mrb_const_remove(_mrb, mod, id);
}

void
RubyState::const_set(const mrb_value mod, const mrb_sym id,
    const mrb_value value)
{
	mrb_const_set(_mrb, mod, id, value);
}

double
RubyState::cstr_to_dbl(const char *s, const mrb_bool badcheck)
{
	auto str = mrb_str_new_cstr(_mrb, s);
	return mrb_str_to_dbl(_mrb, str, badcheck);
}

mrb_value
RubyState::cstr_to_inum(const char *s, const mrb_int base,
    const mrb_bool badcheck)
{
	auto str = mrb_str_new_cstr(_mrb, s);
	return mrb_str_to_integer(_mrb, str, base, badcheck);
}

mrb_bool
RubyState::cv_defined(const mrb_value mod, const mrb_sym sym)
{
	return mrb_cv_defined(_mrb, mod, sym);
}

mrb_value
RubyState::cv_get(const mrb_value mod, const mrb_sym sym)
{
	return mrb_cv_get(_mrb, mod, sym);
}

void
RubyState::cv_set(const mrb_value mod, const mrb_sym sym, const mrb_value v)
{
	mrb_cv_set(_mrb, mod, sym, v);
}

void *
RubyState::data_check_get_ptr(const mrb_value value, const mrb_data_type *type)
{
	return mrb_data_check_get_ptr(_mrb, value, type);
}

void
RubyState::data_check_type(const mrb_value value, const mrb_data_type *type)
{
	mrb_data_check_type(_mrb, value, type);
}

void *
RubyState::data_get_ptr(const mrb_value value, const mrb_data_type *type)
{
	return mrb_data_get_ptr(_mrb, value, type);
}

const char *
RubyState::debug_get_filename(const mrb_irep *irep, const uint32_t pc)
{
	return mrb_debug_get_filename(_mrb, irep, pc);
}

int32_t
RubyState::debug_get_line(const mrb_irep *irep, const uint32_t pc)
{
	return mrb_debug_get_line(_mrb, irep, pc);
}

mrb_irep_debug_info *
RubyState::debug_info_alloc(mrb_irep *irep)
{
	return mrb_debug_info_alloc(_mrb, irep);
}

mrb_irep_debug_info_file *
RubyState::debug_info_append_file(mrb_irep_debug_info *info,
    const char *filename, uint16_t *lines, const uint32_t start_pos,
    const uint32_t end_pos)
{
	return mrb_debug_info_append_file(_mrb, info, filename, lines,
	    start_pos, end_pos);
}

void
RubyState::debug_info_free(mrb_irep_debug_info *d)
{
	mrb_debug_info_free(_mrb, d);
}

void *
RubyState::default_allocf(void *data, const size_t size, void *ptr)
{
	return mrb_default_allocf(_mrb, data, size, ptr);
}

void
RubyState::define_alias(RClass *c, const char *a, const char *b)
{
	mrb_define_alias(_mrb, c, a, b);
}

void
RubyState::define_alias_id(RClass *c, const mrb_sym a, const mrb_sym b)
{
	mrb_define_alias_id(_mrb, c, a, b);
}

RClass *
RubyState::define_class(const char *name, RClass *super)
{
	return mrb_define_class(_mrb, name, super);
}

RClass *
RubyState::define_class_id(const mrb_sym name, RClass *super)
{
	return mrb_define_class_id(_mrb, name, super);
}

void
RubyState::define_class_method_id(RClass *cla, const mrb_sym name,
    const mrb_func_t fun, const mrb_aspec aspec)
{
	mrb_define_class_method_id(_mrb, cla, name, fun, aspec);
}

RClass *
RubyState::define_class_under_id(RClass *outer, const mrb_sym name,
    RClass *super)
{
	return mrb_define_class_under_id(_mrb, outer, name, super);
}

void
RubyState::define_const(RClass *cla, const char *name, const mrb_value val)
{
	mrb_define_const(_mrb, cla, name, val);
}

void
RubyState::define_const_id(RClass *cla, const mrb_sym name, const mrb_value val)
{
	mrb_define_const_id(_mrb, cla, name, val);
}

void
RubyState::define_global_const(const char *name, const mrb_value val)
{
	mrb_define_global_const(_mrb, name, val);
}

void
RubyState::define_method_id(RClass *c, const mrb_sym mid, const mrb_func_t func,
    const mrb_aspec aspec)
{
	mrb_define_method_id(_mrb, c, mid, func, aspec);
}

void
RubyState::define_method_raw(RClass *c, const mrb_sym mid,
    const mrb_method_t meth)
{
	mrb_define_method_raw(_mrb, c, mid, meth);
}

RClass *
RubyState::define_module(const char *name)
{
	return mrb_define_module(_mrb, name);
}

void
RubyState::define_module_function_id(RClass *cla, const mrb_sym name,
    const mrb_func_t fun, const mrb_aspec aspec)
{
	mrb_define_module_function_id(_mrb, cla, name, fun, aspec);
}

RClass *
RubyState::define_module_id(const mrb_sym name)
{
	return mrb_define_module_id(_mrb, name);
}

RClass *
RubyState::define_module_under_id(RClass *outer, const mrb_sym name)
{
	return mrb_define_module_under_id(_mrb, outer, name);
}

void
RubyState::define_singleton_method(RObject *cla, const char *name,
    const mrb_func_t fun, const mrb_aspec aspec)
{
	mrb_define_singleton_method(_mrb, cla, name, fun, aspec);
}

void
RubyState::define_singleton_method_id(RObject *cla, const mrb_sym name,
    const mrb_func_t fun, const mrb_aspec aspec)
{
	mrb_define_singleton_method_id(_mrb, cla, name, fun, aspec);
}

mrb_value
RubyState::ensure(const mrb_func_t body, const mrb_value b_data,
    const mrb_func_t ensure, const mrb_value e_data)
{
	return mrb_ensure(_mrb, body, b_data, ensure, e_data);
}

mrb_value
RubyState::ensure_array_type(const mrb_value self)
{
	return mrb_ensure_array_type(_mrb, self);
}

mrb_value
RubyState::ensure_hash_type(const mrb_value hash)
{
	return mrb_ensure_hash_type(_mrb, hash);
}

mrb_value
RubyState::ensure_string_type(const mrb_value str)
{
	return mrb_ensure_string_type(_mrb, str);
}

mrb_bool
RubyState::eql(const mrb_value obj1, const mrb_value obj2)
{
	return mrb_eql(_mrb, obj1, obj2);
}

mrb_bool
RubyState::equal(const mrb_value obj1, const mrb_value obj2)
{
	return mrb_equal(_mrb, obj1, obj2);
}

mrb_value
RubyState::exc_backtrace(const mrb_value exc)
{
	return mrb_exc_backtrace(_mrb, exc);
}

RClass *
RubyState::exc_get_id(const mrb_sym name)
{
	return mrb_exc_get_id(_mrb, name);
}

mrb_value
RubyState::exc_new(RClass *c, const char *ptr, const size_t len)
{
	return mrb_exc_new(_mrb, c, ptr, len);
}

mrb_value
RubyState::exc_new_str(RClass *c, const mrb_value str)
{
	return mrb_exc_new_str(_mrb, c, str);
}

void
RubyState::exc_raise(const mrb_value exc)
{
	mrb_exc_raise(_mrb, exc);
}

mrb_value
RubyState::f_raise(const mrb_value val)
{
	return mrb_f_raise(_mrb, val);
}

mrb_value
RubyState::fiber_alive_p(const mrb_value fib)
{
	return mrb_fiber_alive_p(_mrb, fib);
}

mrb_value
RubyState::fiber_resume(const mrb_value fib, const mrb_int argc,
    const mrb_value *argv)
{
	return mrb_fiber_resume(_mrb, fib, argc, argv);
}

mrb_value
RubyState::fiber_yield(const mrb_int argc, const mrb_value *argv)
{
	return mrb_fiber_yield(_mrb, argc, argv);
}

void
RubyState::field_write_barrier(RBasic *b1, RBasic *b2)
{
	mrb_field_write_barrier(_mrb, b1, b2);
}

mrb_value
RubyState::integer_to_str(const mrb_value x, const mrb_int base)
{
	return mrb_integer_to_str(_mrb, x, base);
}

mrb_value
RubyState::float_to_integer(const mrb_value val)
{
	return mrb_float_to_integer(_mrb, val);
}

double
RubyState::float_read(const char *str, char **endptr)
{
	return mrb_float_read(str, endptr);
}

int
RubyState::float_to_cstr(char *buf, const size_t len, const char *fmt,
    const mrb_float f)
{
	/* no clue why mrb has this as a standalone function, but trying to
	 * keep compatibility with DR where possible */
	return snprintf(buf, len, fmt, f);
}

mrb_value
RubyState::float_to_str(const mrb_value x, const char *fmt)
{
	return mrb_float_to_str(_mrb, x, fmt);
}

mrb_value
RubyState::format(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	const auto value = vformat(format, ap);
	va_end(ap);
	return str_new_cstr(string_cstr(value));
}

void
RubyState::free(void *ptr)
{
	mrb_free(_mrb, ptr);
}

void
RubyState::free_context(mrb_context *c)
{
	mrb_free_context(_mrb, c);
}

void
RubyState::frozen_error(void *frozen_obj)
{
	mrb_frozen_error(_mrb, frozen_obj);
}

void
RubyState::full_gc()
{
	mrb_full_gc(_mrb);
}

mrb_bool
RubyState::func_basic_p(const mrb_value obj, const mrb_sym mid,
    const mrb_func_t func)
{
	return mrb_func_basic_p(_mrb, obj, mid, func);
}

mrb_value
RubyState::funcall(const mrb_value val, const char *name, const mrb_int argc,
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
	return funcall_argv(val, intern_cstr(name), argc, argv.data());
}

mrb_value
RubyState::funcall_argv(const mrb_value val, const mrb_sym name,
    const mrb_int argc, const mrb_value *argv)
{
	return mrb_funcall_argv(_mrb, val, name, argc, argv);
}

mrb_value
RubyState::funcall_id(const mrb_value val, const mrb_sym mid,
    const mrb_int argc, ...)
{
	va_list ap;
	va_start(ap, argc);
	std::vector<mrb_value> argv;
	argv.reserve(argc);
	for (mrb_int i = 0; i < argc; ++i)
		argv.push_back(va_arg(ap, mrb_value));
	return funcall_argv(val, mid, argc, argv.data());
}

mrb_value
RubyState::funcall_with_block(const mrb_value val, const mrb_sym name,
    const mrb_int argc, const mrb_value *argv, const mrb_value block)
{
	return mrb_funcall_with_block(_mrb, val, name, argc, argv, block);
}

void
RubyState::garbage_collect()
{
	mrb_garbage_collect(_mrb);
}

void
RubyState::gc_mark(RBasic *obj)
{
	mrb_gc_mark(_mrb, obj);
}

void
RubyState::gc_protect(const mrb_value obj)
{
	mrb_gc_protect(_mrb, obj);
}

void
RubyState::gc_register(const mrb_value obj)
{
	mrb_gc_register(_mrb, obj);
}

void
RubyState::gc_unregister(const mrb_value obj)
{
	mrb_gc_unregister(_mrb, obj);
}

RProc *
RubyState::generate_code(mrb_parser_state *p)
{
	return mrb_generate_code(_mrb, p);
}

mrb_value
RubyState::get_arg1()
{
	return mrb_get_arg1(_mrb);
}

mrb_int
RubyState::get_argc()
{
	return mrb_get_argc(_mrb);
}

mrb_int
RubyState::get_args_a(const mrb_args_format format, void **ptr)
{
	return mrb_get_args_a(_mrb, format, ptr);
}

const mrb_value *
RubyState::get_argv()
{
	return mrb_get_argv(_mrb);
}

mrb_value
RubyState::get_backtrace()
{
	return mrb_get_backtrace(_mrb);
}

mrb_value
RubyState::gv_get(const mrb_sym sym)
{
	return mrb_gv_get(_mrb, sym);
}

void
RubyState::gv_remove(const mrb_sym sym)
{
	mrb_gv_remove(_mrb, sym);
}

void
RubyState::gv_set(const mrb_sym sym, const mrb_value val)
{
	mrb_gv_set(_mrb, sym, val);
}

void
RubyState::hash_check_kdict(const mrb_value self)
{
	// assert each key is a symbol
	const auto mrb = _mrb;
	const auto keys = hash_keys(self);
	const mrb_int len = RARRAY_LEN(keys);
	for (mrb_int i = 0; i < len; ++i) {
		const auto key = ary_entry(keys, i);
		if (mrb_symbol_p(key)) continue;
		mrb_raise(_mrb, E_ARGUMENT_ERROR,
		    "keyword argument hash with non symbol keys");
	}
}

mrb_value
RubyState::hash_clear(const mrb_value hash)
{
	return mrb_hash_clear(_mrb, hash);
}

mrb_value
RubyState::hash_delete_key(const mrb_value hash, const mrb_value key)
{
	return mrb_hash_delete_key(_mrb, hash, key);
}

mrb_value
RubyState::hash_dup(const mrb_value hash)
{
	return mrb_hash_dup(_mrb, hash);
}

mrb_bool
RubyState::hash_empty_p(const mrb_value self)
{
	return mrb_hash_empty_p(_mrb, self);
}

mrb_value
RubyState::hash_fetch(const mrb_value hash, const mrb_value key,
    const mrb_value def)
{
	return mrb_hash_fetch(_mrb, hash, key, def);
}

void
RubyState::hash_foreach(RHash *hash, mrb_hash_foreach_func *func, void *p)
{
	mrb_hash_foreach(_mrb, hash, func, p);
}

mrb_value
RubyState::hash_get(const mrb_value hash, const mrb_value key)
{
	return mrb_hash_get(_mrb, hash, key);
}

mrb_bool
RubyState::hash_key_p(const mrb_value hash, const mrb_value key)
{
	return mrb_hash_key_p(_mrb, hash, key);
}

mrb_value
RubyState::hash_keys(const mrb_value hash)
{
	return mrb_hash_keys(_mrb, hash);
}

void
RubyState::hash_merge(const mrb_value hash1, const mrb_value hash2)
{
	mrb_hash_merge(_mrb, hash1, hash2);
}

mrb_value
RubyState::hash_new()
{
	return mrb_hash_new(_mrb);
}

mrb_value
RubyState::hash_new_capa(const mrb_int capa)
{
	return mrb_hash_new_capa(_mrb, capa);
}

void
RubyState::hash_set(const mrb_value hash, const mrb_value key,
    const mrb_value val)
{
	mrb_hash_set(_mrb, hash, key, val);
}

mrb_int
RubyState::hash_size(const mrb_value hash)
{
	return mrb_hash_size(_mrb, hash);
}

mrb_value
RubyState::hash_values(const mrb_value hash)
{
	return mrb_hash_values(_mrb, hash);
}

void
RubyState::include_module(RClass *cla, RClass *included)
{
	mrb_include_module(_mrb, cla, included);
}

void
RubyState::incremental_gc()
{
	mrb_incremental_gc(_mrb);
}

mrb_value
RubyState::inspect(const mrb_value obj)
{
	return mrb_inspect(_mrb, obj);
}

mrb_value
RubyState::instance_new(const mrb_value cv)
{
	return mrb_instance_new(_mrb, cv);
}

mrb_sym
RubyState::intern(const char *str, const size_t len)
{
	return mrb_intern(_mrb, str, len);
}

mrb_sym
RubyState::intern_check(const char *str, const size_t len)
{
	return mrb_intern_check(_mrb, str, len);
}

mrb_sym
RubyState::intern_check_cstr(const char *str)
{
	return mrb_intern_check_cstr(_mrb, str);
}

mrb_sym
RubyState::intern_check_str(const mrb_value str)
{
	return mrb_intern_check_str(_mrb, str);
}

mrb_sym
RubyState::intern_cstr(const char *str)
{
	return mrb_intern_cstr(_mrb, str);
}

mrb_sym
RubyState::intern_static(const char *str, const size_t len)
{
	return mrb_intern_static(_mrb, str, len);
}

mrb_sym
RubyState::intern_str(const mrb_value str)
{
	return mrb_intern_str(_mrb, str);
}

void
RubyState::iv_copy(const mrb_value dst, const mrb_value src)
{
	return mrb_iv_copy(_mrb, dst, src);
}

mrb_bool
RubyState::iv_defined(const mrb_value val, const mrb_sym sym)
{
	return mrb_iv_defined(_mrb, val, sym);
}

void
RubyState::iv_foreach(const mrb_value obj, mrb_iv_foreach_func *func, void *p)
{
	mrb_iv_foreach(_mrb, obj, func, p);
}

mrb_value
RubyState::iv_get(const mrb_value obj, const mrb_sym sym)
{
	return mrb_iv_get(_mrb, obj, sym);
}

void
RubyState::iv_name_sym_check(const mrb_sym sym)
{
	mrb_iv_name_sym_check(_mrb, sym);
}

mrb_bool
RubyState::iv_name_sym_p(const mrb_sym sym)
{
	return mrb_iv_name_sym_p(_mrb, sym);
}

mrb_value
RubyState::iv_remove(const mrb_value obj, const mrb_sym sym)
{
	return mrb_iv_remove(_mrb, obj, sym);
}

void
RubyState::iv_set(const mrb_value obj, const mrb_sym sym, const mrb_value v)
{
	mrb_iv_set(_mrb, obj, sym, v);
}

mrb_value
RubyState::load_detect_file_cxt(FILE *fp, mrb_ccontext *c)
{
	return mrb_load_detect_file_cxt(_mrb, fp, c);
}

mrb_value
RubyState::load_exec(mrb_parser_state *p, mrb_ccontext *c)
{
	return mrb_load_exec(_mrb, p, c);
}

mrb_value
RubyState::load_file(FILE *fh)
{
	return mrb_load_file(_mrb, fh);
}

mrb_value
RubyState::load_file_cxt(FILE *fh, mrb_ccontext *cxt)
{
	return mrb_load_file_cxt(_mrb, fh, cxt);
}

mrb_value
RubyState::load_irep(const uint8_t *buf)
{
	return mrb_load_irep(_mrb, buf);
}

mrb_value
RubyState::load_irep_buf(const void *buf, const size_t len)
{
	return mrb_load_irep_buf(_mrb, buf, len);
}

mrb_value
RubyState::load_irep_buf_cxt(const void *buf, const size_t len,
    mrb_ccontext *cxt)
{
	return mrb_load_irep_buf_cxt(_mrb, buf, len, cxt);
}

mrb_value
RubyState::load_irep_cxt(const uint8_t *buf, mrb_ccontext *cxt)
{
	return mrb_load_irep_cxt(_mrb, buf, cxt);
}

mrb_value
RubyState::load_irep_file(FILE *fh)
{
	return mrb_load_irep_file(_mrb, fh);
}

mrb_value
RubyState::load_irep_file_cxt(FILE *fh, mrb_ccontext *cxt)
{
	return mrb_load_irep_file_cxt(_mrb, fh, cxt);
}

mrb_value
RubyState::load_nstring(const char *s, const size_t len)
{
	return mrb_load_nstring(_mrb, s, len);
}

mrb_value
RubyState::load_nstring_cxt(const char *s, const size_t len, mrb_ccontext *cxt)
{
	return mrb_load_nstring_cxt(_mrb, s, len, cxt);
}

mrb_value
RubyState::load_proc(const RProc *proc)
{
	return mrb_load_proc(_mrb, proc);
}

mrb_value
RubyState::load_string(const char *s)
{
	return mrb_load_string(_mrb, s);
}

mrb_value
RubyState::load_string_cxt(const char *s, mrb_ccontext *cxt)
{
	return mrb_load_string_cxt(_mrb, s, cxt);
}

void *
RubyState::malloc(const size_t n)
{
	return mrb_malloc(_mrb, n);
}

void *
RubyState::malloc_simple(const size_t n)
{
	return mrb_malloc_simple(_mrb, n);
}

mrb_method_t
RubyState::method_search(RClass *cls, const mrb_sym sym)
{
	return mrb_method_search(_mrb, cls, sym);
}

mrb_method_t
RubyState::method_search_vm(RClass **cls, const mrb_sym sym)
{
	return mrb_method_search_vm(_mrb, cls, sym);
}

void
RubyState::mod_cv_set(RClass *c, const mrb_sym sym, const mrb_value v)
{
	mrb_mod_cv_set(_mrb, c, sym, v);
}

RClass *
RubyState::module_get_id(const mrb_sym name)
{
	return mrb_module_get_id(_mrb, name);
}

RClass *
RubyState::module_get_under_id(RClass *outer, const mrb_sym name)
{
	return mrb_module_get_under_id(_mrb, outer, name);
}

RClass *
RubyState::module_new()
{
	return mrb_module_new(_mrb);
}

void
RubyState::mt_foreach(RClass *cls, mrb_mt_foreach_func *fn, void *ctx)
{
	mrb_mt_foreach(_mrb, cls, fn, ctx);
}

/*485:mrb_name_error(mrb_state *mrb, mrb_sym id, const char *fmt, ...)
486-{
487-  va_list ap;
488-
489-  va_start(ap, fmt);
490-
491-  mrb_value exc = error_va(mrb, E_NAME_ERROR, fmt, ap);
492-  va_end(ap);
493-  mrb_iv_set(mrb, exc, MRB_IVSYM(name), mrb_symbol_value(id));
494-  mrb_exc_raise(mrb, exc);
495-}
*/
void
RubyState::name_error(const mrb_sym id, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	const auto msg = vformat(fmt, ap);
	va_end(ap);
	const auto mrb = _mrb;
	const auto exc = exc_new_str(E_NAME_ERROR, msg);
	iv_set(exc, intern_cstr("name"), mrb_symbol_value(id));
	exc_raise(exc);
}

void
RubyState::no_method_error(const mrb_sym id, const mrb_value args,
    const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	const auto msg = vformat(fmt, ap);
	va_end(ap);
	const auto mrb = _mrb;
	const auto exc = exc_new_str(E_NOMETHOD_ERROR, msg);
	iv_set(exc, intern_cstr("name"), mrb_symbol_value(id));
	iv_set(exc, intern_cstr("args"), args);
	exc_raise(exc);
}

void
RubyState::notimplement()
{
	mrb_notimplement(_mrb);
}

mrb_value
RubyState::notimplement_m(const mrb_value msg)
{
	return mrb_notimplement_m(_mrb, msg);
}

mrb_value
RubyState::num_minus(const mrb_value x, const mrb_value y)
{
	return mrb_num_sub(_mrb, x, y);
}

mrb_value
RubyState::num_mul(const mrb_value x, const mrb_value y)
{
	return mrb_num_mul(_mrb, x, y);
}

mrb_value
RubyState::num_plus(const mrb_value x, const mrb_value y)
{
	return mrb_num_add(_mrb, x, y);
}

RBasic *
RubyState::obj_alloc(const mrb_vtype vtype, RClass *cls)
{
	return mrb_obj_alloc(_mrb, vtype, cls);
}

mrb_value
RubyState::obj_as_string(const mrb_value obj)
{
	return mrb_obj_as_string(_mrb, obj);
}

RClass *
RubyState::obj_class(const mrb_value obj)
{
	return mrb_obj_class(_mrb, obj);
}

const char *
RubyState::obj_classname(const mrb_value obj)
{
	return mrb_obj_classname(_mrb, obj);
}

mrb_value
RubyState::obj_clone(const mrb_value self)
{
	return mrb_obj_clone(_mrb, self);
}

mrb_value
RubyState::obj_dup(const mrb_value obj)
{
	return mrb_obj_dup(_mrb, obj);
}

mrb_bool
RubyState::obj_eq(const mrb_value a, const mrb_value b)
{
	return mrb_obj_eq(_mrb, a, b);
}

mrb_bool
RubyState::obj_equal(const mrb_value a, const mrb_value b)
{
	return mrb_obj_equal(_mrb, a, b);
}

mrb_value
RubyState::obj_freeze(const mrb_value obj)
{
	return mrb_obj_freeze(_mrb, obj);
}

mrb_int
RubyState::obj_id(const mrb_value obj)
{
	return mrb_obj_id(obj);
}

mrb_value
RubyState::obj_inspect(const mrb_value self)
{
	return mrb_obj_inspect(_mrb, self);
}

mrb_bool
RubyState::obj_is_instance_of(const mrb_value obj, RClass *c)
{
	return mrb_obj_is_instance_of(_mrb, obj, c);
}

mrb_bool
RubyState::obj_is_kind_of(const mrb_value obj, RClass *c)
{
	return mrb_obj_is_kind_of(_mrb, obj, c);
}

mrb_bool
RubyState::obj_iv_defined(RObject *obj, const mrb_sym sym)
{
	return mrb_obj_iv_defined(_mrb, obj, sym);
}

mrb_value
RubyState::obj_iv_get(RObject *obj, const mrb_sym sym)
{
	return mrb_obj_iv_get(_mrb, obj, sym);
}

void
RubyState::obj_iv_set(RObject *obj, const mrb_sym sym, const mrb_value v)
{
	mrb_obj_iv_set(_mrb, obj, sym, v);
}

mrb_value
RubyState::obj_new(RClass *c, const mrb_int argc, const mrb_value *argv)
{
	return mrb_obj_new(_mrb, c, argc, argv);
}

mrb_bool
RubyState::obj_respond_to(RClass *c, const mrb_sym mid)
{
	return mrb_obj_respond_to(_mrb, c, mid);
}

mrb_sym
RubyState::obj_to_sym(const mrb_value name)
{
	return mrb_obj_to_sym(_mrb, name);
}

mrb_bool
RubyState::object_dead_p(RBasic *object)
{
	return mrb_object_dead_p(_mrb, object);
}

mrb_state *
RubyState::open_allocf(const mrb_allocf f, void *ud)
{
	return mrb_open_allocf(f, ud);
}

mrb_state *
RubyState::open_core(const mrb_allocf f, void *ud)
{
	return mrb_open_core(f, ud);
}

void
RubyState::p(const mrb_value obj)
{
	mrb_p(_mrb, obj);
}

mrb_parser_state *
RubyState::parse_file(FILE *fh, mrb_ccontext *cxt)
{
	return mrb_parse_file(_mrb, fh, cxt);
}

mrb_parser_state *
RubyState::parse_nstring(const char *str, const size_t len, mrb_ccontext *cxt)
{
	return mrb_parse_nstring(_mrb, str, len, cxt);
}

mrb_parser_state *
RubyState::parse_string(const char *str, mrb_ccontext *cxt)
{
	return mrb_parse_string(_mrb, str, cxt);
}

void
RubyState::parser_free(mrb_parser_state *ps)
{
	mrb_parser_free(ps);
}

mrb_sym
RubyState::parser_get_filename(mrb_parser_state *ps, const uint16_t idx)
{
	return mrb_parser_get_filename(ps, idx);
}

mrb_parser_state *
RubyState::parser_new()
{
	return mrb_parser_new(_mrb);
}

void
RubyState::parser_parse(mrb_parser_state *ps, mrb_ccontext *cxt)
{
	mrb_parser_parse(ps, cxt);
}

void
RubyState::parser_set_filename(mrb_parser_state *ps, const char *filename)
{
	mrb_parser_set_filename(ps, filename);
}

void
RubyState::prepend_module(RClass *cla, RClass *prepended)
{
	mrb_prepend_module(_mrb, cla, prepended);
}

void
RubyState::print_backtrace()
{
	mrb_print_backtrace(_mrb);
}

void
RubyState::print_error()
{
	mrb_print_error(_mrb);
}

mrb_value
RubyState::proc_cfunc_env_get(const mrb_int idx)
{
	return mrb_proc_cfunc_env_get(_mrb, idx);
}

RProc *
RubyState::proc_new_cfunc(const mrb_func_t fn)
{
	return mrb_proc_new_cfunc(_mrb, fn);
}

RProc *
RubyState::proc_new_cfunc_with_env(const mrb_func_t func, const mrb_int argc,
    const mrb_value *argv)
{
	return mrb_proc_new_cfunc_with_env(_mrb, func, argc, argv);
}

mrb_value
RubyState::protect(const mrb_func_t body, const mrb_value data, mrb_bool *state)
{
	return mrb_protect(_mrb, body, data, state);
}

mrb_value
RubyState::ptr_to_str(void *p)
{
	return mrb_ptr_to_str(_mrb, p);
}

enum mrb_range_beg_len
RubyState::range_beg_len(const mrb_value range, mrb_int *begp, mrb_int *lenp,
    const mrb_int len, const mrb_bool trunc)
{
	return mrb_range_beg_len(_mrb, range, begp, lenp, len, trunc);
}

mrb_value
RubyState::range_new(const mrb_value start, const mrb_value end,
    const mrb_bool exclude)
{
	return mrb_range_new(_mrb, start, end, exclude);
}

RRange *
RubyState::range_ptr(const mrb_value range)
{
	return mrb_range_ptr(_mrb, range);
}

mrb_irep *
RubyState::read_irep(const uint8_t *buf)
{
	return mrb_read_irep(_mrb, buf);
}

mrb_irep *
RubyState::read_irep_buf(const void *buf, const size_t len)
{
	return mrb_read_irep_buf(_mrb, buf, len);
}

void *
RubyState::realloc(void *buf, const size_t len)
{
	return mrb_realloc(_mrb, buf, len);
}

void *
RubyState::realloc_simple(void *buf, const size_t len)
{
	return mrb_realloc_simple(_mrb, buf, len);
}

void
RubyState::remove_method(RClass *c, const mrb_sym sym)
{
	mrb_remove_method(_mrb, c, sym);
}

mrb_value
RubyState::rescue(const mrb_func_t body, const mrb_value b_data,
    const mrb_func_t rescue, const mrb_value r_data)
{
	return mrb_rescue(_mrb, body, b_data, rescue, r_data);
}

mrb_value
RubyState::rescue_exceptions(const mrb_func_t body, const mrb_value b_data,
    const mrb_func_t rescue, const mrb_value r_data, const mrb_int len,
    RClass **classes)
{
	return mrb_rescue_exceptions(_mrb, body, b_data, rescue, r_data, len,
	    classes);
}

mrb_bool
RubyState::respond_to(const mrb_value obj, const mrb_sym mid)
{
	return mrb_respond_to(_mrb, obj, mid);
}

void
RubyState::show_copyright()
{
	mrb_show_copyright(_mrb);
}

void
RubyState::show_version()
{
	mrb_show_version(_mrb);
}

mrb_value
RubyState::singleton_class(const mrb_value val)
{
	return mrb_singleton_class(_mrb, val);
}

RClass *
RubyState::singleton_class_ptr(const mrb_value val)
{
	return mrb_singleton_class_ptr(_mrb, val);
}

void
RubyState::stack_extend(const mrb_int i)
{
	mrb_stack_extend(_mrb, i);
}

void
RubyState::state_atexit(const mrb_atexit_func func)
{
	mrb_state_atexit(_mrb, func);
}

mrb_value
RubyState::str_append(const mrb_value str, const mrb_value str2)
{
	return mrb_str_append(_mrb, str, str2);
}

mrb_value
RubyState::str_cat(const mrb_value str, const char *ptr, const size_t len)
{
	return mrb_str_cat(_mrb, str, ptr, len);
}

mrb_value
RubyState::str_cat_cstr(const mrb_value str, const char *ptr)
{
	return mrb_str_cat_cstr(_mrb, str, ptr);
}

mrb_value
RubyState::str_cat_str(const mrb_value str, const mrb_value str2)
{
	return mrb_str_cat_str(_mrb, str, str2);
}

int
RubyState::str_cmp(const mrb_value str1, const mrb_value str2)
{
	return mrb_str_cmp(_mrb, str1, str2);
}

void
RubyState::str_concat(const mrb_value self, const mrb_value other)
{
	mrb_str_concat(_mrb, self, other);
}

mrb_value
RubyState::str_dup(const mrb_value str)
{
	return mrb_str_dup(_mrb, str);
}

mrb_bool
RubyState::str_equal(const mrb_value str1, const mrb_value str2)
{
	return mrb_str_equal(_mrb, str1, str2);
}

mrb_int
RubyState::str_index(const mrb_value str, const char *p, const mrb_int len,
    const mrb_int offset)
{
	return mrb_str_index(_mrb, str, p, len, offset);
}

mrb_value
RubyState::str_intern(const mrb_value self)
{
	return mrb_str_intern(_mrb, self);
}

void
RubyState::str_modify(RString *s)
{
	mrb_str_modify(_mrb, s);
}

void
RubyState::str_modify_keep_ascii(RString *s)
{
	mrb_str_modify_keep_ascii(_mrb, s);
}

mrb_value
RubyState::str_new(const char *p, const size_t len)
{
	return mrb_str_new(_mrb, p, len);
}

mrb_value
RubyState::str_new_capa(const size_t capa)
{
	return mrb_str_new_capa(_mrb, capa);
}

mrb_value
RubyState::str_new_static(const char *p, const size_t len)
{
	return mrb_str_new_static(_mrb, p, len);
}

mrb_value
RubyState::str_plus(const mrb_value a, const mrb_value b)
{
	return mrb_str_plus(_mrb, a, b);
}

mrb_value
RubyState::str_resize(const mrb_value str, const mrb_int len)
{
	return mrb_str_resize(_mrb, str, len);
}

mrb_value
RubyState::str_substr(const mrb_value str, const mrb_int beg, const mrb_int len)
{
	return mrb_str_substr(_mrb, str, beg, len);
}

char *
RubyState::str_to_cstr(const mrb_value str)
{
	return mrb_str_to_cstr(_mrb, str);
}

double
RubyState::str_to_dbl(const mrb_value str, const mrb_bool badcheck)
{
	return mrb_str_to_dbl(_mrb, str, badcheck);
}

mrb_value
RubyState::str_to_integer(const mrb_value str, const mrb_int base,
    const mrb_bool badcheck)
{
	return mrb_str_to_integer(_mrb, str, base, badcheck);
}

const char *
RubyState::string_cstr(const mrb_value str)
{
	return mrb_string_cstr(_mrb, str);
}

const char *
RubyState::string_value_cstr(mrb_value *str)
{
	return mrb_string_value_cstr(_mrb, str);
}

const char *
RubyState::sym_dump(const mrb_sym sym)
{
	return mrb_sym_dump(_mrb, sym);
}

const char *
RubyState::sym_name(const mrb_sym sym)
{
	return mrb_sym_name(_mrb, sym);
}

const char *
RubyState::sym_name_len(const mrb_sym sym, mrb_int *len)
{
	return mrb_sym_name_len(_mrb, sym, len);
}

mrb_value
RubyState::sym_str(const mrb_sym sym)
{
	return mrb_sym_str(_mrb, sym);
}

void
RubyState::sys_fail(const char *mesg)
{
	mrb_sys_fail(_mrb, mesg);
}

mrb_value
RubyState::top_run(const RProc *proc, const mrb_value self,
    const mrb_int stack_keep)
{
	return mrb_top_run(_mrb, proc, self, stack_keep);
}

mrb_value
RubyState::top_self()
{
	return mrb_top_self(_mrb);
}

mrb_value
RubyState::type_convert(const mrb_value val, const mrb_vtype type,
    const mrb_sym method)
{
	return mrb_type_convert(_mrb, val, type, method);
}

mrb_value
RubyState::type_convert_check(const mrb_value val, const mrb_vtype type,
    const mrb_sym method)
{
	return mrb_type_convert_check(_mrb, val, type, method);
}

void
RubyState::undef_class_method(RClass *cls, const char *name)
{
	return mrb_undef_class_method(_mrb, cls, name);
}

void
RubyState::undef_class_method_id(RClass *cls, const mrb_sym name)
{
	return mrb_undef_class_method_id(_mrb, cls, name);
}

void
RubyState::undef_method(RClass *cla, const char *name)
{
	return mrb_undef_method(_mrb, cla, name);
}

void
RubyState::undef_method_id(RClass *cls, const mrb_sym sym)
{
	return mrb_undef_method_id(_mrb, cls, sym);
}

/* ReSharper disable once CppParameterMayBeConst */
mrb_value
RubyState::vformat(const char *format, va_list ap)
{
	return mrb_vformat(_mrb, format, ap);
}

mrb_value
RubyState::vm_const_get(const mrb_sym sym)
{
	return mrb_vm_const_get(_mrb, sym);
}

void
RubyState::vm_const_set(const mrb_sym sym, const mrb_value val)
{
	mrb_vm_const_set(_mrb, sym, val);
}

mrb_value
RubyState::vm_cv_get(const mrb_sym sym)
{
	return mrb_vm_cv_get(_mrb, sym);
}

void
RubyState::vm_cv_set(const mrb_sym sym, const mrb_value val)
{
	mrb_vm_cv_set(_mrb, sym, val);
}

RClass *
RubyState::vm_define_class(const mrb_value v1, const mrb_value v2,
    const mrb_sym sym)
{
	return mrb_vm_define_class(_mrb, v1, v2, sym);
}

RClass *
RubyState::vm_define_module(const mrb_value val, const mrb_sym sym)
{
	return mrb_vm_define_module(_mrb, val, sym);
}

mrb_value
RubyState::vm_exec(const RProc *proc, const mrb_code *iseq)
{
	return mrb_vm_exec(_mrb, proc, iseq);
}

mrb_value
RubyState::vm_run(const RProc *proc, const mrb_value self,
    const mrb_int stack_keep)
{
	return mrb_vm_run(_mrb, proc, self, stack_keep);
}

mrb_value
RubyState::vm_special_get(const mrb_sym sym)
{
	return mrb_vm_special_get(_mrb, sym);
}

void
RubyState::vm_special_set(const mrb_sym sym, const mrb_value val)
{
	mrb_vm_special_set(_mrb, sym, val);
}

void
RubyState::warn(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	auto msg = vformat(fmt, ap);
	va_end(ap);
	mrb_warn(_mrb, "%s", string_cstr(msg));
}

mrb_value
RubyState::word_boxing_cptr_value(void *ptr)
{
	return mrb_word_boxing_cptr_value(_mrb, ptr);
}

mrb_value
RubyState::word_boxing_float_value(const mrb_float fl)
{
	return mrb_word_boxing_float_value(_mrb, fl);
}

mrb_value
RubyState::word_boxing_int_value(const mrb_int n)
{
	return mrb_boxing_int_value(_mrb, n);
}

void
RubyState::write_barrier(RBasic *rb)
{
	mrb_write_barrier(_mrb, rb);
}

mrb_value
RubyState::yield(const mrb_value b, const mrb_value arg)
{
	return mrb_yield(_mrb, b, arg);
}

mrb_value
RubyState::yield_argv(const mrb_value b, const mrb_int argc,
    const mrb_value *argv)
{
	return mrb_yield_argv(_mrb, b, argc, argv);
}

mrb_value
RubyState::yield_with_class(const mrb_value b, const mrb_int argc,
    const mrb_value *argv, const mrb_value self, RClass *c)
{
	return mrb_yield_with_class(_mrb, b, argc, argv, self, c);
}

mrb_value
RubyState::obj_value(void *p)
{
	return mrb_obj_value(p);
}

mrb_value
RubyState::int_value(const mrb_int i)
{
	return mrb_int_value(_mrb, i);
}

mrb_value
RubyState::float_value(const mrb_float f)
{
	return mrb_float_value(_mrb, f);
}

mrb_value
RubyState::symbol_value(const mrb_sym i)
{
	return mrb_symbol_value(i);
}

mrb_int
RubyState::str_strlen(RString *str)
{
	const mrb_value v = mrb_obj_value(str);
	return RSTRING_LEN(v);
}

mrb_value
RubyState::string_type(const mrb_value str)
{
	return mrb_ensure_string_type(_mrb, str);
}

mrb_int
RubyState::string_value_len(const mrb_value str)
{
	return RSTRING_LEN(str);
}

const char *
RubyState::string_value_ptr(const mrb_value str)
{
	return RSTRING_PTR(str);
}

mrb_float
RubyState::to_flo(const mrb_value x)
{
	return mrb_float(ensure_float_type(x));
}

mrb_value
RubyState::to_int(const mrb_value val)
{
	return mrb_to_int(_mrb, val);
}

mrb_value
RubyState::to_str(const mrb_value val)
{
	return mrb_to_str(_mrb, val);
}
