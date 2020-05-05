#ifndef ALTITUDE_ESTIMATOR_H
#define ALTITUDE_ESTIMATOR_H

#include <ros/ros.h>

#include <mrs_lib/lkf.h>
#include <mrs_lib/param_loader.h>
#include <mrs_lib/profiler.h>

#include <mrs_msgs/MavrosDiagnostics.h>
#include <mrs_msgs/MavrosState.h>

#include <string>
#include <vector>
#include <mutex>

#include "types.h"

#define ALT_DT 0.01

namespace mrs_odometry
{


  class AltitudeEstimator {

  public:
    AltitudeEstimator(const std::string &estimator_name, const std::vector<bool> &fusing_measurement, const std::vector<alt_H_t> &H_multi, const alt_Q_t &Q,
                      const std::vector<alt_R_t> &R_multi);

    bool        doPrediction(const double input, const double dt);
    bool        doPrediction(const double input);
    bool        doCorrection(const Eigen::VectorXd &measurement, int measurement_type);
    bool        getStates(Eigen::MatrixXd &states);
    bool        getN(int& n);
    bool        getState(int state_id, Eigen::VectorXd &state);
    std::string getName(void);
    bool        setState(int state_id, const Eigen::VectorXd &state);
    bool        setR(double cov, int measurement_type);
    bool        getR(double &cov, int measurement_type);
    bool        getCovariance(Eigen::MatrixXd &cov);
    bool        setCovariance(const Eigen::MatrixXd &cov);
    bool        reset(const Eigen::MatrixXd &states);

  private:
    std::string                  m_estimator_name;
    std::vector<bool>            m_fusing_measurement;
    int    m_n_states;
    size_t m_n_measurement_types;

    // State transition matrix
    alt_A_t              m_A;

    // Input matrix
    alt_B_t              m_B;

    // Array with mapping matrices for each fused measurement
    std::vector<alt_H_t> m_H_multi;

    // Process covariance matrix
    alt_Q_t              m_Q;

    // Array with covariances of each fused measurement
    std::vector<alt_R_t> m_R_multi;


    // Default dt
    double m_dt = ALT_DT;
    double m_dt_sq = m_dt*m_dt;

    // Kalman filter - the core of the estimator 
    std::unique_ptr<lkf_alt_t> mp_lkf;

    // Variable for holding the current state and covariance 
    alt_statecov_t m_sc;

    std::mutex mutex_lkf;

    bool m_is_initialized = false;
  };

}  // namespace mrs_odometry

#endif
