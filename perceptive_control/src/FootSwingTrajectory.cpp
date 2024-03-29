/*
 * @Author: haoyun 
 * @Date: 2023-05-25 16:41:13
 * @LastEditors: haoyun 
 * @LastEditTime: 2023-05-25 16:43:05
 * @FilePath: /elevation_map_ws/src/perceptive_control/src/FootSwingTrajectory.cpp
 * @Description: from MIT-Cheetah code
 * 
 * Copyright (c) 2023 by HAR-Lab, All Rights Reserved. 
 */

#include "trajectory_planning/Interpolation.h"
#include "trajectory_planning/FootSwingTrajectory.h"

/*!
 * Compute foot swing trajectory with a bezier curve
 * @tparam T
 * @param phase
 * @param swingTime
 */
template <typename T>
void FootSwingTrajectory<T>::computeSwingTrajectoryBezier(T phase, T swingTime) {
  _p = Interpolate::cubicBezier<Vec3<T>>(_p0, _pf, phase);
  _v = Interpolate::cubicBezierFirstDerivative<Vec3<T>>(_p0, _pf, phase) / swingTime;
  _a = Interpolate::cubicBezierSecondDerivative<Vec3<T>>(_p0, _pf, phase) / (swingTime * swingTime);

  T zp, zv, za;

  if(phase < T(0.5)) {
    zp = Interpolate::cubicBezier<T>(_p0[2], _p0[2] + _height, phase * 2);
    zv = Interpolate::cubicBezierFirstDerivative<T>(_p0[2], _p0[2] + _height, phase * 2) * 2 / swingTime;
    za = Interpolate::cubicBezierSecondDerivative<T>(_p0[2], _p0[2] + _height, phase * 2) * 4 / (swingTime * swingTime);
  } else {
    zp = Interpolate::cubicBezier<T>(_p0[2] + _height, _pf[2], phase * 2 - 1);
    zv = Interpolate::cubicBezierFirstDerivative<T>(_p0[2] + _height, _pf[2], phase * 2 - 1) * 2 / swingTime;
    za = Interpolate::cubicBezierSecondDerivative<T>(_p0[2] + _height, _pf[2], phase * 2 - 1) * 4 / (swingTime * swingTime);
  }

  _p[2] = zp;
  _v[2] = zv;
  _a[2] = za;
}

template class FootSwingTrajectory<double>;
template class FootSwingTrajectory<float>;

