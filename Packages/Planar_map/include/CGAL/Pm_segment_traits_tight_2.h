// ======================================================================
//
// Copyright (c) 1997 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
//
// release       : $$
// release_date  : $$
//
// file          : include/CGAL/Pm_segment_traits_tight_2.h
// package       : Planar_map (5.87)
// maintainer    : Eyal Flato        <flato@math.tau.ac.il>
// source        : 
// revision      : 
// revision_date : 
// author(s)     : Iddo Hanniel      <hanniel@math.tau.ac.il>
//                 Eyal Flato        <flato@post.tau.ac.il>
//                 Oren Nechushtan   <theoren@math.tau.ac.il>
//                 Eti Ezra          <estere@post.tau.ac.il>
//                 Shai Hirsch       <shaihi@post.tau.ac.il>
//                 Eugene Lipovetsky <eug@post.tau.ac.il>
//                 Efi Fogel         <efif@post.tau.ac.il>
//                 Ron Wein          <wein@post.tau.ac.il>
//
// coordinator   : Tel-Aviv University (Dan Halperin <halperin@math.tau.ac.il>)
//
// Chapter       : 
// ======================================================================
#ifndef CGAL_PM_SEGMENT_TRAITS_2_H
#define CGAL_PM_SEGMENT_TRAITS_2_H

#include <CGAL/Planar_map_2/Pm_segment_utilities_2.h>
#include <CGAL/tags.h>

CGAL_BEGIN_NAMESPACE

template <class Kernel_>
class Pm_segment_traits_tight_2 : public Kernel_
{
public:
  typedef Kernel_                         Kernel;

  // Categories:
  typedef Tag_true                        Has_left_category;
    
  // Traits objects
  typedef typename Kernel::Point_2        Point_2;
  typedef typename Kernel::Segment_2      X_curve_2;

  // Backward compatability    
  typedef Point_2                         Point;
  typedef X_curve_2                       X_curve;

protected:
  // Functors:
  typedef typename Kernel::Is_vertical_2        Is_vertical_2;
  typedef typename Kernel::Construct_vertex_2   Construct_vertex_2;
  typedef typename Kernel::Less_x_2             Less_x_2;
    
  typedef CGAL::Counterclockwise_in_between_for_segments_2<Kernel, X_curve_2>
                                                Counterclockwise_in_between_2;

protected:
  inline Counterclockwise_in_between_2 counterclockwise_in_between_2_object()
    const
  { return Counterclockwise_in_between_2(); }

public:
  // Creation
  Pm_segment_traits_tight_2() {}

  // Operations
  // ----------
    
  /*! compare_x() compares the x-coordinates of two given points
   * \param p1 the first point
   * \param p2 the second point
   * \return LARGER if x(p1) > x(p2), SMALLER if x(p1) < x(p2), or else EQUAL
   *
   * \todo replace indirect use compare_x() with compare_x_2()
   */
  Comparison_result compare_x(const Point_2 & p1, const Point_2 & p2) const
  { return compare_x_2_object()(p1, p2); }

  /*! compare_xy() compares lexigoraphically the two points by x, then by y.
   * \param p1 the first point
   * \param p2 the second point
   * \return LARGER if x(p1) > x(p2), or if x(p1) = x(p2) and y(p1) > y(p2); 
   *         SMALLER if x(p1) < x(p2), or if x(p1) = x(p2) and y(p1) < y(p2);
   *         or else EQUAL
   */
  Comparison_result compare_xy(const Point_2 & p1, const Point_2 & p2) const
  { return compare_xy_2_object()(p1, p2); }

  /*! curve_is_vertical()
   * \param cv the curve
   * \return true iff the curve is vertical
   *
   * \todo replace indirect use curve_is_vertical() with is_vertical_2()
   */
  bool curve_is_vertical(const X_curve_2 & cv) const 
  { return is_vertical_2_object()(cv); }

  /*! curve_is_in_x_range()
   * \param cv the curve
   * \param q the point
   * \return true if q is in the x range of cv
   *
   * \todo Intorduce Is_in_x_range_2() or perhaps Is_in_x_closed_range_2()
   * in kernel. Currently, this is implemented using existing traits (kernel)
   * functions (curve_source(), curve_target()) that return the source and
   * target points by value, which is not as efficient as possible.
   */
  bool curve_is_in_x_range(const X_curve_2 & cv, const Point_2 & q) const
  {
    Construct_vertex_2 construct_vertex = construct_vertex_2_object();
    const Point_2 & source = construct_vertex(cv, 0);
    const Point_2 & target = construct_vertex(cv, 1);
    Less_x_2 less_x = less_x_2_object();
    return !((less_x(source, q) && less_x(target, q)) ||
             (less_x(q, source) && less_x(q, target)));
  }

  /*! curve_compare_at_x() compares the y-coordinate of two given curves at
   * the x-coordinate of a given point
   * Preconditions: The point q is in the x range of the two curves.
   * \param cv1 the first curve
   * \param cv2 the second curve
   * \param q the point
   * \return LARGER if cv1(x(q)) > cv2(x(q)), SMALLER if cv1(x(q)) < cv2(x(q),
   *  or else EQUAL.
   * \todo replace indirect use curve_compare_at_x() with compare_y_at_x_2()
   */
  Comparison_result curve_compare_at_x(const X_curve_2 & cv1, 
				       const X_curve_2 & cv2, 
				       const Point_2 & q) const
  {
    CGAL_precondition(curve_is_in_x_range(cv1, q));
    CGAL_precondition(curve_is_in_x_range(cv2, q));

    return compare_y_at_x_2_object()(q, cv1, cv2);
  }

  Comparison_result curve_compare_at_x_left(const X_curve_2 & cv1,
                                            const X_curve_2 & cv2, 
                                            const Point_2 & q) const 
  {
    // The two curves must not be vertical.
    CGAL_precondition(! curve_is_vertical(cv1));
    CGAL_precondition(! curve_is_vertical(cv2));

    // The two curve must be defined at q and also to its left.
    CGAL_precondition_code(
        Construct_vertex_2 construct_vertex = construct_vertex_2_object();
	Less_x_2 less_x = less_x_2_object();
	const Point_2 & source1 = construct_vertex(cv1, 0);
	const Point_2 & target1 = construct_vertex(cv1, 1);
	const Point_2 & source2 = construct_vertex(cv2, 0);
	const Point_2 & target2 = construct_vertex(cv2, 1);
	);

    CGAL_precondition (less_x(source1, q) || less_x(target1, q));
    CGAL_precondition (!(less_x(source1, q) && less_x(target1, q)));
    
    CGAL_precondition (less_x(source2, q) || less_x(target2, q));
    CGAL_precondition (!(less_x(source2, q) && less_x(target2, q)));
    
    // Since the curves are continuous, if they are not equal at q, the same
    // result also applies to q's left.
    Comparison_result r = compare_y_at_x_2_object()(q, cv1, cv2);
    if (r != EQUAL) return r;     
    
    // <cv2> and <cv1> meet at a point with the same x-coordinate as q
    // compare their derivatives.
    return compare_slope_2_object()(cv2, cv1);
  }

  /*! curve_compare_at_x_right() compares the y value of two curves in an
   * epsilon environment to the right of the x value of the input point
   * Preconditions: The point q is in the x range of the two curves, and both
   * of them must be also be defined to its right.
   */
  Comparison_result curve_compare_at_x_right(const X_curve_2 & cv1,
                                             const X_curve_2 & cv2, 
                                             const Point_2 & q) const 
  {
    // The two curves must not be vertical.
    CGAL_precondition(! curve_is_vertical(cv1));
    CGAL_precondition(! curve_is_vertical(cv2));

    // The two curve must be defined at q and also to its right.
    CGAL_precondition_code(
        Construct_vertex_2 construct_vertex = construct_vertex_2_object();
	Less_x_2 less_x = less_x_2_object();
	const Point_2 & source1 = construct_vertex(cv1, 0);
	const Point_2 & target1 = construct_vertex(cv1, 1);
	const Point_2 & source2 = construct_vertex(cv2, 0);
	const Point_2 & target2 = construct_vertex(cv2, 1);
	);

    CGAL_precondition (less_x(q, source1) || less_x(q, target1));
    CGAL_precondition (!(less_x(q, source1) && less_x(q, target1)));
    
    CGAL_precondition (less_x(q, source2) || less_x(q, target2));
    CGAL_precondition (!(less_x(q, source2) && less_x(q, target2)));

    // Since the curves are continuous, if they are not equal at q, the same
    // result also applies to q's left.
    Comparison_result r = curve_compare_at_x(cv1, cv2, q);
    if (r != EQUAL) return r;     
    
    // <cv1> and <cv2> meet at a point with the same x-coordinate as q
    // compare their derivatives.
    return compare_slope_2_object()(cv1, cv2);
  }
    
  /*! Return the curve-point status of the input objects.
   * \pre p must be in the x-range of cv.
   */
  Comparison_result curve_get_point_status (const X_curve_2 & cv, 
					    const Point_2 & p) const
  {
    CGAL_precondition(curve_is_in_x_range(cv, p));

    Comparison_result res = compare_y_at_x_2_object()(p, cv);
    if (res == SMALLER)
      return (LARGER);
    else if (res == LARGER)
      return (SMALLER);
    return (EQUAL);
  }

  /*! \todo replace indirect use curve_is_same() with equal_2()
   */
  bool curve_is_same(const X_curve_2 & cv1,const X_curve_2 & cv2) const
  { return equal_2_object()(cv1, cv2); }

  /*! \todo replace indirect use point_is_same() with equal_2()
   */
  bool point_is_same(const Point_2 & p1, const Point_2 & p2) const
  { return equal_2_object()(p1, p2); }
  
  /*! \todo replace indirect use curve_source() with construct_vertex_2()
   */
  Point_2 curve_source(const X_curve_2 & cv) const 
  { return construct_vertex_2_object()(cv, 0); }

  /*! \todo replace indirect use curve_source() with construct_vertex_2()
   */
  Point_2 curve_target(const X_curve_2 & cv) const 
  { return construct_vertex_2_object()(cv, 1); }
};

CGAL_END_NAMESPACE

#endif
