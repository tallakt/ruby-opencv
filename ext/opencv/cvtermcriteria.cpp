/************************************************************

   cvtermcriteria.cpp -

   $Author: lsxi $

   Copyright (C) 2005-2006 Masakazu Yonekura

************************************************************/
#include "cvtermcriteria.h"
/*
 * Document-class: OpenCV::CvTermCriteria
 *
 * CvTermCriteria has parameter "max" and "eps".
 * "max" is the maximum repetition frequency.
 * "eps" is a minimum difference value during current and previous state
 * (It is different to which state "eps" refer depending on the method).
 *
 * Because the name of CvTermCriteria seems to be very long, it has alias named CvTerm.
 */
__NAMESPACE_BEGIN_OPENCV
__NAMESPACE_BEGIN_CVTERMCRITERIA

VALUE rb_klass;
    
VALUE
rb_class()
{
  return rb_klass;
}

VALUE
rb_allocate(VALUE klass)
{
  CvTermCriteria *ptr;
  return Data_Make_Struct(klass, CvTermCriteria, 0, -1, ptr);
}

/*
 * call-seq:
 *   CvTermCriteria.new([max = 0][,eps = 0.0]) -> obj
 *   CvTermCriteria.new(int) = CvTermCriteria.new(int, 0.0)
 *   CvTermCriteria.new(float) = CvTermCriteria.new(0, float)
 *
 * Create new term criteria.
 */
VALUE
rb_initialize(int argc, VALUE *argv, VALUE self)
{
  VALUE max, eps;
  rb_scan_args(argc, argv, "02", &max, &eps);
  int type = 0;
  if (!NIL_P(max))
    type |= CV_TERMCRIT_ITER;
  if (!NIL_P(eps))
    type |= CV_TERMCRIT_EPS;
  try {
    *CVTERMCRITERIA(self) = cvTermCriteria(type, IF_INT(max, 0), IF_DBL(eps, 0.0));
  }
  catch (cv::Exception& e) {
    raise_cverror(e);
  }
  return self;
}

/*
 * call-seq:
 *   type -> int
 *
 * Return a combination of CV_TERMCRIT_ITER and CV_TERMCRIT_EPS
 */
VALUE
rb_type(VALUE self)
{
  return INT2NUM(CVTERMCRITERIA(self)->type);
}

/*
 * call-seq:
 *   max -> int or nil
 *
 * Return the maximum repetition frequency.
 */
VALUE
rb_max(VALUE self)
{
  CvTermCriteria *ptr = CVTERMCRITERIA(self);
  if (ptr->type & CV_TERMCRIT_ITER)
    return INT2NUM(ptr->max_iter);
  else
    return Qnil;
}

/*
 * call-seq:
 *   max = <i>val</i> -> self
 *
 * Set the maximum repetition frequency.
 * If <i>val</i> is 0 (or negative value), repetition frequency is disregarded.
 */
VALUE
rb_set_max(VALUE self, VALUE max_value)
{
  CvTermCriteria *ptr = CVTERMCRITERIA(self);
  int max = NUM2INT(max_value);
  if (max > 0) {
    ptr->type |= CV_TERMCRIT_ITER;
    ptr->max_iter = max;
  }
  else {
    ptr->type ^= CV_TERMCRIT_ITER;
    ptr->max_iter = 0;
  }
  return self;
}

/*
 * call-seq:
 *   eps -> float or nil
 *
 * Return the minimum difference value during current and previous state.
 */
VALUE
rb_eps(VALUE self)
{
  CvTermCriteria *ptr = CVTERMCRITERIA(self);
  if (ptr->type & CV_TERMCRIT_EPS)
    return rb_float_new(ptr->epsilon);
  else
    return Qnil;
}

/*
 * call-seq:
 *   eps = <i>val</i> -> self
 *
 * Set the minimum difference value during current and previous state.
 * If <i>val</i> is 0.0 (or negative value), the minimum difference value
 * during current and previous state is disregarded.
 */
VALUE
rb_set_eps(VALUE self, VALUE eps_value)
{
  CvTermCriteria *ptr = CVTERMCRITERIA(self);
  double eps = NUM2DBL(eps_value);
  if (eps > 0) {
    ptr->type = ptr->type | CV_TERMCRIT_EPS;
    ptr->epsilon = eps;
  }
  else {
    ptr->type = ptr->type ^ CV_TERMCRIT_EPS;
    ptr->epsilon = 0;
  }
  return self;
}

VALUE
new_object(CvTermCriteria criteria)
{
  VALUE object = rb_allocate(rb_klass);
  *CVTERMCRITERIA(object) = criteria;
  return object;
}

void
init_ruby_class()
{
#if 0
  // For documentation using YARD
  VALUE opencv = rb_define_module("OpenCV");
#endif

  if (rb_klass)
    return;
  /* 
   * opencv = rb_define_module("OpenCV");
   * 
   * note: this comment is used by rdoc.
   */
  VALUE opencv = rb_module_opencv();
  
  rb_klass = rb_define_class_under(opencv, "CvTermCriteria", rb_cObject);
  /* CvTermCriteria: class */
  rb_define_const(opencv, "CvTerm", rb_klass);
  rb_define_alloc_func(rb_klass, rb_allocate);
  rb_define_method(rb_klass, "initialize", RUBY_METHOD_FUNC(rb_initialize), -1);
  rb_define_method(rb_klass, "type", RUBY_METHOD_FUNC(rb_type), 0);
  rb_define_method(rb_klass, "max", RUBY_METHOD_FUNC(rb_max), 0);
  rb_define_method(rb_klass, "max=", RUBY_METHOD_FUNC(rb_set_max), 1);
  rb_define_method(rb_klass, "eps", RUBY_METHOD_FUNC(rb_eps), 0);
  rb_define_method(rb_klass, "eps=", RUBY_METHOD_FUNC(rb_set_eps), 1);
  rb_define_alias(rb_klass, "epsilon", "eps");
  rb_define_alias(rb_klass, "epsilon=", "eps=");
}

__NAMESPACE_END_CVTERMCRITERIA
__NAMESPACE_END_OPENCV

