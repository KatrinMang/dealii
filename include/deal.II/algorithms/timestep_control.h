// ---------------------------------------------------------------------
//
// Copyright (C) 2010 - 2018 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE.md at
// the top level directory of deal.II.
//
// ---------------------------------------------------------------------


#ifndef dealii_time_step_control_h
#define dealii_time_step_control_h

#include <deal.II/base/smartpointer.h>
#include <deal.II/base/subscriptor.h>

#include <deal.II/lac/vector_memory.h>

#include <cstdio>

DEAL_II_NAMESPACE_OPEN

// Forward declaration
#ifndef DOXYGEN
class ParameterHandler;
#endif

namespace Algorithms
{
  /**
   * Control class for timestepping schemes. Its main task is determining the
   * size of the next time step and the according point in the time interval.
   * Additionally, it controls writing the solution to a file.
   *
   * The size of the next time step is determined as follows:
   * <ol>
   * <li> According to the strategy, the step size is tentatively added to the
   * current time.
   * <li> If the resulting time exceeds the final time of the interval, the
   * step size is reduced in order to meet this time.
   * <li> If the resulting time is below the final time by just a fraction of
   * the step size, the step size is increased in order to meet this time.
   * <li> The resulting step size is used from the current time.
   * </ol>
   *
   * The variable @p print_step can be used to control the amount of output
   * generated by the timestepping scheme.
   */
  class TimestepControl : public Subscriptor
  {
  public:
    /**
     * Constructor setting default values
     */
    TimestepControl(double start      = 0.,
                    double final      = 1.,
                    double tolerance  = 1.e-2,
                    double start_step = 1.e-2,
                    double print_step = -1.,
                    double max_step   = 1.);

    /**
     * Declare the control parameters for parameter handler.
     */
    static void
    declare_parameters(ParameterHandler &param);

    /**
     * Read the control parameters from a parameter handler.
     *
     * This function also calls restart() to reset all other internal
     * parameters of this class to their appropriate values based on
     * the parameters just read.
     */
    void
    parse_parameters(ParameterHandler &param);

    /**
     * Return the left end of the time interval.
     */
    double
    start() const;
    /**
     * Return the right end of the time interval. The control mechanism
     * ensures that the final time step ends at this point.
     */
    double
    final() const;
    /**
     * Return the tolerance value controlling the time steps.
     */
    double
    tolerance() const;
    /**
     * Return the size of the current time step.
     */
    double
    step() const;

    /**
     * Return the current time.
     */
    double
    now() const;

    /**
     * Compute the size of the next step and return true if it differs from
     * the current step size. Advance the current time by the new step size.
     */
    bool
    advance();

    /**
     * Set start value.
     */
    void
    start(double);
    /**
     * Set final time value.
     */
    void
    final(double);
    /**
     * Set tolerance
     */
    void
    tolerance(double);

    /**
     * Set size of the first step. This may be overwritten by the time
     * stepping strategy.
     *
     * @param[in] step The size of the first step, which may be overwritten by
     * the time stepping strategy.
     */
    void
    start_step(const double step);

    /**
     * Set size of the maximum step size.
     */
    void
    max_step(double);

    /**
     * Set now() equal to start(). Initialize step() and print() to their
     * initial values.
     */
    void
    restart();

    /**
     * Return true if this timestep should be written to disk.
     */
    bool
    print();

    /**
     * Set the output name template.
     */
    void
    file_name_format(const char *);

    /**
     * Return the output name template.
     */
    const char *
    file_name_format();

  private:
    /**
     * The beginning of the time interval.
     */
    double start_val;

    /**
     *The end of the time interval.
     */
    double final_val;

    /**
     * The tolerance value controlling the time steps.
     */
    double tolerance_val;

    /**
     * The size of the first step.
     */
    double start_step_val;

    /**
     * The maximum step size.
     */
    double max_step_val;

    /**
     * The minimum step size.
     */
    double min_step_val;

    /**
     * The size of the current time step. This may differ from @p step_val, if
     * we aim at @p final_val.
     */
    double current_step_val;

    /**
     * The size of the current time step determined by the strategy. This may
     * differ from @p current_step_val, if we aim at @p final_val.
     */
    double step_val;

    /**
     * The current time.
     */
    double now_val;

    /**
     * Determines the approximate time interval between generated outputs.
     * If negative, output will be generated at all time steps.
     */
    double print_step;

    /**
     * If current time exceeds this value, it is time to generate the output.
     */
    double next_print_val;

    /**
     * Output file name template.
     */
    char format[30];
  };


  inline double
  TimestepControl::start() const
  {
    return start_val;
  }


  inline double
  TimestepControl::final() const
  {
    return final_val;
  }


  inline double
  TimestepControl::step() const
  {
    return current_step_val;
  }


  inline double
  TimestepControl::tolerance() const
  {
    return tolerance_val;
  }


  inline double
  TimestepControl::now() const
  {
    return now_val;
  }


  inline void
  TimestepControl::start(double t)
  {
    start_val = t;
  }


  inline void
  TimestepControl::final(double t)
  {
    final_val = t;
  }


  inline void
  TimestepControl::tolerance(double t)
  {
    tolerance_val = t;
  }


  inline void
  TimestepControl::start_step(const double t)
  {
    start_step_val = t;
  }


  inline void
  TimestepControl::max_step(double t)
  {
    max_step_val = t;
  }


  inline void
  TimestepControl::restart()
  {
    now_val          = start_val;
    step_val         = start_step_val;
    current_step_val = step_val;
    if (print_step > 0.)
      next_print_val = now_val + print_step;
    else
      next_print_val = now_val - 1.;
  }


  inline void
  TimestepControl::file_name_format(const char *fmt)
  {
    strcpy(format, fmt);
  }


  inline const char *
  TimestepControl::file_name_format()
  {
    return format;
  }
} // namespace Algorithms

DEAL_II_NAMESPACE_CLOSE

#endif
