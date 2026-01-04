/* SPDX-License-Identifier: ISC */

#include "euler/app/dragonruby/ruby_state.h"

#include <vector>

#include <mruby/hash.h>
#include <mruby/istruct.h>

#include "euler/app/dragonruby/state.h"
#include "euler/util/error.h"

using euler::app::dragonruby::MRubyState;

MRubyState::MRubyState(mrb_state *mrb, drb_api_t *api)
    : _mrb(mrb)
    , _api(*api)
{
}

void
MRubyState::raise(RClass *c, const char *msg)
{
	_api.mrb_raise(_mrb, c, msg);
}

void
MRubyState::raisef(RClass *c, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	const auto value = vformat(fmt, ap);
	va_end(ap);
	const char *message = string_cstr(value);
	raise(c, message);
}

RClass *
MRubyState::module_get(const char *name)
{
	return _api.mrb_module_get(_mrb, name);
}

RClass *
MRubyState::module_get_under(RClass *outer, const char *name)
{
	return _api.mrb_module_get_under(_mrb, outer, name);
}

RClass *
MRubyState::define_module_under(RClass *outer, const char *name)
{
	return _api.mrb_define_module_under(_mrb, outer, name);
}

RClass *
MRubyState::class_get_under(RClass *outer, const char *name)
{
	return _api.mrb_class_get_under(_mrb, outer, name);
}
mrb_bool
MRubyState::class_ptr_p(const mrb_value obj)
{
	switch (mrb_type(obj)) {
	case MRB_TT_CLASS:
	case MRB_TT_SCLASS:
	case MRB_TT_MODULE: return TRUE;
	default: return FALSE;
	}
}

RClass *
MRubyState::define_class_under(RClass *outer, const char *name, RClass *super)
{
	return _api.mrb_define_class_under(_mrb, outer, name, super);
}

void
MRubyState::define_module_function(RClass *cla, const char *name,
    const mrb_func_t fun, const mrb_aspec aspec)
{
	return _api.mrb_define_module_function(_mrb, cla, name, fun, aspec);
}

void
MRubyState::define_method(RClass *cla, const char *name, const mrb_func_t func,
    const mrb_aspec aspec)
{
	return _api.mrb_define_method(_mrb, cla, name, func, aspec);
}

void
MRubyState::define_class_method(RClass *cla, const char *name,
    const mrb_func_t fun, const mrb_aspec aspec)
{
	return _api.mrb_define_class_method(_mrb, cla, name, fun, aspec);
}

mrb_int
MRubyState::get_args(const mrb_args_format format, ...)
{
	va_list ap;
	va_start(ap, format);
	const mrb_int result = get_args_v(format, nullptr, &ap);
	va_end(ap);
	return result;
}

mrb_value
MRubyState::str_new_cstr(const char *str)
{
	return _api.mrb_str_new_cstr(_mrb, str);
}

RData *
MRubyState::data_object_alloc(RClass *klass, void *datap,
    const mrb_data_type *type)
{
	return _api.mrb_data_object_alloc(_mrb, klass, datap, type);
}

mrb_value
MRubyState::Float(const mrb_value val)
{
	return _api.mrb_Float(_mrb, val);
}

mrb_value
MRubyState::Integer(const mrb_value val)
{
	return _api.mrb_Integer(_mrb, val);
}

mrb_irep *
MRubyState::add_irep()
{
	return _api.mrb_add_irep(_mrb);
}

void
MRubyState::alias_method(RClass *c, const mrb_sym a, const mrb_sym b)
{
	_api.mrb_alias_method(_mrb, c, a, b);
}

mrb_value
MRubyState::any_to_s(const mrb_value obj)
{
	return _api.mrb_any_to_s(_mrb, obj);
}

void
MRubyState::argnum_error(const mrb_int argc, const int min, const int max)
{
	_api.mrb_argnum_error(_mrb, argc, min, max);
}

mrb_value
MRubyState::ary_clear(const mrb_value self)
{
	return _api.mrb_ary_clear(_mrb, self);
}

void
MRubyState::ary_concat(const mrb_value self, const mrb_value other)
{
	_api.mrb_ary_concat(_mrb, self, other);
}

mrb_value
MRubyState::ary_entry(const mrb_value ary, const mrb_int offset)
{
	return _api.mrb_ary_entry(ary, offset);
}

mrb_value
MRubyState::ary_join(const mrb_value ary, const mrb_value sep)
{
	return _api.mrb_ary_join(_mrb, ary, sep);
}

void
MRubyState::ary_modify(RArray *ary)
{
	_api.mrb_ary_modify(_mrb, ary);
}

mrb_value
MRubyState::ary_new()
{
	return _api.mrb_ary_new(_mrb);
}

mrb_value
MRubyState::ary_new_capa(const mrb_int capa)
{
	return _api.mrb_ary_new_capa(_mrb, capa);
}

mrb_value
MRubyState::ary_new_from_values(const mrb_int size, const mrb_value *vals)
{
	return _api.mrb_ary_new_from_values(_mrb, size, vals);
}

mrb_value
MRubyState::ary_pop(const mrb_value ary)
{
	return _api.mrb_ary_pop(_mrb, ary);
}

void
MRubyState::ary_push(const mrb_value array, const mrb_value value)
{
	_api.mrb_ary_push(_mrb, array, value);
}

mrb_value
MRubyState::ary_ref(const mrb_value ary, const mrb_int n)
{
	return _api.mrb_ary_ref(_mrb, ary, n);
}

void
MRubyState::ary_replace(const mrb_value self, const mrb_value other)
{
	return _api.mrb_ary_replace(_mrb, self, other);
}

mrb_value
MRubyState::ary_resize(const mrb_value ary, const mrb_int new_len)
{
	return _api.mrb_ary_resize(_mrb, ary, new_len);
}

void
MRubyState::ary_set(const mrb_value ary, const mrb_int n, const mrb_value val)
{
	return _api.mrb_ary_set(_mrb, ary, n, val);
}

mrb_value
MRubyState::ary_shift(const mrb_value self)
{
	return _api.mrb_ary_shift(_mrb, self);
}

mrb_value
MRubyState::ary_splat(const mrb_value value)
{
	return _api.mrb_ary_splat(_mrb, value);
}

mrb_value
MRubyState::ary_splice(const mrb_value self, const mrb_int head,
    const mrb_int len, const mrb_value rpl)
{
	return _api.mrb_ary_splice(_mrb, self, head, len, rpl);
}

mrb_value
MRubyState::ary_unshift(const mrb_value self, const mrb_value item)
{
	return _api.mrb_ary_unshift(_mrb, self, item);
}

mrb_value
MRubyState::assoc_new(const mrb_value car, const mrb_value cdr)
{
	return _api.mrb_assoc_new(_mrb, car, cdr);
}

mrb_value
MRubyState::attr_get(const mrb_value obj, const mrb_sym id)
{
	return _api.mrb_attr_get(_mrb, obj, id);
}

void
MRubyState::bug(const char *fmt, ...)
{
	_api.mrb_bug(_mrb, fmt);
}

void *
MRubyState::calloc(const size_t count, const size_t size)
{
	return _api.mrb_calloc(_mrb, count, size);
}

mrb_value
MRubyState::check_array_type(const mrb_value self)
{
	return _api.mrb_check_array_type(_mrb, self);
}

mrb_value
MRubyState::check_hash_type(const mrb_value hash)
{
	return _api.mrb_check_hash_type(_mrb, hash);
}

mrb_value
MRubyState::check_intern(const char *str, const size_t len)
{
	return _api.mrb_check_intern(_mrb, str, len);
}

mrb_value
MRubyState::check_intern_cstr(const char *str)
{
	return _api.mrb_check_intern_cstr(_mrb, str);
}

mrb_value
MRubyState::check_intern_str(const mrb_value str)
{
	return _api.mrb_check_intern_str(_mrb, str);
}

mrb_value
MRubyState::check_string_type(const mrb_value str)
{
	return _api.mrb_check_string_type(_mrb, str);
}

void
MRubyState::check_type(const mrb_value x, const mrb_vtype t)
{
	_api.mrb_check_type(_mrb, x, t);
}

mrb_bool
MRubyState::class_defined(const char *name)
{
	return _api.mrb_class_defined(_mrb, name);
}

mrb_bool
MRubyState::class_defined_id(const mrb_sym name)
{
	return _api.mrb_class_defined_id(_mrb, name);
}

mrb_bool
MRubyState::class_defined_under(RClass *outer, const char *name)
{
	return _api.mrb_class_defined_under(_mrb, outer, name);
}

mrb_bool
MRubyState::class_defined_under_id(RClass *outer, const mrb_sym name)
{
	return _api.mrb_class_defined_under_id(_mrb, outer, name);
}

RClass *
MRubyState::class_get(const char *name)
{
	return _api.mrb_class_get(_mrb, name);
}

RClass *
MRubyState::class_get_id(const mrb_sym name)
{
	return _api.mrb_class_get_id(_mrb, name);
}

RClass *
MRubyState::class_get_under_id(RClass *outer, const mrb_sym name)
{
	return _api.mrb_class_get_under_id(_mrb, outer, name);
}

const char *
MRubyState::class_name(RClass *klass)
{
	return _api.mrb_class_name(_mrb, klass);
}

RClass *
MRubyState::class_new(RClass *super)
{
	return _api.mrb_class_new(_mrb, super);
}

mrb_value
MRubyState::class_path(RClass *c)
{
	return _api.mrb_class_path(_mrb, c);
}

RClass *
MRubyState::class_real(RClass *cl)
{
	return _api.mrb_class_real(cl);
}

void
MRubyState::close()
{
	return _api.mrb_close(_mrb);
}

RProc *
MRubyState::closure_new_cfunc(const mrb_func_t func, const int nlocals)
{
	return _api.mrb_closure_new_cfunc(_mrb, func, nlocals);
}

mrb_int
MRubyState::cmp(const mrb_value obj1, const mrb_value obj2)
{
	return _api.mrb_cmp(_mrb, obj1, obj2);
}

mrb_bool
MRubyState::const_defined(const mrb_value value, const mrb_sym sym)
{
	return _api.mrb_const_defined(_mrb, value, sym);
}

mrb_bool
MRubyState::const_defined_at(const mrb_value mod, const mrb_sym id)
{
	return _api.mrb_const_defined_at(_mrb, mod, id);
}

mrb_value
MRubyState::const_get(const mrb_value value, const mrb_sym sym)
{
	return _api.mrb_const_get(_mrb, value, sym);
}

void
MRubyState::const_remove(const mrb_value value, const mrb_sym sym)
{
	_api.mrb_const_remove(_mrb, value, sym);
}

void
MRubyState::const_set(const mrb_value value, const mrb_sym sym,
    const mrb_value new_value)
{
	_api.mrb_const_set(_mrb, value, sym, new_value);
}

mrb_value
MRubyState::convert_to_integer(const mrb_value val, const mrb_int base)
{
	return _api.mrb_convert_to_integer(_mrb, val, base);
}

double
MRubyState::cstr_to_dbl(const char *s, const mrb_bool badcheck)
{
	return _api.mrb_cstr_to_dbl(_mrb, s, badcheck);
}

mrb_value
MRubyState::cstr_to_inum(const char *s, const mrb_int base,
    const mrb_bool badcheck)
{
	return _api.mrb_cstr_to_inum(_mrb, s, base, badcheck);
}

mrb_bool
MRubyState::cv_defined(const mrb_value mod, const mrb_sym sym)
{
	return _api.mrb_cv_defined(_mrb, mod, sym);
}

mrb_value
MRubyState::cv_get(const mrb_value mod, const mrb_sym sym)
{
	return _api.mrb_cv_get(_mrb, mod, sym);
}

void
MRubyState::cv_set(const mrb_value mod, const mrb_sym sym, const mrb_value v)
{
	_api.mrb_cv_set(_mrb, mod, sym, v);
}

void *
MRubyState::data_check_get_ptr(const mrb_value value, const mrb_data_type *type)
{
	return _api.mrb_data_check_get_ptr(_mrb, value, type);
}

void
MRubyState::data_check_type(const mrb_value value, const mrb_data_type *type)
{
	return _api.mrb_data_check_type(_mrb, value, type);
}

void *
MRubyState::data_get_ptr(const mrb_value value, const mrb_data_type *type)
{
	return _api.mrb_data_get_ptr(_mrb, value, type);
}

const char *
MRubyState::debug_get_filename(const mrb_irep *irep, const uint32_t pc)
{
	return _api.mrb_debug_get_filename(_mrb, irep, pc);
}

int32_t
MRubyState::debug_get_line(const mrb_irep *irep, const uint32_t pc)
{
	return _api.mrb_debug_get_line(_mrb, irep, pc);
}

mrb_irep_debug_info *
MRubyState::debug_info_alloc(mrb_irep *irep)
{
	return _api.mrb_debug_info_alloc(_mrb, irep);
}

mrb_irep_debug_info_file *
MRubyState::debug_info_append_file(mrb_irep_debug_info *info,
    const char *filename, uint16_t *lines, const uint32_t start_pos,
    const uint32_t end_pos)
{
	return _api.mrb_debug_info_append_file(_mrb, info, filename, lines,
	    start_pos, end_pos);
}

void
MRubyState::debug_info_free(mrb_irep_debug_info *d)
{
	_api.mrb_debug_info_free(_mrb, d);
}

void *
MRubyState::default_allocf(void *data, const size_t size, void *ptr)
{
	return _api.mrb_default_allocf(_mrb, data, size, ptr);
}

void
MRubyState::define_alias(RClass *c, const char *a, const char *b)
{
	_api.mrb_define_alias(_mrb, c, a, b);
}

void
MRubyState::define_alias_id(RClass *c, const mrb_sym a, const mrb_sym b)
{
	_api.mrb_define_alias_id(_mrb, c, a, b);
}

RClass *
MRubyState::define_class(const char *name, RClass *super)
{
	return _api.mrb_define_class(_mrb, name, super);
}

RClass *
MRubyState::define_class_id(const mrb_sym name, RClass *super)
{
	return _api.mrb_define_class_id(_mrb, name, super);
}

void
MRubyState::define_class_method_id(RClass *cla, const mrb_sym name,
    const mrb_func_t fun, const mrb_aspec aspec)
{
	return _api.mrb_define_class_method_id(_mrb, cla, name, fun, aspec);
}

RClass *
MRubyState::define_class_under_id(RClass *outer, const mrb_sym name,
    RClass *super)
{
	return _api.mrb_define_class_under_id(_mrb, outer, name, super);
}

void
MRubyState::define_const(RClass *cla, const char *name, const mrb_value val)
{
	_api.mrb_define_const(_mrb, cla, name, val);
}

void
MRubyState::define_const_id(RClass *cla, const mrb_sym name,
    const mrb_value val)
{
	_api.mrb_define_const_id(_mrb, cla, name, val);
}

void
MRubyState::define_global_const(const char *name, const mrb_value val)
{
	_api.mrb_define_global_const(_mrb, name, val);
}

void
MRubyState::define_method_id(RClass *c, const mrb_sym mid,
    const mrb_func_t func, const mrb_aspec aspec)
{
	_api.mrb_define_method_id(_mrb, c, mid, func, aspec);
}

void
MRubyState::define_method_raw(RClass *c, const mrb_sym mid,
    const mrb_method_t meth)
{
	_api.mrb_define_method_raw(_mrb, c, mid, meth);
}

RClass *
MRubyState::define_module(const char *name)
{
	return _api.mrb_define_module(_mrb, name);
}

void
MRubyState::define_module_function_id(RClass *cla, const mrb_sym name,
    const mrb_func_t fun, const mrb_aspec aspec)
{
	_api.mrb_define_module_function_id(_mrb, cla, name, fun, aspec);
}

RClass *
MRubyState::define_module_id(const mrb_sym name)
{
	return _api.mrb_define_module_id(_mrb, name);
}

RClass *
MRubyState::define_module_under_id(RClass *outer, const mrb_sym name)
{
	return _api.mrb_define_module_under_id(_mrb, outer, name);
}

void
MRubyState::define_singleton_method(RObject *cla, const char *name,
    const mrb_func_t fun, const mrb_aspec aspec)
{
	_api.mrb_define_singleton_method(_mrb, cla, name, fun, aspec);
}

void
MRubyState::define_singleton_method_id(RObject *cla, const mrb_sym name,
    const mrb_func_t fun, const mrb_aspec aspec)
{
	_api.mrb_define_singleton_method_id(_mrb, cla, name, fun, aspec);
}

mrb_value
MRubyState::ensure(const mrb_func_t body, const mrb_value b_data,
    const mrb_func_t ensure, const mrb_value e_data)
{
	return _api.mrb_ensure(_mrb, body, b_data, ensure, e_data);
}

mrb_value
MRubyState::ensure_array_type(const mrb_value self)
{
	return _api.mrb_ensure_array_type(_mrb, self);
}

mrb_value
MRubyState::ensure_hash_type(const mrb_value hash)
{
	return _api.mrb_ensure_hash_type(_mrb, hash);
}

mrb_value
MRubyState::ensure_string_type(const mrb_value str)
{
	return _api.mrb_ensure_string_type(_mrb, str);
}

mrb_bool
MRubyState::eql(const mrb_value obj1, const mrb_value obj2)
{
	return _api.mrb_eql(_mrb, obj1, obj2);
}

mrb_bool
MRubyState::equal(const mrb_value obj1, const mrb_value obj2)
{
	return _api.mrb_equal(_mrb, obj1, obj2);
}

mrb_value
MRubyState::exc_backtrace(const mrb_value exc)
{
	return _api.mrb_exc_backtrace(_mrb, exc);
}

RClass *
MRubyState::exc_get_id(const mrb_sym name)
{
	return _api.mrb_exc_get_id(_mrb, name);
}

mrb_value
MRubyState::exc_new(RClass *c, const char *ptr, const size_t len)
{
	return _api.mrb_exc_new(_mrb, c, ptr, len);
}

mrb_value
MRubyState::exc_new_str(RClass *c, const mrb_value str)
{
	return _api.mrb_exc_new_str(_mrb, c, str);
}

void
MRubyState::exc_raise(const mrb_value exc)
{
	return _api.mrb_exc_raise(_mrb, exc);
}

mrb_value
MRubyState::f_raise(const mrb_value val)
{
	return _api.mrb_f_raise(_mrb, val);
}

mrb_value
MRubyState::fiber_alive_p(const mrb_value fib)
{
	return _api.mrb_fiber_alive_p(_mrb, fib);
}

mrb_value
MRubyState::fiber_resume(const mrb_value fib, const mrb_int argc,
    const mrb_value *argv)
{
	return _api.mrb_fiber_resume(_mrb, fib, argc, argv);
}

mrb_value
MRubyState::fiber_yield(const mrb_int argc, const mrb_value *argv)
{
	return _api.mrb_fiber_yield(_mrb, argc, argv);
}

void
MRubyState::field_write_barrier(RBasic *b1, RBasic *b2)
{
	_api.mrb_field_write_barrier(_mrb, b1, b2);
}

mrb_value
MRubyState::fixnum_to_str(const mrb_value x, const mrb_int base)
{
	return _api.mrb_fixnum_to_str(_mrb, x, base);
}

mrb_value
MRubyState::flo_to_fixnum(const mrb_value val)
{
	return _api.mrb_flo_to_fixnum(_mrb, val);
}

double
MRubyState::float_read(const char *str, char **endptr)
{
	return _api.mrb_float_read(str, endptr);
}

int
MRubyState::float_to_cstr(char *buf, const size_t len, const char *fmt,
    const mrb_float f)
{
	return _api.mrb_float_to_cstr(_mrb, buf, len, fmt, f);
}

mrb_value
MRubyState::float_to_str(const mrb_value x, const char *fmt)
{
	return _api.mrb_float_to_str(_mrb, x, fmt);
}

mrb_value
MRubyState::format(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	const auto value = vformat(format, ap);
	va_end(ap);
	return str_new_cstr(string_cstr(value));
}

void
MRubyState::free(void *ptr)
{
	_api.mrb_free(_mrb, ptr);
}

void
MRubyState::free_context(mrb_context *c)
{
	_api.mrb_free_context(_mrb, c);
}

void
MRubyState::frozen_error(void *frozen_obj)
{
	_api.mrb_frozen_error(_mrb, frozen_obj);
}

void
MRubyState::full_gc()
{
	_api.mrb_full_gc(_mrb);
}

mrb_bool
MRubyState::func_basic_p(const mrb_value obj, const mrb_sym mid,
    const mrb_func_t func)
{
	return _api.mrb_func_basic_p(_mrb, obj, mid, func);
}

mrb_value
MRubyState::funcall(const mrb_value val, const char *name, const mrb_int argc,
    ...)
{
	va_list ap;
	va_start(ap, argc);
	std::vector<mrb_value> argv;
	argv.reserve(argc);
	for (mrb_int i = 0; i < argc; ++i)
		argv.push_back(va_arg(ap, mrb_value));
	va_end(ap);
	const mrb_sym sym = intern_cstr(name);
	return funcall_argv(val, sym, argc, argv.data());
}

mrb_value
MRubyState::funcall_argv(const mrb_value val, const mrb_sym name,
    const mrb_int argc, const mrb_value *argv)
{
	return _api.mrb_funcall_argv(_mrb, val, name, argc, argv);
}

mrb_value
MRubyState::funcall_id(const mrb_value val, const mrb_sym mid,
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
MRubyState::funcall_with_block(const mrb_value val, const mrb_sym name,
    const mrb_int argc, const mrb_value *argv, const mrb_value block)
{
	return _api.mrb_funcall_with_block(_mrb, val, name, argc, argv, block);
}

void
MRubyState::garbage_collect()
{
	_api.mrb_garbage_collect(_mrb);
}

void
MRubyState::gc_mark(RBasic *obj)
{
	_api.mrb_gc_mark(_mrb, obj);
}

void
MRubyState::gc_protect(const mrb_value obj)
{
	_api.mrb_gc_protect(_mrb, obj);
}

void
MRubyState::gc_register(const mrb_value obj)
{
	_api.mrb_gc_register(_mrb, obj);
}

void
MRubyState::gc_unregister(const mrb_value obj)
{
	_api.mrb_gc_unregister(_mrb, obj);
}

RProc *
MRubyState::generate_code(mrb_parser_state *p)
{
	return _api.mrb_generate_code(_mrb, p);
}

mrb_value
MRubyState::get_arg1()
{
	return _api.mrb_get_arg1(_mrb);
}

mrb_int
MRubyState::get_argc()
{
	return _api.mrb_get_argc(_mrb);
}

const mrb_value *
MRubyState::get_argv()
{
	return _api.mrb_get_argv(_mrb);
}

mrb_value
MRubyState::get_backtrace()
{
	return _api.mrb_get_backtrace(_mrb);
}

mrb_value
MRubyState::gv_get(const mrb_sym sym)
{
	return _api.mrb_gv_get(_mrb, sym);
}

void
MRubyState::gv_remove(const mrb_sym sym)
{
	return _api.mrb_gv_remove(_mrb, sym);
}

void
MRubyState::gv_set(const mrb_sym sym, const mrb_value val)
{
	return _api.mrb_gv_set(_mrb, sym, val);
}

void
MRubyState::hash_check_kdict(const mrb_value self)
{
	const auto keys = mrb_hash_keys(_mrb, self);
	const mrb_int len = RARRAY_LEN(keys);
	for (mrb_int i = 0; i < len; ++i) {
		const auto key = ary_entry(keys, i);
		if (mrb_symbol_p(key)) continue;
		throw util::ArgumentError(state(),
		    "keyword argument with non symbol keys");
	}
}

mrb_value
MRubyState::hash_clear(const mrb_value hash)
{
	return _api.mrb_hash_clear(_mrb, hash);
}

mrb_value
MRubyState::hash_delete_key(const mrb_value hash, const mrb_value key)
{
	return _api.mrb_hash_delete_key(_mrb, hash, key);
}

mrb_value
MRubyState::hash_dup(const mrb_value hash)
{
	return _api.mrb_hash_dup(_mrb, hash);
}

mrb_bool
MRubyState::hash_empty_p(const mrb_value self)
{
	return _api.mrb_hash_empty_p(_mrb, self);
}

mrb_value
MRubyState::hash_fetch(const mrb_value hash, const mrb_value key,
    const mrb_value def)
{
	return _api.mrb_hash_fetch(_mrb, hash, key, def);
}

void
MRubyState::hash_foreach(RHash *hash, mrb_hash_foreach_func *func, void *p)
{
	return _api.mrb_hash_foreach(_mrb, hash, func, p);
}

mrb_value
MRubyState::hash_get(const mrb_value hash, const mrb_value key)
{
	return _api.mrb_hash_get(_mrb, hash, key);
}

mrb_bool
MRubyState::hash_key_p(const mrb_value hash, const mrb_value key)
{
	return _api.mrb_hash_key_p(_mrb, hash, key);
}

mrb_value
MRubyState::hash_keys(const mrb_value hash)
{
	return _api.mrb_hash_keys(_mrb, hash);
}

void
MRubyState::hash_merge(const mrb_value hash1, const mrb_value hash2)
{
	return _api.mrb_hash_merge(_mrb, hash1, hash2);
}

mrb_value
MRubyState::hash_new()
{
	return _api.mrb_hash_new(_mrb);
}

mrb_value
MRubyState::hash_new_capa(const mrb_int capa)
{
	return _api.mrb_hash_new_capa(_mrb, capa);
}

void
MRubyState::hash_set(const mrb_value hash, const mrb_value key,
    const mrb_value val)
{
	_api.mrb_hash_set(_mrb, hash, key, val);
}

mrb_int
MRubyState::hash_size(const mrb_value hash)
{
	return _api.mrb_hash_size(_mrb, hash);
}

mrb_value
MRubyState::hash_values(const mrb_value hash)
{
	return _api.mrb_hash_values(_mrb, hash);
}

void
MRubyState::include_module(RClass *cla, RClass *included)
{
	_api.mrb_include_module(_mrb, cla, included);
}

void
MRubyState::incremental_gc()
{
	_api.mrb_incremental_gc(_mrb);
}

mrb_value
MRubyState::inspect(const mrb_value obj)
{
	return _api.mrb_inspect(_mrb, obj);
}

mrb_value
MRubyState::instance_new(const mrb_value cv)
{
	return _api.mrb_instance_new(_mrb, cv);
}

mrb_sym
MRubyState::intern(const char *str, const size_t len)
{
	return _api.mrb_intern(_mrb, str, len);
}

mrb_sym
MRubyState::intern_check(const char *str, const size_t len)
{
	return _api.mrb_intern_check(_mrb, str, len);
}

mrb_sym
MRubyState::intern_check_cstr(const char *str)
{
	return _api.mrb_intern_check_cstr(_mrb, str);
}

mrb_sym
MRubyState::intern_check_str(const mrb_value str)
{
	return _api.mrb_intern_check_str(_mrb, str);
}

mrb_sym
MRubyState::intern_cstr(const char *str)
{
	return _api.mrb_intern_cstr(_mrb, str);
}

mrb_sym
MRubyState::intern_static(const char *str, const size_t len)
{
	return _api.mrb_intern_static(_mrb, str, len);
}

mrb_sym
MRubyState::intern_str(const mrb_value str)
{
	return _api.mrb_intern_str(_mrb, str);
}

void
MRubyState::iv_copy(const mrb_value dst, const mrb_value src)
{
	return _api.mrb_iv_copy(_mrb, dst, src);
}

mrb_bool
MRubyState::iv_defined(const mrb_value obj, const mrb_sym sym)
{
	return _api.mrb_iv_defined(_mrb, obj, sym);
}

void
MRubyState::iv_foreach(const mrb_value obj, mrb_iv_foreach_func *func, void *p)
{
	return _api.mrb_iv_foreach(_mrb, obj, func, p);
}

mrb_value
MRubyState::iv_get(const mrb_value obj, const mrb_sym sym)
{
	return _api.mrb_iv_get(_mrb, obj, sym);
}

void
MRubyState::iv_name_sym_check(const mrb_sym sym)
{
	return _api.mrb_iv_name_sym_check(_mrb, sym);
}

mrb_bool
MRubyState::iv_name_sym_p(const mrb_sym sym)
{
	return _api.mrb_iv_name_sym_p(_mrb, sym);
}

mrb_value
MRubyState::iv_remove(const mrb_value obj, const mrb_sym sym)
{
	return _api.mrb_iv_remove(_mrb, obj, sym);
}

void
MRubyState::iv_set(const mrb_value obj, const mrb_sym sym, const mrb_value v)
{
	return _api.mrb_iv_set(_mrb, obj, sym, v);
}

mrb_value
MRubyState::load_detect_file_cxt(FILE *fp, mrbc_context *c)
{
	return _api.mrb_load_detect_file_cxt(_mrb, fp, c);
}

mrb_value
MRubyState::load_exec(mrb_parser_state *p, mrbc_context *c)
{
	return _api.mrb_load_exec(_mrb, p, c);
}

mrb_value
MRubyState::load_file(FILE *file)
{
	return _api.mrb_load_file(_mrb, file);
}

mrb_value
MRubyState::load_file_cxt(FILE *file, mrbc_context *cxt)
{
	return _api.mrb_load_file_cxt(_mrb, file, cxt);
}

mrb_value
MRubyState::load_irep(const uint8_t *data)
{
	return _api.mrb_load_irep(_mrb, data);
}

mrb_value
MRubyState::load_irep_buf(const void *data, const size_t len)
{
	return _api.mrb_load_irep_buf(_mrb, data, len);
}

mrb_value
MRubyState::load_irep_buf_cxt(const void *data, const size_t len,
    mrbc_context *ctx)
{
	return _api.mrb_load_irep_buf_cxt(_mrb, data, len, ctx);
}

mrb_value
MRubyState::load_irep_cxt(const uint8_t *data, mrbc_context *ctx)
{
	return _api.mrb_load_irep_cxt(_mrb, data, ctx);
}

mrb_value
MRubyState::load_irep_file(FILE *file)
{
	return _api.mrb_load_irep_file(_mrb, file);
}

mrb_value
MRubyState::load_irep_file_cxt(FILE *data, mrbc_context *ctx)
{
	return _api.mrb_load_irep_file_cxt(_mrb, data, ctx);
}

mrb_value
MRubyState::load_nstring(const char *s, const size_t len)
{
	return _api.mrb_load_nstring(_mrb, s, len);
}

mrb_value
MRubyState::load_nstring_cxt(const char *s, const size_t len, mrbc_context *cxt)
{
	return _api.mrb_load_nstring_cxt(_mrb, s, len, cxt);
}

mrb_value
MRubyState::load_proc(const RProc *proc)
{
	return _api.mrb_load_proc(_mrb, proc);
}

mrb_value
MRubyState::load_string(const char *s)
{
	return _api.mrb_load_string(_mrb, s);
}

mrb_value
MRubyState::load_string_cxt(const char *s, mrbc_context *cxt)
{
	return _api.mrb_load_string_cxt(_mrb, s, cxt);
}

mrb_value
MRubyState::make_exception(const mrb_int argc, const mrb_value *argv)
{
	return _api.mrb_make_exception(_mrb, argc, argv);
}

void *
MRubyState::malloc(const size_t size)
{
	return _api.mrb_malloc(_mrb, size);
}

void *
MRubyState::malloc_simple(const size_t size)
{
	return _api.mrb_malloc_simple(_mrb, size);
}

mrb_method_t
MRubyState::method_search(RClass *cl, const mrb_sym sym)
{
	return _api.mrb_method_search(_mrb, cl, sym);
}

mrb_method_t
MRubyState::method_search_vm(RClass **ptr, const mrb_sym sym)
{
	return _api.mrb_method_search_vm(_mrb, ptr, sym);
}

void
MRubyState::mod_cv_set(RClass *c, const mrb_sym sym, const mrb_value v)
{
	return _api.mrb_mod_cv_set(_mrb, c, sym, v);
}

RClass *
MRubyState::module_get_id(const mrb_sym name)
{
	return _api.mrb_module_get_id(_mrb, name);
}

RClass *
MRubyState::module_get_under_id(RClass *outer, const mrb_sym name)
{
	return _api.mrb_module_get_under_id(_mrb, outer, name);
}

RClass *
MRubyState::module_new()
{
	return _api.mrb_module_new(_mrb);
}

void
MRubyState::mt_foreach(RClass *cls, mrb_mt_foreach_func *fn, void *ptr)
{
	_api.mrb_mt_foreach(_mrb, cls, fn, ptr);
}

void
MRubyState::name_error(const mrb_sym id, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	const auto message = vformat(fmt, ap);
	va_end(ap);
	_api.mrb_name_error(_mrb, id, string_cstr(message));
}

void
MRubyState::no_method_error(const mrb_sym id, const mrb_value args,
    const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	const auto message = vformat(fmt, ap);
	va_end(ap);
	_api.mrb_no_method_error(_mrb, id, args, string_cstr(message));
}

void
MRubyState::notimplement()
{
	_api.mrb_notimplement(_mrb);
}

mrb_value
MRubyState::notimplement_m(const mrb_value m)
{
	return _api.mrb_notimplement_m(_mrb, m);
}

mrb_value
MRubyState::num_minus(const mrb_value x, const mrb_value y)
{
	return _api.mrb_num_minus(_mrb, x, y);
}

mrb_value
MRubyState::num_mul(const mrb_value x, const mrb_value y)
{
	return _api.mrb_num_mul(_mrb, x, y);
}

mrb_value
MRubyState::num_plus(const mrb_value x, const mrb_value y)
{
	return _api.mrb_num_plus(_mrb, x, y);
}

RBasic *
MRubyState::obj_alloc(const mrb_vtype type, RClass *cls)
{
	return _api.mrb_obj_alloc(_mrb, type, cls);
}

mrb_value
MRubyState::obj_as_string(const mrb_value obj)
{
	return _api.mrb_obj_as_string(_mrb, obj);
}

RClass *
MRubyState::obj_class(const mrb_value obj)
{
	return _api.mrb_obj_class(_mrb, obj);
}

const char *
MRubyState::obj_classname(const mrb_value obj)
{
	return _api.mrb_obj_classname(_mrb, obj);
}

mrb_value
MRubyState::obj_clone(const mrb_value self)
{
	return _api.mrb_obj_clone(_mrb, self);
}

mrb_value
MRubyState::obj_dup(const mrb_value obj)
{
	return _api.mrb_obj_dup(_mrb, obj);
}

mrb_bool
MRubyState::obj_eq(const mrb_value a, const mrb_value b)
{
	return _api.mrb_obj_eq(_mrb, a, b);
}

mrb_bool
MRubyState::obj_equal(const mrb_value a, const mrb_value b)
{
	return _api.mrb_obj_equal(_mrb, a, b);
}

mrb_value
MRubyState::obj_freeze(const mrb_value obj)
{
	return _api.mrb_obj_freeze(_mrb, obj);
}

mrb_int
MRubyState::obj_id(const mrb_value obj)
{
	return _api.mrb_obj_id(obj);
}

mrb_value
MRubyState::obj_inspect(const mrb_value self)
{
	return _api.mrb_obj_inspect(_mrb, self);
}

mrb_bool
MRubyState::obj_is_instance_of(const mrb_value obj, RClass *c)
{
	return _api.mrb_obj_is_instance_of(_mrb, obj, c);
}

mrb_bool
MRubyState::obj_is_kind_of(const mrb_value obj, RClass *c)
{
	return _api.mrb_obj_is_kind_of(_mrb, obj, c);
}

mrb_bool
MRubyState::obj_iv_defined(RObject *obj, const mrb_sym sym)
{
	return _api.mrb_obj_iv_defined(_mrb, obj, sym);
}

mrb_value
MRubyState::obj_iv_get(RObject *obj, const mrb_sym sym)
{
	return _api.mrb_obj_iv_get(_mrb, obj, sym);
}

void
MRubyState::obj_iv_set(RObject *obj, const mrb_sym sym, const mrb_value v)
{
	return _api.mrb_obj_iv_set(_mrb, obj, sym, v);
}

mrb_value
MRubyState::obj_new(RClass *c, const mrb_int argc, const mrb_value *argv)
{
	return _api.mrb_obj_new(_mrb, c, argc, argv);
}

mrb_bool
MRubyState::obj_respond_to(RClass *c, const mrb_sym mid)
{
	return _api.mrb_obj_respond_to(_mrb, c, mid);
}

mrb_sym
MRubyState::obj_to_sym(const mrb_value name)
{
	return _api.mrb_obj_to_sym(_mrb, name);
}

mrb_bool
MRubyState::object_dead_p(RBasic *object)
{
	return _api.mrb_object_dead_p(_mrb, object);
}

mrb_state *
MRubyState::open_allocf(const mrb_allocf f, void *ud)
{
	return _api.mrb_open_allocf(f, ud);
}

mrb_state *
MRubyState::open_core(const mrb_allocf f, void *ud)
{
	return _api.mrb_open_core(f, ud);
}

void
MRubyState::p(const mrb_value str)
{
	_api.mrb_p(_mrb, str);
}

mrb_parser_state *
MRubyState::parse_file(FILE *file, mrbc_context *ctx)
{
	return _api.mrb_parse_file(_mrb, file, ctx);
}

mrb_parser_state *
MRubyState::parse_nstring(const char *str, const size_t len, mrbc_context *ctx)
{
	return _api.mrb_parse_nstring(_mrb, str, len, ctx);
}

mrb_parser_state *
MRubyState::parse_string(const char *str, mrbc_context *ctx)
{
	return _api.mrb_parse_string(_mrb, str, ctx);
}

void
MRubyState::parser_free(mrb_parser_state *ctx)
{
	return _api.mrb_parser_free(ctx);
}

mrb_sym
MRubyState::parser_get_filename(mrb_parser_state *p, const uint16_t idx)
{
	return _api.mrb_parser_get_filename(p, idx);
}

mrb_parser_state *
MRubyState::parser_new()
{
	return _api.mrb_parser_new(_mrb);
}

void
MRubyState::parser_parse(mrb_parser_state *p, mrbc_context *ctx)
{
	_api.mrb_parser_parse(p, ctx);
}

void
MRubyState::parser_set_filename(mrb_parser_state *p, const char *str)
{
	_api.mrb_parser_set_filename(p, str);
}

#if 0
void *
MRubyState::pool_alloc(mrb_pool *pool, const size_t len)
{
	return _api.mrb_pool_alloc(pool, len);
}

mrb_bool
MRubyState::pool_can_realloc(mrb_pool *pool, void *ptr, const size_t len)
{
	return _api.mrb_pool_can_realloc(pool, ptr, len);
}

void
MRubyState::pool_close(mrb_pool *poo)
{
	return _api.mrb_pool_close(poo);
}

mrb_pool *
MRubyState::pool_open()
{
	return _api.mrb_pool_open(_mrb);
}

void *
MRubyState::pool_realloc(mrb_pool *pool, void *ptr, const size_t oldlen,
    const size_t newlen)
{
	return _api.mrb_pool_realloc(pool, ptr, oldlen, newlen);
}
#endif

void
MRubyState::prepend_module(RClass *cla, RClass *prepended)
{
	_api.mrb_prepend_module(_mrb, cla, prepended);
}

void
MRubyState::print_backtrace()
{
	_api.mrb_print_backtrace(_mrb);
}

void
MRubyState::print_error()
{
	_api.mrb_print_error(_mrb);
}

mrb_value
MRubyState::proc_cfunc_env_get(const mrb_int idx)
{
	return _api.mrb_proc_cfunc_env_get(_mrb, idx);
}

RProc *
MRubyState::proc_new_cfunc(const mrb_func_t fn)
{
	return _api.mrb_proc_new_cfunc(_mrb, fn);
}

RProc *
MRubyState::proc_new_cfunc_with_env(const mrb_func_t func, const mrb_int argc,
    const mrb_value *argv)
{
	return _api.mrb_proc_new_cfunc_with_env(_mrb, func, argc, argv);
}

mrb_value
MRubyState::protect(const mrb_func_t body, const mrb_value data,
    mrb_bool *state)
{
	return _api.mrb_protect(_mrb, body, data, state);
}

mrb_value
MRubyState::ptr_to_str(void *p)
{
	return _api.mrb_ptr_to_str(_mrb, p);
}

enum mrb_range_beg_len
MRubyState::range_beg_len(const mrb_value range, mrb_int *begp, mrb_int *lenp,
    const mrb_int len, const mrb_bool trunc)
{
	return _api.mrb_range_beg_len(_mrb, range, begp, lenp, len, trunc);
}

mrb_value
MRubyState::range_new(const mrb_value start, const mrb_value end,
    const mrb_bool exclude)
{
	return _api.mrb_range_new(_mrb, start, end, exclude);
}

RRange *
MRubyState::range_ptr(const mrb_value range)
{
	return _api.mrb_range_ptr(_mrb, range);
}

mrb_irep *
MRubyState::read_irep(const uint8_t *data)
{
	return _api.mrb_read_irep(_mrb, data);
}

mrb_irep *
MRubyState::read_irep_buf(const void *data, const size_t len)
{
	return _api.mrb_read_irep_buf(_mrb, data, len);
}

void *
MRubyState::realloc(void *data, const size_t len)
{
	return _api.mrb_realloc(_mrb, data, len);
}

void *
MRubyState::realloc_simple(void *data, const size_t len)
{
	return _api.mrb_realloc_simple(_mrb, data, len);
}

void
MRubyState::remove_method(RClass *c, const mrb_sym sym)
{
	return _api.mrb_remove_method(_mrb, c, sym);
}

mrb_value
MRubyState::rescue(const mrb_func_t body, const mrb_value b_data,
    const mrb_func_t rescue, const mrb_value r_data)
{
	return _api.mrb_rescue(_mrb, body, b_data, rescue, r_data);
}

mrb_value
MRubyState::rescue_exceptions(const mrb_func_t body, const mrb_value b_data,
    const mrb_func_t rescue, const mrb_value r_data, const mrb_int len,
    RClass **classes)
{
	return _api.mrb_rescue_exceptions(_mrb, body, b_data, rescue, r_data,
	    len, classes);
}

mrb_bool
MRubyState::respond_to(const mrb_value obj, const mrb_sym mid)
{
	return _api.mrb_respond_to(_mrb, obj, mid);
}

void
MRubyState::show_copyright()
{
	_api.mrb_show_copyright(_mrb);
}

void
MRubyState::show_version()
{
	_api.mrb_show_version(_mrb);
}

mrb_value
MRubyState::singleton_class(const mrb_value val)
{
	return _api.mrb_singleton_class(_mrb, val);
}

RClass *
MRubyState::singleton_class_ptr(const mrb_value val)
{
	return _api.mrb_singleton_class_ptr(_mrb, val);
}

void
MRubyState::stack_extend(const mrb_int n)
{
	_api.mrb_stack_extend(_mrb, n);
}

void
MRubyState::state_atexit(const mrb_atexit_func func)
{
	_api.mrb_state_atexit(_mrb, func);
}

mrb_value
MRubyState::str_append(const mrb_value str, const mrb_value str2)
{
	return _api.mrb_str_append(_mrb, str, str2);
}

mrb_value
MRubyState::str_cat(const mrb_value str, const char *ptr, const size_t len)
{
	return _api.mrb_str_cat(_mrb, str, ptr, len);
}

mrb_value
MRubyState::str_cat_cstr(const mrb_value str, const char *ptr)
{
	return _api.mrb_str_cat_cstr(_mrb, str, ptr);
}

mrb_value
MRubyState::str_cat_str(const mrb_value str, const mrb_value str2)
{
	return _api.mrb_str_cat_str(_mrb, str, str2);
}

int
MRubyState::str_cmp(const mrb_value str1, const mrb_value str2)
{
	return _api.mrb_str_cmp(_mrb, str1, str2);
}

void
MRubyState::str_concat(const mrb_value self, const mrb_value other)
{
	return _api.mrb_str_concat(_mrb, self, other);
}

mrb_value
MRubyState::str_dup(const mrb_value str)
{
	return _api.mrb_str_dup(_mrb, str);
}

mrb_bool
MRubyState::str_equal(const mrb_value str1, const mrb_value str2)
{
	return _api.mrb_str_equal(_mrb, str1, str2);
}

mrb_int
MRubyState::str_index(const mrb_value str, const char *p, const mrb_int len,
    const mrb_int offset)
{
	return _api.mrb_str_index(_mrb, str, p, len, offset);
}

mrb_value
MRubyState::str_intern(const mrb_value self)
{
	return _api.mrb_str_intern(_mrb, self);
}

void
MRubyState::str_modify(RString *s)
{
	return _api.mrb_str_modify(_mrb, s);
}

void
MRubyState::str_modify_keep_ascii(RString *s)
{
	return _api.mrb_str_modify_keep_ascii(_mrb, s);
}

mrb_value
MRubyState::str_new(const char *p, const size_t len)
{
	return _api.mrb_str_new(_mrb, p, len);
}

mrb_value
MRubyState::str_new_capa(const size_t capa)
{
	return _api.mrb_str_new_capa(_mrb, capa);
}

mrb_value
MRubyState::str_new_static(const char *p, const size_t len)
{
	return _api.mrb_str_new_static(_mrb, p, len);
}

mrb_value
MRubyState::str_plus(const mrb_value a, const mrb_value b)
{
	return _api.mrb_str_plus(_mrb, a, b);
}

mrb_value
MRubyState::str_resize(const mrb_value str, const mrb_int len)
{
	return _api.mrb_str_resize(_mrb, str, len);
}

mrb_int
MRubyState::str_strlen(RString *str)
{
	return _api.mrb_str_strlen(_mrb, str);
}

mrb_value
MRubyState::str_substr(const mrb_value str, const mrb_int beg,
    const mrb_int len)
{
	return _api.mrb_str_substr(_mrb, str, beg, len);
}

char *
MRubyState::str_to_cstr(const mrb_value str)
{
	return _api.mrb_str_to_cstr(_mrb, str);
}

double
MRubyState::str_to_dbl(const mrb_value str, const mrb_bool badcheck)
{
	return _api.mrb_str_to_dbl(_mrb, str, badcheck);
}

mrb_value
MRubyState::str_to_inum(const mrb_value str, const mrb_int base,
    const mrb_bool badcheck)
{
	return _api.mrb_str_to_inum(_mrb, str, base, badcheck);
}

const char *
MRubyState::string_cstr(const mrb_value str)
{
	return _api.mrb_string_cstr(_mrb, str);
}

mrb_value
MRubyState::string_type(const mrb_value str)
{
	return _api.mrb_string_type(_mrb, str);
}

const char *
MRubyState::string_value_cstr(mrb_value *str)
{
	return _api.mrb_string_value_cstr(_mrb, str);
}

mrb_int
MRubyState::string_value_len(const mrb_value str)
{
	return _api.mrb_string_value_len(_mrb, str);
}

const char *
MRubyState::string_value_ptr(const mrb_value str)
{
	return _api.mrb_string_value_ptr(_mrb, str);
}

const char *
MRubyState::sym_dump(const mrb_sym sym)
{
	return _api.mrb_sym_dump(_mrb, sym);
}

const char *
MRubyState::sym_name(const mrb_sym sym)
{
	return _api.mrb_sym_name(_mrb, sym);
}

const char *
MRubyState::sym_name_len(const mrb_sym sym, mrb_int *len)
{
	return _api.mrb_sym_name_len(_mrb, sym, len);
}

mrb_value
MRubyState::sym_str(const mrb_sym sym)
{
	return _api.mrb_sym_str(_mrb, sym);
}

void
MRubyState::sys_fail(const char *mesg)
{
	return _api.mrb_sys_fail(_mrb, mesg);
}

mrb_float
MRubyState::to_flo(const mrb_value x)
{
	return _api.mrb_to_flo(_mrb, x);
}

mrb_value
MRubyState::to_int(const mrb_value val)
{
	return _api.mrb_to_int(_mrb, val);
}

mrb_value
MRubyState::to_str(const mrb_value val)
{
	return _api.mrb_to_str(_mrb, val);
}

mrb_value
MRubyState::top_run(const RProc *proc, const mrb_value self,
    const mrb_int stack_keep)
{
	return _api.mrb_top_run(_mrb, proc, self, stack_keep);
}

mrb_value
MRubyState::top_self()
{
	return _api.mrb_top_self(_mrb);
}

mrb_value
MRubyState::type_convert(const mrb_value val, const mrb_vtype type,
    const mrb_sym method)
{
	return _api.mrb_type_convert(_mrb, val, type, method);
}

mrb_value
MRubyState::type_convert_check(const mrb_value val, const mrb_vtype type,
    const mrb_sym method)
{
	return _api.mrb_type_convert_check(_mrb, val, type, method);
}

void
MRubyState::undef_class_method(RClass *cls, const char *name)
{
	return _api.mrb_undef_class_method(_mrb, cls, name);
}

void
MRubyState::undef_class_method_id(RClass *cls, const mrb_sym name)
{
	return _api.mrb_undef_class_method_id(_mrb, cls, name);
}

void
MRubyState::undef_method(RClass *cla, const char *name)
{
	return _api.mrb_undef_method(_mrb, cla, name);
}

void
MRubyState::undef_method_id(RClass *cla, const mrb_sym name)
{
	return _api.mrb_undef_method_id(_mrb, cla, name);
}

/* ReSharper disable once CppParameterMayBeConst */
mrb_value
MRubyState::vformat(const char *format, va_list ap)
{
	return _api.mrb_vformat(_mrb, format, ap);
}

mrb_value
MRubyState::vm_const_get(const mrb_sym sym)
{
	return _api.mrb_vm_const_get(_mrb, sym);
}

void
MRubyState::vm_const_set(const mrb_sym sym, const mrb_value val)
{
	_api.mrb_vm_const_set(_mrb, sym, val);
}

mrb_value
MRubyState::vm_cv_get(const mrb_sym sym)
{
	return _api.mrb_vm_cv_get(_mrb, sym);
}

void
MRubyState::vm_cv_set(const mrb_sym sym, const mrb_value val)
{
	return _api.mrb_vm_cv_set(_mrb, sym, val);
}

RClass *
MRubyState::vm_define_class(const mrb_value v1, const mrb_value v2,
    const mrb_sym sym)
{
	return _api.mrb_vm_define_class(_mrb, v1, v2, sym);
}

RClass *
MRubyState::vm_define_module(const mrb_value val, const mrb_sym sym)
{
	return _api.mrb_vm_define_module(_mrb, val, sym);
}

mrb_value
MRubyState::vm_exec(const RProc *proc, const mrb_code *iseq)
{
	return _api.mrb_vm_exec(_mrb, proc, iseq);
}

mrb_value
MRubyState::vm_run(const RProc *proc, const mrb_value self,
    const mrb_int stack_keep)
{
	return _api.mrb_vm_run(_mrb, proc, self, stack_keep);
}

mrb_value
MRubyState::vm_special_get(const mrb_sym sym)
{
	return _api.mrb_vm_special_get(_mrb, sym);
}

void
MRubyState::vm_special_set(const mrb_sym sym, const mrb_value val)
{
	_api.mrb_vm_special_set(_mrb, sym, val);
}

void
MRubyState::warn(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	const auto message = vformat(fmt, ap);
	va_end(ap);
	_api.mrb_warn(_mrb, string_cstr(message));
}

mrb_value
MRubyState::word_boxing_cptr_value(void *ptr)
{
	return _api.mrb_word_boxing_cptr_value(_mrb, ptr);
}

mrb_value
MRubyState::word_boxing_float_value(const mrb_float value)
{
	return _api.mrb_word_boxing_float_value(_mrb, value);
}

mrb_value
MRubyState::word_boxing_int_value(const mrb_int value)
{
	return _api.mrb_word_boxing_int_value(_mrb, value);
}

void
MRubyState::write_barrier(RBasic *b)
{
	_api.mrb_write_barrier(_mrb, b);
}

mrb_value
MRubyState::yield(const mrb_value b, const mrb_value arg)
{
	return _api.mrb_yield(_mrb, b, arg);
}

mrb_value
MRubyState::yield_argv(const mrb_value b, const mrb_int argc,
    const mrb_value *argv)
{
	return _api.mrb_yield_argv(_mrb, b, argc, argv);
}

mrb_value
MRubyState::yield_with_class(const mrb_value b, const mrb_int argc,
    const mrb_value *argv, const mrb_value self, RClass *c)
{
	return _api.mrb_yield_with_class(_mrb, b, argc, argv, self, c);
}

mrb_value
MRubyState::obj_value(void *p)
{
	return _api.mrb_obj_value(p);
}

mrb_value
MRubyState::int_value(const mrb_int i)
{
	return _api.mrb_int_value(_mrb, i);
}

mrb_value
MRubyState::float_value(const mrb_float f)
{
	return _api.mrb_float_value(_mrb, f);
}

mrb_value
MRubyState::symbol_value(const mrb_sym i)
{
	return _api.mrb_symbol_value(i);
}

euler::util::Reference<euler::app::dragonruby::State>
MRubyState::state() const
{
	return _state.strengthen();
}

mrb_state *
MRubyState::mrb() const
{
	return _mrb;
}

/* ReSharper disable once CppDFAConstantParameter */
mrb_int
MRubyState::get_args_v(mrb_args_format format, void **ptr, va_list *ap)
{
	mrb_state *mrb = _mrb;
#define GET_ARG(_type) (ptr ? ((_type)(*ptr++)) : va_arg((*ap), _type))
	const char *fmt = format;
	char c;
	mrb_int i = 0;
	mrb_int argc = _mrb->c->ci->argc;
	mrb_value *array_argv = _mrb->c->ci->stack + 1;
	mrb_bool argv_on_stack = argc >= 0;
	mrb_bool opt = FALSE;
	mrb_bool opt_skip = TRUE;
	mrb_bool given = TRUE;
	mrb_value kdict;
	mrb_bool reqkarg = FALSE;
	int argc_min = 0, argc_max = 0;

	if (!argv_on_stack) {
		auto a = mrb_ary_ptr(*array_argv);
		array_argv = ARY_PTR(a);
		argc = ARY_LEN(a);
	}

#define ARGV array_argv

	while ((c = *fmt++)) {
		switch (c) {
		case '|': opt = TRUE; break;
		case '*':
			opt_skip = FALSE;
			argc_max = -1;
			if (!reqkarg) reqkarg = strchr(fmt, ':') ? TRUE : FALSE;
			goto check_exit;
		case '!': break;
		case ':':
			reqkarg = TRUE;
			/* fall through */
		case '&':
		case '?':
			if (opt) opt_skip = FALSE;
			break;
		default:
			if (!opt) argc_min++;
			argc_max++;
			break;
		}
	}

check_exit:
	if (reqkarg && argc > argc_min && mrb_hash_p(kdict = ARGV[argc - 1])) {
		hash_check_kdict(kdict);
		argc--;
	} else {
		kdict = mrb_nil_value();
	}

	opt = FALSE;
	i = 0;
	while ((c = *format++)) {
		mrb_value *argv = ARGV;
		mrb_bool altmode;

		switch (c) {
		case '|':
		case '*':
		case '&':
		case '?':
		case ':': break;
		default:
			if (argc <= i) {
				if (opt) {
					given = FALSE;
				} else {
					mrb_argnum_error(_mrb, argc, argc_min,
					    argc_max);
				}
			}
			break;
		}

		if (*format == '!') {
			format++;
			altmode = TRUE;
		} else {
			altmode = FALSE;
		}

		switch (c) {
		case 'o': {
			mrb_value *p;

			p = GET_ARG(mrb_value *);
			if (i < argc) { *p = argv[i++]; }
		} break;
		case 'C': {
			mrb_value *p;

			p = GET_ARG(mrb_value *);
			if (i < argc) {
				mrb_value ss;

				ss = argv[i++];
				if (!class_ptr_p(ss)) {
					mrb_raisef(_mrb, E_TYPE_ERROR,
					    "%v is not class/module", ss);
				}
				*p = ss;
			}
		} break;
		case 'c': {
			RClass **p;

			p = GET_ARG(struct RClass **);
			if (i < argc) {
				mrb_value ss;

				ss = argv[i++];
				if (!class_ptr_p(ss)) {
					mrb_raisef(_mrb, E_TYPE_ERROR,
					    "%v is not class/module", ss);
				}
				*p = mrb_class_ptr(ss);
			}
		} break;
		case 'S': {
			mrb_value *p;

			p = GET_ARG(mrb_value *);
			if (i < argc) {
				*p = argv[i++];
				if (!(altmode && mrb_nil_p(*p))) {
					_api.mrb_to_str(_mrb, *p);
				}
			}
		} break;
		case 'A': {
			mrb_value *p;

			p = GET_ARG(mrb_value *);
			if (i < argc) {
				*p = argv[i++];
				if (!(altmode && mrb_nil_p(*p))) {
					mrb_check_type(_mrb, *p, MRB_TT_ARRAY);
				}
			}
		} break;
		case 'H': {
			mrb_value *p;

			p = GET_ARG(mrb_value *);
			if (i < argc) {
				*p = argv[i++];
				if (!(altmode && mrb_nil_p(*p))) {
					mrb_check_type(_mrb, *p, MRB_TT_HASH);
				}
			}
		} break;
		case 's': {
			const char **ps = nullptr;
			mrb_int *pl = nullptr;

			ps = GET_ARG(const char **);
			pl = GET_ARG(mrb_int *);
			if (i < argc) {
				mrb_value ss;
				ss = argv[i++];
				if (altmode && mrb_nil_p(ss)) {
					*ps = nullptr;
					*pl = 0;
				} else {
					_api.mrb_to_str(_mrb, ss);
					*ps = RSTRING_PTR(ss);
					*pl = RSTRING_LEN(ss);
				}
			}
		} break;
		case 'z': {
			const char **ps;

			ps = GET_ARG(const char **);
			if (i < argc) {
				mrb_value ss;
				ss = argv[i++];
				if (altmode && mrb_nil_p(ss)) {
					*ps = nullptr;
				} else {
					_api.mrb_to_str(_mrb, ss);
					*ps = RSTRING_CSTR(_mrb, ss);
				}
			}
		} break;
		case 'a': {
			const mrb_value **pb;
			mrb_int *pl;

			pb = GET_ARG(const mrb_value **);
			pl = GET_ARG(mrb_int *);
			if (i < argc) {
				mrb_value aa;
				aa = argv[i++];
				if (altmode && mrb_nil_p(aa)) {
					*pb = nullptr;
					*pl = 0;
				} else {
					RArray *a;
					mrb_check_type(_mrb, aa, MRB_TT_ARRAY);
					a = mrb_ary_ptr(aa);
					*pb = ARY_PTR(a);
					*pl = ARY_LEN(a);
				}
			}
		} break;
		case 'I': {
			void **p;

			p = GET_ARG(void **);
			if (i < argc) {
				mrb_value ss;
				ss = argv[i++];
				if (!mrb_istruct_p(ss)) {
					mrb_raisef(_mrb, E_TYPE_ERROR,
					    "%v is not inline struct", ss);
				}
				*p = mrb_istruct_ptr(ss);
			}
		} break;
#ifndef MRB_NO_FLOAT
		case 'f': {
			mrb_float *p;

			p = GET_ARG(mrb_float *);
			if (i < argc) { *p = _api.mrb_to_flo(_mrb, argv[i++]); }
		} break;
#endif
		case 'i': {
			mrb_int *p;

			p = GET_ARG(mrb_int *);
			if (i < argc) {
				*p = mrb_integer(
				    _api.mrb_to_int(_mrb, argv[i++]));
			}
		} break;
		case 'b': {
			mrb_bool *boolp = GET_ARG(mrb_bool *);

			if (i < argc) {
				mrb_value b = argv[i++];
				*boolp = mrb_test(b);
			}
		} break;
		case 'n': {
			mrb_sym *symp;

			symp = GET_ARG(mrb_sym *);
			if (i < argc) {
				mrb_value ss;

				ss = argv[i++];
				*symp = mrb_obj_to_sym(_mrb, ss);
			}
		} break;
		case 'd': {
			void **datap;
			mrb_data_type const *type;

			datap = GET_ARG(void **);
			type = GET_ARG(struct mrb_data_type const *);
			if (i < argc) {
				mrb_value dd = argv[i++];
				if (altmode && mrb_nil_p(dd)) {
					*datap = nullptr;
				} else {
					*datap
					    = mrb_data_get_ptr(_mrb, dd, type);
				}
			}
		} break;

		case '&': {
			mrb_value *p, *bp;

			p = GET_ARG(mrb_value *);
			if (_mrb->c->ci->argc < 0) {
				bp = _mrb->c->ci->stack + 2;
			} else {
				bp = _mrb->c->ci->stack + _mrb->c->ci->argc + 1;
			}
			if (altmode && mrb_nil_p(*bp)) {
				mrb_raise(_mrb, E_ARGUMENT_ERROR,
				    "no block given");
			}
			*p = *bp;
		} break;
		case '|':
			if (opt_skip && i == argc) goto finish;
			opt = TRUE;
			break;
		case '?': {
			mrb_bool *p;

			p = GET_ARG(mrb_bool *);
			*p = given;
		} break;

		case '*': {
			const mrb_value **var;
			mrb_int *pl;
			mrb_bool nocopy
			    = (altmode || !argv_on_stack) ? TRUE : FALSE;

			var = GET_ARG(const mrb_value **);
			pl = GET_ARG(mrb_int *);
			if (argc > i) {
				*pl = argc - i;
				if (*pl > 0) {
					if (nocopy) {
						*var = argv + i;
					} else {
						mrb_value args
						    = mrb_ary_new_from_values(
							_mrb, *pl, argv + i);
						RARRAY(args)->c = nullptr;
						*var = RARRAY_PTR(args);
					}
				}
				i = argc;
			} else {
				*pl = 0;
				*var = nullptr;
			}
		} break;

		case ':': {
			mrb_value ksrc = mrb_hash_p(kdict)
			    ? mrb_hash_dup(_mrb, kdict)
			    : mrb_hash_new(_mrb);
			const mrb_kwargs *kwargs = GET_ARG(const mrb_kwargs *);
			mrb_value *rest;

			if (kwargs == nullptr) {
				rest = nullptr;
			} else {
				uint32_t kwnum = kwargs->num;
				uint32_t required = kwargs->required;
				const mrb_sym *kname = kwargs->table;
				mrb_value *values = kwargs->values;
				uint32_t j;
				static constexpr uint32_t keyword_max = 40;

				if (kwnum > keyword_max || required > kwnum) {
					mrb_raise(_mrb, E_ARGUMENT_ERROR,
					    "keyword number is too large");
				}

				for (j = required; j > 0;
				    j--, kname++, values++) {
					mrb_value k = mrb_symbol_value(*kname);
					if (!mrb_hash_key_p(_mrb, ksrc, k)) {
						mrb_raisef(_mrb,
						    E_ARGUMENT_ERROR,
						    "missing keyword: %n",
						    *kname);
					}
					*values = mrb_hash_delete_key(_mrb,
					    ksrc, k);
					mrb_gc_protect(_mrb, *values);
				}

				for (j = kwnum - required; j > 0;
				    j--, kname++, values++) {
					mrb_value k = mrb_symbol_value(*kname);
					if (mrb_hash_key_p(_mrb, ksrc, k)) {
						*values = mrb_hash_delete_key(
						    _mrb, ksrc, k);
						mrb_gc_protect(_mrb, *values);
					} else {
						*values = mrb_undef_value();
					}
				}

				rest = kwargs->rest;
			}

			if (rest) {
				*rest = ksrc;
			} else if (!mrb_hash_empty_p(_mrb, ksrc)) {
				ksrc = mrb_hash_keys(_mrb, ksrc);
				ksrc = RARRAY_PTR(ksrc)[0];
				mrb_raisef(_mrb, E_ARGUMENT_ERROR,
				    "unknown keyword: %v", ksrc);
			}
		} break;

		default:
			mrb_raisef(_mrb, E_ARGUMENT_ERROR,
			    "invalid argument specifier %c", c);
		}
	}

#undef ARGV
#undef GET_ARG
	if (!c && argc > i) {
		mrb_argnum_error(_mrb, argc, argc_min, argc_max);
	}
finish:
	return i;
}
